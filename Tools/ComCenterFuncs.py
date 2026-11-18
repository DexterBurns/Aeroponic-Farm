#JSON library for prosessing JSON's
import json
import serial
from serial.threaded import LineReader, ReaderThread
import random

#Opening port to device
#device_Port = "COM3"
#ser = serial.Serial(device_Port, 115200, timeout=1)
dummy = serial.serial_for_url("loop://", timeout=1)

# Our class inhereits the traits of LineReader.
# We have the variables within it too.
class ESP32Reader(LineReader):

    TERMINATOR = b"\n"

    def __init__(self):
        super().__init__()
        self.latest_line = None

    # Function that LineReader will call when a complete message is received.
    def handle_line(self, line):
        print("Message Received: ", line)
        self.latest_line = line

#Initialize the reader thread, and expose the object
serial_reader = ReaderThread(dummy, ESP32Reader)
serial_reader.start()
global received_message
transport, esp32_reader_object = serial_reader.connect()


#Function to handle replies. For this, we'll just have the ESP32 fill out the JSON for every parameter by default.
def handleReplies(reply):
    try:
        message = json.loads(reply)
    except json.JSONDecodeError:
        print("Received invalid JSON")
        return None

    if not isinstance(message, dict):
        print("Expected a JSON object")
        return None

    print("Reply successfully received from ESP32")

    # .get is safer as it allows a fallback incase that value is missing 
    esp32_replies = {
        "Status": message.get("Status", "N/A"),
        "Mode": message.get("Mode", "N/A"),
        "ADC": message.get("ADC", "N/A"),
        "PSI": message.get("PSI", "N/A"),
        "Misting Interval": message.get("Misting Interval", "N/A"),
        "Misting Duration": message.get("Misting Duration", "N/A"),
        "Solendoid 1 Status": message.get("Solendoid 1 Status", "N/A"),
        "Solendoid 2 Status": message.get("Solendoid 2 Status", "N/A"),
        "Solendoid 3 Status": message.get("Solendoid 3 Status", "N/A"),
        "Solendoid 4 Status": message.get("Solendoid 4 Status", "N/A"),
        "Cycle": message.get("Cycle", "N/A"),
        "Pump Status": message.get("Pump Status", "N/A"),

        # Data saved in the ESP32's flash memory
        "Saved Misting Interval": message.get("Saved Misting Interval", "N/A"),
        "Saved Misting Duration": message.get("Saved Misting Duration", "N/A"),
        "Saved Mode": message.get("Saved Mode", "N/A"),
        "Saved Cycle": message.get("Saved Cycle", "N/A"),
        "Saved Minimum PSI": message.get("Saved Minimum PSI", "N/A"),
        "Saved Maximum PSI": message.get("Saved Maximum PSI", "N/A"),
        "Saved Minimum PSI ADC": message.get("Saved Minimum PSI ADC", "N/A"),
        "Saved Maximum PSI ADC": message.get("Saved Maximum PSI ADC", "N/A"),
    }

    return esp32_replies

def FakeReplies():

    garbage_messages = {
        "Status": random.randint(0,100),
        "Mode": random.choice(["Idle, Busy"]),
        "ADC": random.randint(0,100),
        "PSI": random.randint(0,100),
        "Misting Interval": random.randint(0,100),
        "Misting Duration": random.randint(0,100),
        "Solendoid 1 Status": random.choice(["Open, Closed"]),
        "Solendoid 2 Status": random.choice(["Open, Closed"]),
        "Solendoid 3 Status": random.choice(["Open, Closed"]),
        "Solendoid 4 Status": random.choice(["Open, Closed"]),
        "Cycle": random.choice(["Day, Night"]),
        "Pump Status": random.choice(["On, Off"]),

        # Data saved in the ESP32's flash memory
        "Saved Misting Interval": random.randint(0,100),
        "Saved Misting Duration": random.randint(0,100),
        "Saved Mode": random.choice(["Auto, Manual"]),
        "Saved Cycle": random.choice(["Day, Night"]),
        "Saved Minimum PSI": random.randint(0,100),
        "Saved Maximum PSI": random.randint(0,100),
        "Saved Minimum PSI ADC": random.randint(0,100),
        "Saved Maximum PSI ADC": random.randint(0,100),
    }

    #Dump raw message and send to ESP32
    json_Message = json.dumps(garbage_messages)
    dummy.write((json_Message + "\n").encode("utf=8"))

    return garbage_messages

#Function to set and update day/night cycle parameters.
#Day = 0, Night = 1
def dayNightCycleSetAndUpdate(misting_interval, misting_duration, day_or_night, save_to_memory):

    misting_interval_ms = misting_interval * 1000
    misting_duration_ms = misting_duration * 1000

    if (day_or_night == 0):
        cycle = "day"

    elif(day_or_night == 1):
        cycle = "night"

    message = {
        "type": "auto_mode",
        "parameter": "update_auto_mode",
        "settings": {
            "misting_interval": misting_interval_ms,
            "misting_duration": misting_duration_ms,
            "day_night": cycle,
            "save_to_memory": save_to_memory
        }
    }

    #Send JSON Message out to ESP32
    sendJSONMessage(message)

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
    sendJSONMessage(message)
    
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

    #response = ser.readline().decode().strip()
    response = dummy.readline().decode().strip()

    print(response)

def closeSerialConnections():
    #ser.close()
    dummy.close()
