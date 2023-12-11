// need to send client information
#include <WiFi.h>
#include "AdafruitIO_WiFi.h"

#define IO_USERNAME "zahid23"
#define IO_KEY "aio_kntj82Rr91BRr04G4oGbWzC0C9Ly"
#define WIFI_SSID "Nihal"       // Wi-Fi credintial
#define WIFI_PASS "nihal@165"   // Password

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Create an Adafruit IO Feed
AdafruitIO_Feed *remainingCredit = io.feed("remaining-credit");
AdafruitIO_Feed *lastExpense = io.feed("last-expense");
AdafruitIO_Feed *StationStatus = io.feed("filling-station-status");

bool stationState = true;

void serverInitialization() {
  // Connect to Wi-Fi
  io.connect();
  // Wait for connection
  while (io.status() < AIO_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Adafruit IO...");
  }
  Serial.println("Connected to Adafruit IO");
}

void putCreditStatusToSever(float currentBalance) {
  remainingCredit->save(currentBalance);
}

void putExpenseToServer(float lastExpenseCredit) {
  lastExpense->save(lastExpenseCredit);
}

void putStationStatusToServer(bool currentState){
  StationStatus->save(currentState);
}