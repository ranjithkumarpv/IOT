#include <SoftwareSerial.h>
#include <stdlib.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 7
// replace with your channel's thingspeak API key
String apiKey = "SHNTYJSXXONVGG8W";

SoftwareSerial ser(10,11); // RX, TX
float temp;
int smokeA0 = A1;
int sensorThres = 550;
int ir=0;
int irs=0;
int Smaoke = 0;
int gas = 0;
int Fire = 0;
void setup() 
{
        Serial.begin(9600);
        pinMode(smokeA0, INPUT);//gas
        pinMode(2, INPUT); //Pin 2 as INPUT ir
        pinMode(3, OUTPUT); //PIN 3 as OUTPUT led
        pinMode(A0, INPUT);//TEMP
        pinMode(4, OUTPUT);// FAN
         pinMode(5, OUTPUT);//buzzer
          ser.begin(115200);
  
  // reset ESP8266
  ser.println("AT+RST");  
  delay (10000);
  ser.println("AT");
  while(ser.available())
  {
    Serial.write(ser.read());
  }
  Serial.println("Sending AT command");
  while(ser.available())
  {
    Serial.write(ser.read());
  }
  delay (5000);
 ser.println("AT+CWMODE=1");
 delay (5000);
  
  ser.println("AT+CWMODE?");
  while(ser.available())
  {
    Serial.write(ser.read());
  }
  Serial.println("Setting mode");
  delay (5000);
  ser.println("AT+CWJAP=\"AptechJayanagar17\",\"Londan@1980\"");
  delay (15000);
  while(ser.available())
  {
    Serial.write(ser.read());
  }
  Serial.println("Joining AP List");
  delay (8000);  
}

void loop() 
{
  DHT.read11(DHT11_PIN);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
      if (ir ==1) 
      { 
        irs=1;
        digitalWrite(5, HIGH);   // turn the LED/Buzz ON
        delay(1000);                       // wait for 100 msecond
        digitalWrite(5, LOW);   // turn the LED/Buzz OFF
        delay(1000);                       // wait for 100 msecond
      } else {
        irs=0;
        } 
    delay(500);
    ir =digitalRead(2); 
    if (ir ==1) 
      {
        digitalWrite(3, HIGH);   // turn the LED/Buzz ON
        delay(1000);                       // wait for 100 msecond
        digitalWrite(3, LOW);   // turn the LED/Buzz OFF
        delay(1000);                       // wait for 100 msecond
      }         
   temp = analogRead(A0);  
   temp = temp * 0.48828125;
   if(temp>=35)
   {
    
        digitalWrite(4,HIGH);//fan
        delay(1000);
        Fire=1;
        
   }else{
    Fire =0;
    }
   
   int analogSensor = analogRead(smokeA0);
   Serial.print("gs=");
   Serial.println(analogSensor);
  if (analogSensor > sensorThres)
    {
    digitalWrite(5, HIGH);//buzzer
    delay(1000);
    digitalWrite(5, LOW);
    delay(1000);
    gas=1;
    
    }else{
    
    gas=0;
      }
    

   int val = 0;
  int val1 = 0;
  int val2 = 0;
  int val3 = 0;
  int val4 = 0;
  val = DHT.temperature;
  Serial.println("Analog reading");
  // convert to string
  char buf[16];
  String strTemp = String(val, DEC); 
  Serial.println("Printing parameter");
  Serial.println(strTemp);
  
    val1 = DHT.humidity;
  Serial.println("Analog reading");
  // convert to string

  String strHum = String(val1, DEC); 
  Serial.println("Printing parameter");
  Serial.println(strHum);

    val2 = Fire;
  Serial.println("Analog reading");
  // convert to string

  String strFire = String(val2, DEC); 
  Serial.println("Printing parameter");
  Serial.println(strFire);

   val3 = gas;
  Serial.println("Analog reading");
  // convert to string

  String strgas = String(val3, DEC); 
  Serial.println("Printing parameter");
  Serial.println(strgas);
  
      val4 = irs;
  Serial.println("Analog reading");
  // convert to string

  String strirs = String(val4, DEC); 
  Serial.println("Printing parameter");
  Serial.println(strirs);

  
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd); 
  while(ser.available())
  {
    Serial.write(ser.read());
  }
  
  Serial.println("TCP connection");
  
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(strTemp);
  getStr +="&field2=";
  getStr += String(strHum);
  getStr +="&field3=";
  getStr += String(strFire);
   getStr +="&field4=";
  getStr += String(strgas);
   getStr +="&field5=";
  getStr += String(strirs);
  getStr += "\r\n\r\n";

  Serial.println("Got string");

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  while(ser.available())
  {
    Serial.write(ser.read());
  }

  if(ser.find(">")){
    ser.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
 delay(20000);  
  }
