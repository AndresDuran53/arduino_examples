int analogValue = 0;
float voltage = 0;
int ledDelay = 2000;

void setup() {
  pinMode(0,INPUT);
}

void loop(){
 analogValue = analogRead(0);
 voltage = 0.0048*analogValue;
 Serial.print("Analog: ");
 Serial.println(analogValue);
 Serial.print("Voltage = ");
 Serial.println(voltage);
 delay(1000);
 if ( voltage >= 3.0 )
  {
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
  }
}
