#include <Arduino.h>
#include "button.h"
#include "motor.h"
#include "ptransducer.h"



// Define buzzer pin
#define BUZZER_PIN 21

bool motor_flag = false;
uint32_t curr_pressure = 0;

int default_time = 2000; //placeholder of 2 seconds for anything that might need a timer 

// put function declarations here:
void setSolenoid(int, int);
void activateMotor(int, int);
void activateAllSolenoids(int);
void activate1by1Solenoids(int);
bool getStartState(int);

void taskRunner(int, int); //will run the task and for a set amount of time.


void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);                         // give USB/UART a moment
  Serial.println("BOOT");  

  //Setting pinmode of Start pin
  initButton();


  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 1000);
  delay(250);
  noTone(BUZZER_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  //If start pin is LOW (button pressed) we can start the system. 
  if (readButton() == LOW){

    Serial.printf("Start Button State: %d\n", readButton());

    activateMotorWithDelay(8000);
    //activateAllSolenoids(default_time);
    setSolenoid(SOLENOID_1, 5000);

    delay(2000); //Delay to let the system settle

  }
  
  //If start pin in HIGH (button is unpressed) we loop until it is
  else if (readButton() == HIGH)
  {
    bool start_state = readButton();
    Serial.printf("Start Button State: %d\n", start_state);
  }


}

