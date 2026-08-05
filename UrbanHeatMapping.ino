#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_AMG88xx.h>
#include <DHT.h>
#include <TinyGPSPlus.h>

#define SIMULATION_MODE false // Set to false when connecting real hardware

const int dhtPin = 5;      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11

Adafruit_AMG88xx amg;
DHT dht(dhtPin, DHTTYPE);
TinyGPSPlus gps;

// EEPROM Storage Configuration
struct LogRecord {
  uint32_t timeHHMMSS;
  float lat;
  float lon;
  float ambTemp;
  float hum;
  float irTemp;
};

// 4096 bytes EEPROM on Mega 2560. First 2 bytes store the recordCount.
const int MAX_RECORDS = (4096 - sizeof(uint16_t)) / sizeof(LogRecord); 
uint16_t recordCount = 0;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE]; // 64 floats (256 bytes)

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // Hardware Serial 1 for GPS on Mega 2560
  
  while (!Serial) delay(10);
  
  Serial.println(F("Urban Heat Mapping - Initialization Started"));
  Serial.println(F("Send 'E' to Export data via USB."));
  Serial.println(F("Send 'C' to Clear memory."));

  // Load record count from EEPROM address 0
  EEPROM.get(0, recordCount);
  if (recordCount == 0xFFFF || recordCount > MAX_RECORDS) {
    recordCount = 0; // Initialize if empty or corrupt
  }
  
  Serial.print(F("Current records in memory: "));
  Serial.print(recordCount);
  Serial.print(F(" / "));
  Serial.println(MAX_RECORDS);

  Wire.begin();

  Serial.println(F("Initializing AMG8833..."));
#if SIMULATION_MODE
  Serial.println(F("SIMULATION MODE: Bypassing AMG8833 check."));
#else
  if (!amg.begin()) {
    Serial.println(F("AMG8833 not found! Check wiring."));
    while (1) delay(10);
  }
#endif

  Serial.println(F("Initializing DHT11..."));
#if SIMULATION_MODE
  Serial.println(F("SIMULATION MODE: Bypassing DHT11 check."));
#else
  dht.begin();
#endif

  Serial.println(F("Initialization Complete."));
}

void loop() {
  // Check for USB commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'E' || cmd == 'e') {
      exportData();
    } else if (cmd == 'C' || cmd == 'c') {
      clearData();
    }
  }

  // Parse GPS
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }

  static unsigned long lastLogTime = 0;
  if (millis() - lastLogTime > 5000) { // Log every 5 seconds to conserve memory
    lastLogTime = millis();
    logData();
  }
}

void logData() {
  if (recordCount >= MAX_RECORDS) {
    Serial.println(F("Storage FULL! Stop logging."));
    return;
  }

  LogRecord record;

#if SIMULATION_MODE
  record.ambTemp = 25.0 + random(-20, 20) / 10.0;
  record.hum = 45.0 + random(-50, 50) / 10.0;
  
  for(int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    pixels[i] = record.ambTemp + 2.0 + random(-5, 15) / 10.0;
  }
#else
  record.ambTemp = dht.readTemperature();
  record.hum = dht.readHumidity();
  
  if (isnan(record.ambTemp)) record.ambTemp = 0.0;
  if (isnan(record.hum)) record.hum = 0.0;

  amg.readPixels(pixels);
#endif
  
  record.irTemp = (pixels[27] + pixels[28] + pixels[35] + pixels[36]) / 4.0; 

  record.timeHHMMSS = 0;
  if (gps.time.isValid()) {
    record.timeHHMMSS = gps.time.hour() * 10000UL + gps.time.minute() * 100UL + gps.time.second();
  } else {
    // If no GPS time, just log millis as a fallback indicator (converted to mock HHMMSS)
    record.timeHHMMSS = (millis() / 1000) % 100; 
  }

  record.lat = gps.location.isValid() ? gps.location.lat() : 0.0;
  record.lon = gps.location.isValid() ? gps.location.lng() : 0.0;

  // Save to EEPROM
  int address = sizeof(uint16_t) + (recordCount * sizeof(LogRecord));
  EEPROM.put(address, record);
  
  recordCount++;
  EEPROM.put(0, recordCount); // Update counter

  Serial.print(F("Logged record "));
  Serial.print(recordCount);
  Serial.print(F("/"));
  Serial.println(MAX_RECORDS);
}

void exportData() {
  Serial.println(F("--- EXPORT START ---"));
  Serial.println(F("Timestamp,Latitude,Longitude,AmbientTemp_C,Humidity_Pct,CenterIRTemp_C"));
  
  for (uint16_t i = 0; i < recordCount; i++) {
    LogRecord record;
    int address = sizeof(uint16_t) + (i * sizeof(LogRecord));
    EEPROM.get(address, record);
    
    // Format timestamp back to HH:MM:SS
    char timeStr[10];
    uint8_t h = record.timeHHMMSS / 10000;
    uint8_t m = (record.timeHHMMSS / 100) % 100;
    uint8_t s = record.timeHHMMSS % 100;
    sprintf(timeStr, "%02d:%02d:%02d", h, m, s);
    
    Serial.print(timeStr); Serial.print(",");
    Serial.print(record.lat, 6); Serial.print(",");
    Serial.print(record.lon, 6); Serial.print(",");
    Serial.print(record.ambTemp, 2); Serial.print(",");
    Serial.print(record.hum, 2); Serial.print(",");
    Serial.println(record.irTemp, 2);
  }
  Serial.println(F("--- EXPORT END ---"));
}

void clearData() {
  recordCount = 0;
  EEPROM.put(0, recordCount);
  Serial.println(F("Memory cleared. Ready for new flight."));
}
