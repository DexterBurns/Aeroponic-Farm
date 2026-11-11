import tkinter as tk
from tkinter import ttk

import json

#This is our window object
root = tk.Tk()

#Title and geometry 
root.title("ESP32 Command Center")
root.geometry("500x600")

#Creating a frame within so that we can organize our buttons and create grids 
#Saying to create our frame within root
panel = ttk.LabelFrame(root, text ="Command Center", padding=16)
panel.grid()

#Create Button for manual/Auto mode:
manAuto = tk.IntVar(); # Creating the variable to hold that result 
ttk.Radiobutton(root, text="Manual Mode", variable = manAuto, value=1).grid(row=0, sticky=tk.W)
ttk.Radiobutton(root, text="Auto Mode", variable = manAuto, value=2).grid(row=1, sticky=tk.W)


#Section for the Day/Night Cycle -------------------------------------------------------
panel = ttk.LabelFrame(root, text ="Day/Night Cycle", padding=16).grid(row=2, column=0, padx=10, pady=10)


#Create Button for manual/Auto mode -------------------------------
manAuto = tk.IntVar(); # Creating the variable to hold that result 
ttk.Radiobutton(root, text="Day Mode", variable = manAuto, value=1).grid(row=3, sticky=tk.W)
ttk.Radiobutton(root, text="Night Mode", variable = manAuto, value=2).grid(row=4, sticky=tk.W)


#Misting Interval Section -------------------------------------------------------
panel = ttk.LabelFrame(root, text ="Misting Interval", padding=16).grid(row=5, column=0, padx=10, pady=10)

# Horizontal Slider for Misting Interval
horizontal_scale = tk.Scale(root, from_=1000, to=6000, orient='horizontal').grid(row=5, column=0, padx=10, pady=10)

#----------






#Might need a section here for when we want to display the current settings we have applied on the ESP32.

root.mainloop()


#Function to handle replies. 
def handleReplies(reply):

    try:
        message = json.loads(reply)
    except json.JSONDecodeError:
        status_label.config(text="Received invalid JSON")
        return

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
    
