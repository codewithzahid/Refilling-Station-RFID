#include <SPI.h>
#include <MFRC522.h>
#include "UserInfo.h"

#define SS_PIN 21  // SDA
#define RST_PIN 5  // RST

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

unsigned long lastReadTime = 0;
bool clientAcessStatus = false;
bool balanceChecked = false;
float availableBalance = 0.0;

int block = 2;
byte floatBytes[4];

void cardInitialize() {
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RC522 initialized");
  delay(100);
  // Prepare the security key for the read and write functions.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  delay(2000);
  Serial.println("Enter your card: ");
}

void checkUserAccessID() {
  if (millis() - lastReadTime >= 1000) {
    lastReadTime = millis();
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      Serial.println("User card detected!");
      delay(1000);
      // read uid from the card
      String uidString = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uidString += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
      }
      scanClientInformation(uidString);
      clientAcessStatus = true;
    }
  }
}

// authentication
MFRC522::StatusCode authenticateCard(int blockNumber) {
  int largestModulo4Number = blockNumber / 4 * 4;
  int trailerBlock = largestModulo4Number + 3;

  // Authenticate the card
  return mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
}

// write block
MFRC522::StatusCode writeBlock(int blockNumber, byte arrayAddress[]) {
  // Check if it's a data block
  if (blockNumber > 2 && (blockNumber + 1) % 4 == 0) {
    Serial.println("Error: Trying to write to a trailer block");
    return MFRC522::STATUS_ERROR;
  }

  // Authenticate the card
  MFRC522::StatusCode authStatus = authenticateCard(blockNumber);
  if (authStatus != MFRC522::STATUS_OK) {
    return authStatus;
  }

  // Write to the block
  return mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);
}

// read block
MFRC522::StatusCode readBlock(int blockNumber, byte arrayAddress[]) {
  MFRC522::StatusCode authStatus = authenticateCard(blockNumber);
  if (authStatus != MFRC522::STATUS_OK) {
    return authStatus;
  }
  byte bufferSize = 18;
  return mfrc522.MIFARE_Read(blockNumber, arrayAddress, &bufferSize);
}

float readFloatFromBlock(int blockNumber) {
  byte floatBytes[4];
  readBlock(blockNumber, floatBytes);
  float readFloatValue;
  memcpy(&readFloatValue, floatBytes, sizeof(float));
  return readFloatValue;
}

void updateBalance(float currentAmount) {
  byte floatBytes[4];
  memcpy(floatBytes, &currentAmount, sizeof(float));
  MFRC522::StatusCode writeStatus = writeBlock(block, floatBytes);
}

void checkAvailableBalance() {
  availableBalance = readFloatFromBlock(block);
  Serial.print("Balance: BDT ");
  Serial.print(availableBalance);
  Serial.println();
}