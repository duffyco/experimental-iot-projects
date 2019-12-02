int incomingByte = 0;

void setup() 
{
  Serial.begin(115200);  
    Serial.println( "AT+RST\r\n" );
  delay( 3000 );

}

void loop() 
{
  String content = "";
  char character;

  while(Serial.available()) {
      character = Serial.read();
      content.concat(character);
  }

  if (content != "") {
    Serial.print("Found: " );
    Serial.println(content);
  }
}
