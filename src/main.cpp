#include <Arduino.h>
#include "button.h"
#include "motor.h"
#include "ptransducer.h"
#include "serialComms.h"
#include "solenoids.h"
#include "tasks.h"
#include "scheduler.h"
#include "gui_telemetry/GuiCommands.h"
#include "gui_telemetry/GuiTelemetry.h"
#include <Preferences.h>

// Define buzzer pin
#define BUZZER_PIN 21

bool motor_flag = false;
uint32_t curr_pressure = 0;

TaskHandle_t task1_handle = NULL; // Handler for task 

bool state_flag = false; //for the button state message

// put function prototypes here:
void setSolenoid(int, int);
void activateMotor(int, int);
void activateAllSolenoids(int);
void activate1by1Solenoids(int);
bool getStartState(int);
void calculatePressureTask(void *parameters);
void guiTelemetryTask(void *parameters);
void serialCommandTask(void *parameters);
void processGuiCommand(const GuiCommand &command, Preferences &commandPreferences);
bool startGuiTask(int taskChosen, int taskTime);
int state_count = 0;
bool bttn_state = HIGH; // Initial state of button

// variables related RTOS Task1
void task1(void *parameter);
bool task1_flag = false;
struct TaskData {
  int task_chosen = 0;
  int task_time = 0;
  volatile bool task_state = false;
} task1_parameters;

volatile int misting_interval = 300000; //5 minutes in milliseconds
volatile int misting_time = 5000; //5 seconds
volatile bool day_night_flag;

//For holding saved data 
Preferences misting_params_main;
volatile int saved_interval_main;
volatile int saved_length_main;
volatile int saved_daynight_main;
volatile int continue_cycle_main;
volatile bool automatic_cycle_active = false;
volatile bool start_button_active = false;


void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);                         // give USB/UART a moment
  Serial.println("BOOT");
  
  Serial.printf("Total heap: %d bytes\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM: %d bytes\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d bytes\n", ESP.getFreePsram());

  //Setting pinmode of Start pin
  initButton();

  //Init of all functions
  initSolenoids();
  initTransducer();
  initMotor();

  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 100);
  delay(250);
  noTone(BUZZER_PIN);

  // Create RTOS Task that will constantly calculate and monitor pressure
  xTaskCreate(calculatePressureTask, "Pressure Calc", 2048, NULL, 4, NULL);

  // Init Scheduler parameters
  schedulerInit();
  // Init main misting parameters
  misting_params_main.begin("mstng_prms", false);
  saved_interval_main = misting_params_main.getInt("mst_ntrvl", 300000); // Default to 5 minutes if value not found
  saved_length_main = misting_params_main.getInt("mst_lngth", 5000); // Default to 5 seconds if value not found
  saved_daynight_main = misting_params_main.getInt("day_night", 1); // Default to day mode if value not found
  continue_cycle_main = misting_params_main.getInt("cont_cycle", 0); // Default to not continue cycle after power loss
  misting_interval = saved_interval_main;
  misting_time = saved_length_main;
  day_night_flag = saved_daynight_main == 0;

  if (xTaskCreate(guiTelemetryTask, "GUI Telemetry", 4096, NULL, 1, NULL) != pdPASS) {
    Serial.println("Failed to create GUI telemetry task.");
  }
  if (xTaskCreate(serialCommandTask, "GUI Command Reader", 4096, NULL, 2, NULL) != pdPASS) {
    Serial.println("Failed to create GUI command reader task.");
  }

}

