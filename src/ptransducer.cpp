#include <Arduino.h>
#include <movingAvg.h>
#include "ptransducer.h"
#include "motor.h"
#include "button.h"
#include "solenoids.h"
#include <Preferences.h>
#include "Smoothed.h" // Sensor smoothing library

// Define ADC Pin for the pressure pump
#define TRANSDUCER_PIN 10

/* PREFS, saved memory, be careful editing this!*/
// Save min ADC pressure value, max ADC pressure value

int off_duration_scale = 7; // Scale for repressurizing debug function

// loading and setup for saving the min and max pressures calculated for 
Preferences save_data;

/* Smoothing variable */
Smoothed <int> p_transducer_ADC;

// Scope to constructor, and define variables
pressureStruct::pressureStructInit()
{
    minPressure_ADC.begin(SMOOTHED_EXPONENTIAL, 10);
    maxPressure_ADC.begin(SMOOTHED_EXPONENTIAL, 10);
    currentPressure_ADC.begin(SMOOTHED_EXPONENTIAL, 10);
    currentPressure_PSI.begin(SMOOTHED_EXPONENTIAL, 10);

    // See if there is a previous value for the ADC's minimum and maximum pressures
    int adc_minPressure_hold = save_data.getInt("minADC", -1); 
    int adc_maxPressure_hold = save_data.getInt("maxADC", -1);

    if(adc_minPressure_hold != -1){ //if the value does exist, replace the minPressure with it
        adc_minPressure = adc_minPressure_hold;
    }

    if(adc_maxPressure_hold != -1){ //if the value does exist, replace the maxPressure with it
        adc_maxPressure = adc_maxPressure_hold;
    }

}
// Make the global instance volatile
pressureStruct pressureData;

// Initialize transducer
void initTransducer(){

    analogReadResolution(12);
    // set ADC pin 35 attenuation. Voltage after the divider should be around 2.5 volts
    analogSetPinAttenuation(TRANSDUCER_PIN, ADC_6db);

    //Setting pinmode of pressure pin
    pinMode(TRANSDUCER_PIN, INPUT);

    save_data.begin("press_minmax", false); //activates the namespace and inits it

    /* Calling Data from memory based on last min and max pressure saved.*/
    int adc_minPressure_hold = save_data.getInt("minADC", -1); 
    int adc_maxPressure_hold = save_data.getInt("maxADC", -1);

    Serial.printf("Loaded min ADC Pressure:%d\n", adc_minPressure_hold);
    Serial.printf("Loaded max ADC Pressure:%d\n", adc_maxPressure_hold);
    vTaskDelay(pdMS_TO_TICKS(5000));

    /* Initializing Pressure Structure with Smoothing */
    pressureData.pressureStructInit();

}

// Function to read transducer value. Returns ADC value
int readTransducer() {
    int pressure = analogRead(TRANSDUCER_PIN);
    return pressure;
}

// Maps the pressure read to the calibrated range. Receives an ADC value, returns psi
int pressureMap(int pressure){
    // Mapping ADC pressure to PSI pressure.
    int psi_pressure = map(pressure, pressureData.adc_minPressure, pressureData.adc_maxPressure, pressureData.psi_minPressure, pressureData.psi_maxPressure);
    //psi_pressure = constrain(psi_pressure, pressureData.psi_minPressure, pressureData.psi_maxPressure);
    return psi_pressure;
}

int getPressureInPSI(){
    int psi = pressureData.currentPressure_PSI.get();
    return psi;
}

// Function that is perpetually run by the pressure task to always have pressure calculated and ready to go.
int calculatePressure(){

    // Get pressure and feed it into exp average in pressure data 
    int pressure = readTransducer(); // get pressure in ADC value
    pressureData.currentPressure_ADC.add(pressure); //Smoothing sensor value

    // Get pressure and feed into moving PSI exponential
    int avg_pressure_in_psi = pressureMap(pressureData.currentPressure_ADC.get());
    pressureData.currentPressure_PSI.add(avg_pressure_in_psi);
    vTaskDelay(pdMS_TO_TICKS(5)); //Ease on cpu cycles
    return 1; // return psi value of the averaged pressure.
}

