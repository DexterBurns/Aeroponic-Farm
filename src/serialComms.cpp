#include <Arduino.h>
#include "serialComms.h"
#include "string"

/* All communication related functionality will be happening in here. It will also include the 
table for all the meanings of the functions that I'd like to happen during testing. Some tasks will also be run from here. */

// Delimeter for the text received
char *delimiter = ",";
String task_value;
String task_time; 


void serialCommsInit(){

    Serial.begin(115200);
    delay(200);                         // give USB/UART a moment
    Serial.println("Serial Comms Initialized"); 

}

// Function to read our command from the serial monitor. 
int readFromSerial(){

    // Flags for if a msg was received and a time was received
    bool task_msg_rec = false;
    bool time_msg_rec = false;
    // Flags to track when we printed a message for enter a message and enter a time so we dont flood the serial comms
    bool task_msg_flag = false;
    bool time_msg_flag = false;

    // Keep looping until a message is received
    while(task_msg_rec == false){

        vTaskDelay(pdMS_TO_TICKS(100)); //delay to keep the system from being overworked during the loop.
        if(task_msg_flag == false){
            // Find a way to parse. Maybe do messages separately.
            Serial.println("Enter the task you want to perform: \n");
            task_msg_flag = true;
            
        }

        // If we got a message, now we ask for the time to perform the task 
        if(Serial.available() > 0 ){
            // Message is consumed, serial available should read -1 now
            task_value = Serial.readStringUntil('\n'); // Consumed message should be the task to perform
            int int_task_value = task_value.toInt();

            // Logic to check and make sure user actually wants task 0 and not accidentally
            // activating task 0 because .toInt has failed
            if (int_task_value == 0){ 
                Serial.println("Task value is 0. Did you mean to do this? 1 == yes, 0 == no: \n");
                bool user_ans = false;
                long user_resp;
                while (user_ans == false)
                {
                    vTaskDelay(pdMS_TO_TICKS(100)); //Delay to reduce system resources
                    if(Serial.available() > 0){ //If the user has responded
                        user_resp = Serial.parseInt(); //consume the response
                        if(user_resp == 1){
                            Serial.println("Alright, Continuing with task 0 then.\n"); //continue with the task, as user meant to use task 0
                            user_ans = true;
                        }
                        else{
                            Serial.println("Error occured in task parsing, user did not mean to use task 0. Returning to start state. \n");
                            return -1; 
                        }
                    }
                }
            } 

            task_msg_rec = true;

            //Once we got the task, time to ask for the time
            if(task_msg_rec == true){
                
                while(time_msg_rec == false){

                    vTaskDelay(pdMS_TO_TICKS(100)); //delay to keep the system from being overworked during the loop.

                    if(time_msg_flag == false){
                        Serial.println("Enter the time you want the task to run: \n");
                        time_msg_flag = true;
                    }

                    // If there is something in the serial buffer, means the time was sent
                    if(Serial.available() > 0 ){

                        task_time = Serial.read();
                        time_msg_rec = true;
                    }

                }

            }
        }
        
    }

    return task_msg_rec, time_msg_rec;
}



