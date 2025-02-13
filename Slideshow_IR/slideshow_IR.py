import serial
import pyautogui  # Library for controlling the keyboard and mouse

ser = serial.Serial('COM3', 9600)  # Replace 'COMX' with your Arduino's COM port

while True:
    if ser.in_waiting > 0:
        data = ser.readline().decode().strip()
        print("Received data:", data)
        pyautogui.press('right')  # Simulate Right Arrow for next slide

        # Add similar conditional blocks for other actions based on received messages
