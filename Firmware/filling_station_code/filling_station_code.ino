#include "MasterCard.h"

const int fillingPumpPin = 12;
float currentAmount = 0.0;

void setup() {
  Serial.begin(115200);
  pinMode(fillingPumpPin, OUTPUT);
  serverInitialization();
  cardInitialize();
  digitalWrite(fillingPumpPin, LOW);
  putStationStatusToServer(0);
}

void loop() {
  if(clientAcessStatus == false){
    checkUserAccessID();
  }
  else if(clientAcessStatus == true){
    checkAvailableBalance();
    delay(1000);
    float updatedBalance = availableBalance - 0.5;
    putExpenseToServer(updatedBalance);
    if(updatedBalance <= 15.0 && updatedBalance > 0.0){
      digitalWrite(fillingPumpPin, HIGH);
      putStationStatusToServer(1);
      Serial.println("Balance is Low!");
      updateBalance(updatedBalance);
      putCreditStatusToSever(updatedBalance);
    }
    else if(updatedBalance <= 0.0){
      digitalWrite(fillingPumpPin, LOW);
      putStationStatusToServer(0);
      userData->save("Connection Lost or maybe don't have sufficient balance!");
      Serial.println("Connection Lost or maybe don't have sufficient balance!");
      clientAcessStatus = false;
    }else{
      putStationStatusToServer(1);
      digitalWrite(fillingPumpPin, HIGH);
      Serial.println("Pump on");
      updateBalance(updatedBalance);
      putCreditStatusToSever(updatedBalance);
    }
    delay(1000);
  }
}
