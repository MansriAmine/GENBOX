#include <PN532_HSU.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <SoftwareSerial.h>

// SoftwareSerial for NFC communication (using Pins 52 and 53 for RX, TX)
SoftwareSerial SWSerial(10, 11); 
PN532_SWHSU pn532swhsu(SWSerial);
PN532 nfc(pn532swhsu);

// Global variables for NFC tag reading
byte nuidPICC[7];        // UID for the NFC tag
unsigned long tagId = 0;  // Stores the tag ID as a number

void setup(void) {
    // Start serial communication for debugging and NFC
    Serial.begin(9600);

    // Initialize NFC communication
    SWSerial.begin(115200);
    nfc.begin();

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

// Function to handle NFC tag reading and output
void processNFC() {
    Serial.println("Waiting for NFC tag...");

    // Read and display NFC tag
    unsigned long Tag = readNFC();
    if (Tag != 0) {
        Serial.print("Tag ID (Numeric): ");
        Serial.println(Tag);
    }

    // Delay to avoid reading the same tag multiple times
    delay(1000);
}

void loop(void) {
    processNFC(); // Call the function to handle NFC tag processing
}
