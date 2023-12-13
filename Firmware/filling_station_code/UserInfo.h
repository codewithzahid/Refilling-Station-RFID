#include "AdServer.h"

String UserName = "Zahid Hasan";
int UserID = 202301;
String UIDAddress = "c0e60232";  // must need to encrypted the UID address

void scanClientInformation(String cardAddress) {
  if (cardAddress == UIDAddress) {
    userData->save("User Name: " + UserName + "\nUser ID: " + UserID);
    Serial.println("Hi, " + UserName + ". Welcome to the Filling Station. Here your User ID: " + UserID);
  } else {
    Serial.println("Access Denied! Please enter the valid card.");
    userData->save("Access Denied! Please enter the valid card.");
  }
}
