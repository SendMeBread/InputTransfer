import subprocess
import multiprocessing
import os
import time
import keyboard

address = input("Enter multicast server IP address:\n")
port = input("Enter multicast server port:\n")
def run_script(name):
    for root, _, files in os.walk("/Subhosts"):
        for file in files:
            if name in file:
                filepath = os.path.join(root, file)
                subprocess.run(["python3", filepath, address, port]) #If on windows, change 'python3' to 'python'
    subprocess.run(["python3"]) #If on windows, change 'python3' to 'python'

curr_time = time.time()

inputList = []
while curr_time + 30 > time.time():
    event = keyboard.read_event()
    if event.event_type == keyboard.KEY_DOWN:
        if event.name not in inputList:
            inputList.append(event.name)
if __name__ == "__main__":
    file_names = inputList
    
    with multiprocessing.Pool() as pool:
        pool.map(run_script, inputList)