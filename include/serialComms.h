// Motor Debug Header Files
#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <Arduino.h>
#include <Preferences.h>

void SerialCommsInit();
int readFromSerial(int&, int&);
int readFromSerialforDayandNightParameters(int&, int&, bool&, Preferences &);
void flushReceiveBuffer();


#endif