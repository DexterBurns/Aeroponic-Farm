#include <Arduino.h>
#include "button.h"
#define START_PIN 9

bool start_flag = false;

void initButton(){

    pinMode(START_PIN, INPUT_PULLUP);

    bool start_state = digitalRead(START_PIN);

}

bool readButton(){
    bool state = digitalRead(START_PIN);
    return state;
}


