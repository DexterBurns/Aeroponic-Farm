#include <Arduino.h>
#include "tasks.h"
#include "solenoids.h"
#include "motor.h"
#include "button.h"
#include "ptransducer.h"
#include "scheduler.h"

// Define buzzer pin
#define BUZZER_PIN 21

// Upper and lower limit for pressure in the system
#define PSI_UPPER_LIMIT 100
#define PSI_LOWER_LIMIT 75
int night_scale = 2; //What to scale misting times by when it is night time.
int delay_time = 50; //Delay time

/*Function to run a full cycle depending on day/night flag, misting time length, cycle time */
int dayNightMistingCycle(int misting_interval, int misting_length, bool day_night_flag){ //True for Day, False for Night

    unsigned long start_time = millis(); // Start time
    bool cycle_flag = true; //flag that breaks us out of the cycle function if needed.
    while(cycle_flag == true){
            
        checkPressureAndRepressurize();

        //If we are in a day cycle, mist if we passed our misting interval
        if(day_night_flag == true){

            Serial.println("Starting Daytime misting Cycle.");
            buzzerAlert(3);
            //If the day interval time has passed, then mist and wait the interval time in milliseconds
            setSolenoid(1, misting_length);
            checkPressureAndRepressurize();
            vTaskDelay(pdMS_TO_TICKS(3000));//Small delay to let pressure stabilize

            setSolenoid(2, misting_length);
            checkPressureAndRepressurize();
            vTaskDelay(pdMS_TO_TICKS(3000));//Small delay to let pressure stabilize

            setSolenoid(3, misting_length);
            checkPressureAndRepressurize();
            vTaskDelay(pdMS_TO_TICKS(3000));//Small delay to let pressure stabilize

            setSolenoid(4, misting_length);
            checkPressureAndRepressurize();

            Serial.println("Misting Complete, now in daytime wait period. Press Button to leave, if needed.");
            for(int x = 0; x <= misting_interval/delay_time; x++){

                vTaskDelay(pdMS_TO_TICKS(delay_time));
                bool button = readButton();

                if (button == LOW){// break if the button was pressed
                    Serial.println("Button pressed, leaving Day/Night Program");
                    cycle_flag = false;
                    break;
                }

            }
            
            start_time = millis();
        }

        //If we are in a day cycle, mist if we passed our misting interval
        else if(day_night_flag == false){

            buzzerAlert(3);
            Serial.println("Starting Nighttime misting Cycle.");

            //If the day interval time has passed, then mist and wait the interval time in milliseconds
            setSolenoid(1, misting_length);
            checkPressureAndRepressurize();
            vTaskDelay(pdMS_TO_TICKS(3000));//Small delay to let pressure stabilize

            setSolenoid(2, misting_length);
            checkPressureAndRepressurize();
            vTaskDelay(pdMS_TO_TICKS(3000));//Small delay to let pressure stabilize

            setSolenoid(3, misting_length);
            checkPressureAndRepressurize();
            vTaskDelay(pdMS_TO_TICKS(3000));//Small delay to let pressure stabilize

            setSolenoid(4, misting_length);
            checkPressureAndRepressurize();

            Serial.println("Misting Complete, now in nighttime wait period. Press Button to leave, if needed.");
            for(int x = 0; x <= (misting_interval/delay_time)*night_scale; x++){

                vTaskDelay(pdMS_TO_TICKS(delay_time));
                bool button = readButton();

                if (button == LOW){// break if the button was pressed
                    Serial.println("Button pressed, leaving Day/Night Program");
                    cycle_flag = false;
                    break;
                }

            }
            
            start_time = millis();
            
            
        }

        vTaskDelay(pdMS_TO_TICKS(10000)); //Let user see message in time
        return 1;
    }
}

void checkPressureAndRepressurize(){
    if(pressureData.currentPressure_PSI.get() <= PSI_LOWER_LIMIT){
        //repressurize function
        pressurizeToMaxPSI();
        Serial.println("Max Pressure Reached!\n");
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