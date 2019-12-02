// written By : Mohannad Rawashdeh
// http://WWW.Genotronex.com
// genotronex@gmail.com
// http://www.instructables.com/member/Mohannad+Rawashdeh/
// this code to test EZ0 Sonar sensor using Serial method .
// Connect Tx....D8
// Connect Rx ....D9
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9,true); // RX, TX
int BW=4;
char *buffer;
byte x;
char array[3];
int counter=0;
void setup() {
  // put your setup code here, to run once:
  // set the data rate for the SoftwareSerial port
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(BW,OUTPUT);
  digitalWrite(BW,LOW);
  delay(250);
  Serial.println("Calibrartion Cycle ");
  delay(150);
}

void reading(){
 
  mySerial.println(1);
while (mySerial.available())
{
   x= mySerial.readBytes(buffer,1);
   if(*buffer==0x52){ 
   x= mySerial.readBytes(buffer,1);
   array[0]=*buffer; 
   x= mySerial.readBytes(buffer,1);
   array[1]=*buffer; 
   x= mySerial.readBytes(buffer,1);
   array[2]=*buffer;
   }
}

delayMicroseconds(220);
}
void loop() {
  // put your main code here, to run repeatedly:
  reading();
  int Final_inch=(array[0]-48)*100 + (array[1]-48)*10 +(array[2]-48) ;
  float Final_cm=Final_inch*2.54;
  Serial.print(Final_inch);
  Serial.println(" Inch ");
  Serial.print(Final_cm);
  Serial.println(" cm ");
  delay(200);
}

