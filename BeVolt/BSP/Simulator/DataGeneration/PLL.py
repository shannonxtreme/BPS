import csv

file = "BSP/Simulator/DataGeneration/Data/PLL.csv"

"""
@brief change the clock frequency
    Function to be called by simulate.py
"""
CurrentFrequency = 0


def Change_Frequency(newFrequency):
    with open(file, 'w') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow([newFrequency])
        global Current_Frequency 
        Current_Frequency = int(newFrequency)

def Get_Frequency():
    global CurrentFrequency
    return int(Current_Frequency)

def Print_Frequency():
    with open(file, 'w') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow([Current_Frequency])