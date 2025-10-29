#include <Arduino.h>
#define START_PIN 9

// Defining solenoids and their GPIO pins they are connected to.
#define SOLENOID_1 2
#define SOLENOID_2 41
#define SOLENOID_3 42
#define SOLENOID_4 45

// Defining the relay for the motor, on relay 1
#define MOTOR_RELAY 1

// Define buzzer pin
#define BUZZER_PIN 21

// Define ADC Pin for the pressure pump
#define PRESSURE_PIN 10

bool motor_flag = false;
uint32_t curr_pressure = 0;

// put function declarations here:
int readPressure(int);
void setSolenoid(int);
void activateMotor(int, int);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  analogReadResolution(12);
  // set ADC pin 35 attenuation. Voltage after the divider should be around 2.5 volts
  analogSetPinAttenuation(START_PIN, ADC_6db);

  //Setting pinmode of Start pin
  pinMode(START_PIN, INPUT);

  // Setting pinmode of solenoid pins
  pinMode(SOLENOID_1, OUTPUT);
  pinMode(SOLENOID_2, OUTPUT);
  pinMode(SOLENOID_3, OUTPUT);
  pinMode(SOLENOID_4, OUTPUT);

  // Setting pinmode of motor relay 
  pinMode(MOTOR_RELAY, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 1000);
  delay(250);
  noTone(BUZZER_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //If start pin is LOW (button unpresses) we can start the system. 
  if (digitalRead(START_PIN) == LOW){

    activateMotor(MOTOR_RELAY, 5000);

    /* setSolenoid(SOLENOID_1);
    setSolenoid(SOLENOID_2);
    setSolenoid(SOLENOID_3);
    setSolenoid(SOLENOID_4); */

  }
  
  //If start pin in HIGH (button is pressed) we can start the system
  else if (digitalRead(START_PIN) == HIGH)
  {
    ;
  }


}

// Activate the motor for a certain amount of time 
void activateMotor(int motorpin, int time_ms){

  digitalWrite(motorpin, HIGH);

  delay(time_ms);

  digitalWrite(motorpin, LOW);
}

void setSolenoid(int solenoid){

  digitalWrite(solenoid, HIGH);
  Serial.printf("Solenoid %d is on.", solenoid);
  delay(2000);
  digitalWrite(solenoid, LOW);
  Serial.printf("Solenoid %d is off.", solenoid);

}

// put function definitions here:
int readPressure(int pin) {

  uint32_t pressure = 0;

  pressure = analogRead(pin);
  
  return pressure;
}