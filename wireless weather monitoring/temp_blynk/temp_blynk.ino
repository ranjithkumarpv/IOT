  

 
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6b628808070242d1a95d5ac4c81361b1"; //Enter the Auth code which was send by Blink

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Realme 1";  //Enter your WIFI Name
char pass[] = "23456789";  //Enter your WIFI Password
int val;
int tempPin = A0;
 
SimpleTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
   val = analogRead(tempPin);
  float mv = ( val/1024.0)*3300; 
  float t= mv /10;
  Serial.println(t);
  delay(1000);
  
   // or dht.readTemperature(true) for Fahrenheit

  if (isnan(t)) {
    Serial.println("Failed to read from TEMP sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  //V5 is for Humidity
   Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);
 

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}

