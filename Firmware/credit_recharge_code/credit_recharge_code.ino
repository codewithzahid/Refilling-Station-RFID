#include "CreditCard.h"

void setup() {
  Serial.begin(115200);
  cardInitialize();
}

void loop() {
  if (clientAcessStatus == false) {
    checkUserAccessID();
  } else if (clientAcessStatus == true) {
    if (balanceChecked == false) {
      checkAvailableBalance();
    }
    if(balanceChecked == true){
      rechargeBalance();
    }
  }
}