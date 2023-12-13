#include "MasterCard.h"

const int fillingPumpPin = 12;
float currentAmount = 0.0;
unsigned long lastcheck = 0, lastcheck2 = 0, lastcheck3 = 0, lastcheck4 = 0;

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

    if(millis() - lastcheck > 50000){
      putExpenseToServer(updatedBalance);
      lastcheck = millis();
    }
    if(updatedBalance <= 15.0 && updatedBalance > 0.0){
      digitalWrite(fillingPumpPin, HIGH);
      if(millis() - lastcheck2 > 50000){
      putStationStatusToServer(1);
      putCreditStatusToSever(updatedBalance);
      lastcheck2 = millis();
    }
      Serial.println("Balance is Low!");
      updateBalance(updatedBalance);
    }
    else if(updatedBalance <= 0.0){
      if(millis() - lastcheck3 > 50000){
      putStationStatusToServer(0);
      userData->save("Connection Lost or maybe don't have sufficient balance!");
      lastcheck3 = millis();
    }
      digitalWrite(fillingPumpPin, LOW);
      Serial.println("Connection Lost or maybe don't have sufficient balance!");
      clientAcessStatus = false;
    }else{
      if(millis() - lastcheck4 > 50000){
      putStationStatusToServer(1);
      putCreditStatusToSever(updatedBalance);
      lastcheck4 = millis();
    }
      
      digitalWrite(fillingPumpPin, HIGH);
      Serial.println("Pump on");
      updateBalance(updatedBalance);
    }
    delay(1000);
  }
}
