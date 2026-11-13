#JSON library for prosessing JSON's
import json
import ComCenter 
import serial

#Opening port to device
device_Port = "COM3"
ser = serial.Serial(device_Port, 115200, timeout=1)

#Function to handle replies. For this, we'll just have the ESP32 fill out the JSON for every parameter by default.
def handleReplies(reply):

    try:
        message = json.loads(reply)
    except json.JSONDecodeError:
        print("Received invalid JSON")
        return 1

    #Using this area to handle how messages get placed into the JSON
    #This give us everything under status. We can parse the statuses now.
    esp32_Status = message["status"] #Checking if ESP32 is still okay
    esp32_Mode = message["Mode"]
    esp32_ADC1 = message["ADC"]
    esp32_PSI = message["PSI"]
    esp32_MistInt = message["Misting Interval"]
    esp32_MistDur = message["Misting Duration"]
    esp32_Sol1Status = message["Solendoid 1 Status"]
    esp32_Sol2Status = message["Solendoid 2 Status"]
    esp32_Sol3Status = message["Solendoid 3 Status"]
    esp32_Sol4Status = message["Solendoid 4 Status"]
    esp32_CycleStatus = message["Cycle"]
    esp32_PumpStatus = message["Pump Status"]

    #-----------------------------------------------------
    #Data the esp32 currently has saved in flash memory
    esp32_S_MistInt = message["Saved Misting Interval"]
    esp32_S_MistDur = message["Saved Misting Duration"]
    esp32_S_Mode = message["Saved Mode"]
    esp32_S_Cycle = message["Saved Cycle"]
    esp32_S_MinPSI = message["Saved Minimum PSI"]
    esp32_S_MaxPSI = message["Saved Maximum PSI"]
    esp32_S_MinPSIADC = message["Saved Minimum PSI ADC"]
    esp32_S_MaxPSIADC = message["Saved Maximum PSI ADC"]

    return 0

#ESP32 send open solenoid message 
def solenoidOpenMessage(solenoid_num, open_time):

    #Converting seconds to milliseconds
    open_time = open_time * 1000

    message = {
        "type": "command",
        "parameter": "solenoid_open",
        "settings": {
            "solenoid": solenoid_num,
            "misting_time": open_time
        }
    }

    #Send message to ESP32
    json_message = json.dumps(message)

#Function that packs send the JSON over serial
def sendJSONMessage(json_Message):
    ser.write((json_Message + "\n").encode("utf=8"))