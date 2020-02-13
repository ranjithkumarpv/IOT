#include <LiquidCrystal.h>
LiquidCrystal LCD(2,3,4,5,6,7);
#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
int i;

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  LCD.begin(16,2);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("I am waiting for card...");
  pinMode(8,OUTPUT);
}

void loop() {
  {
    LCD.clear();
LCD.print("Insert the coin");  
  delay(1000);
  // put your main code here, to run repeatedly:
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(1000);
  LCD.clear();
LCD.print("Insert the coin");  
  
 if (strID.indexOf("4C:56:C7:73") >= 0) {  //put your own tap card key;
    for(i=5;i>0;i--)
    {
      digitalWrite(8,HIGH);
      Serial.println(i);
      LCD.clear();
      LCD.print("    CHARGING");
      LCD.setCursor(8,1);
      LCD.print(i);
      delay(1000);
      }
     Serial.println("Insert the coin");
     LCD.clear();
     digitalWrite(8,LOW);
     LCD.print("Insert the coin");  
  }
  else if (strID.indexOf("31:C1:64:A3") >= 0) {  //put your own tap card key;
    for(i=10;i>0;i--)
    {
      digitalWrite(8,HIGH);
      Serial.println(i);
       LCD.clear();
       LCD.print("    CHARGING");
       LCD.setCursor(8,1);
      LCD.print(i);
     
       delay(1000);
     }
     Serial.println("Insert the coin");
      LCD.clear();
     digitalWrite(8,LOW);
     LCD.print("Insert the coin");
  }
  else if (strID.indexOf("B2:4A:DE:73") >= 0) {  //put your own tap card key;
    for(i=15;i>0;i--)
    {
      digitalWrite(8,HIGH);
      Serial.println(i);
       LCD.clear();
       LCD.print("    CHARGING");
       LCD.setCursor(8,1);
      LCD.print(i);
     
      delay(1000);
      }
      Serial.println("Insert the coin");
      LCD.clear();
     digitalWrite(8,LOW);
       LCD.print("Insert the coin");
    
}
}
}
