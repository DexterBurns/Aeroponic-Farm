#include <Arduino.h>
#include <adcComms.h>
#include <ModbusMaster.h>

// RX and TX pins of the ESP32 for the RS485 connection.
#define ADCRX_PIN 18;
#define ADCTX_PIN 17;



//Opening Serial Connection
void initADC(){

    // Opening serial connection. RS485 is on the Serial1 UART
    Serial1.begin(9600, SERIAL_8N1, ADCRX_PIN, ADCTX_PIN)

    // Create the Modbus object we'll be using. Address of ADC is 1
    ModbusMaster adc_bus;
    adc_bus.begin(1, Serial1);

    //Set the mode of each channel on the ADC 

}

