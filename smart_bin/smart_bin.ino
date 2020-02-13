#include <Servo.h>
#include <SoftwareSerial.h>
void SendMessage();
SoftwareSerial mySerial(2,3);
char msg;

Servo myservo;  
int pos = 0;
Servo myservo1;  
int pos1 = 0; 
int flag=0;   


const int trigPin1 = 4; // ultra 2
const int echoPin1 = 5;

const int trigPin2 = 6; //ultra 3
const int echoPin2 = 7;

const int trigPin3 = 8; //ultra 3
const int echoPin3 = 9;

const int motorPin3  = 10; // Pin  7 of L293
const int motorPin4  = 11;  // Pin  2 of L293
void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module 
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  // put your setup code here, to run once:
  

  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

   pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
   Serial.begin(9600); // Starts the serial communication
   
   pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
   Serial.begin(9600); // Starts the serial communication
    myservo.attach(12);
    myservo1.attach(13);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
}

void loop() 
  {         
          int duration1, distance1;   //ultr2
         
           digitalWrite(trigPin1, HIGH);
            delay(200);
            digitalWrite(trigPin1, HIGH);
            delay(200);
            digitalWrite(trigPin1, LOW);
            duration1 = pulseIn(echoPin1, HIGH);
            distance1 = (duration1 / 2) / 29.1;
            Serial.print("ultra_distance_1:");
            Serial.println(distance1);
            if(distance1<20)
              {
                for (pos = 0; pos <= 180; pos += 1) 
                { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo.write(pos);              // tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
                }
              for (pos = 180; pos >= 0; pos -= 1) 
                { // goes from 180 degrees to 0 degrees
                myservo.write(pos);              // tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
                }
              }
              
                                  
            int duration2, distance2;   //ultr2
            digitalWrite(trigPin2, HIGH);
            delay(200);
            digitalWrite(trigPin2, HIGH);
            delay(200);
            digitalWrite(trigPin2, LOW);
            duration2 = pulseIn(echoPin2, HIGH);
            distance2 = (duration2 / 2) / 29.1;
            Serial.print("ultra_distance_3:");
            Serial.println(distance2);
                 
            if(distance2<20)
              {
              digitalWrite(motorPin3, HIGH);
              digitalWrite(motorPin4, LOW);
              delay(2000);             
              digitalWrite(motorPin3, LOW);
              digitalWrite(motorPin4, HIGH);
              delay(2000);                           
              digitalWrite(motorPin3, HIGH);
              digitalWrite(motorPin4, LOW);
              delay(2000);             
              digitalWrite(motorPin3, LOW);
              digitalWrite(motorPin4, HIGH);
              delay(2000);
              digitalWrite(motorPin3, LOW);
              digitalWrite(motorPin4, LOW); 
              delay(2000);   
              
              
                for (pos1 = 0; pos1 <= 180; pos1 += 1) 
                { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
                }
              for (pos1 = 180; pos1 >= 0; pos1 -= 1) 
                { // goes from 180 degrees to 0 degrees
                myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
                }
              }
                            
              int duration3, distance3;   //ultr1
              digitalWrite(trigPin3, HIGH);
              delay(500);
              digitalWrite(trigPin3, HIGH);
              delay(500);
              digitalWrite(trigPin3, LOW);
              // Measure the pulse input in echo pin
              duration3 = pulseIn(echoPin3, HIGH);
              // Distance is half the duration divided by 29.1
              distance3 = (duration3 / 2) / 29.1;
              Serial.print("ultra_distance_4:");
              Serial.println(distance3);
                
              if(distance3<20 && flag==0)
              {   Serial.println("gsm");
                  SendMessage();
                  flag=1;
              }
   }     
                           
 

void SendMessage()

{
  Serial.print("hi");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+6383578516\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("I am SMS from SMART BIN");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
 // Serial.write(mySerial.read());

}
