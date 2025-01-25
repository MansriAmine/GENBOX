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

  // Calculate total brightness (sum of RGB values)
  int totalBrightness = R + G + B;

  // Avoid division by zero
  if (totalBrightness == 0) {
    totalBrightness = 1;
  }

  // Normalize RGB values based on total brightness
  float normalizedR = (float)R / totalBrightness;
  float normalizedG = (float)G / totalBrightness;
  float normalizedB = (float)B / totalBrightness;

  // Print normalized RGB values to Serial Monitor
  Serial.print("Normalized R: ");
  Serial.print(normalizedR, 3); // Print up to 3 decimal places
  Serial.print(" | Normalized G: ");
  Serial.print(normalizedG, 3);
  Serial.print(" | Normalized B: ");
  Serial.println(normalizedB, 3);

  // Detect colors based on normalized RGB ratios
  if (normalizedR > 0.3 && normalizedG > 0.3 && normalizedB < 0.2) {
    Serial.println("Detected Color: YELLOW");

    // Move servo to 90 degrees if yellow is detected
    myServo.write(90);
    delay(1000);  // Hold position for 1 second
    myServo.write(0);  // Reset servo to 0 degrees
  } else if (normalizedR > 0.8 && normalizedG > 0.8 && normalizedB > 0.8) {
    Serial.println("Detected Color: WHITE");
  } else if (normalizedR < 0.1 && normalizedG < 0.1 && normalizedB < 0.1) {
    Serial.println("Detected Color: BLACK");
  } else if (normalizedR > 0.4 && normalizedG < 0.2 && normalizedB < 0.2) {
    Serial.println("Detected Color: RED");
  } else if (normalizedB > normalizedR && normalizedB > normalizedG) {
    Serial.println("Detected Color: BLUE");
  } else {
    Serial.println("Detected Color: UNKNOWN");
  }

  delay(200);
}
