#include <Servo.h>

#define S0 10
#define S1 11
#define S2 12
#define S3 13
#define OUT 14

#define LedRed    A2
#define LedGreen  A0
#define LedBlue   A1

int R, G, B = 0;

// Create a servo object
Servo myServo;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Set LED pins as outputs
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(LedBlue, OUTPUT);

  // Set Serial communication
  Serial.begin(9600);

  // Attach servo to pin 7
  myServo.attach(7);

  // Set LED brightness using PWM (values range from 0 to 255)
  analogWrite(LedRed, 128);   // Reduce Red LED brightness
  analogWrite(LedGreen, 128); // Reduce Green LED brightness
  analogWrite(LedBlue, 128);  // Reduce Blue LED brightness
}

void loop() {
  // Reading Red filtered photodiodes
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  R = pulseIn(OUT, LOW);  // Reading the Red frequency
  delay(100);

  // Reading Green filtered photodiodes
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  G = pulseIn(OUT, LOW);  // Reading the Green frequency
  delay(100);

  // Reading Blue filtered photodiodes
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  B = pulseIn(OUT, LOW);  // Reading the Blue frequency
  delay(100);

  // Print RGB Sensor Values
  Serial.print("R= ");
  Serial.print(R);
  Serial.print(" | ");
  Serial.print("G= ");
  Serial.print(G);
  Serial.print(" | ");
  Serial.print("B= ");
  Serial.println(B);

  // Detect colors based on RGB values and print the result
  if (R > 250 && G > 250 && B > 250 && R < 350 && G < 350 && B < 350) {
    Serial.println("Detected Color: WHITE");
  } else if (R > 2100 && G > 2050 && B > 1650 && R < 2200 && G < 2150 && B < 1750) {
    Serial.println("Detected Color: BLACK");
  } else if (R > 200 && G > 400 && B > 800 && R < 300 && G < 550 && B < 950) {
    Serial.println("Detected Color: YELLOW");
    
    // Move servo to 90 degrees if yellow is detected
    myServo.write(90);
    delay(1000);  // Hold position for 1 second
    myServo.write(0);  // Reset servo to 0 degrees
  } else if (R > 250 && G > 1250 && B > 950 && R < 350 && G < 1350 && B < 1050) {
    Serial.println("Detected Color: RED");
  } else if (R > 1250 && G > 800 && B > 450 && R < 1350 && G < 900 && B < 550) {
    Serial.println("Detected Color: BLUE");
  } else {
    Serial.println("Detected Color: UNKNOWN");
  }

  delay(200);
}
