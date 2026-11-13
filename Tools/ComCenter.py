import tkinter as tk
from tkinter import ttk
import json

import ComCenterFuncs as cf 


#Hides all the buttons related to manual mode
def hideManualModeUI():

    manMode_Button.grid_remove()
    

def checkMode():

    # If the manual button is selected. Hide all the manual buttons
    if manAuto == 1:
        mist_IntervalSlider.grid_remove()
        mist_DurationSlider.grid_remove()

#Function to run our PSI calibration test
def psiCalibration():
    hold = 0




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
sol1_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(1, sol1_Slider.get()))
sol1_Button.grid(row=1, column=1, padx=10, pady=10)

sol2_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal")
sol2_Slider.grid(row=2, column=0, padx=10, pady=10)
sol2_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(2, sol2_Slider.get()))
sol2_Button.grid(row=2, column=1, padx=10, pady=10)

sol3_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal")
sol3_Slider.grid(row=3, column=0, padx=10, pady=10)
sol3_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(3, sol3_Slider.get()))
sol3_Button.grid(row=3, column=1, padx=10, pady=10)

sol4_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal")
sol4_Slider.grid(row=4, column=0, padx=10, pady=10)
sol4_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(4, sol4_Slider.get()))
sol4_Button.grid(row=4, column=1, padx=10, pady=10)

run_MotorSlider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal", label="Seconds")
run_MotorSlider.grid(row=5, column=0, padx=10, pady=10)

recal_PSI = ttk.Button(debug_Frame, text="Calibrate", command=psiCalibration)


#Might need a section here for when we want to display the current settings we have applied on the ESP32.


root.mainloop()