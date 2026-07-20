import csv
import random
import datetime
import math
import os

# Configuration
NUM_DATA_POINTS = 120  # 2 minutes of data at 1Hz
START_LAT = 34.0522    # Los Angeles (Downtown)
START_LON = -118.2437
LAT_INCREMENT = 0.00005  # roughly 5.5 meters per second North
LON_INCREMENT = 0.00002  # roughly 2 meters per second East

# Hotspot configuration (simulating dark roofs or asphalt absorbing heat)
HOTSPOTS = [
    {"index": 30, "radius": 10, "intensity": 8.0},
    {"index": 80, "radius": 15, "intensity": 14.0}
]

# Get the directory of the current script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
# Save the output file one level up in the main project folder
OUTPUT_FILE = os.path.join(SCRIPT_DIR, "..", "datalog.csv")

def generate_data():
    data = []
    
    # Start the timestamp at a fixed time for consistency, e.g., 14:00:00
    current_time = datetime.datetime.now().replace(hour=14, minute=0, second=0, microsecond=0)
    current_lat = START_LAT
    current_lon = START_LON
    
    base_ambient_temp = 25.0
    base_humidity = 45.0
    base_pressure = 1012.0

    for i in range(NUM_DATA_POINTS):
        # 1. Timestamp (HH:MM:SS)
        timestamp = current_time.strftime("%H:%M:%S")
        
        # 2. GPS
        # Add slight random drift to the movement
        current_lat += LAT_INCREMENT + random.uniform(-0.000005, 0.000005)
        current_lon += LON_INCREMENT + random.uniform(-0.000005, 0.000005)
        
        # 3. Ambient Sensors (add slight sensor noise)
        ambient_temp = base_ambient_temp + random.uniform(-0.2, 0.2)
        humidity = base_humidity + random.uniform(-1.0, 1.0)
        pressure = base_pressure + random.uniform(-0.5, 0.5)
        
        # 4. IR Center Temp
        # Base IR temp is usually slightly higher than ambient for urban ground
        base_ir = ambient_temp + 2.0 + random.uniform(-0.5, 0.5)
        
        # Apply Urban Heat Island hotspots
        ir_temp = base_ir
        for hotspot in HOTSPOTS:
            distance = abs(i - hotspot["index"])
            if distance < hotspot["radius"]:
                # Gaussian bell curve effect for the hotspot
                effect = hotspot["intensity"] * math.exp(-(distance**2) / (hotspot["radius"]**2 / 2))
                ir_temp += effect
        
        # Format the numbers to match the Arduino output
        data.append([
            timestamp,
            f"{current_lat:.6f}",
            f"{current_lon:.6f}",
            f"{ambient_temp:.2f}",
            f"{humidity:.2f}",
            f"{pressure:.2f}",
            f"{ir_temp:.2f}"
        ])
        
        # Increment time by 1 second (1Hz sampling rate)
        current_time += datetime.timedelta(seconds=1)
        
    return data

def main():
    print("Generating mock drone flight data...")
    data = generate_data()
    
    headers = [
        "Timestamp", "Latitude", "Longitude", 
        "AmbientTemp_C", "Humidity_Pct", "Pressure_hPa", "CenterIRTemp_C"
    ]
    
    print(f"Writing {len(data)} rows to {os.path.abspath(OUTPUT_FILE)}...")
    with open(OUTPUT_FILE, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(headers)
        writer.writerows(data)
        
    print("Success! Mock data generated.")

if __name__ == "__main__":
    main()
