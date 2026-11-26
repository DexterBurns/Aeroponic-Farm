#ifndef ADCCOMMS_H
#define ADCCOMMS_H


//Variable that Start/Stops our perpetual ADC readings
bool adcStartStop = false;

//Function to initialize Serial connection to the ADC. and open the RS485 Connections
int initADC();

int readADC();

//Channels run from 0 to 7, and return the volts in in mV
int getADCValue(int requested_channel);

#endif