import csv
import random
import datetime
import math
import os
import json

# Configuration
GRID_WIDTH = 40
GRID_HEIGHT = 30
NUM_DATA_POINTS = GRID_WIDTH * GRID_HEIGHT  # 1200 points total
START_LAT = 34.0833    # San Bernardino Valley College (South edge)
START_LON = -117.3160  # West edge
GRID_STEP = 0.0002     # Distance between each point (~22 meters)

# Hotspot configuration (simulating dark roofs or asphalt absorbing heat)
# Using grid coordinates (col, row) instead of index
HOTSPOTS = [
    {"col": 10, "row": 20, "radius": 5, "intensity": 9.0},
    {"col": 28, "row": 15, "radius": 8, "intensity": 14.0},
    {"col": 20, "row": 5, "radius": 4, "intensity": 7.0}
]

# Get the directory of the current script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
# Save the output file into the data_logs folder so it gets picked up automatically
OUTPUT_FILE = os.path.join(SCRIPT_DIR, "..", "data_logs", "mock_data.csv")
NASA_FILE = os.path.join(SCRIPT_DIR, "nasa_baseline.json")

def load_nasa_baseline():
    if os.path.exists(NASA_FILE):
        with open(NASA_FILE, 'r') as f:
            return json.load(f)
    return None

def generate_data():
    data = []
    
    # Start the timestamp at a fixed time for consistency, e.g., 14:00:00
    current_time = datetime.datetime.now().replace(hour=14, minute=0, second=0, microsecond=0)
    
    # Load NASA baseline
    nasa_data = load_nasa_baseline()
    if nasa_data:
        base_ambient_temp = nasa_data["ambient_temp_c"]
        base_surface_temp = nasa_data["surface_temp_c"]
        print(f"Using NASA Baseline -> Ambient: {base_ambient_temp}°C, Surface: {base_surface_temp}°C")
    else:
        base_ambient_temp = 25.0
        base_surface_temp = 27.0
        print("NASA data not found. Using default baselines.")
        
    base_humidity = 45.0

    for i in range(NUM_DATA_POINTS):
        row = i // GRID_WIDTH
        col = i % GRID_WIDTH
        
        # 1. Timestamp (HH:MM:SS)
        timestamp = current_time.strftime("%H:%M:%S")
        
        # 2. GPS - Calculate grid position with a tiny bit of human wobble
        current_lat = START_LAT + (row * GRID_STEP) + random.uniform(-0.000002, 0.000002)
        current_lon = START_LON + (col * GRID_STEP) + random.uniform(-0.000002, 0.000002)
        
        # 3. Ambient Sensors (add slight sensor noise)
        ambient_temp = base_ambient_temp + random.uniform(-0.2, 0.2)
        humidity = base_humidity + random.uniform(-1.0, 1.0)
        
        # 4. IR Center Temp
        base_ir = base_surface_temp + random.uniform(-0.5, 0.5)
        
        # Apply Urban Heat Island hotspots based on grid distance
        ir_temp = base_ir
        for hotspot in HOTSPOTS:
            dist_col = col - hotspot["col"]
            dist_row = row - hotspot["row"]
            distance = math.sqrt(dist_col**2 + dist_row**2)
            
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
            f"{ir_temp:.2f}"
        ])
        
        # Increment time by 5 seconds (simulating walking to the next grid point)
        current_time += datetime.timedelta(seconds=5)
        
    return data

def main():
    print("Generating mock walking grid survey data...")
    data = generate_data()
    
    headers = [
        "Timestamp", "Latitude", "Longitude", 
        "AmbientTemp_C", "Humidity_Pct", "CenterIRTemp_C"
    ]
    
    print(f"Writing {len(data)} rows to {os.path.abspath(OUTPUT_FILE)}...")
    with open(OUTPUT_FILE, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(headers)
        writer.writerows(data)
        
    print("Success! Mock data generated.")

if __name__ == "__main__":
    main()
