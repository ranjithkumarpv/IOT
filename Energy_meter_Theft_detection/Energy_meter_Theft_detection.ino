
#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Software serial for ESP8266      

//Connectivity
#define DEBUG true
String mySSID = "pula";     // WiFi SSID   (Change to your WiFi SSID)
String myPWD = "spaceevil"; // WiFi Password   (Change to your WiFi Password)
String myAPI = "NUQJ5C00597JMIJ5";   // API Key (Change to your Thingspeak Write API Key)
String myHOST = "api.thingspeak.com"; // Thingspeak apn 
String myPORT = "80";

// Current measurement
const int Source_current =A0;
const int Load_current =A1;
const unsigned long sampleTime = 100000UL;                   
const unsigned long numSamples = 250UL;   
const unsigned long sampleInterval = sampleTime/numSamples;
int adc_zero;
float rms;  
int refsig = 200; //for converting the analog signal coming from hall sensor to digital through arduino code
int val;//the digital value of the incoming analog signals
int prev_val = 0;
int t, cur_t; //time variables
float Voltage = 230; //AC Voltage 230V
float Power_Watts, Amount, RPM; 
bool Theft =false, turnoff=false;
int buzzer =8;
int poweroff =9;
double load=0, source =0;

// Setup executes only in once
void setup()
{
  Serial.begin(9600);
  espSerial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(poweroff, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(poweroff, LOW);
  Serial.println("Connecting to WiFi");
  
  while(Connect()!=0)  // Trying to connect to WiFi 
  {
    Serial.println("Re-trying to connect WiFi");
    delay(5000);
    //Connect();
  }
  determineVQ(Source_current); 

}

// Main loop will keep on executing
void loop()
{
  //Serial.print("Source Voltage: ");determineVQ(Source_current);
  //Serial.print("Load Voltage: ");determineVQ(Load_current);
  if(turnoff!=true){
      source =ReadCurrent(Source_current); //Reading Current from Current Sensor
      Serial.print("Source Current: "); Serial.println(source);
      Power_Watts = Voltage * rms;  //Total Power = Voltage * Current 
      Amount = Power_Watts *1*2; // Amount = Total power*no.of hours*(Rs/unit)
      Rpm();  // Calculating Revolutions per second (RPM)
      
      load = ReadCurrent(Load_current);
      //float loss=load-0.1;
      //float excess= load+0.1;
      Serial.print("Load_current: "); Serial.println(load);
      
      //if (source < excess or source > loss){
      //if((load<0.1) && (source<0.18 && source<0.18 ))  // If current sensor wire removed for theft detection
      if((load < 0.15) && (source<0.12))  //If meter wire removed for theft detection
      {
          Theft =true;
          digitalWrite(buzzer, HIGH);
          Send_data(); // Send data to Thingspeak Cloud
          digitalWrite(poweroff, HIGH);
          turnoff=true;
          delay(2000);
          digitalWrite(buzzer, LOW);
      }
#if 0      //else if( (0.25 < source > 0.3) && (0.45 < load > 0.55))
      else if( source >0.28 && load >0.25)
      {
        Serial.println("");
        Serial.println("INPUT POWER IS NOT AVAILABLE");
        Serial.println("");
      }
#endif
      else
      {
          Theft =false;
          turnoff=false;
          digitalWrite(poweroff, LOW);
          Send_data(); // Send data to Thingspeak Cloud
          //digitalWrite(buzzer, LOW);
      }
      Serial.print("Theft detected: ");
      Serial.println(Theft);   
      
      //Send_data(); // Send data to Thingspeak Cloud
  }
  delay(1000); // Wait for another loop
}

//Connect to wifi
int Connect()  // Connect to WiFi
{
    espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
    espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
    espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
    //delay(1000);
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
    Serial.println("Sending data to Cloud");
    String sendData = "GET /update?api_key="+ myAPI +"&field1="+String(Amount)+"&field2="+String(Power_Watts)+"&field3="+String(source)+"&field4="+String(Theft);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    //delay(1000);
    espSerial.println(sendData);
    //delay(1000);
    if( espSerial.find("SEND OK")) 
    {   
      Serial.println("Packet sent");
    } 
    Serial.print("Values to be sent: ");
    Serial.print("Amount: ");Serial.print(Amount);
    Serial.print(",  "); 
    Serial.print("Power Watts: ");Serial.print(Power_Watts);
    Serial.print(",  ");
    Serial.print("Current: ");Serial.print(source);
    //Serial.print(",  ");
    Serial.print("Theft: ");Serial.println(Theft);      
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(1000);
    return 0;
}

int determineVQ(int pin) //Average Quiscent voltage for Current calculation
{
  long VQ = 0;                                
  for (int i=0; i<5000; i++)  //read 5000 samples to stabilise value
  { 
    VQ += analogRead(pin);
    delay(1);                   //depends on sampling (on filter capacitor), can be 1/80000 (80kHz) max.
  }
  VQ /= 5000;
  Serial.print(map(VQ, 0, 1023, 0, 5000));
  Serial.println(" mV");
  adc_zero = (int)(VQ);
  return adc_zero;
}

float ReadCurrent(int pin)
{
  unsigned long currentAcc = 0;
  unsigned int count = 0;
  unsigned long prevMicros = micros() - sampleInterval ;
  while (count < numSamples)
  {
    if (micros() - prevMicros >= sampleInterval)
    {
      int adc_raw = analogRead(pin) - adc_zero;
      currentAcc += (unsigned long)(adc_raw * adc_raw);
      ++count;
      prevMicros += sampleInterval;
    }
  }
  rms = sqrt((float)currentAcc/(float)numSamples) * (75.7576 / 1024.0);
  Serial.print("Current :");
  Serial.println(rms);
  return rms;
}

int Rpm()//Measure RPM
 {
   float sig = analogRead(A0); //read raw value of hall sensor
   if (sig > refsig) val = HIGH; //convert it to digital 0,1 form
   else val = LOW;
   if (prev_val == 0 && val == 1) { //check for rising edge
     cur_t = micros();
    
     RPM =(1000000 * 60 / (cur_t - t));
     t = micros();
   }
   prev_val = val;
   //Serial.print("RPM: ");
   //Serial.println(RPM);
   return 0;
 }
