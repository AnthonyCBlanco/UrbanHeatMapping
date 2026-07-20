# Urban Heat Mapping

A UAV-based thermal mapping payload and data pipeline designed to collect high-resolution surface temperature data and compare it against moderate-resolution NASA satellite datasets (like Landsat 8/9 and ECOSTRESS).

## 🚀 Project Overview

The goal of this project is to investigate the urban heat island effect by capturing localized, low-altitude thermal signatures using a custom drone payload. This ground-truth data is then processed and visualized alongside corresponding NASA satellite data to analyze the spatial and temporal differences in temperature readings.

## 🛠️ Hardware Payload

The data collection system is built on an **Arduino Uno R3** operating independently of the drone's primary flight controller. 

**Sensors & Components:**
* **AMG8833 IR Thermal Module:** Captures an 8x8 pixel array of surface temperatures.
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

* `Adafruit AMG88xx Library` (and `Adafruit BusIO`)
* `Adafruit BME280 Library` (and `Adafruit Unified Sensor`)
* `TinyGPSPlus` (by Mikal Hart)
* `SoftwareSerial` (built-in to Arduino IDE)

### Wiring Details
* **GPS (NEO-6M):** Connects via SoftwareSerial (RX = Pin 4, TX = Pin 3)
* **SD Card:** SPI interface, default Chip Select (`CS`) is Pin 10
* **I2C Sensors (AMG8833 & BME280):** SDA = A4, SCL = A5

## 📝 License

See the [LICENSE](LICENSE) file for details.