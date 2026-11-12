import tkinter as tk
from tkinter import ttk

import json


#Hides all the buttons related to manual mode
def hideManualModeUI():

    manMode_Button.grid_remove()
    

def checkMode():

    # If the manual button is selected. Hide all the manual buttons
    if manAuto == 1:
        mist_IntervalSlider.grid_remove()
        mist_DurationSlider.grid_remove()


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

    
#This is our window object
root = tk.Tk()

#Title and geometry 
root.title("ESP32 Command Center")
root.geometry("500x600")

#Creating a frame within so that we can organize our buttons and create grids 

#Create Button for manual/Auto mode:
manAuto = tk.IntVar(); # Creating the variable to hold that result 
manMode_Button = ttk.Radiobutton(root, text="Manual Mode", variable = manAuto, value=1, command=checkMode)
manMode_Button.grid(row=0, sticky=tk.W)

autoMode_Button = ttk.Radiobutton(root, text="Auto Mode", variable = manAuto, value=2, command=checkMode)
autoMode_Button.grid(row=1, sticky=tk.W)

#Misting Interval Section -------------------------------------------------------

# Horizontal Slider for Misting Interval
mist_IntervalSlider = tk.Scale(root, from_=1000, to=6000, orient='horizontal')
mist_IntervalSlider.grid(row=5, column=0, padx=10, pady=10)

mist_DurationSlider = tk.Scale(root, from_=1000, to=6000, orient='horizontal')
mist_DurationSlider.grid(row=6, column=0, padx=10, pady=10)

# Frame for debug functions 
debug_Frame = ttk.LabelFrame(root, text="Debug Functions", padding=10)
debug_Frame.grid(row=7, column=0, padx=10, pady=10)

#---------- Debug Functions  ----------------------------------------------------
sol1_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal")
sol1_Slider.grid(row=1, column=0, padx=10, pady=10)

sol2_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal")
sol2_Slider.grid(row=2, column=0, padx=10, pady=10)

sol3_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal")
sol3_Slider.grid(row=3, column=0, padx=10, pady=10)

sol4_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal")
sol4_Slider.grid(row=4, column=0, padx=10, pady=10)

run_MotorSlider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal", label="Seconds")
run_MotorSlider.grid(row=5, column=0, padx=10, pady=10)

recal_PSI = ttk.Button(debug_Frame, text="Calibrate", command=psiCalibration)




#Might need a section here for when we want to display the current settings we have applied on the ESP32.


root.mainloop()