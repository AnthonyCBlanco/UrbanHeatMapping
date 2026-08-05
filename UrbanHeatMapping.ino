#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start Serial for Web Logger communication
  Serial.begin(115200);
  
  // Initialize DHT11
  dht.begin();
  
  Serial.println("SYSTEM_READY");
}

void loop() {
  // Read humidity and temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius
  
  // Check if readings failed
  if (isnan(h) || isnan(t)) {
    Serial.println("ERROR:Failed to read from DHT sensor!");
  } else {
    // Print in a simple, easy to parse CSV format for the Web Logger
    // Format: TEMP,HUMIDITY
    Serial.print(t);
    Serial.print(",");
    Serial.println(h);
  }
  
  // Wait 2 seconds between measurements (DHT11 limitation)
  delay(2000);
}
