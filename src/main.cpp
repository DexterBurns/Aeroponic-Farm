#include <Arduino.h>
#include "button.h"
#include "motor.h"
#include "ptransducer.h"

// Defining solenoids and their GPIO pins they are connected to.
#define SOLENOID_1 2
#define SOLENOID_2 41
#define SOLENOID_3 42
#define SOLENOID_4 45

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


void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);                         // give USB/UART a moment
  Serial.println("BOOT");  

  //Setting pinmode of Start pin
  initButton();

  // Setting pinmode of solenoid pins
  pinMode(SOLENOID_1, OUTPUT);
  pinMode(SOLENOID_2, OUTPUT);
  pinMode(SOLENOID_3, OUTPUT);
  pinMode(SOLENOID_4, OUTPUT);

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

// Activate Solenoids 1 at a time
void activate1by1Solenoids(int time){
  
    setSolenoid(SOLENOID_1, default_time);
    setSolenoid(SOLENOID_2, default_time);
    setSolenoid(SOLENOID_3, default_time);
    setSolenoid(SOLENOID_4, default_time);

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


// Function to set individual solenoids with a timer
void setSolenoid(int solenoid, int time_delay){

  digitalWrite(solenoid, HIGH);
  Serial.printf("Solenoid %d is on.\n", solenoid);
  delay(time_delay);
  digitalWrite(solenoid, LOW);
  Serial.printf("Solenoid %d is off.\n", solenoid);
  delay(time_delay);

}

// put function definitions here:
