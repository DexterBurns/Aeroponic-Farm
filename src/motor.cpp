#include <Arduino.h>
#include <ptransducer.h>

// Defining the relay for the motor, on relay 1
#define MOTOR_RELAY 1

void initMotor(){

  // Setting pinmode of motor relay 
  pinMode(MOTOR_RELAY, OUTPUT);

}

void startMotor(){

    digitalWrite(MOTOR_RELAY, HIGH);
}

void stopMotor(){
    digitalWrite(MOTOR_RELAY, LOW);
}

// Activate the motor for a certain amount of time 
void activateMotorWithDelay(int time_ms){

  startMotor();

  delay(time_ms);

  stopMotor();
}


