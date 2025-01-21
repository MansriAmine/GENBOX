// Define the pins for the HC-SR04 sensor
#define TRIG_PIN 9
#define ECHO_PIN 10

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Set the trigger pin as output and the echo pin as input
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send a 10us pulse to trigger the measurement
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance
  float distance = (duration * 0.034) / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wait for a short period before the next measurement
  delay(330);
}
