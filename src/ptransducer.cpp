#include <Arduino.h>
#include <movingAvg.h>
#include "ptransducer.h"
#include "motor.h"
#include "button.h"

// Define ADC Pin for the pressure pump
#define TRANSDUCER_PIN 10

// Upper and lower limit for pressure in the system
#define PSI_UPPER_LIMIT 90
#define PSI_LOWER_LIMIT 80

// Object to hold the moving average
movingAvg avgPressure(10);

void initTransducer(){

    analogReadResolution(12);
    // set ADC pin 35 attenuation. Voltage after the divider should be around 2.5 volts
    analogSetPinAttenuation(TRANSDUCER_PIN, ADC_6db);

    //Setting pinmode of pressure pin
    pinMode(TRANSDUCER_PIN, INPUT);

}

// Function to read transducer value
int readTransducer(int pin) {

    int pressure = analogRead(pin);

    return pressure;

}

// Function to call that will always update the moving average
int avgdTransducerReadings(){
    int current_pressure = avgPressure.reading(readTransducer(TRANSDUCER_PIN));
    return current_pressure;
}

// Function to send pressure data to seral monitor

// Debug function to run the motor to get to the system's upper limit. 
/* This function needs to keep running the motor until we get to the upper limit. Once we are there, stop the motor. 
We should be outputting the readings to the serial monitor as well. So we can know exactly how actual psi compares to readings in digital values.*/

void testPSILimits_debug(unsigned long start_time, int duration){

    // Convert seconds to milliseconds
    duration = duration*1000;

    // while we are under the duration limit. keep the motor on, and listen for the signal to turn it off
    while( millis() < (start_time + duration) ){
        
        // Keep the motor running
        startMotor();
        
        // Check pressure to make sure it doesnt overpressurize. Add this later.
        
        // Listen for button press to interrupt the pressurization
        if(readButton() == LOW){
            stopMotor();
            break;
        }

    }

}


// Add function that interrupts everything to repressurize system again

// Add function logic that detects when the pressure is under a certain amount for too long we send an error.