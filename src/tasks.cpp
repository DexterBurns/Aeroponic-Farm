#include <Arduino.h>
#include "tasks.h"
#include "solenoids.h"
#include "motor.h"
#include "button.h"
#include "ptransducer.h"


// Function to figure out which task was requested and running it.
int taskRunner(int chosen_task, int task_time){

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
        
        default:
            // code block
            Serial.println("No matching command. Try again.\n");
            return -1;
        }
        Serial.println("Task Ran Successfully.\n");
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
        
        default:
            return -1;
        }
        Serial.println("Task Picked Successfully.\n");
        return 1;
}