#include <Arduino.h>

// Define ADC Pin for the pressure pump
#define TRANSDUCER_PIN 10


uint32_t readTransducer(int pin) {

  uint32_t pressure = analogRead(pin);
  
  return pressure;
}