#include <Arduino.h>
#include <adcComms.h>
#include <ModbusMaster.h>
#include <atomic>

/* Just a note that all returned values are in millivolts and should be converted back to Volts as needed. */

// RX and TX pins of the ESP32 for the RS485 connection.
#define ADCRX_PIN 18
#define ADCTX_PIN 17

// Defining our adc modbus object
ModbusMaster adc_bus;


// Structure defining the readings of each ADC value
struct adcValues {

    std::atomic<uint16_t> channel_0;
    std::atomic<uint16_t> channel_1;
    std::atomic<uint16_t> channel_2;
    std::atomic<uint16_t> channel_3;    
    std::atomic<uint16_t> channel_4;
    std::atomic<uint16_t> channel_5;
    std::atomic<uint16_t> channel_6;
    std::atomic<uint16_t> channel_7;
};  

adcValues adc_currentReadings;

// Opening serial connection. RS485 is on the Serial1 UART
Serial1.begin(9600, SERIAL_8N1, ADCRX_PIN, ADCTX_PIN)

//Opening Serial Connection and initializing ADC
int initADC(){

    // Create the Modbus object we'll be using. Address of ADC is 1
    adc_bus.begin(1, Serial1);

    //Set the mode of each channel on the ADC. All of them will be mode 0
    //for voltage measurement. Check waveshare docs for mode 
    for (int i = 0;  i <= 7; i++){

        //Setting the data in the local buffer as 0 for each position. 8 positions
        //as that is the size of the waceshare adc buffer
        adc_bus.setTransmitBuffer(i, 0);

    }

    // Now to send the call to write the values in our buffer to the ADC
    uint8_t result = adc_bus.writeMultipleRegisters(0x1000, 8)

    // Check result against success value
    if (result == adc.ku8MBSuccess){
        Serial.println("Initial Settings set to Voltage Measurement Successful")

        // Initial settings change was successful
        return 0; 
    }

    // Configuration failed. Print error.
    else{
        Serial.print("Configuration failed. Error: 0x, Trying again.");
        return -1;
    }

}

//Function to read ADC Values. All values are voltage in millivolts. 
int readADC(){

    // Get the readings from the ADC into the buffer
    uint8_t result = adc_bus.readInputRegisters(0x0001, 8);

    // If we got a successful read
    if (result == adc.ku8MBSuccess){

        // Set our struct values byreading the input buffer
        adc_currentReadings.channel_0 = adc_bus.getResponseBuffer(0);
        adc_currentReadings.channel_1 = adc_bus.getResponseBuffer(1);
        adc_currentReadings.channel_2 = adc_bus.getResponseBuffer(2);
        adc_currentReadings.channel_3 = adc_bus.getResponseBuffer(3);
        adc_currentReadings.channel_4 = adc_bus.getResponseBuffer(4);
        adc_currentReadings.channel_5 = adc_bus.getResponseBuffer(5);
        adc_currentReadings.channel_6 = adc_bus.getResponseBuffer(6);
        adc_currentReadings.channel_7 = adc_bus.getResponseBuffer(7);

        result = 0;
        return result;
    }

    // If ther read was unsuccessful
    else{
        result = -1;
        return result;
    }
    
}

// Getter function for things to ask which value a function would like from the ADC
int getADCValue(int requested_channel){

    // if the request was invalid, out of range send a failure
    switch (requested_channel) {
        case 0: return adc_currentReadings.channel_0;
        case 1: return adc_currentReadings.channel_1;
        case 2: return adc_currentReadings.channel_2;
        case 3: return adc_currentReadings.channel_3;
        case 4: return adc_currentReadings.channel_4;
        case 5: return adc_currentReadings.channel_5;
        case 6: return adc_currentReadings.channel_6;
        case 7: return adc_currentReadings.channel_7;
        default: return -1; //Return this if the channel is out of scope

    }

}

int perpetualADCRead(){

    while (adcStartStop == true){
        
    }
}

