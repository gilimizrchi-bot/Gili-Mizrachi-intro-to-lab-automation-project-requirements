import csv
import serial
import serial.tools.list_ports
import time
from pathlib import Path

PORT = 'COM4'  # Change this to your Arduino port, for example COM3 or COM4
BAUD_RATE = 9600
CSV_PATH = Path(__file__).with_name('session_data.csv')

print('Available ports:')
for port in serial.tools.list_ports.comports():
    print(port.device)

try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
except serial.SerialException as exc:
    print(f'Could not open {PORT}: {exc}')
    print('Please close the Arduino Serial Monitor, then try again.')
    print('If needed, change PORT to another detected COM port above.')
    raise SystemExit(1)

time.sleep(2)

with CSV_PATH.open('w', newline='') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(['time_ms', 'angle', 'buzzer_state'])

    print('Waiting for data from Arduino...')
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if not line:
            continue

        if line == 'START':
            print('Session started')
            continue
        if line == 'STOP':
            print('Session stopped')
            break

        parts = line.split(',')
        if len(parts) == 3:
            writer.writerow(parts)
            csv_file.flush()
            print(line)

ser.close()
print(f'CSV saved to {CSV_PATH}')
