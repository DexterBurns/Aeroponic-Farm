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

// random needed variables
bool motor_flag = false;
uint32_t curr_pressure = 0;
bool start_flag = false;
int default_time = 2000; //placeholder of 2 seconds for anything that might need a timer 

// put function declarations here:
int readPressure(int);
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
  analogReadResolution(12);
  // set ADC pin 35 attenuation. Voltage after the divider should be around 2.5 volts
  analogSetPinAttenuation(PRESSURE_PIN, ADC_6db);

  //Setting pinmode of Start pin
  pinMode(START_PIN, INPUT_PULLUP);

  //Setting pinmode of pressure pin
  pinMode(PRESSURE_PIN, INPUT);

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
  
  bool start_state = digitalRead(START_PIN);

  //If start pin is LOW (button pressed) we can start the system. 
  if (digitalRead(START_PIN) == LOW){

    
    activateMotor(MOTOR_RELAY, 8000);
    //activateAllSolenoids(default_time);
    setSolenoid(SOLENOID_1, 5000);

    Serial.printf("Start Button State: %d\n", start_state);
 
    delay(2000); //Delay to let the system settle

  }
  
  //If start pin in HIGH (button is unpressed) we loop until it is
  else if (digitalRead(START_PIN) == HIGH)
  {
    bool start_state = digitalRead(START_PIN);
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

// Activate the motor for a certain amount of time 
void activateMotor(int motorpin, int time_ms){

  digitalWrite(motorpin, HIGH);

  delay(time_ms);

  digitalWrite(motorpin, LOW);
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
int readPressure(int pin) {

  uint32_t pressure = 0;

  pressure = analogRead(pin);
  
  return pressure;
}