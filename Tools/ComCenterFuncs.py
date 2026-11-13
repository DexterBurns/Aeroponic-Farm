#JSON library for prosessing JSON's
import json
import ComCenter 
import serial

#Opening port to device
device_Port = "COM3"
ser = serial.Serial(device_Port, 115200, timeout=1)
dummy = serial.serial_for_url("loop://", timeout=1)

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
    
#Function to run motor for x amount of seconds
def runMotorMessage(run_Time):

    run_Time = run_Time * 1000

    message = {
        "type": "command",
        "parameter": "run_motor",
        "settings": {
            "run_time": run_Time
        }
    }

    sendJSONMessage(message)

# I want this to start in 15 seconds after the command is received.
# Make a note the ESP-32 should beep as it is aobut to start the test, and maybe a
# note to mention that the button on the
def calibratePSI():

    message = {
        "type": "calibration",
        "parameter": "calibrate_PSI",
        "settings": {
            "start time": 15000 
        }
    }

    sendJSONMessage(message)

#Function that packs send the JSON over serial
def sendJSONMessage(raw_Message):

    #Dump raw message and send to ESP32
    json_Message = json.dumps(raw_Message)
    dummy.write((json_Message + "\n").encode("utf=8"))

    #Til we get the device setup and working
    #ser.write((json_Message + "\n").encode("utf=8"))

#Function to test Serial messages sent from Command Center
def loopBack(serial_Message):


    dummy.write((serial_Message + "\n").encode("utf=8"))

    response = ser.readline().decode().strip()

    print(response)

def closeSerialConnections():
    ser.close()
    dummy.close()
