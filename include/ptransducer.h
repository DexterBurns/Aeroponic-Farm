// Motor Debug Header Files
#ifndef PTRANSDUCER_H
#define PTRANSDUCER_H

void initTransducer();

// Function to read transducer in that moment
int readTransducer();

// Function to make a moving average of the last five transducer readings
int avgdTransducerReadings();

#endif