import subprocess
import multiprocessing
import os
import time
from pynput import keyboard, mouse
def on_keyDown(key):
    if key not in inputList:
        inputList.append(key)
def on_move(x, y):
    if "cursorX" not in inputList and "cursorY" not in inputList:
        inputList.append("cursorX")
        inputList.append("cursorY")
def on_click(x, y, button, pressed):
    if button == button.left:
        if "left" not in inputList:
            inputList.append("left")
    elif button == button.right:
        if "right" not in inputList:
            inputList.append("right")
    elif button == button.middle:
        if "middle" not in inputList:
            inputList.append("middle")
def on_scroll(x, y, dx, dy):
    if "scroll" not in inputList:
        inputList.append("scroll")
address = input("Enter multicast server IP address:\n")
port = input("Enter multicast server port:\n")
def run_script(name):
    for root, _, files in os.walk("/Subhosts"):
        for file in files:
            if name in file:
                filepath = os.path.join(root, file)
                subprocess.run(["python3", filepath, address, port])
inputList = []
with mouse.Listener(on_move=on_move, on_click=on_click, on_scroll=on_scroll) as mListener:
    mListener.join()
with keyboard.Listener(on_press=on_keyDown) as kListener:
    kListener.join()
time.sleep(20)
if __name__ == "__main__":
    file_names = inputList
    
    with multiprocessing.Pool() as pool:
        pool.map(run_script, inputList)