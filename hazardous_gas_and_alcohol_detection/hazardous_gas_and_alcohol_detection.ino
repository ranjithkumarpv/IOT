#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
SoftwareSerial mySerial(12,13);
char msg;


const int rs = 10, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int smokeA0 = A0; 
int smokeA1 = A1; 
int thresh=900;
int thresh1=700;
void setup()

{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module 
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(smokeA0,INPUT);
  pinMode(smokeA1,INPUT); 
  pinMode(7,OUTPUT);
  //Serial.println("s:send msg");
  //Serial.println("d:dail call");
  //Serial.println("r:receive message");
}

void loop()

{       lcd.clear();  
int analogSensor1 = analogRead(smokeA1);
      Serial.print("Pin A1: ");
      Serial.println(analogSensor1);
      int analogSensor = analogRead(smokeA0);
      Serial.print("Pin A0: ");
      Serial.println(analogSensor);  
       if(analogSensor1>thresh1)
      {
      lcd.setCursor(0,0);      
      lcd.print("alcoholdetd");
      //Serial.println("alcoholdetd");
    digitalWrite(7,HIGH);
       //lcd.print("  no" );
      SendMessage1();
      }  
      if(analogSensor>thresh)
      {
      lcd.setCursor(0,1);      
      lcd.print("hazard gas dtcd");
      digitalWrite(7,HIGH);
      SendMessage(); 
      } 
      
}
 void SendMessage()
{
  
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+6383578516\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("hazard gas dtcd");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
 // Serial.write(mySerial.read());

}
 void SendMessage1()
{
  
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+6383578516\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("alcohol detected");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
 // Serial.write(mySerial.read());

}
