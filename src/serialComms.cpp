#include <Arduino.h>
#include "serialComms.h"
#include "string"

/* All communication related functionality will be happening in here. It will also include the 
table for all the meanings of the functions that I'd like to happen during testing. Some tasks will also be run from here. */

// Delimeter for the text received
long task_value;
long task_time; 


void serialCommsInit(){

    Serial.begin(115200);
    delay(200);                         // give USB/UART a moment
    Serial.println("Serial Comms Initialized"); 

}

// Function to read our command from the serial monitor. 
int readFromSerial(int &msg, int &time){

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
            flushReceiveBuffer(); //Make sure buffer is clear
            Serial.println("Enter the task you want to perform:\n");
            task_msg_flag = true;
        }

        // If we got a message, now we ask for the time to perform the task 
        if(Serial.available() > 0 ){
            vTaskDelay(pdMS_TO_TICKS(1000)); 
            // Message is consumed, serial available should read -1 now
            task_value = Serial.parseInt(); // Consumed message should be the task to perform
            Serial.printf("Task received: %d\n", task_value);
            flushReceiveBuffer(); //Make sure buffer is clear

            // Logic to check and make sure user actually wants task 0 and not accidentally
            // activating task 0 because .toInt has failed
            if (task_value < 0){ 
                Serial.println("Task value is less than 0. Breaking back to start state.\n");
                return -1;
            } 

            task_msg_rec = true;

            //Once we got the task, time to ask for the time
            if(task_msg_rec == true){
                
                while(time_msg_rec == false){

                    vTaskDelay(pdMS_TO_TICKS(1000)); //delay to keep the system from being overworked during the loop.

                    if(time_msg_flag == false){
                        Serial.println("Enter the time you want the task to run: \n");
                        time_msg_flag = true;
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        
                    }

                    // If there is something in the serial buffer, means the time was sent
                    if(Serial.available() > 0 ){
                        task_time = Serial.parseInt();
                        Serial.printf("Time received: %d\n", task_time);
                        time_msg_rec = true;
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        flushReceiveBuffer(); //Make sure buffer is clear
                    }

                }

            }
        }
        
    }

    msg = task_value;
    time = task_time;
    return 0;
}


// Function to read our command from the serial monitor. 
int readFromSerialforDayandNightParameters(int &misting_interval, int &misting_time, bool &day_night_flag){

    // Flags for if a msg was received and a time was received
    bool misting_interval_msg_rec = false;
    bool misting_time_msg_rec = false;
    bool day_night_msg_rec = false;

    // status Flags to track when we printed a message for enter a message and enter a time so we dont flood the serial comms
    bool misting_interval_msg_flag = false;
    bool misting_time_msg_flag = false;
    bool day_night_msg_flag = false;

    int misting_length;
    int misting_value;
    int day_night_value;

    // Keep looping until a message is received
    while(misting_interval_msg_rec == false){

        vTaskDelay(pdMS_TO_TICKS(100)); //delay to keep the system from being overworked during the loop.
        if(misting_interval_msg_flag == false){
            // Find a way to parse. Maybe do messages separately.
            flushReceiveBuffer(); //Make sure buffer is clear
            Serial.println("Enter the cycle misting interval:\n");
            misting_interval_msg_flag = true;
        }

        // If we got a message, now we ask for the time to perform the task 
        if(Serial.available() > 0 ){
            vTaskDelay(pdMS_TO_TICKS(1000)); 
            // Message is consumed, serial available should read -1 now
            misting_value = Serial.parseInt(); // Consumed message should be the task to perform
            Serial.printf("Misting Interval Received: %d\n", misting_value);
            flushReceiveBuffer(); //Make sure buffer is clear

            // Logic to check and make sure user actually wants task 0 and not accidentally
            // activating task 0 because .toInt has failed
            if (misting_value < 0){ 
                Serial.println("Misting value is less than 0. Breaking back to start state.\n");
                return -1;
            } 

            misting_interval_msg_rec = true;

            //Once we got the misting intervals, time to ask for the time
            if(misting_interval_msg_rec == true){
                
                while(misting_time_msg_rec == false){

                    vTaskDelay(pdMS_TO_TICKS(1000)); //delay to keep the system from being overworked during the loop.

                    if(misting_time_msg_flag == false){
                        Serial.println("Enter the length of time for misters to be on: \n");
                        misting_time_msg_flag = true;
                        vTaskDelay(pdMS_TO_TICKS(1000));

                    }

                    // If there is something in the serial buffer, means the time was sent
                    if(Serial.available() > 0 ){
                        misting_length = Serial.parseInt();
                        Serial.printf("Misting Duration Received: %d\n", misting_length);
                        misting_time_msg_rec = true;
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        flushReceiveBuffer(); //Make sure buffer is clear
                    }
                    
                }

            }

            //Since we got the intervals and time, now ask for the day/night
            if(day_night_msg_rec == false){

                while(day_night_msg_rec == false){

                    vTaskDelay(pdMS_TO_TICKS(1000));

                    if(day_night_msg_flag == false){

                        Serial.println("Enter if this is day or night cycle(0=day, 1=night)\n");
                        day_night_msg_flag = true;
                        vTaskDelay(pdMS_TO_TICKS(1000));
                    }

                    // If there is something in the serial buffer, means day/night was sent
                    if(Serial.available() > 0 ){
                        day_night_value = Serial.parseInt();
                        Serial.printf("Day/Night Received: %d\n", day_night_value);
                        day_night_msg_rec = true;
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        flushReceiveBuffer(); //Make sure buffer is clear
                        if(day_night_value >= 1){
                            day_night_flag = false;
                        }
                        else{
                            day_night_flag = true;
                        }
                    }

                }
            }
        }
        
    }

    misting_interval = misting_value;
    misting_time = misting_length;
    return 0;
}

// Function to flush the receive buffer
void flushReceiveBuffer(){
    while(Serial.available()){
        Serial.read();
    }       
}
