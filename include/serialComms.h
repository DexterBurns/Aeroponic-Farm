// Motor Debug Header Files
#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <Arduino.h>

void SerialCommsInit();
int readFromSerial(int&, int&);
int readFromSerialforDayandNightParameters(int&, int&, bool&);
void flushReceiveBuffer();


#endif