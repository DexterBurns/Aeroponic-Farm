#include <Arduino.h>
#include <adcComms.h>
#include <ModbusMaster.h>

// RX and TX pins of the ESP32 for the RS485 connection.
#define ADCRX_PIN 18;
#define ADCTX_PIN 17;

//Opening Serial Connection and initializing ADC
ModbusMaster initADC(){

    // Opening serial connection. RS485 is on the Serial1 UART
    Serial1.begin(9600, SERIAL_8N1, ADCRX_PIN, ADCTX_PIN)

    // Create the Modbus object we'll be using. Address of ADC is 1
    ModbusMaster adc_bus;
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

        return adc_bus;
    
    }

    // Configuration failed. Print error.
    else{
        Serial.print("Configuration failed. Error: 0x");
        return 1;
    }

}

//Function to read ADC Values. All values are voltage in millivolts. 
void readADC(ModbusMaster* adc_bus){

    // Get the readings from the ADC into the buffer
    uint8_t result = adc_bus->readInputRegisters(0x0001, 8);
    
}

//Function to retry connection to ADC


