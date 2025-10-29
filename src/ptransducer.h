// Motor Debug Header Files
#ifndef PTRANSDUCER_H
#define PTRANSDUCER_H

// Function to read transducer in that moment
uint32_t readTransducer();

// Function to make a moving average of the last five transducer readings
float avgdTransducerReadings();


#endif