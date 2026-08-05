#include <DHT.h>

// Define the pin the DHT11 is connected to
const int dhtPin = 7; 

// Define the sensor type
#define DHTTYPE DHT11

// Initialize the DHT sensor
DHT dht(dhtPin, DHTTYPE);

void setup() {
  // Start the serial connection to talk to the computer
  Serial.begin(9600);
  
  // Wait a moment for the serial monitor to open
  while (!Serial) delay(10); 
  
  Serial.println(F("DHT11 Test Started! Waiting 3 seconds for sensor to warm up..."));
  delay(3000); // Give the sensor time to stabilize after power on
  
  // Start the sensor
  dht.begin();
}

void loop() {
  // The DHT11 is a slow sensor, you must wait at least 2 seconds between readings!
  delay(2000); 

  // Read the humidity (in percentage)
  float humidity = dht.readHumidity();
  
  // Read the temperature as Celsius
  float temperature = dht.readTemperature();

  // Check if any reads failed (this usually means a wiring issue)
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor! Check your wiring."));
    return;
  }

  // Print the results to the Serial Monitor
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  |  "));
  
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F(" °C"));
}
