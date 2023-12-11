#include "MasterCard.h"
const int fillingPumpPin = 12;
float currentAmount = 0.0;

void setup() {
  Serial.begin(115200);
  pinMode(fillingPumpPin, OUTPUT);
  cardInitialize();
  digitalWrite(fillingPumpPin, LOW);
}

void loop() {
  if(clientAcessStatus == false){
    checkUserAccessID();
  }
  else if(clientAcessStatus == true){
    checkAvailableBalance();
    delay(1000);
    float updatedBalance = availableBalance - 0.5;
    if(updatedBalance <= 15.0 && updatedBalance > 0.0){
      Serial.println("Pump on but balance is low");
      Serial.println("Balance is Low!");
    }
    else if(updatedBalance <= 0.0){
      Serial.println("Pump off");
      Serial.println("You don't have sufficient balance");
    }else{
      Serial.println("Pump on");
      updateBalance(updatedBalance);
    }
    delay(1000);
  }
}
