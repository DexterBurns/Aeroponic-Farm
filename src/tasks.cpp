#include <Arduino.h>
#include "tasks.h"
#include "solenoids.h"
#include "motor.h"
#include "button.h"
#include "ptransducer.h"
#include "scheduler.h"


// Function to figure out which task was requested and running it.
int taskRunner(int chosen_task, int task_time, int misting_interval, int misting_length, bool day_night_flag){

    switch(chosen_task) {
        case OPEN_ALL_SOLENOIDS:
            // code block
            Serial.println("Open all solenoids chosen.\n");
            activateAllSolenoids(task_time);
            break;

        case OPEN_SOLENOID_1:
            // code block
            Serial.println("Open solenoid 1 chosen.\n");
            setSolenoid(1,task_time);
            break;

        case OPEN_SOLENOID_2:
            Serial.println("Open solenoid 2 chosen.\n");
            setSolenoid(2,task_time);
            break;

        case OPEN_SOLENOID_3:
            Serial.println("Open solenoid 3 chosen.\n");
            setSolenoid(3,task_time);
            break;

        case OPEN_SOLENOID_4:
            Serial.println("Open solenoid 4 chosen.\n");
            setSolenoid(4,task_time);
            break;

        case START_MOTOR:
            Serial.println("Start Motor chosen.\n");
            activateMotorWithDelay(task_time);
            break;

        case MAX_PSI_TEST:
            Serial.println("Max PSI pressure test chosen.\n");
            config_pTransducer_debug(task_time);
            break;

        case PRESSURE_CALIBRATION:
            Serial.println("Pressure Calibration test chosen.\n");
            findPressureRangeWithADC();
            break;

        case DAY_NIGHT_CYCLE:
            Serial.println("DAY/NIGHT Cycle Function chosen.\n");
            dayNightMistingCycle(misting_interval, misting_length, day_night_flag);
            break;
        
        default:
            // code block
            Serial.println("No matching command. Try again.\n");
            return -1;
        }
        Serial.println("Task Ran Successfully.\n");
        vTaskDelay(pdMS_TO_TICKS(500)); //Ease on cpu cycles
        return 1;
}

// Function to figure out which task was requested
int taskParser(int chosen_task){

    switch(chosen_task) {
        case OPEN_ALL_SOLENOIDS:
            break;

        case OPEN_SOLENOID_1:
            break;

        case OPEN_SOLENOID_2:
            break;

        case OPEN_SOLENOID_3:
            break;

        case OPEN_SOLENOID_4:
            break;

        case START_MOTOR:
            break;

        case MAX_PSI_TEST:
            break;

        case PRESSURE_CALIBRATION:
            break;

        case DAY_NIGHT_CYCLE:
            return 8; //return this so we know to ask for more information
            break;
        
        default:
            return -1;
        }
        Serial.println("Task Picked Successfully.\n");
        return 1;
}