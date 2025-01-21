#include <PN532_HSU.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <SoftwareSerial.h>
#include <Servo.h>  // Library for controlling a servo motor
#include "DHT.h"

// Servo motor setup
Servo myServo;  // Create a Servo object
const int servoPin = 9;  // Define the servo pin

// SoftwareSerial for NFC communication (using Pins 10 and 11 for RX, TX)
SoftwareSerial SWSerial(10, 11);
PN532_SWHSU pn532swhsu(SWSerial);
PN532 nfc(pn532swhsu);

// Global variables for NFC tag reading
byte nuidPICC[7];        // UID for the NFC tag
unsigned long tagId = 0;  // Stores the tag ID as a number

// List of acceptable RFID tag IDs (replace with actual tag IDs)
const unsigned long acceptableTags[] = {
    1234567890,  // Replace with your actual tag ID
    2345678901,  // Add more tags here
    3456789012,
    3992526884,
    1091333959
};

// DHT sensor setup
#define DHTPIN 2     // Digital pin 2 for DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// HC-SR04 sensor setup
#define TRIG_PIN 5
#define ECHO_PIN 6

// MQ-7 Gas sensor setup
#define MQ7_ANALOG_PIN A0
#define MQ7_DIGITAL_PIN 3

// Vibration sensor setup
#define VIBRATION_SENSOR_PIN 4

// Arrays to hold 10 readings for each sensor
float temperature[10], humidity[10], distance[10], gasAnalog[10], gasDigital[10], vibration[10];

// Timer to control the 500ms intervals and 5-second limit
unsigned long previousMillis = 0;
int counter = 0;

// Setup function
void setup(void) {
    Serial.begin(9600);

    // Initialize NFC communication
    SWSerial.begin(115200);
    nfc.begin();

    // Initialize Servo motor
    myServo.attach(servoPin);  // Attach the servo to the defined pin
    myServo.write(0);  // Set initial position (0 degrees)

    // Initialize DHT sensor
    dht.begin();

    // Set up HC-SR04 and gas/vibration sensors
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(MQ7_DIGITAL_PIN, INPUT);
    pinMode(VIBRATION_SENSOR_PIN, INPUT);

    // Check for PN532 module
    while (true) {
        uint32_t versiondata = nfc.getFirmwareVersion();
        if (versiondata) {
            Serial.println("Found chip PN5");
            nfc.SAMConfig();  // Configure the NFC module in SAM mode
            Serial.println("Place the NFC tag to write data using Key A...");
            break;
        } else {
            Serial.println("Didn't find PN53x Module. Trying again...");
            delay(100); // Retry after a short delay
        }
    }
}

// Converts NFC tag byte array to a numeric value
unsigned long tagToNumber(byte id[7], uint8_t uidLength) {
    unsigned long tagId = 0;
    for (byte i = 0; i < uidLength; i++) {
        tagId = (tagId << 8) | id[i]; // Shift bits and add each byte
    }
    return tagId;
}

// Read NFC tag and return as a number
unsigned long readNFC() {
    uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 }; // Array for storing UID
    uint8_t uidLength;

    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 200)) {
        for (uint8_t i = 0; i < uidLength; i++) {
            nuidPICC[i] = uid[i]; // Store the read UID
        }
        tagId = tagToNumber(nuidPICC, uidLength); // Convert to numeric
    }
    return tagId;
}

// Check if the scanned tag is in the list of acceptable tags
bool isTagAccepted(unsigned long tag) {
    for (int i = 0; i < sizeof(acceptableTags) / sizeof(acceptableTags[0]); i++) {
        if (acceptableTags[i] == tag) {
            return true;  // Tag is found in the list
        }
    }
    return false;  // Tag is not found
}

// Function to handle NFC tag reading and servo control
void processNFC() {
    Serial.println("Waiting for NFC tag...");

    // Read and display NFC tag
    unsigned long Tag = readNFC();
    if (Tag != 0) {
        Serial.print("Tag ID (Numeric): ");
        Serial.println(Tag);

        // Check if the tag is accepted
        if (isTagAccepted(Tag)) {
            //Tag = false ;
            Serial.println("Authorized tag! Moving servo.");
            myServo.write(90);  // Move the servo to 90 degrees (example action)
            delay(2000);        // Keep the servo in the new position for 2 seconds
            myServo.write(0);   // Return the servo to 0 degrees
        } else {
            Serial.println("Unauthorized tag.");
        }
        tagId = 0 ;
    }

    // Delay to avoid reading the same tag multiple times
    //delay(1000);
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

// Collect and post readings from the sensors
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

void loop() {
  unsigned long currentMillis = millis();

  // Collect readings from sensors every 500 milliseconds
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

  // Independently process NFC tags
  processNFC();
}
