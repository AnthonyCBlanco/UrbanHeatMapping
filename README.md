# Urban Heat Mapping

A UAV-based thermal mapping payload and data pipeline designed to collect high-resolution surface temperature data and compare it against moderate-resolution NASA satellite datasets (like Landsat 8/9 and ECOSTRESS).

## 🚀 Project Overview

The goal of this project is to investigate the urban heat island effect by capturing localized, low-altitude thermal signatures using a custom drone payload. This ground-truth data is then processed and visualized alongside corresponding NASA satellite data to analyze the spatial and temporal differences in temperature readings.

## 🛠️ Hardware Payload

The data collection system is built on an **Arduino Mega 2560 R3** operating independently of the drone's primary flight controller. 

**Sensors & Components:**
* **AMG8833 IR Thermal Module:** Captures an 8x8 pixel array of surface temperatures.
* **DHT11 Sensor:** Records ambient temperature and humidity.
* **NEO-6M GPS Module:** Provides exact geospatial coordinates and timestamps for all readings.
* **EEPROM Storage:** Data is compressed and stored on the Mega's internal non-volatile EEPROM memory.

## 💻 Software Architecture

The project is divided into several interconnected systems:

1. **On-Board Data Logging (C++):** An Arduino sketch (`UrbanHeatMapping.ino`) that polls the thermal and ambient sensors alongside the GPS unit, writing structured binary data to the internal EEPROM during flight.
2. **Data Processing Pipeline (Python):** Scripts to extract, clean, and format the raw flight logs into GeoJSON or similar web-ready formats. *(In Development)*
3. **NASA API Integration (Python):** Tools to query NASA Earthdata APIs or Google Earth Engine for corresponding thermal satellite imagery. *(In Development)*
4. **Web Dashboard (HTML/JS):** An interactive Leaflet.js map for visualizing the drone's heat map overlay against the satellite data. *(In Development)*

## 🚦 Getting Started with the Payload

### Prerequisites

To compile and upload the payload sketch, you'll need the Arduino IDE and the following libraries installed via the Library Manager:

* `Adafruit AMG88xx Library` (and `Adafruit BusIO`)
* `DHT sensor library` (by Adafruit, requires `Adafruit Unified Sensor`)
* `TinyGPSPlus` (by Mikal Hart)

### Wiring Details
* **GPS (NEO-6M):** Connects to `Serial1` (RX1 = Pin 19, TX1 = Pin 18)
* **I2C Sensors (AMG8833):** SDA = Pin 20, SCL = Pin 21
* **DHT11 Sensor:** Data pin connects to Digital Pin 5

### 💾 Extracting Data
Because data is stored on the internal EEPROM, you must export it to your computer via USB after the flight:
1. Connect the Arduino to your laptop via USB.
2. Open the **Arduino IDE Serial Monitor** (Set baud rate to 115200).
3. Type `E` and press enter to **Export** the data. The Arduino will print the CSV data which you can copy and paste into a file.
4. Type `C` and press enter to **Clear** the memory before your next flight.

## 📝 License

See the [LICENSE](LICENSE) file for details.