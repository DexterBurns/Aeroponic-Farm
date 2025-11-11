#include <Arduino.h>
#include "solenoids.h"

// Defining solenoids and their GPIO pins they are connected to.
#define SOLENOID_1 2
#define SOLENOID_2 41
#define SOLENOID_3 42
#define SOLENOID_4 45

void initSolenoids(){

    // Setting pinmode of solenoid pins
    pinMode(SOLENOID_1, OUTPUT);
    pinMode(SOLENOID_2, OUTPUT);
    pinMode(SOLENOID_3, OUTPUT);
    pinMode(SOLENOID_4, OUTPUT);

}


// Function to set individual solenoids with a timer
void setSolenoid(int solenoid, int time_delay){

  digitalWrite(solenoid, HIGH);
  Serial.printf("Solenoid %d is on.\n", solenoid);
  delay(time_delay);
  digitalWrite(solenoid, LOW);
  Serial.printf("Solenoid %d is off.\n", solenoid);
  delay(time_delay);

}

// Activate All solenoids at once
void activateAllSolenoids(int time){
  
  digitalWrite(SOLENOID_1, HIGH);
  digitalWrite(SOLENOID_2, HIGH);
  digitalWrite(SOLENOID_3, HIGH);
  digitalWrite(SOLENOID_4, HIGH);

  delay(time);

  digitalWrite(SOLENOID_1, LOW);
  digitalWrite(SOLENOID_2, LOW);
  digitalWrite(SOLENOID_3, LOW);
  digitalWrite(SOLENOID_4, LOW);

}

// Activate Solenoids 1 at a time
void activate1by1Solenoids(int time){
  
    setSolenoid(SOLENOID_1, default_time);
    setSolenoid(SOLENOID_2, default_time);
    setSolenoid(SOLENOID_3, default_time);
    setSolenoid(SOLENOID_4, default_time);

}

