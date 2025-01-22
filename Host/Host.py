import subprocess
import multiprocessing
import os
import time
import keyboard
import pyautogui

address = input("Enter multicast server IP address:\n")
port = input("Enter multicast server port:\n")
def run_script(name):
    for root, _, files in os.walk("/Subhosts"):
        for file in files:
            if name in file:
                filepath = os.path.join(root, file)
                subprocess.run(["python3", filepath, address, port]) #If on windows, change 'python3' to 'python' #If on windows, change 'python3' to 'python'

curr_time = time.time()
x1, y1 = pyautogui.position()
inputList = []
while curr_time + 30 > time.time():
    x2, y2 = pyautogui.position()
    if x2 != x1:
        inputList.append("CursorX")
    if y2 != y1:
        inputList.append("CursorY")
    event = keyboard.read_event()
    if event.event_type == keyboard.KEY_DOWN:
        if event.name not in inputList:
            inputList.append(event.name.upper())
if __name__ == "__main__":
    file_names = inputList
    
    with multiprocessing.Pool() as pool:
        pool.map(run_script, inputList)