int analogPin = 0;     // potentiometer wiper (middle terminal) connected to analog pin 3
int digitalPin =2;                       // outside leads to ground and +5V
int val = 0;           // variable to store the value read
int val2 = 0;
int previousValue = 0;
boolean on = false;
boolean previousState = false;
unsigned int count = 0;

void setup()
{
  Serial.begin(9600);          //  setup serial
}

void loop()
{
  val = analogRead(analogPin); 
  val2 = digitalRead(digitalPin); 

  Serial.println( val );
  Serial.println( val2 );
  delay( 1000 );
}

