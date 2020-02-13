

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   7
#define CSN_PIN 8

const byte slaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int dataToSend[5];
int count=0;
char input[12];

void setup() {

    Serial.begin(9600);

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

//====================

void loop() {

        
        dataToSend[1] = 1;
        dataToSend[2] = 2;
        dataToSend[3] = 3;
        dataToSend[4] = 4;
      if(Serial.available()>0)
        {
         count = 0;
      while(Serial.available() && count < 12)          // Read 12 characters and store them in input array
      {
         input[count] = Serial.read();
         count++;
         delay(5);
      }
        delay(500);
         if(input[11]=='B')
        {
          dataToSend[1]=30;
        }
       else if(input[11]=='6')
        {
          dataToSend[2]=30;
         }
     else if(input[11]=='A')
        {
          dataToSend[3]=40;
        }
       
        } 
        
     
        send();
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
      Serial.print(dataToSend[1]);
      delay(300);
      Serial.print(dataToSend[2]);
      delay(300);
      Serial.print(dataToSend[3]);
      delay(300);
      Serial.print(dataToSend[4]);
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
