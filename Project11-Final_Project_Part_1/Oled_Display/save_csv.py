import csv
import serial
import time
from pathlib import Path

PORT = 'COM3'  # Change if needed
BAUD_RATE = 9600
CSV_PATH = Path(__file__).with_name('session_data.csv')

ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
time.sleep(2)

with CSV_PATH.open('w', newline='') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(['time_ms', 'angle', 'buzzer_state'])

    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if not line:
            continue
        if line == 'START':
            continue
        if line == 'STOP':
            break
        parts = line.split(',')
        if len(parts) == 3:
            writer.writerow(parts)
            csv_file.flush()

ser.close()
print(f'Saved CSV to {CSV_PATH}')