// Keep in mind, all functions will be run via FreeRTOS. 
void loop() {

  bttn_state = readButton();
  if (bttn_state == LOW &&
      task1_parameters.task_state == false &&
      continue_cycle_main == 0 &&
      automatic_cycle_active == false) {
    start_button_active = true;
    while (readButton() == LOW) {
      vTaskDelay(pdMS_TO_TICKS(20));
    }

    continue_cycle_main = 1;
    start_button_active = false;
    if (task1_parameters.task_state) {
      continue_cycle_main = 0;
      Serial.println("Automatic cycle start cancelled: another task is running.");
    } else {
      misting_params_main.putInt("cont_cycle", 1);
      automatic_cycle_active = true;
      dayNightMistingCycle(misting_interval, misting_time, day_night_flag, misting_params_main);
      automatic_cycle_active = false;
      misting_params_main.putInt("cont_cycle", 0);
      continue_cycle_main = 0;
    }
  }

  //If start pin in HIGH (button is unpressed) we loop until it is
  else if (bttn_state == HIGH && continue_cycle_main == 0)
  {
    
      Serial.printf("Start Button State: %d\n", bttn_state);
      state_flag = true;
      int pressure_reading = getPressureInPSI(); //might be 0 at start
      Serial.printf("Pressure Reading: %d\n", pressure_reading);
      vTaskDelay(pdMS_TO_TICKS(500));

      //Print all saved parameters for debug
      Serial.printf("Saved Misting Interval: %d ms\n", saved_interval_main);
      Serial.printf("Saved Misting Length: %d ms\n", saved_length_main);
      Serial.printf("Saved Day/Night Flag: %d\n", saved_daynight_main);
      Serial.printf("Continue Cycle after Power Loss Flag: %d\n", continue_cycle_main);

  }

  else if (continue_cycle_main == 1 && bttn_state == HIGH && task1_parameters.task_state == false){
    saved_daynight_main = misting_params_main.getInt("day_night", 1); // Refresh in case it was changed
    saved_interval_main = misting_params_main.getInt("mst_ntrvl", 300000); // Refresh in case it was changed
    saved_length_main = misting_params_main.getInt("mst_lngth", 5000); // Refresh in case it was changed
    misting_interval = saved_interval_main;
    misting_time = saved_length_main;
    day_night_flag = saved_daynight_main == 0;
    Serial.println("Resuming previous cycle after power loss.");
    Serial.printf("Using saved parameters - Interval: %d ms, Length: %d ms, Day/Night(0=day, 1=night): %d\n", saved_interval_main, saved_length_main, saved_daynight_main);
    automatic_cycle_active = true;
    dayNightMistingCycle(misting_interval, misting_time, day_night_flag, misting_params_main);
    automatic_cycle_active = false;
    misting_params_main.putInt("cont_cycle", 0); //means we broke out of the cycle so dont reenter
    continue_cycle_main = 0; //just incase
  }

  else{
    Serial.println("Task is already running. Please wait until it is finished.");
    vTaskDelay(pdMS_TO_TICKS(1000)); //Ease on cpu cycles
  }
}

void task1(void *parameters){

  TaskData *task_data = (TaskData *)parameters; //Casting back to our original type
  if (task_data->task_chosen == PRESSURE_CALIBRATION && task_data->task_time > 0) {
    Serial.printf("Pressure calibration starts in %d ms.\n", task_data->task_time);
    vTaskDelay(pdMS_TO_TICKS(task_data->task_time));
  }

  int parser_response = taskRunner(task_data->task_chosen, task_data->task_time, 0, 0, false, misting_params_main); //Calling the task we want performed.

  if(parser_response == 1){
    Serial.println("Successful run.Closing RTOS Task.\n");
  }

  else{
    Serial.println("Failed run. Closing RTOS Task.\n");
    sendGuiCommandAck(false, "Task failed.");
  }

  task_data->task_state = false;

  vTaskDelete(NULL); //delete task after we are done with it
  
}

// Task that perpetually calculates pressure in psi
void calculatePressureTask(void *parameters){

  for(;;){

    //Calculate Pressure
    int success = calculatePressure();
    
  }

}

