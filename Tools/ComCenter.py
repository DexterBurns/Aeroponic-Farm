import tkinter as tk
from tkinter import ttk
import json

import ComCenterFuncs as cf 

#Close serial connections when window is closed. Avoids errors
def on_close():
    cf.closeSerialConnections()
    root.destroy()
    
#Hides all the buttons related to manual mode
def hideManualModeUI():

    manMode_Button.grid_remove()
    
#Widget to reset debug commands as needed. 
def resetDebugGUI():

    for widget in debug_Frame.winfo_children():

        if isinstance(widget, (ttk.Button, tk.Button) ):
            widget.config(state="normal")

        if isinstance(widget, (tk.Scale, ttk.Scale)):
            widget.set(0)


def checkMode():

    # If the Auto Button is selected, hide the manual buttons. Show the Auto Settings
    if manAuto.get() == 0:
        debug_Frame.grid_remove()
        auto_Modeframe.grid()    

    # If the manual Button is selected, hide the Auto Buttons. Show only the manual settings 
    if manAuto.get() == 1:
        auto_Modeframe.grid_remove()
        debug_Frame.grid(row=7, column=0, padx=10, pady=10)


#This is our window object
root = tk.Tk()

#Title and geometry 
root.title("ESP32 Command Center")
root.geometry("500x600")

#Creating a frame within so that we can organize our buttons and create grids 

#Create Button for manual/Auto mode:
manAuto = tk.IntVar(value=0); # Creating the variable to hold that result 
manMode_Button = ttk.Radiobutton(root, text="Auto Mode", variable = manAuto, value=0, command=checkMode)
manMode_Button.grid(row=0, sticky=tk.W)

autoMode_Button = ttk.Radiobutton(root, text="Manual Mode", variable = manAuto, value=1, command=checkMode)
autoMode_Button.grid(row=1, sticky=tk.W)

#-----------Auto Mode Frame --------------------------------------------
auto_Modeframe = ttk.LabelFrame(root, text="Auto Mode Functions", padding=10)
auto_Modeframe.grid(row=6, column =0, padx=10, pady=10)
auto_Modeframe.grid_remove()


# Horizontal Slider for Misting Interval
mist_IntervalSlider = tk.Scale(auto_Modeframe, from_=1, to=6, orient='horizontal', label="Misting Interval")
mist_IntervalSlider.grid(row=1, column=0, padx=10, pady=10)

mist_DurationSlider = tk.Scale(auto_Modeframe, from_=1, to=6, orient='horizontal', label="Misting Duration")
mist_DurationSlider.grid(row=2, column=0, padx=10, pady=10)

day_NightVariable = tk.IntVar(value=0) # Assume daytime misting at first
day_button = ttk.Radiobutton(auto_Modeframe, text="Set for Day Cycle", variable = day_NightVariable, value=0)
day_button.grid(row=3, column=0, padx=10, pady=10)

night_button = ttk.Radiobutton(auto_Modeframe, text="Set for Night Cycle", variable = day_NightVariable, value=1)
night_button.grid(row=4, column=0, padx=10, pady=10)

# Button to send Auto Settings to the ESP32
send_AutoParams = ttk.Button(auto_Modeframe, text="Send Auto Params to ESP32", command=lambda: cf.dayNightCycleSetAndUpdate
                             (mist_IntervalSlider.get(),
                              mist_DurationSlider.get(),
                              day_NightVariable.get(),
                              memory_Enabled.get()))
send_AutoParams.grid(row=5, column=0, padx=10, pady=10)

memory_Enabled = tk.BooleanVar(value=False)
memory_Button = ttk.Checkbutton(auto_Modeframe, text="Save Params to ESP32", variable=memory_Enabled)
memory_Button.grid(row=6, column=0, padx=10, pady=10)


#-----------Debug Frame ---------------------------------------------------------
# Frame for debug functions 
debug_Frame = ttk.LabelFrame(root, text="Debug Functions", padding=10)
debug_Frame.grid(row=7, column=0, padx=10, pady=10)
debug_Frame.grid_remove()

#---------- Debug Functions  ----------------------------------------------------
sol1_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal", label="Solenoid 1")
sol1_Slider.grid(row=1, column=0, padx=10, pady=10)
sol1_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(1, sol1_Slider.get()))
sol1_Button.grid(row=1, column=1, padx=10, pady=10)

sol2_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal", label = "Solenoid 2")
sol2_Slider.grid(row=2, column=0, padx=10, pady=10)
sol2_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(2, sol2_Slider.get()))
sol2_Button.grid(row=2, column=1, padx=10, pady=10)

sol3_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal", label="Solenoid 3")
sol3_Slider.grid(row=3, column=0, padx=10, pady=10)
sol3_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(3, sol3_Slider.get()))
sol3_Button.grid(row=3, column=1, padx=10, pady=10)

sol4_Slider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal", label="Solenoid 4")
sol4_Slider.grid(row=4, column=0, padx=10, pady=10)
sol4_Button = tk.Button(debug_Frame, text="Send", command=lambda: cf.solenoidOpenMessage(4, sol4_Slider.get()))
sol4_Button.grid(row=4, column=1, padx=10, pady=10)

run_MotorSlider = tk.Scale(debug_Frame, from_=0, to=10, orient="horizontal", label="Seconds")
run_MotorSlider.grid(row=5, column=0, padx=10, pady=10)
run_MotorButton = ttk.Button(debug_Frame, text = "Send Motor Request", command = lambda: cf.runMotorMessage(run_MotorSlider.get()))
run_MotorButton.grid(row=5, column=1,padx=10, pady=10)

recal_PSI = ttk.Button(debug_Frame, text="Begin Calibration Test", command = lambda: cf.calibratePSI())
recal_PSI.grid(row=6, column=0, padx=10, pady=10)

reset_Debug_Buttons = ttk.Button(debug_Frame, text="Reset all Parameters", command = resetDebugGUI)
reset_Debug_Buttons.grid(row=7, column = 0, padx = 10, pady = 10) 

#Might need a section here for when we want to display the current settings we have applied on the ESP32.
#------------------Data Table------------------------------------------------------------

status_Table = ttk.Treeview(root, columns=("params","values"), show="headings", height=25)
status_Table.grid(row=0, column=1, padx= 10, pady=10, rowspan=8)
status_Table.heading("params", text="Parameter")
status_Table.heading("values", text="Values")

#Dictionary of all the things I'd like inside of the table. Values are default until we receive readings 
data_Table_Dict = {
    "status": "N/A",
    "Mode": "N/A",
    "ADC": "N/A",
    "PSI": "N/A",
    "Misting Interval": "N/A",
    "Misting Duration": "N/A",
    "Solendoid 1 Status": "N/A",
    "Solendoid 2 Status": "N/A",
    "Solendoid 3 Status": "N/A",
    "Solendoid 4 Status": "N/A",
    "Cycle": "N/A",
    "Pump Status": "N/A",

    # Data saved in the ESP32's flash memory
    "Saved Misting Interval": "N/A",
    "Saved Misting Duration": "N/A",
    "Saved Mode": "N/A",
    "Saved Cycle": "N/A",
    "Saved Minimum PSI": "N/A",
    "Saved Maximum PSI": "N/A",
    "Saved Minimum PSI ADC": "N/A",
    "Saved Maximum PSI ADC": "N/A",
}

# Creating the list of parameters that will be displayed
for parameter, default_value in data_Table_Dict.items():
    status_Table.insert('', "end", iid=parameter, values=(parameter, default_value))
  

#Cleanup
root.protocol("WM_DELETE_WINDOW", on_close)
root.mainloop()