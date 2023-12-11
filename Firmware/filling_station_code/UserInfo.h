String UserName = "Zahid Hasan";
int UserID = 202301;
String UIDAddress = "c0e60232";  // must need to encrypted the UID address

void scanClientInformation(String cardAddress) {
  if (cardAddress == UIDAddress) {
    Serial.println("Hi, " + UserName + ". Welcome to the Filling Station. Here your User ID: " + UserID);
  }
  else {
    Serial.println("Access Denied! Please enter the valid card.");
  }
}
