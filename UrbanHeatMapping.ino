#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_MLX90640.h>
#include <Adafruit_BME280.h>
#include <TinyGPSPlus.h>

const int chipSelect = 53;

Adafruit_MLX90640 mlx;
Adafruit_BME280 bme;
TinyGPSPlus gps;
File dataFile;

float frame[32 * 24]; 

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  
  while (!Serial) delay(10);
  
  Serial.println("Urban Heat Mapping - Initialization Started");

  Wire.begin();
  Wire.setClock(400000); 

  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
  
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Timestamp,Latitude,Longitude,AmbientTemp_C,Humidity_Pct,Pressure_hPa,CenterIRTemp_C");
    dataFile.close();
  } else {
    Serial.println("Error opening datalog.csv");
  }

  Serial.println("Initializing MLX90640...");
  if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found! Check wiring.");
    while (1) delay(10);
  }
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_2_HZ);

  Serial.println("Initializing BME280...");
  if (!bme.begin(0x76, &Wire)) { 
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1) delay(10);
  }

  Serial.println("Initialization Complete.");
}

void loop() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }

  static unsigned long lastLogTime = 0;
  if (millis() - lastLogTime > 1000) {
    lastLogTime = millis();
    logData();
  }
}

void logData() {
  float ambientTemp = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  if (mlx.getFrame(frame) != 0) {
    Serial.println("Failed to read MLX90640 frame");
    return;
  }
  
  float centerIRTemp = frame[367]; 

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
    Serial.println("Error opening datalog.csv for writing");
  }
}
