// Define the pins where the analog and digital outputs of MQ-7 are connected
#define MQ7_ANALOG_PIN A0
#define MQ7_DIGITAL_PIN 2

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Set the digital output pin as input (optional)
  pinMode(MQ7_DIGITAL_PIN, INPUT);
}

void loop() {
  // Read the analog value from the MQ-7 sensor
  int analogValue = analogRead(MQ7_ANALOG_PIN);

  // Read the digital value from the MQ-7 sensor (optional)
  int digitalValue = digitalRead(MQ7_DIGITAL_PIN);

  // Print the results to the Serial Monitor
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print("  Digital Value: ");
  Serial.println(digitalValue);

  // Wait for a short period before reading again
  delay(1000);
}
