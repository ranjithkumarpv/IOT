#include <LiquidCrystal.h>
LiquidCrystal lcd(4,5,6,7,8,9);
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3,INPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(13,OUTPUT);
  lcd.clear();
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
}


void loop() {
  // read the pushbutton input pin:
  
   if ((digitalRead(2) == HIGH) && (digitalRead(3) == LOW)) {
    lcd.clear();
    lcd.print(" AUTOMATIC MODE ");
     Serial.print(" AUTOMATIC MODE ");
     digitalWrite(13,HIGH);
     delay(500);
      digitalWrite(10,HIGH);
     digitalWrite(11,LOW);
     delay(1000);
    }

   if ((digitalRead(2) == LOW)&&(digitalRead(3)) == HIGH) {
    lcd.clear();
    lcd.print("  MANUAL  MODE  ");
    Serial.print("  MANUAL  MODE  ");
    digitalWrite(13,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    delay(1000);
   }
}
