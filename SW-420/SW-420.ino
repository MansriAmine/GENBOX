// Define the pin where the digital output of SW-420 is connected
#define VIBRATION_SENSOR_PIN 2

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Set the vibration sensor pin as input
  pinMode(VIBRATION_SENSOR_PIN, INPUT);
}

void loop() {
  // Read the state of the vibration sensor
  int sensorState = digitalRead(VIBRATION_SENSOR_PIN);

  // Check if vibration is detected
  if (sensorState == HIGH) {
    // Print the result to the Serial Monitor
    Serial.println("Vibration detected!");
  } else {
    // Print the result to the Serial Monitor
    Serial.println("No vibration.");
  }

  // Wait for a short period before reading again
  delay(500);
}
