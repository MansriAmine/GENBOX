#include <SPI.h>
#include <MFRC522.h>

// Broches du RC522
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// UID autorisé (remplace avec l'UID de ton badge)
const byte authorizedUID[4] = {0xED, 0xF9, 0x20, 0x24};

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("🔄 Système prêt. Approche ton badge RFID...");
}

void printUID(byte *uid, byte size) {
    Serial.print("Carte détectée, UID: ");
    for (byte i = 0; i < size; i++) {
        Serial.print(uid[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

// Vérifie si l'UID est autorisé
bool isAuthorized(byte *uid) {
    return memcmp(uid, authorizedUID, 4) == 0;
}

void loop() {
    // Message avant de scanner le badge
    Serial.println("🔄 En attente d'un badge...");
    delay(1000);  // Délai avant de commencer à scanner

    // Vérifie si une nouvelle carte est détectée
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    printUID(mfrc522.uid.uidByte, mfrc522.uid.size);

    if (isAuthorized(mfrc522.uid.uidByte)) {
        Serial.println("✅ Accès autorisé !");
    } else {
        Serial.println("⛔ Accès refusé !");
    }

    // Arrêter la communication avec la carte
    mfrc522.PICC_HaltA();
    
    delay(2000); // Temps entre chaque check du badge
}
