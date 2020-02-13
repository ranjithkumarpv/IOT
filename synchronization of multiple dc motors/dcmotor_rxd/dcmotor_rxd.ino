#include <LiquidCrystal.h>
LiquidCrystal lcd(3,4,5,6,9,10);
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   7
#define CSN_PIN 8

const byte thisSlaveAddress = 'R';

RF24 radio(CE_PIN, CSN_PIN);

int dataReceived; // this must match dataToSend in the TX
bool newData = false;


void setup() 
  {

    Serial.begin(9600);
    lcd.begin(16,2);
    lcd.println(" Dc Motor sync ");
    delay(500);
    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(0, thisSlaveAddress);
    radio.startListening();
    pinMode(2,OUTPUT);
 //   pinMode(3,OUTPUT);
    lcd.clear();
   }

void loop() 
  {
    getData();
    showData();
  }

void getData() 
    {
        if ( radio.available() ) 
            {
                radio.read( &dataReceived, sizeof(dataReceived) );
                newData = true;
            }
    }

void showData()
    {
        if (newData == true) 
            {
             Serial.print("Data received ");
             Serial.println(dataReceived);
                  if(dataReceived==1)
                      {
                        Serial.println("hii");
                        digitalWrite(2,HIGH);
//  digitalWrite(3,LOW);
lcd.clear();
 // delay(1000);

    
   lcd.println("   SYNCHRONUS  MODE      ");
   lcd.setCursor(0,1);
   lcd.println("      v1=5v v2=5v           ");
   delay(1000);
                      }
                  else if(dataReceived==0)
                      {
                       digitalWrite(2,LOW);
  //digitalWrite(3,LOW);
  lcd.clear();
//  delay(1000);

    
              lcd.println("  ASYNCHRONOUS MODE  ");
              lcd.setCursor(0,1);
              lcd.println("      v1=0v v2=5v      ");
              delay(1000);
             
                      }
                      else if(dataReceived==2)
                      {
                        digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  lcd.clear();
  //delay(1000);
    
  lcd.println("    OFF MODE         ");
  lcd.setCursor(0,1);
  lcd.println("      v1=0v v2=0v      "); 
      delay(1000);                  
                      }
            
            
            newData = false;
          }
    }
