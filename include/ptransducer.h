// Motor Debug Header Files
#ifndef PTRANSDUCER_H
#define PTRANSDUCER_H


struct pressureStruct {
    movingAvg currentPressure_ADC; //moving avg of pressure in ADC values
    movingAvg currentPressure_PSI; //moving avg of pressure in PSI
    movingAvg minPressure_ADC; //used to calclate the average of the minimum pressure, once
    movingAvg maxPressure_ADC; //used to calculate the avg of the maximum pressure, once

    int psi_minPressure  = 0;
    int psi_maxPressure  = 100;
    int adc_minPressure  = 900; // default value for 0 psi from ADC, this is just to hold the calculated moving average
    int adc_maxPressure  = 3000; //default value for 100 psi from ADC, to hold the calculated moving average

    pressureStruct();
};

// For global access
extern volatile pressureStruct pressureData;

void initTransducer();
// Function to read transducer in that moment
int readTransducer();
int pressureMap(int);
int calculatePressure();
int pressurizeToMaxPSI();
int findPressureRangeWithADC();
int getPressureInPSI();
// Function to make a moving average of the last five transducer readings
int avgdTransducerReadings();

void testPSILimits_debug(unsigned long, int);

// Function to help facilitate getting the ptransducer values
// and matching it with the pressure read on the manual pressure reader
void config_pTransducer_debug(int);

#endif