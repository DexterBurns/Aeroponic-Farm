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
tk.Radiobutton(root, Text="Manual Mode", variable = manAuto, value=1)
tk.Radiobutton(root, Text="Auto Mode", variable = manAuto, value=2)











root.mainloop()



def send_command():
    