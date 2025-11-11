#include <Arduino.h>
#include "string"

/* All communication related functionality will be happening in here. It will also include the 
table for all the meanings of the functions that I'd like to happen during testing. Some tasks will also be run from here. */

// Delimeter for the text received
char *delimiter = ",";
int task_value;
int task_time; 


void serialCommsInit(){

    Serial.begin(115200);
    delay(200);                         // give USB/UART a moment
    Serial.println("Serial Comms Initialized"); 

}

// Function to read our command from the serial monitor. 
int readAndRunCommand(){

    // Find a way to parse. Maybe do messages separately.
    Serial.println("Enter the task you want to perform: \n");
    task_value = Serial.read();

    Serial.println("Enter the time in seconds you'd like to perform it: \n");
    task_time = Serial.read();

    return task_value, task_time;

}