void guiTelemetryTask(void *parameters) {
  Preferences telemetry_preferences;
  if (!telemetry_preferences.begin("mstng_prms", true)) {
    Serial.println("Failed to open saved parameters for GUI telemetry.");
    vTaskDelete(NULL);
    return;
  }

  for (;;) {
    GuiTelemetryData telemetry;
    const int saved_day_night = telemetry_preferences.getInt("day_night", 1);
    const bool automatic_cycle_running = automatic_cycle_active || continue_cycle_main == 1;

    telemetry.status = (task1_parameters.task_state || automatic_cycle_running) ? "Running" : "Idle";
    telemetry.mode = automatic_cycle_running ? "Auto" : "Manual";
    telemetry.adc = pressureData.currentPressure_ADC.get();
    telemetry.psi = getPressureInPSI();
    telemetry.mistingIntervalMs = misting_interval;
    telemetry.mistingDurationMs = misting_time;
    telemetry.solenoid1Open = digitalRead(2) == HIGH;
    telemetry.solenoid2Open = digitalRead(41) == HIGH;
    telemetry.solenoid3Open = digitalRead(42) == HIGH;
    telemetry.solenoid4Open = digitalRead(45) == HIGH;
    telemetry.cycle = day_night_flag ? "Day" : "Night";
    telemetry.pumpOn = digitalRead(1) == HIGH;
    telemetry.savedMistingIntervalMs = telemetry_preferences.getInt("mst_ntrvl", 300000);
    telemetry.savedMistingDurationMs = telemetry_preferences.getInt("mst_lngth", 5000);
    telemetry.savedMode = "N/A";
    telemetry.savedCycle = saved_day_night == 0 ? "Day" : "Night";
    telemetry.savedMinimumPsi = pressureData.psi_minPressure;
    telemetry.savedMaximumPsi = pressureData.psi_maxPressure;
    telemetry.savedMinimumPsiAdc = pressureData.adc_minPressure;
    telemetry.savedMaximumPsiAdc = pressureData.adc_maxPressure;

    sendGuiTelemetry(telemetry);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void serialCommandTask(void *parameters) {
  Preferences commandPreferences;
  if (!commandPreferences.begin("mstng_prms", false)) {
    Serial.println("Failed to open saved parameters for GUI commands.");
    vTaskDelete(NULL);
    return;
  }

  char line[384];
  size_t lineLength = 0;
  bool discardLine = false;

  for (;;) {
    while (Serial.available() > 0) {
      const int incoming = Serial.read();
      if (incoming < 0) {
        break;
      }

      if (incoming == '\r') {
        continue;
      }

      if (incoming == '\n') {
        if (discardLine) {
          sendGuiCommandAck(false, "JSON command is too long.");
        } else if (lineLength > 0) {
          line[lineLength] = '\0';
          GuiCommand command;
          const GuiCommandParseResult result = parseGuiCommand(line, command);
          if (result == GuiCommandParseResult::Success) {
            processGuiCommand(command, commandPreferences);
          } else if (result == GuiCommandParseResult::InvalidJson) {
            sendGuiCommandAck(false, "Invalid JSON.");
          } else if (result == GuiCommandParseResult::InvalidSettings) {
            sendGuiCommandAck(false, "Command settings are invalid or out of range.");
          } else {
            sendGuiCommandAck(false, "Unsupported command.");
          }
        }

        lineLength = 0;
        discardLine = false;
      } else if (!discardLine) {
        if (lineLength < sizeof(line) - 1) {
          line[lineLength++] = static_cast<char>(incoming);
        } else {
          lineLength = 0;
          discardLine = true;
        }
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void processGuiCommand(const GuiCommand &command, Preferences &commandPreferences) {
  if (command.type == GuiCommandType::UpdateAutoMode) {
    if (command.saveToMemory) {
      const size_t intervalBytes = commandPreferences.putInt("mst_ntrvl", command.mistingIntervalMs);
      const size_t durationBytes = commandPreferences.putInt("mst_lngth", command.mistingDurationMs);
      const size_t cycleBytes = commandPreferences.putInt("day_night", command.dayCycle ? 0 : 1);
      if (intervalBytes != sizeof(int32_t) ||
          durationBytes != sizeof(int32_t) ||
          cycleBytes != sizeof(int32_t)) {
        sendGuiCommandAck(false, "Could not save auto-mode settings.");
        return;
      }

      saved_interval_main = command.mistingIntervalMs;
      saved_length_main = command.mistingDurationMs;
      saved_daynight_main = command.dayCycle ? 0 : 1;
    }

    misting_interval = command.mistingIntervalMs;
    misting_time = command.mistingDurationMs;
    day_night_flag = command.dayCycle;
    sendGuiCommandAck(true, "Auto-mode settings updated.");
    return;
  }

  if (start_button_active || automatic_cycle_active || continue_cycle_main == 1) {
    sendGuiCommandAck(false, "System is busy.");
    return;
  }

  bool started = false;
  const char *response = "Task accepted.";
  if (command.type == GuiCommandType::OpenSolenoid) {
    started = startGuiTask(OPEN_SOLENOID_1 + command.solenoid - 1, command.runTimeMs);
  } else if (command.type == GuiCommandType::RunMotor) {
    started = startGuiTask(START_MOTOR, command.runTimeMs);
  } else if (command.type == GuiCommandType::CalibratePressure) {
    started = startGuiTask(PRESSURE_CALIBRATION, command.calibrationStartDelayMs);
    response = "Calibration queued.";
  }

  sendGuiCommandAck(started, started ? response : "System is busy or could not start the task.");
}

bool startGuiTask(int taskChosen, int taskTime) {
  if (start_button_active || automatic_cycle_active ||
      continue_cycle_main == 1 || task1_parameters.task_state) {
    return false;
  }

  task1_parameters.task_chosen = taskChosen;
  task1_parameters.task_time = taskTime;
  task1_parameters.task_state = true;

  if (xTaskCreate(task1, "GUI Task", 4096, &task1_parameters, 5, NULL) != pdPASS) {
    task1_parameters.task_state = false;
    return false;
  }

  return true;
}

