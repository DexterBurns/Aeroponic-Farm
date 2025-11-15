// Motor Debug Header Files
#ifndef PTRANSDUCER_H
#define PTRANSDUCER_H

void initTransducer();

// Function to read transducer in that moment
int readTransducer(int);

// Function to make a moving average of the last five transducer readings
int avgdTransducerReadings();

void testPSILimits_debug(unsigned long, int);

// Function to help facilitate getting the ptransducer values
// and matching it with the pressure read on the manual pressure reader
void config_pTransducer_debug(int);

#endif