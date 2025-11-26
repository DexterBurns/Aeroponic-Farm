#include <Arduino.h>
#include "button.h"
#include "motor.h"
#include "ptransducer.h"
#include "serialComms.h"
#include "solenoids.h"
#include "tasks.h"
#include "scheduler.h"

// Define buzzer pin
#define BUZZER_PIN 21

bool motor_flag = false;
uint32_t curr_pressure = 0;

TaskHandle_t task1_handle = NULL; // Handler for task 

bool state_flag = false; //for the button state message

// put function declarations here:
void setSolenoid(int, int);
void activateMotor(int, int);
void activateAllSolenoids(int);
void activate1by1Solenoids(int);
bool getStartState(int);
int state_count = 0;
bool bttn_state = HIGH; // Initial state of button

// variables related RTOS Task1
void task1(void *parameter);
bool task1_flag = false;
struct TaskData {
  int task_chosen = 0;
  int task_time = 0;
  bool task_state = false;
} task1_parameters;

int misting_interval = 300000; //5 minutes in milliseconds
int misting_time = 5000; //5 seconds
bool day_night_flag;


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
  tone(BUZZER_PIN, 1000);
  delay(250);
  noTone(BUZZER_PIN);

  // Create RTOS Task that will constantly calculate and monitor pressure
  xTaskCreate(calculatePressureTask, "Pressure Calc", 2048, NULL, 4, NULL);
  
}

// Keep in mind, all functions will be run via FreeRTOS. 
void loop() {

  bttn_state = readButton();
  //If start pin is LOW (button pressed) we can start the system. 
  if (bttn_state == LOW){

    Serial.printf("Start button has been pressed. State: %d\n", bttn_state); //ONce start button has been pressed.
    int task_msg;
    int task_time;
    int comm_result = 0;
    comm_result = readFromSerial(task_msg, task_time); //Get the task and task time 
    int runnerResult = taskParser(task_msg); //Get which task it is we're asking for.

    if (comm_result == -1){ //message failed
      Serial.printf("Comms have failed, returning back to wait mode.");
    }
  
    //Command picked was successful and comms successful
    else if(runnerResult == 1 && comm_result != -1){
      
      //Setting task parameters 
      task1_parameters.task_chosen = task_msg;
      task1_parameters.task_time = task_time;

      //flag to make sure we dont run the task twice
      if(task1_parameters.task_state == false){
        //Creating a separate FreeRTOS task to run the task for the system we want. 
        xTaskCreate(task1, "Task 1", 4096, &task1_parameters, 5, NULL);
        task1_parameters.task_state = true;

      }

      else if(task1_parameters.task_state == true){
        Serial.printf("Task Still running. Please Wait");
      }
      
    }

    else if(runnerResult == 8){ //we know the day/night task was requested. get more information
        readFromSerialforDayandNightParameters(misting_interval, misting_time, day_night_flag);
        dayNightMistingCycle(misting_interval, misting_time, day_night_flag);
    }

    else{
      Serial.printf("Comms Unuseccessful. Restarting.");
      vTaskDelay(pdMS_TO_TICKS(5000)); //Ease on cpu cycles
    }

    state_flag = false; //We can print the button state again now.

  }
  
  //If start pin in HIGH (button is unpressed) we loop until it is
  else if (bttn_state == HIGH)
  {
    
      Serial.printf("Start Button State: %d\n", bttn_state);
      state_flag = true;
      int pressure_reading = getPressureInPSI(); //might be 0 at start
      Serial.printf("Pressure Reading: %d\n", pressure_reading);
      vTaskDelay(pdMS_TO_TICKS(500));

  }
}

void task1(void *parameters){

  TaskData *task_data = (TaskData *)parameters; //Casting back to our original type
  int parser_response = taskRunner(task_data->task_chosen, task_data->task_time); //Calling the task we want performed.

  if(parser_response == 1){
    Serial.println("Successful run.Closing RTOS Task.\n");
  }

  else{
    Serial.println("Failed run. Closing RTOS Task.\n");
  }

  task_data->task_state = false;

  vTaskDelete(NULL); //delete task after we are done with it
  
}

// Task that perpetually calculates pressure in psi
void calculatePressureTask(){

  for(;;){

    //Calculate Pressure
    int success = calculatePressure();
    
  }

}

