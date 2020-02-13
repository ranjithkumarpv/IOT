#include <SoftwareSerial.h>
#include <stdlib.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 7
// LED 
int ledPin = 13;
// SensorInput analog input (A0)
int SensorInput = 0;
int sensor_pin = A0;

int output_value ;
// replace with your channel's thingspeak API key
String apiKey = "BU1ADIJRQ28U5GRP";

SoftwareSerial ser(2,3); // RX, TX

// this runs once
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);    

  // enable debug serial
  Serial.begin(9600); 
  // enable software serial
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
  ser.println("AT+CWJAP=\"OnePlus 6\",\"onetwothree\"");
  delay (15000);
  while(ser.available())
  {
    Serial.write(ser.read());
  }
  Serial.println("Joining AP List");
  delay (8000);  
}

// the loop 
void loop() {  
  // blink LED on board
  DHT.read11(DHT11_PIN);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    
    delay(5000);

   output_value= analogRead(sensor_pin);

   output_value = map(output_value,550,0,0,100);

   Serial.print("Mositure : ");

   Serial.print(output_value);

   Serial.println("%");

   delay(1000);
  digitalWrite(ledPin, HIGH);   
  Serial.println("LED goes high");
  delay(3000);               
  digitalWrite(ledPin, LOW);
  // read the value from SensorInput.
  // read 10 values for averaging.
  int val = 0;
  int val1 = 0;
  int val2 = 0;
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

    val2 = output_value;
  Serial.println("Analog reading");
  // convert to string

  String strSoil = String(val2, DEC); 
  Serial.println("Printing parameter");
  Serial.println(strSoil);
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
  getStr += String(strSoil);
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
