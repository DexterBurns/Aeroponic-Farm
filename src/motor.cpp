#include <Arduino.h>
#include <ptransducer.h>

// Defining the relay for the motor, on relay 1
#define MOTOR_RELAY 1

void initMotor(){

  // Setting pinmode of motor relay 
  pinMode(MOTOR_RELAY, OUTPUT);
  digitalWrite(MOTOR_RELAY, LOW);

}

void startMotor(){

    digitalWrite(MOTOR_RELAY, HIGH);
}

void stopMotor(){
    digitalWrite(MOTOR_RELAY, LOW);
}

// Activate the motor for a certain amount of time 
void activateMotorWithDelay(int time_ms){

  // get time started
  unsigned long start_time = millis();
  unsigned long stop_time = start_time + time_ms;

  startMotor();
  vTaskDelay(pdMS_TO_TICKS(time_ms)); // Delaying task using RTOS. Saves CPU cycles
  stopMotor();

}

// Activate motor with delay using millis()






