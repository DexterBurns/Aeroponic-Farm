#include <Arduino.h>
#include <movingAvg.h>
#include "ptransducer.h"
#include "motor.h"
#include "button.h"
#include "solenoids.h"

// Define ADC Pin for the pressure pump
#define TRANSDUCER_PIN 10

// Upper and lower limit for pressure in the system
#define PSI_UPPER_LIMIT 90
#define PSI_LOWER_LIMIT 80

int off_duration_scale = 7;

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

    // while we are under the duration limit. keep the motor on, and listen for the signal to turn it off
    while( millis() - start_time < (unsigned long)duration){
        
        // Keep the motor running
        startMotor();
        Serial.println("Don't forget to press button for emergency stop!\n");
        
        // Check pressure to make sure it doesnt overpressurize. Add this later.

        // Listen for button press to interrupt the pressurization
        if(readButton() == LOW){
            stopMotor();
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(100));

    }

}

/* Goal of this function is to have the system repressurize itself repeatedly 
so that we can read the manual pressure and associate it with a digital value in our code. 
1. Turn motor on until button press, to get to the pressure we want.
2. Motor will turn on for x seconds, and then stay off for x*8 seconds. (number is arbitrary)
3. Print to serial what the digital pressure transducer is reading, during all of this. */
void  config_pTransducer_debug(int on_duration){

    int off_duration = on_duration*off_duration_scale;
    // converting ms to seconds, just for ease of use
    int on_duration_sec = on_duration/1000;
    int off_duration_sec = (on_duration*off_duration_scale)/1000;
    bool f_flag = true; //flag that breaks the loop
    bool motor_on_flag = true;
    int count = 0;
    int delay = 100; //how much we want the system to delay every cycle

    while(f_flag == true){

        if(motor_on_flag == true){
            for(int count = 0; count < (on_duration/delay); count++){
                startMotor();
                int p = readTransducer();
                Serial.printf("Pressure Reading: %d\n", p);
                if(readButton() == LOW){f_flag = false; break;}
                vTaskDelay(pdMS_TO_TICKS(delay)); //delay task by 100ms to let other system functions run
            }
            motor_on_flag = false; //On cycle complete, turn motor off 
        }

        else if(motor_on_flag ==false){ //motor has finished the on cycle, now time for off cycle
            for(int count = 0; count < (off_duration/delay); count++){
                stopMotor();
                int p = readTransducer();
                Serial.printf("Pressure Reading: %d\n", p);
                if(readButton() == LOW){f_flag = false; break;}
                vTaskDelay(pdMS_TO_TICKS(delay)); //delay task by 100ms to let other system functions run
            }
            motor_on_flag = true; //off-cycle complete, time to turn motor back on
        }

        else if(f_flag == false){
            f_flag = false;
            Serial.printf("Transducer Debug Stopped. All solenoids will now be opened.\n");
            break; 
        }

    }
    //Don't forget, if we break, we must let the pressure in the system out.
    activateAllSolenoids(10000);
}

// Add function that interrupts everything to repressurize system again

// Add function logic that detects when the pressure is under a certain amount for too long we send an error.