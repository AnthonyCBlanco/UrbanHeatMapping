### **1. Hardware & Payload System**

This system handles the physical collection of data and the operation of the aircraft.

- **UAV Platform:** The base drone kit, including the frame, motors, electronic speed controllers (ESCs), battery, and primary flight controller.
    
- **Microcontroller:** The Arduino board acting as the central brain for the sensor payload, operating independently from the drone's primary flight controller to prevent interference.
    
- **Thermal Sensor:** An infrared array or thermal camera module compatible with Arduino (e.g., MLX90640 or AMG8833) to capture surface temperature readings.
    
- **GPS Module:** A precise GPS unit connected to the Arduino to tag every temperature reading with exact latitude and longitude coordinates.
    
- **Power Management:** A dedicated power supply or step-down converter to power the Arduino and sensors without draining the main drone battery unpredictably.
    

### **2. On-Board Data Logging System**

This system dictates how the Arduino processes and saves the data while in the air.

- **Microcontroller Script:** C++ instruction sets uploaded to the Arduino to poll the thermal sensor and GPS module at a synchronized frequency.
    
- **Data Formatting:** Structuring the incoming data into a clean, appendable format (like CSV) containing the timestamp, latitude, longitude, and temperature value.
    
- **Storage Module:** A micro-SD card breakout board wired to the Arduino to physically log the data during flight, preventing data loss in case of wireless telemetry failure.
    

### **3. Data Processing & Backend Pipeline**

This system bridges your raw flight data with the comparative NASA datasets.

- **Extraction & Cleaning:** Python scripts to parse the raw CSV files from the SD card, remove anomalies, and format the data for web use.
    
- **Cloud Storage:** A NoSQL database, such as Firebase or MongoDB Atlas, to securely store and organize the cleaned flight logs.
    
- **NASA API Integration:** Scripts to query NASA datasets (like Landsat 8/9 or ECOSTRESS) via APIs or Google Earth Engine, pulling satellite thermal data for the exact coordinates and times of your flights.
    

### **4. Visualization & Web Dashboard**

This system serves as the final deliverable to communicate your findings clearly.

- **Web Hosting:** A live environment utilizing Google Cloud Platform or Render to host your dashboard.
    
- **Mapping Library:** An interactive web mapping tool (such as Leaflet.js or Mapbox) to plot the geospatial data.
    
- **Visual Overlay:** Interface elements that allow users to toggle between the drone's heat map layer and the NASA satellite heat map layer.
    
- **Statistical Output:** Visual graphs comparing the temperature deltas between the two data sources at specific waypoints.
    

### **Data Comparison Strategy**

|**Feature**|**Drone Telemetry**|**NASA Satellite Data**|
|---|---|---|
|**Spatial Resolution**|High (Centimeters to meters)|Moderate (30 to 100 meters)|
|**Temporal Control**|On-demand flight times|Fixed orbital schedules|
|**Atmospheric Interference**|Minimal (Low altitude)|High (Requires atmospheric correction)|
|**Data Structure**|Custom JSON/CSV payloads|Standardized GeoTIFFs / API payloads|

### **5. Flight Operations & Testing System**

This encompasses the physical execution of the project and regulatory compliance.

- **Zone Selection:** Identifying high-impact testing environments, such as major warehouse districts or concrete-heavy transport corridors in San Bernardino, to guarantee distinct thermal signatures.
    
- **Flight Planning:** Mapping automated or semi-automated grid flight paths to ensure even data collection over the target area.
    
- **Safety & Compliance:** Adhering to FAA Part 107 regulations, checking airspace restrictions, and establishing pre-flight safety checklists.

#nasa-cagsc #projects #summer2026

