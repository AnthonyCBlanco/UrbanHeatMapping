#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_AMG88xx.h>
#include <Adafruit_BME280.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#define SIMULATION_MODE true // Set to false when connecting real hardware

const int chipSelect = 10; // Uno R3 default SPI SS
const int rxPin = 4;       // SoftwareSerial RX for GPS
const int txPin = 3;       // SoftwareSerial TX for GPS

SoftwareSerial ss(rxPin, txPin);

Adafruit_AMG88xx amg;
Adafruit_BME280 bme;
TinyGPSPlus gps;
File dataFile;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE]; // 64 floats (256 bytes)

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
  
  while (!Serial) delay(10);
  
  // Use F() macro to store literal strings in flash memory, saving SRAM
  Serial.println(F("Urban Heat Mapping - Initialization Started"));

  Wire.begin();

  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    while (1);
  }
  Serial.println(F("card initialized."));
  
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(F("Timestamp,Latitude,Longitude,AmbientTemp_C,Humidity_Pct,Pressure_hPa,CenterIRTemp_C"));
    dataFile.close();
  } else {
    Serial.println(F("Error opening datalog.csv"));
  }

  Serial.println(F("Initializing AMG8833..."));
#if SIMULATION_MODE
  Serial.println(F("SIMULATION MODE: Bypassing AMG8833 check."));
#else
  if (!amg.begin()) {
    Serial.println(F("AMG8833 not found! Check wiring."));
    while (1) delay(10);
  }
#endif

  Serial.println(F("Initializing BME280..."));
#if SIMULATION_MODE
  Serial.println(F("SIMULATION MODE: Bypassing BME280 check."));
#else
  if (!bme.begin(0x76, &Wire)) { 
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1) delay(10);
  }
#endif

  Serial.println(F("Initialization Complete."));
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  static unsigned long lastLogTime = 0;
  if (millis() - lastLogTime > 1000) {
    lastLogTime = millis();
    logData();
  }
}

void logData() {
#if SIMULATION_MODE
  float ambientTemp = 25.0 + random(-20, 20) / 10.0;
  float humidity = 45.0 + random(-50, 50) / 10.0;
  float pressure = 1012.0 + random(-10, 10) / 10.0;
  
  for(int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    pixels[i] = ambientTemp + 2.0 + random(-5, 15) / 10.0;
  }
#else
  float ambientTemp = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  amg.readPixels(pixels);
#endif
  
  // Extract center temperature (average of the middle 4 pixels of the 8x8 array)
  float centerIRTemp = (pixels[27] + pixels[28] + pixels[35] + pixels[36]) / 4.0; 

  String timestamp = "NO_TIME";
  if (gps.time.isValid()) {
    char timeStr[10];
    sprintf(timeStr, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
    timestamp = String(timeStr);
  }

  float lat = gps.location.isValid() ? gps.location.lat() : 0.0;
  float lng = gps.location.isValid() ? gps.location.lng() : 0.0;

  String dataString = timestamp + ",";
  dataString += String(lat, 6) + ",";
  dataString += String(lng, 6) + ",";
  dataString += String(ambientTemp, 2) + ",";
  dataString += String(humidity, 2) + ",";
  dataString += String(pressure, 2) + ",";
  dataString += String(centerIRTemp, 2);

  Serial.println(dataString);

  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  } else {
    Serial.println(F("Error opening datalog.csv for writing"));
  }
}
