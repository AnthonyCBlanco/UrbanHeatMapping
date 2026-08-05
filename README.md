# NASA Urban Heat Mapping System

A handheld thermal mapping data acquisition system designed to collect physical micro-climate surface data across an urban campus and compare it against historical NASA satellite baseline datasets.

## Project Overview

The goal of this project is to investigate the urban heat island effect at San Bernardino Valley College. Due to hardware constraints and a pivot from a UAV payload to a handheld scanner, this project focuses heavily on **Data Architecture and Software Engineering**. 

We built a custom browser-based Data Acquisition System that fuses live physical sensor data (via Arduino over USB) with the laptop's onboard HTML5 Geolocation API as the user walks the campus. This physical data is then processed through a Python pipeline that queries the NASA POWER REST API for historical baseline comparisons, culminating in an interactive web dashboard.

## System Architecture

The project is divided into four interconnected software systems:

1. **Hardware Firmware (Arduino):** A lightweight C++ sketch (`UrbanHeatMapping.ino`) that acts as a pure sensor node. It reads the DHT11 sensor every 2 seconds and streams live telemetry (Temperature & Humidity) to the USB Serial port.
2. **Web Data Acquisition System (HTML/JS):** A custom web application (`logger/index.html`) that uses the modern **Web Serial API** to read the live stream from the Arduino, while simultaneously using the **HTML5 Geolocation API** to track the laptop's physical path. It merges the data in real-time and allows the user to download a consolidated `datalog.csv`.
3. **NASA Integration & Pipeline (Python):** 
    * `fetch_nasa_data.py`: Queries the open NASA POWER REST API to establish the true regional baseline temperatures based on satellite observations for the campus coordinates.
    * `process_data.py`: Ingests the collected CSV, formats it into standardized web-ready spatial data (`GeoJSON`), and fuses it with the NASA baselines.
4. **Interactive Web Dashboard (HTML/JS):** A stunning, modern web application (`dashboard/index.html`) built with Leaflet.js that visualizes the physical data walk as a continuous, fluid thermal gradient over the campus map, allowing side-by-side comparison with the NASA dataset.

## Getting Started

### 1. Arduino Setup
* Flash the `UrbanHeatMapping.ino` sketch to your Arduino Mega.
* **Wiring:** Connect the DHT11 data pin to Digital Pin 7. Ensure power (5V) and Ground are connected.

### 2. Live Data Collection
1. Keep the Arduino plugged into your laptop via USB.
2. Open `logger/index.html` in Google Chrome or Microsoft Edge (requires Web Serial API support).
3. Click **Connect to Arduino**. Select your Arduino's COM port in the browser popup, and grant Location permissions.
4. Walk the campus! The UI will show live tracking and point logging.
5. Click **Download CSV** when finished, and save the file over `datalog.csv` in the root folder.

### 3. Data Processing & Visualization
Run the Python pipeline to pull the NASA baselines and generate the dashboard data:
```bash
python scripts/fetch_nasa_data.py
python scripts/process_data.py
```
*(Note: If you want to preview the dashboard without physical hardware, run `python scripts/generate_mock_data.py` before `process_data.py` to simulate a walking survey).*

Finally, open `dashboard/index.html` to view the interactive thermal map!

## License

See the [LICENSE](LICENSE) file for details.