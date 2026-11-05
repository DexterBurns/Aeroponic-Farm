#include <Arduino.h>
#include "tasks.h"
#include "solenoids.h"
#include "motor.h"
#include "button.h"
#include "ptransducer.h"
#include "scheduler.h"
#include <Preferences.h>

// Define buzzer pin
#define BUZZER_PIN 21

// Upper and lower limit for pressure in the system, sync between files, dont forget to change both!
#define PSI_UPPER_LIMIT 95
#define PSI_LOWER_LIMIT 75
int night_scale = 2; //What to scale misting times by when it is night time.
int delay_time = 50; //Delay time

//For holding saved data 
Preferences misting_params_sched;
int saved_interval_sched;
int saved_length_sched;
int saved_daynight_sched;

void schedulerInit(){
    // Any initialization code for the scheduler can go here
    misting_params_sched.begin("mstng_prms", false);
    saved_interval_sched = misting_params_sched.getInt("mst_ntrvl", 300000); // Default to 5 minutes if value not found
    saved_length_sched = misting_params_sched.getInt("mst_lngth", 5000); // Default to 5 seconds if value not found
    saved_daynight_sched = misting_params_sched.getInt("day_night", 1); // Default to day mode if value not found

}

/*Function to run a full cycle depending on day/night flag, misting time length, cycle time */
int dayNightMistingCycle(int misting_interval, int misting_length, bool day_night_flag, Preferences &misting_params_main){ //True for Day, False for Night

    unsigned long start_time = millis(); // Start time
    bool cycle_flag = true; //flag that breaks us out of the cycle function if needed.

    /* Variables to hold given parameters  */

    while(cycle_flag == true){
            
        checkPressureAndRepressurize();

        //If we are in a day cycle, mist if we passed our misting interval
        if(day_night_flag == true){

            Serial.println("Starting Daytime misting Cycle.");

            //Print misting parameters for debug
            Serial.printf("Misting Interval: %d ms\n", misting_interval);
            Serial.printf("Misting Length: %d ms\n", misting_length);
            Serial.printf("Day/Night Flag: %d\n", day_night_flag);
            Serial.printf("Continue Cycle after Power Loss Flag: %d\n", misting_params_main.getInt("cont_cycle", 0));
            Serial.printf("Saved Misting Interval: %d ms\n", misting_params_main.getInt("mst_ntrvl", 300000));
            Serial.printf("Saved Misting Length: %d ms\n", misting_params_main.getInt("mst_lngth", 5000));
            Serial.printf("Saved Day/Night Flag: %d\n", misting_params_main.getInt("day_night", 1));

            buzzerAlert(3);
            //If the day interval time has passed, then mist and wait the interval time in milliseconds
            setSolenoid(1, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();
            
            setSolenoid(2, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();
            
            setSolenoid(3, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();
            
            setSolenoid(4, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();

            Serial.println("Misting Complete, now in daytime wait period. Press Button to leave, if needed.");
            for(int x = 0; x <= misting_interval/delay_time; x++){

                vTaskDelay(pdMS_TO_TICKS(delay_time));
                bool button = readButton();
                Serial.println("In Daytime wait period..., press button to exit.");

                if (button == LOW){// break if the button was pressed
                    Serial.println("Button pressed, leaving Day/Night Program");
                    cycle_flag = false;
                    misting_params_main.putInt("cont_cycle", 0);
                    break;
                }

            }
            
            start_time = millis();
        }

        //If we are in a day cycle, mist if we passed our misting interval
        else if(day_night_flag == false){

            buzzerAlert(3);
            Serial.println("Starting Nighttime misting Cycle.");
            
            //Print misting parameters for debug
            Serial.printf("Misting Interval: %d ms\n", misting_interval);
            Serial.printf("Misting Length: %d ms\n", misting_length);
            Serial.printf("Day/Night Flag: %d\n", day_night_flag);
            Serial.printf("Continue Cycle after Power Loss Flag: %d\n", misting_params_main.getInt("cont_cycle", 0));
            Serial.printf("Saved Misting Interval: %d ms\n", misting_params_main.getInt("mst_ntrvl", 300000));
            Serial.printf("Saved Misting Length: %d ms\n", misting_params_main.getInt("mst_lngth", 5000));
            Serial.printf("Saved Day/Night Flag: %d\n", misting_params_main.getInt("day_night", 1));

            //If the day interval time has passed, then mist and wait the interval time in milliseconds
            setSolenoid(1, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();
            
            setSolenoid(2, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();

            setSolenoid(3, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();

            setSolenoid(4, misting_length);
            vTaskDelay(pdMS_TO_TICKS(6000));//Small delay to let pressure stabilize
            checkPressureAndRepressurize();

            Serial.println("Misting Complete, now in nighttime wait period. Press Button to leave, if needed.");
            for(int x = 0; x <= (misting_interval/delay_time)*night_scale; x++){

                vTaskDelay(pdMS_TO_TICKS(delay_time));
                bool button = readButton();
                Serial.println("In Nighttime wait period..., press button to exit.");
                if (button == LOW){// break if the button was pressed
                    Serial.println("Button pressed, leaving Day/Night Program");
                    misting_params_main.putInt("cont_cycle", 0);
                    cycle_flag = false;
                    break;
                }

            }
            
            start_time = millis();
            
            
        }

        vTaskDelay(pdMS_TO_TICKS(10000)); //Let user see message in time
        
    }
    return 1;
}

void checkPressureAndRepressurize(){

    if(pressureData.currentPressure_PSI.get() <= PSI_LOWER_LIMIT){
        //repressurize function
        int result = pressurizeToMaxPSI();
        if(result == -1){
            Serial.println("Pressurization failed due to timeout!\n");
            for(;;){
                buzzerAlert(5);
                vTaskDelay(pdMS_TO_TICKS(10000));
            } //halt system
        }
        else if(result == 0){
            Serial.println("Pressurization stopped by user!\n");
        }
        else if(result == 1){
        Serial.println("Max Pressure Reached!\n");
        }
    }
}

void buzzerAlert(int buzzes){


    for(int x; x <= buzzes; x++){
        pinMode(BUZZER_PIN, OUTPUT);
        tone(BUZZER_PIN, 1000);
        vTaskDelay(pdMS_TO_TICKS(500));
        noTone(BUZZER_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}