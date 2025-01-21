#include "DHT.h"

// Define the pin where the data pin of DHT11 is connected
#define DHTPIN 2     // Digital pin 2

// Define the type of DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  // Start the DHT sensor
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements
  delay(1000);

  // Read temperature as Celsius (the default)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the results to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
}
