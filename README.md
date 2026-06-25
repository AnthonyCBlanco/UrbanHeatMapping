# Urban Heat Mapping

A UAV-based thermal mapping payload and data pipeline designed to collect high-resolution surface temperature data and compare it against moderate-resolution NASA satellite datasets (like Landsat 8/9 and ECOSTRESS).

## 🚀 Project Overview

The goal of this project is to investigate the urban heat island effect by capturing localized, low-altitude thermal signatures using a custom drone payload. This ground-truth data is then processed and visualized alongside corresponding NASA satellite data to analyze the spatial and temporal differences in temperature readings.

## 🛠️ Hardware Payload

The data collection system is built on an **Arduino Mega 2560** operating independently of the drone's primary flight controller. 

**Sensors & Components:**
* **MLX90640 IR Thermal Module:** Captures a 32x24 pixel array of surface temperatures.
* **BME280 Sensor:** Records ambient temperature, humidity, and barometric pressure.
* **NEO-6M GPS Module:** Provides exact geospatial coordinates and timestamps for all readings.
* **MicroSD Card Breakout:** Logs the synchronized data streams locally to a CSV file to prevent data loss.

## 💻 Software Architecture

The project is divided into several interconnected systems:

1. **On-Board Data Logging (C++):** An Arduino sketch (`UrbanHeatMapping.ino`) that polls the thermal and ambient sensors alongside the GPS unit, writing structured CSV data to an SD card during flight.
2. **Data Processing Pipeline (Python):** Scripts to extract, clean, and format the raw flight logs into GeoJSON or similar web-ready formats. *(In Development)*
3. **NASA API Integration (Python):** Tools to query NASA Earthdata APIs or Google Earth Engine for corresponding thermal satellite imagery. *(In Development)*
4. **Web Dashboard (HTML/JS):** An interactive Leaflet.js map for visualizing the drone's heat map overlay against the satellite data. *(In Development)*

## 🚦 Getting Started with the Payload

### Prerequisites

To compile and upload the payload sketch, you'll need the Arduino IDE and the following libraries installed via the Library Manager:

* `Adafruit MLX90640` (and `Adafruit BusIO`)
* `Adafruit BME280 Library` (and `Adafruit Unified Sensor`)
* `TinyGPSPlus` (by Mikal Hart)

### Wiring Details
* **GPS (NEO-6M):** Connects to `Serial1` (RX1 = Pin 19, TX1 = Pin 18)
* **SD Card:** SPI interface, default Chip Select (`CS`) is Pin 53
* **I2C Sensors (MLX90640 & BME280):** SDA = Pin 20, SCL = Pin 21

## 📝 License

See the [LICENSE](LICENSE) file for details.