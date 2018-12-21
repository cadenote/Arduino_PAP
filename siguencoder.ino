//Basado en lo publicado en
//https://www.luisllamas.es/arduino-encoder-rotativo/

const int channelPinA = 2;
const int channelPinB = 3;

const int enable = 8;
const int dir = 13;
const int pulso = 12;
 
const int timeThreshold = 1;
long inactivo = 0,timeCounter = 0;
 
const int maxSteps = 800;
volatile int ISRCounter = 0;
int counter = 0;
 
bool IsCW = true;
 
void setup()
{
   pinMode(channelPinA, INPUT_PULLUP);
   pinMode(dir,OUTPUT);
   digitalWrite(dir, HIGH);
   pinMode(enable,OUTPUT);
   digitalWrite(enable, LOW);
   pinMode(pulso,OUTPUT);
   digitalWrite(pulso, LOW);

   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);
   attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);
}
 
void loop()
{
   if (counter != ISRCounter)
   {
      counter = ISRCounter;
      Serial.println(counter);
      inactivo = millis();
   }
   else
   { 
   if (millis() > inactivo + 500) digitalWrite(enable, HIGH);// Desactiva el motor cuando no lo uses
   }
   delay(100);
}
void cuenta( bool clockwise)
{
  digitalWrite(enable, LOW);
  digitalWrite(dir, clockwise);
  if ( clockwise) ISRCounter++;
  else ISRCounter--;
  digitalWrite(pulso, LOW);    // sets the pin off
  delayMicroseconds(10);        // pauses for 10 microseconds
  digitalWrite(pulso, HIGH);   // sets the pin on
  delayMicroseconds(5);        // pauses for 5 microseconds
  digitalWrite(pulso, LOW);    // sets the pin off
  delayMicroseconds(10);        // pauses for 10 microseconds
}


void doEncodeA()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) == digitalRead(channelPinB))
      {
         IsCW = true;
         if (ISRCounter + 1 <= maxSteps) cuenta(IsCW);
      }
      else
      {
         IsCW = false;
         if (ISRCounter - 1 > 0) cuenta(IsCW);
      }
      timeCounter = millis();
   }
}
 
void doEncodeB()
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(channelPinA) != digitalRead(channelPinB))
      {
         IsCW = true;
         if (ISRCounter + 1 <= maxSteps) cuenta(IsCW);
      }
      else
      {
         IsCW = false;
         if (ISRCounter - 1 > 0) cuenta(IsCW);
      }
      timeCounter = millis();
   }
}
