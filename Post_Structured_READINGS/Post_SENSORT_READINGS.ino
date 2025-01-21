#include "DHT.h"

// Define the pin where the data pin of DHT11 is connected
#define DHTPIN 2     // Digital pin 2 for DHT11

// Define the type of DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Define the pins for the HC-SR04 sensor
#define TRIG_PIN 5
#define ECHO_PIN 6

// Define the pins where the analog and digital outputs of MQ-7 are connected (Gas Sensor)
#define MQ7_ANALOG_PIN A0
#define MQ7_DIGITAL_PIN 3

// Define the pin where the digital output of SW-420 is connected (Vibration Sensor)
#define VIBRATION_SENSOR_PIN 4

// Arrays to hold 10 readings for each sensor (2 readings per second for 5 seconds)
float temperature[10], humidity[10], distance[10], gasAnalog[10], gasDigital[10], vibration[10];

// Timer to control the 500ms intervals and 5-second limit
unsigned long previousMillis = 0;
int counter = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MQ7_DIGITAL_PIN, INPUT);
  pinMode(VIBRATION_SENSOR_PIN, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Collect readings every 500 milliseconds
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;

    // Read sensors
    temperature[counter] = dht.readTemperature();
    humidity[counter] = dht.readHumidity();
    distance[counter] = getHCSR04Distance();
    gasAnalog[counter] = analogRead(MQ7_ANALOG_PIN);
    gasDigital[counter] = digitalRead(MQ7_DIGITAL_PIN);
    vibration[counter] = digitalRead(VIBRATION_SENSOR_PIN);

    // Increment counter to collect 10 readings (for 5 seconds)
    counter++;

    // After 10 readings (5 seconds), compile and send the data
    if (counter == 10) {
      postReadings();
      counter = 0; // Reset the counter for the next cycle
    }
  }
}

// Function to read HC-SR04 distance
float getHCSR04Distance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return (duration * 0.034) / 2;
}

void postReadings() {
  String dataString = "";
  
  // Compile data for each reading cycle (0 to 9)
  for (int i = 0; i < 10; i++) {
    dataString += "Temperature: " + String(temperature[i]) + "°C, Humidity: " + String(humidity[i]) + "%, ";
    dataString += "Distance: " + String(distance[i]) + "cm, ";
    dataString += "Gas Analog: " + String(gasAnalog[i]) + ", Gas Digital: " + String(gasDigital[i]) + ", ";
    dataString += "Vibration: " + String(vibration[i] == HIGH ? "Detected" : "Not Detected");
    dataString += "\n";  // Separate readings by newline
  }

  // Post the string (you can replace this with actual posting logic)
  Serial.println("=== Posting Readings ===");
  Serial.println(dataString);
}

