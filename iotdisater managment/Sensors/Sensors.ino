
#include <SoftwareSerial.h>//Software Serial library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266  
#include <dht.h>

dht DHT;
#define DHT11_PIN 7
#define DEBUG true
volatile int FlowPulse; //measuring the rising edges of the signal
int Calc;                               
int flowsensor = 2;    //The pin-2 location of the sensor Always use this pin as we are using interrupt 0


String mySSID = "vv tek";       // WiFi SSID (Change)
String myPWD = "vvteam@123"; // WiFi Password (Change)
String myAPI = "OZ85YNPVZZIHXX0G";   // Thingspeak Write API Key (Change)
String myHOST = "api.thingspeak.com"; // Thingspeak apn 
String myPORT = "80";

const int fire = 10;
const int vibration_Pin =7;
const int Gas_Pin =9;
float temparature, humidity;
int Smoke;
int fire2;
int Motion;
const int ledPin = 13; 

// Setup executes only in once 
void setup()
{
  Serial.begin(9600);
  espSerial.begin(115200);
  Serial.println("Connecting to WiFi");
  int y = Connect();
  while(y!=0)  // Trying to connect to WiFi 
  {
    Serial.println("Re-trying to connect WiFi");
    delay(5000);
    
  }
 
}

// Main loop
void loop ()
{
    //Connect();
    read_sensor(); // Read all sensor data and print
    Send_data();   //Send data to Thingspeak cloud
    flow(); //Flow sensor data and print
}


int read_sensor()
    Serial.println(temparature);
    Serial.println("Humidity");
    Serial.println(humidity);
    
    //Read light sensor data
    fire2 =analogRead(fire);
    Serial.println("Light fire2");  
    Serial.println(fire2);
  
    Motion =digitalRead(vibration_Pin);
    Serial.print("Vibration detected :");
    Serial.println(Motion);
   
    Smoke =digitalRead(Gas_Pin);
    Serial.print("Smoke detected :");
    Serial.println(Smoke);
  
  //Send_data();   //Send data to Thingspeak cloud
}



int Connect()  // Connect to WiFi
{
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  delay(1000);
  return 0;
}


String espData(String command, const int timeout, boolean debug)  //Sending data to ESP using AT Commands (Refer ESP AT Commands)
{
  //Serial.print("AT Command ==> ");
  //Serial.print(command);
  //Serial.println("     ");
  
  String response = "";
  espSerial.println(command); 
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
}


int Send_data() //Push data to Thingspeak cloud
{   
   DHT.read11(DHT11_PIN);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    // Reading dht11 sensor data before sending because of scope memory 
    float temparature = DHT.humidity;
    // Read temperature as Celsius (the default)
    float humidity = DHT.temperature;
    
    Serial.println("Sending data to Cloud");
    String sendData = "GET /update?api_key="+ myAPI +"&field1="+String(temparature)+"&field2="+String(humidity)+"&field3"+String(fire2)+"&field4="+String(Motion)+"&field5="+String(Smoke);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    if( espSerial.find("SEND OK")) 
    {   
      Serial.println("Packet sent");
    } 
  /*  while (espSerial.available()) 
    { 
      Serial.println(espSerial.readString()); 
    } 
      espSerial.println("AT+CIPCLOSE");
     }
     */
    Serial.print("Values to be sent: ");
    Serial.print(temparature);
    Serial.print(",  "); 
    Serial.print(humidity);
    Serial.print(",  ");
    Serial.print(fire2);
    Serial.print(",  ");
    Serial.println(Motion);      
    if(temprature>32)
  {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(1000);
    return 0;
}

void flow() {     
 FlowPulse = 0;      //Set NbTops to 0 ready for calculations
 sei();            //Enables interrupts
 delay (1000);      //Wait 1 second
 cli();            //Disable interrupts
 Calc = (FlowPulse * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 
 Serial.print (Calc, DEC); //Prints the number calculated above
 Serial.println (" L/hour"); //Prints "L/hour"
}

void rpm ()     //This is the function that the interupt calls 
{ 
  FlowPulse++;  //This function measures the rising and falling edge of the hall effect sensors signal
}
