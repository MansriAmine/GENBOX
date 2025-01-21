#include <PN532_HSU.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <SoftwareSerial.h>
#include <Servo.h>  // Library for controlling a servo motor

// SoftwareSerial for NFC communication (using Pins 10 and 11 for RX, TX)
SoftwareSerial SWSerial(10, 11); 
PN532_SWHSU pn532swhsu(SWSerial);
PN532 nfc(pn532swhsu);

// Global variables for NFC tag reading
byte nuidPICC[7];        // UID for the NFC tag
unsigned long tagId = 0;  // Stores the tag ID as a number

// Servo motor setup
Servo myServo;  // Create a Servo object
const int servoPin = 9;  // Define the servo pin

// List of acceptable RFID tag IDs (replace with actual tag IDs)
const unsigned long acceptableTags[] = {
    1234567890,  // Replace with your actual tag ID
    2345678901,  // Add more tags here
    3456789012,
    3992526884,
    1091333959
};

void setup(void) {
    // Start serial communication for debugging and NFC
    Serial.begin(9600);

    // Initialize NFC communication
    SWSerial.begin(115200);
    nfc.begin();

    // Initialize Servo motor
    myServo.attach(servoPin);  // Attach the servo to the defined pin
    myServo.write(0);  // Set initial position (0 degrees)

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

// Function to handle NFC tag reading and output
void processNFC() {
    Serial.println("Waiting for NFC tag...");

    // Read and display NFC tag
    unsigned long Tag = readNFC();
    if (Tag != 0) {
        Serial.print("Tag ID (Numeric): ");
        Serial.println(Tag);

        // Check if the tag is accepted
        if (isTagAccepted(Tag)) {
            Serial.println("Authorized tag! Moving servo.");
            myServo.write(90);  // Move the servo to 90 degrees (example action)
            delay(2000);        // Keep the servo in the new position for 2 seconds
            myServo.write(0);   // Return the servo to 0 degrees
        } else {
            Serial.println("Unauthorized tag.");
        }
    }

    // Delay to avoid reading the same tag multiple times
    delay(1000);
}

void loop(void) {
    processNFC(); // Call the function to handle NFC tag processing
}
