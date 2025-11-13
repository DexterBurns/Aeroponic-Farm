#include <Arduino.h>
#include "button.h"
#include "motor.h"
#include "ptransducer.h"
#include "serialComms.h"
#include "tasks.h"

// Define buzzer pin
#define BUZZER_PIN 21

bool motor_flag = false;
uint32_t curr_pressure = 0;

int default_time = 2000; //placeholder of 2 seconds for anything that might need a timer 

TaskHandle_t task1_handle = NULL; // Handler for task 

// put function declarations here:
void setSolenoid(int, int);
void activateMotor(int, int);
void activateAllSolenoids(int);
void activate1by1Solenoids(int);
bool getStartState(int);

// variables related RTOS Task1
void task1(void *parameter);
bool task1_flag = false;
struct TaskData {
  int task_chosen = 0;
  int task_time = 0;
  bool task_state = false;
} task1_parameters;

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

  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 1000);
  delay(250);
  noTone(BUZZER_PIN);
}

// Keep in mind, all functions will be run via FreeRTOS. 
void loop() {

  //If start pin is LOW (button pressed) we can start the system. 
  if (readButton() == LOW){

    Serial.printf("Start button has been pressed. State: %d\n", readButton()); //ONce start button has been pressed.
    int task_msg, task_time = readFromSerial(); //Get the task and task time 
    int runnerResult = taskParser(task_msg, task_time); //Get which task it is we're asking for.

    //Command picked was successful
    if(runnerResult == 1){
      //Run the RTOS node and task with that node.

      //flag to make sure we dont run the task twice
      if(task1_parameters.task_state == false){
        //Creating a separate FreeRTOS task to run the task for the system we want. 
        xTaskCreate(task1, "Task 1", 2048, &task1_parameters, 5, NULL);

      }
      
    }

    delay(2000); //Delay to let the system settle
  }
  
  //If start pin in HIGH (button is unpressed) we loop until it is
  else if (readButton() == HIGH)
  {
    bool start_state = readButton();
    Serial.printf("Start Button State: %d\n", start_state);
  }


}

void task1(void *parameters){

  TaskData *task_data = (TaskData *)parameters; //Casting back to our original type
  int parser_response = taskParser(task_data->task_chosen, task_data->task_time); //Calling the task we want performed.

  if(parser_response == 1){
    Serial.println("Successful run.Closing RTOS Task.\n");
  }

  else{
    Serial.println("Failed run. Closing RTOS Task.\n");
  }
  
}

