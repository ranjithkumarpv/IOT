#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   7
#define CSN_PIN 8

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

int dataReceived[5]; // this must match dataToSend in the TX
bool newData = false;
int flag1,flag2,flag3,flag4;

void setup() 
  {

    Serial.begin(9600);
    
    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(0, thisSlaveAddress);
    radio.startListening();
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
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
           
             Serial.println(dataReceived[1]);
                  if(dataReceived[1]<4)
                  {
                    int flag1=0;
                    if (flag1==0)
                  
                      {
                        digitalWrite(3,LOW);  
                        digitalWrite(2,HIGH); 
                      }
                      flag1++;
                  }
                  if (dataReceived[1]<4)
                  {
                    if (flag1==1)
                    {
                      digitalWrite(2,LOW);
                      digitalWrite(3,HIGH);
                    }
                    flag1=0;
                  }
            Serial.println(dataReceived[2]);
                 if(dataReceived[2]<10)
                      {
                        flag2=0;
                        if (flag2==0)
                        {
                        digitalWrite(4,HIGH);    
                        digitalWrite(5,LOW);
                        }
                        flag2++;  
                       }
               if (dataReceived[2]<10)
                    {
                      if (flag2==1)
                      {
                        digitalWrite(4,LOW);
                        digitalWrite(5,HIGH);  
                        }
                        flag2=0;
                    }
            Serial.println(dataReceived[3]);
                  if(dataReceived[3]<15)
                      {
                        flag3=0;
                        if (flag3==0)
                        {
                        digitalWrite(9,HIGH);  
                        digitalWrite(10,LOW);
                        }  
                        flag3++;         
                      }
                  if(dataReceived[3]<15)
                      {
                        if (flag3==1)
                        {
                        digitalWrite(9,HIGH);  
                        digitalWrite(10,LOW);
                        }
                        flag3=0;
                      }
                     
            Serial.println(dataReceived[4]);
                 if(dataReceived[4]<15)
                      {
                        
                        digitalWrite(2,LOW);
                        digitalWrite(3,LOW);
                        digitalWrite(4,LOW);
                        digitalWrite(5,LOW);
                        digitalWrite(9,LOW);
                        digitalWrite(10,LOW);
                      }
     
            newData = false;
          }
    }
