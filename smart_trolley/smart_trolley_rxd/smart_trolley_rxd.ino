 #include <LiquidCrystal.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   7
#define CSN_PIN 8
LiquidCrystal lcd(2,3,4,5,9,10);
int count,tot=0;
char input[12];
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

int dataReceived[5]; // this must match dataToSend in the TX
bool newData = false;


void setup() 
  {

    Serial.begin(9600);
    
    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(0, thisSlaveAddress);
    radio.startListening();
    }

void loop() 
  {
    huu();
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
           
             Serial.println(dataReceived[1]);
             if(dataReceived[1]==30)
             {
              tot=tot-30;
              Serial.println(tot);
              lcd.clear();
              lcd.print(tot);
              
              delay(1000);
             }
               Serial.println(dataReceived[2]);
                if(dataReceived[2]==30)
                {
                  tot=tot-30;
              Serial.println(tot);
              lcd.clear();
              lcd.print(tot);
              
              delay(1000);
             
                }
             Serial.println(dataReceived[3]);
             if(dataReceived[3]==40)
             {
              tot=tot-40;
              Serial.println(tot);
              lcd.clear();
              lcd.print(tot);
              delay(1000);
             
             }
                     
            Serial.println(dataReceived[4]);
                            newData = false;
          }
    }
void huu()
    {
      if(Serial.available()>0)
      {
      count = 0;
      while(Serial.available() && count < 12)          // Read 12 characters and store them in input array
      {
         input[count] = Serial.read();
         count++;
         delay(5);
      }
      if(input[11]=='B')
      {
        tot=tot+30;
        Serial.print("Total=");
        Serial.println(tot);
        delay(500);
       }
      else if(input[11]=='6')
      {
       tot=tot+30;
       Serial.print("Total=");
       Serial.println(tot);
       delay(500);
       }
      else if(input[11]=='A')
      {
       tot=tot+40;
       Serial.print("Total=");
       Serial.println(tot);
       delay(500);
       }
      }      }
    
