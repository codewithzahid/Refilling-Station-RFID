// need to send client information
#include <WiFi.h>
#include "AdafruitIO_WiFi.h"

#define IO_USERNAME "zahid23"
#define IO_KEY "aio_kntj82Rr91BRr04G4oGbWzC0C9Ly"
#define WIFI_SSID "Nihal"      // Wi-Fi credintial
#define WIFI_PASS "nihal@165"  // Password

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Create an Adafruit IO Feed
AdafruitIO_Feed *rechargeAmountserver = io.feed("recharge-amount");
AdafruitIO_Feed *recharge = io.feed("recharge");
AdafruitIO_Feed *getUserData = io.feed("get-user-info");
AdafruitIO_Feed *userInfo = io.feed("user-info");
AdafruitIO_Feed *remainingCredit = io.feed("remaining-credit");

bool stationState = true;
int getUserDataState;
int rechargeState;
int rechargeAmount = 0;
#include "CreditCard.h"

void setup() {
  Serial.begin(115200);
  io.connect();
  rechargeAmountserver->onMessage(rechargeAmounthandleMessage);
  recharge->onMessage(rechargehandleMessage);
  getUserData->onMessage(getUserDatahandleMessage);

  while (io.status() < AIO_CONNECTED) {
    delay(500);
    Serial.println("Connecting to the server...");
  }
  Serial.println("Connected to the server.");
  rechargeAmountserver->get();
  recharge->get();
  getUserData->get();
  cardInitialize();
}

void loop() {
  io.run();
  if (clientAcessStatus == false) {
    checkUserAccessID();
  }
  if (clientAcessStatus == true) {
    if (balanceChecked == false) {
      checkAvailableBalance();
    }
    if (balanceChecked == true) {
      if (rechargeState == 1) {
        rechargeBalance();
      }
    }
  }
  if (getUserDataState == 1) {
    if (userAccessDashboard == true) {
      userInfo->save("User Name: " + UserName + "\nUser ID: " + UserID);
    }
    if (userAccessDashboard == false) {
      userInfo->save("Access Denied! Please enter the valid card.");
    }
  }
}


void rechargeAmounthandleMessage(AdafruitIO_Data *data) {
  const char *stringValue = data->value();
  rechargeAmount = atoi(stringValue);
}

void rechargehandleMessage(AdafruitIO_Data *data) {
  const char *stringValue = data->value();
  rechargeState = atoi(stringValue);
}

void getUserDatahandleMessage(AdafruitIO_Data *data) {
  const char *stringValue = data->value();
  getUserDataState = atoi(stringValue);
}