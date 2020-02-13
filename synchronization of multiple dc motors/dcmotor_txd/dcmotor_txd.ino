
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   7
#define CSN_PIN 8

const byte slaveAddress = 'R';

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int dataToSend=1;
int dataToSend1=0;
int dataToSend2=2;

void setup() {

    Serial.begin(9600);

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);

}

//====================

void loop() {
        if(digitalRead(4)==HIGH&&digitalRead(5)==LOW&&digitalRead(6)==LOW)
        {
        digitalWrite(2,HIGH);
        digitalWrite(3,LOW);
        delay(1000);
        //dataToSend[1] = {1};
        send();
        }
        else if(digitalRead(4)==LOW&&digitalRead(5)==HIGH&&digitalRead(6)==LOW)
        {
        digitalWrite(2,HIGH);
        digitalWrite(3,LOW);
        send1();
        delay(1000);
        }
        else if(digitalRead(4)==LOW&&digitalRead(5)==LOW&&digitalRead(6)==HIGH)
        {
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        send2();
        delay(1000);
        }
        
}

//====================

void send() 
  {

      bool rslt;
        {
        rslt = radio.write( &dataToSend, sizeof(dataToSend) );
            // Always use sizeof() as it gives the size as the number of bytes.
            // For example if dataToSend was an int sizeof() would correctly return 2
        }
      Serial.print("Data Sent ");
      Serial.print(dataToSend);
      delay(300);
      
      if (rslt)
         {
          Serial.println("  Acknowledge received");
         }
      else
        {
          Serial.println("  Tx failed");
        }
}
void send1() 
  {

      bool rslt;
        {
        rslt = radio.write( &dataToSend1, sizeof(dataToSend1) );
            // Always use sizeof() as it gives the size as the number of bytes.
            // For example if dataToSend was an int sizeof() would correctly return 2
        }
      Serial.print("Data Sent ");
      Serial.print(dataToSend1);
      delay(300);
      
      if (rslt)
         {
          Serial.println("  Acknowledge received");
         }
      else
        {
          Serial.println("  Tx failed");
        }
}
void send2() 
  {

      bool rslt;
        {
        rslt = radio.write( &dataToSend2, sizeof(dataToSend2) );
            // Always use sizeof() as it gives the size as the number of bytes.
            // For example if dataToSend was an int sizeof() would correctly return 2
        }
      Serial.print("Data Sent ");
      Serial.print(dataToSend2);
      delay(300);
      
      if (rslt)
         {
          Serial.println("  Acknowledge received");
         }
      else
        {
          Serial.println("  Tx failed");
        }
}
