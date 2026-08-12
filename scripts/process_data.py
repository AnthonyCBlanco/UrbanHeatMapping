import csv
import json
import os
import glob

def csv_to_geojson(data_logs_dir, geojson_filepath, nasa_baseline_path):
    features = []
    nasa_baseline_temp = None
    
    # Try to load NASA baseline if it exists
    if os.path.exists(nasa_baseline_path):
        with open(nasa_baseline_path, 'r') as f:
            nasa_data = json.load(f)
            nasa_baseline_temp = nasa_data.get("surface_temp_c")
    
    # Find all CSV files in the data_logs directory
    csv_files = glob.glob(os.path.join(data_logs_dir, "*.csv"))
    
    if not csv_files:
        print(f"Error: No CSV files found in {data_logs_dir}")
        return

    for csv_filepath in csv_files:
        print(f"Processing: {os.path.basename(csv_filepath)}")
        with open(csv_filepath, mode='r') as csv_file:
            reader = csv.DictReader(csv_file)
            
            for row in reader:
                # Extract coordinates (GeoJSON expects Longitude, Latitude)
                try:
                    lat = float(row['Latitude'])
                    lon = float(row['Longitude'])
                except ValueError:
                    continue # Skip invalid rows
                    
                # Build the properties dictionary
                properties = {
                    "Timestamp": row["Timestamp"],
                    "AmbientTemp_C": float(row["AmbientTemp_C"]),
                    "Humidity_Pct": float(row["Humidity_Pct"]),
                    "CenterIRTemp_C": float(row["CenterIRTemp_C"])
                }
                
                # Add delta if we have a baseline
                if nasa_baseline_temp is not None:
                    properties["Delta_C"] = properties["CenterIRTemp_C"] - nasa_baseline_temp
                
                # Construct the GeoJSON Feature
                feature = {
                    "type": "Feature",
                    "geometry": {
                        "type": "Point",
                        "coordinates": [lon, lat]
                    },
                    "properties": properties
                }
                features.append(feature)

    # Create the FeatureCollection
    feature_collection = {
        "type": "FeatureCollection",
        "nasa_baseline_c": nasa_baseline_temp,
        "features": features
    }
    
    # Ensure the output directory exists
    os.makedirs(os.path.dirname(geojson_filepath), exist_ok=True)

    # Write the JS file (bypasses CORS for local double-clicking)
    with open(geojson_filepath, mode='w') as geojson_file:
        geojson_file.write("const geojsonData = ")
        json.dump(feature_collection, geojson_file, indent=2)
        geojson_file.write(";")
        
    print(f"Successfully converted {len(features)} points to {geojson_filepath}")

if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Input is now a directory containing multiple CSVs
    data_logs_dir = os.path.join(script_dir, "..", "data_logs")
    
    # Output JS goes into the dashboard folder
    output_geojson = os.path.join(script_dir, "..", "dashboard", "data.js")
    
    nasa_baseline_path = os.path.join(script_dir, "nasa_baseline.json")
    
    print("Starting data processing pipeline...")
    csv_to_geojson(data_logs_dir, output_geojson, nasa_baseline_path)
