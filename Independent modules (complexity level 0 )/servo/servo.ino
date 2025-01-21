#include <Servo.h>

// Create a Servo object
Servo myServo;

// Define the pin for the servo motor
const int servoPin = 9;  // Adjust this if you are using a different pin

void setup() {
  // Attach the servo motor to the pin
  myServo.attach(servoPin);
}

void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += 1) {
    myServo.write(angle);  // Set the servo to the current angle
    delay(15);             // Wait for the servo to reach the angle
  }

  // Sweep the servo from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 1) {
    myServo.write(angle);  // Set the servo to the current angle
    delay(15);             // Wait for the servo to reach the angle
  }
}