// Function to pressurize system to max. Not a Task, but for 24hr cycle
int pressurizeToMaxPSI(){
    
    bool state_flag = false;
    bool button_state = readButton();
    startMotor();

    Serial.println("Press button for emergency stop, if needed!\n");

    while(state_flag == false){

        button_state = readButton();
        int curr_Pressure = pressureData.currentPressure_PSI.get();

        if(curr_Pressure >= pressureData.psi_maxPressure){
            state_flag = true;
            stopMotor();
        }

        else if(button_state == LOW){
            //Stop motor
            stopMotor();
            state_flag = true;
            Serial.println("Emergency Stop Activated!\n");
            vTaskDelay(pdMS_TO_TICKS(5000)); //Waiting so user can see message
            return -1;
        }
        Serial.printf("Current Pressure: %d PSI\n", curr_Pressure);
        Serial.printf("Target Pressure: %d PSI\n", pressureData.psi_maxPressure);
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    return 1;
    
}

// Function to find the min and max pressure of the system. Should save values to prefs. 
int findPressureRangeWithADC(){

    Serial.println("Now Performing Pressure Calibration....");

    // 3 Beep alarm to signify test is starting.
    for(int x = 0; x < 3; x++){
        tone(21, 500); //never do this again. create a function next time
        vTaskDelay(pdMS_TO_TICKS(250));
        noTone(21);
        vTaskDelay(pdMS_TO_TICKS(250));
    }

    // Open the solenoids and depressurize the system for about 20 seconds
    activateAllSolenoids(20000);
    bool button_state = HIGH;

    Serial.println("Now recording Lower Pressures....");
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Record current pressure ADC value while system unpressurized. For a 10 seconds, every 50 ms. Doing for so long for the system to settle. 
    for (int x = 0; x < 200; x++){
        int pressure = readTransducer(); // I like to see the raw here
        Serial.printf("Current Unpressurized ADC Reading:%d\n", pressure);
        pressureData.minPressure_ADC.add(pressure);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    Serial.println("Now Performing Pressurization. Please press button when ready to STOP at 100PSI!");
    vTaskDelay(pdMS_TO_TICKS(5000));

    //Pressurize system and listen for button press to stop pressurizing
    while(button_state == HIGH){
        int pressure = readTransducer();
        startMotor();
        button_state = readButton();
        Serial.printf("Current Pressurizing ADC Reading:%d\n", pressure);
        vTaskDelay(pdMS_TO_TICKS(50));

        if(button_state == LOW){ //This is just to be safe...
            stopMotor();
            break;
        }
    }

    Serial.println("Pressurization complete! Now recording ADC values for 100 psi!");
    vTaskDelay(pdMS_TO_TICKS(3000));

    // Record current pressure ADC value while system unpressurized. For a 10 seconds, every 50 ms. Doing for so long for the system to settle. 
    for (int x = 0; x < 200; x++){
        int pressure = readTransducer();
        Serial.printf("Current Max Pressure ADC Reading:%d\n", pressure);
        pressureData.maxPressure_ADC.add(pressure);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    // Setting the averages we calculated to the global pressureData structure. 
    pressureData.adc_minPressure = pressureData.minPressure_ADC.get(); // Get calculated moving average
    pressureData.adc_maxPressure = pressureData.maxPressure_ADC.get(); // Get max calculated moving average

    //Also save these to prefs
    save_data.putInt("minADC", pressureData.adc_minPressure);
    save_data.putInt("maxADC", pressureData.adc_maxPressure);

    return 1;

}

/* --------------------------------------------------------------Debug Functions-----------------------------------------------------------------------------------*/

/* Functions that probably wont be used for the full system functionality, but rather during development to help facilitate the actual functions that will be used */

// Function to turn motor on and only back off when button is pressed. 
void testPSILimits_debug(unsigned long start_time, int duration){

    bool button_state = HIGH;
    // while we are under the duration limit. keep the motor on, and listen for the signal to turn it off
    while( millis() - start_time < (unsigned long)duration){
        
        // Keep the motor running
        startMotor();
        Serial.println("Don't forget to press button for emergency stop!\n");
        button_state = readButton();
        
        // Check pressure to make sure it doesnt overpressurize. Add this later.

        // Listen for button press to interrupt the pressurization
        if(button_state == LOW){
            stopMotor();
            return;
        }

        vTaskDelay(pdMS_TO_TICKS(100));

    }

}




/* Goal of this function is to have the system repressurize itself repeatedly 
so that we can read the manual pressure and associate it with a digital value in our code. 
1. Turn motor on until button press, to get to the pressure we want.
2. Motor will turn on for x seconds, and then stay off for x*8 seconds. (number is arbitrary)
3. Print to serial what the digital pressure transducer is reading, during all of this. */
// This function wqas just a safety measure during development to see how high we could
// pressurize the system without 
void  config_pTransducer_debug(int on_duration){

    int off_duration = on_duration*off_duration_scale;
    // converting ms to seconds, just for ease of use
    int on_duration_sec = on_duration/1000;
    int off_duration_sec = (on_duration*off_duration_scale)/1000;
    bool f_flag = true; //flag that breaks the loop
    bool motor_on_flag = true;
    int count = 0;
    int delay = 100; //how much we want the system to delay every cycle
    bool button_state = LOW;

    while(f_flag == true){

        if(motor_on_flag == true){
            for(int count = 0; count < (on_duration/delay); count++){
                startMotor();
                int p = readTransducer();
                Serial.printf("Pressure Reading: %d\n", p);
                button_state = readButton();
                if(button_state == LOW){f_flag = false; break;}
                vTaskDelay(pdMS_TO_TICKS(delay)); //delay task by 100ms to let other system functions run
            }
            motor_on_flag = false; //On cycle complete, turn motor off 
        }

        else if(motor_on_flag ==false){ //motor has finished the on cycle, now time for off cycle
            for(int count = 0; count < (off_duration/delay); count++){
                stopMotor();
                int p = readTransducer();
                Serial.printf("Pressure Reading: %d\n", p);
                button_state = readButton();
                if(button_state == LOW){f_flag = false; break;}
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