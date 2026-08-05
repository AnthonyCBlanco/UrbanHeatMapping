import urllib.request
import json
import os

def fetch_nasa_baseline():
    print("Connecting to NASA POWER REST API...")
    
    # Coordinates for San Bernardino Valley College
    lat = 34.086098
    lon = -117.312661
    
    # We use a fixed hot summer day (Aug 1, 2023) to guarantee strong heat island baselines
    date = "20230801"
    
    # NASA POWER API URL for Air Temp (T2M) and Earth Skin Temp (TS)
    url = f"https://power.larc.nasa.gov/api/temporal/daily/point?parameters=T2M,TS&community=RE&longitude={lon}&latitude={lat}&start={date}&end={date}&format=JSON"
    
    try:
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        with urllib.request.urlopen(req) as response:
            data = json.loads(response.read().decode())
            
            # Extract the temperatures for our specific date
            t2m = data['properties']['parameter']['T2M'][date]
            ts = data['properties']['parameter']['TS'][date]
            
            baseline = {
                "source": "NASA POWER API",
                "date": date,
                "latitude": lat,
                "longitude": lon,
                "ambient_temp_c": t2m,
                "surface_temp_c": ts
            }
            
            print(f"Success! Retrieved NASA Baseline: Air Temp: {t2m}°C, Surface Temp: {ts}°C")
            
            # Save the baseline to be used by our generator
            script_dir = os.path.dirname(os.path.abspath(__file__))
            output_file = os.path.join(script_dir, "nasa_baseline.json")
            
            with open(output_file, 'w') as f:
                json.dump(baseline, f, indent=4)
                
            return baseline
            
    except Exception as e:
        print(f"Error fetching from NASA API: {e}")
        return None

if __name__ == "__main__":
    fetch_nasa_baseline()
