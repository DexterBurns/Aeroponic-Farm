// Motor Debug Header Files
#ifndef ADCCOMMS_H
#define ADCCOMMS_H

#include <Arduino.h>

//Function to initialize Serial connection to the ADC. and open the RS485 Connections
void initADC();

bool readButton();



#endif