#include <Arduino.h>
#include "solenoids.h"

// Defining solenoids and their GPIO pins they are connected to.
#define SOLENOID_1 2
#define SOLENOID_2 41
#define SOLENOID_3 42
#define SOLENOID_4 45

int misting_delay = 1000; //this is the delay needed for the pressure to build up at the nozzle.

void initSolenoids(){

  // Setting pinmode of solenoid pins
  pinMode(SOLENOID_1, OUTPUT);
  pinMode(SOLENOID_2, OUTPUT);
  pinMode(SOLENOID_3, OUTPUT);
  pinMode(SOLENOID_4, OUTPUT);

  digitalWrite(SOLENOID_1, LOW);
  digitalWrite(SOLENOID_2, LOW);
  digitalWrite(SOLENOID_3, LOW);
  digitalWrite(SOLENOID_4, LOW);

}


// Function to set individual solenoids with no timer
void setSolenoid_nodelay(int solenoid){

  switch(solenoid)
  {
  case 1:
    solenoid = SOLENOID_1;
    break;

  case 2:
    solenoid = SOLENOID_2;
    break;

  case 3:
    solenoid = SOLENOID_3;
    break;

  case 4:
    solenoid = SOLENOID_4;
    break;
  
  default:
    Serial.println("Solenoid does not exist.");
    break;
  }

  digitalWrite(solenoid, HIGH);

}


// Function to set individual solenoids with a timer
void setSolenoid(int solenoid, int time_delay){

  switch(solenoid)
  {
  case 1:
    solenoid = SOLENOID_1;
    break;

  case 2:
    solenoid = SOLENOID_2;
    break;

  case 3:
    solenoid = SOLENOID_3;
    break;

  case 4:
    solenoid = SOLENOID_4;
    break;
  
  default:
    Serial.println("Solenoid does not exist.");
    break;
  }

  digitalWrite(solenoid, HIGH);
  Serial.printf("Solenoid %d is on.\n", solenoid);
  vTaskDelay(pdMS_TO_TICKS(time_delay));
  digitalWrite(solenoid, LOW);
  Serial.printf("Solenoid %d is off.\n", solenoid);

}

// Activate All solenoids at once
void activateAllSolenoids(int time){
  
  digitalWrite(SOLENOID_1, HIGH);
  digitalWrite(SOLENOID_2, HIGH);
  digitalWrite(SOLENOID_3, HIGH);
  digitalWrite(SOLENOID_4, HIGH);

  vTaskDelay(pdMS_TO_TICKS(time));

  digitalWrite(SOLENOID_1, LOW);
  digitalWrite(SOLENOID_2, LOW);
  digitalWrite(SOLENOID_3, LOW);
  digitalWrite(SOLENOID_4, LOW);

}

// Activate Solenoids 1 at a time
void activate1by1Solenoids(int time){
  
  setSolenoid(SOLENOID_1, time);
  setSolenoid(SOLENOID_2, time);
  setSolenoid(SOLENOID_3, time);
  setSolenoid(SOLENOID_4, time);

}

void deactivateAllSolenoids(){

  digitalWrite(SOLENOID_1, LOW);
  digitalWrite(SOLENOID_2, LOW);
  digitalWrite(SOLENOID_3, LOW);
  digitalWrite(SOLENOID_4, LOW);


}