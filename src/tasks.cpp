#include <Arduino.h>
#include "tasks.h"

// Add all new functionality to this. might be obsolete now
enum Task {
    OPEN_ALL_SOLENOIDS,
    OPEN_SOLENOID_1,
    OPEN_SOLENOID_2,
    OPEN_SOLENOID_3,
    OPEN_SOLENOID_4,
    START_MOTOR,
    MAX_PSI_TEST,
};


// Function to figure out which task was requested
int taskRunner(int chosen_task, int task_time){

    switch(chosen_task) {
        case 0:
            // code block
            Serial.println("Open all solenoids chosen.\n");
            
            break;

        case 1:
            // code block
            Serial.println("Open solenoid 1 chosen.\n");
            break;
        case 2:
            Serial.println("Open solenoid 2 chosen.\n");
            break;

        case 3:
            Serial.println("Open solenoid 3 chosen.\n");
            break;

        case 4:
            Serial.println("Open solenoid 4 chosen.\n");
            break;

        case 5:
            Serial.println("Start Motor chosen.\n");
            break;

        case 6:
            Serial.println("Max PSI pressure test chosen.\n");
            break;
        
        default:
            // code block
            Serial.println("No matching command. Try again.\n");
            return -1;
            
        }
}