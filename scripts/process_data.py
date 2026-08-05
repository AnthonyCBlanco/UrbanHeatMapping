import csv
import json
import os

def csv_to_geojson(csv_filepath, geojson_filepath):
    features = []
    
    if not os.path.exists(csv_filepath):
        print(f"Error: Could not find {csv_filepath}")
        return

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
    
    # Input CSV is one level up (in the project root)
    input_csv = os.path.join(script_dir, "..", "datalog.csv")
    
    # Output JS goes into the dashboard folder
    output_geojson = os.path.join(script_dir, "..", "dashboard", "data.js")
    
    print("Starting data processing pipeline...")
    csv_to_geojson(input_csv, output_geojson)
