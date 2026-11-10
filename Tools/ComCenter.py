import tkinter as tk
from tkinter import ttk

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






#Might need a section here for when we want to display the current settings we have applied on the ESP32.

root.mainloop()

