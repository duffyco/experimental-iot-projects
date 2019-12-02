#include "SonarEZ0pw.h"
#include <ESP8266WiFi.h>

const char* ssid     = "Snowbirds";  
const char* password = "1=2-30495867";


SonarEZ0pw Sonar(2); // pin D7
float cm_dis=0.00;
float Inch_dis=0.00;
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  cm_dis= Sonar.Distance(cm); // To calculate the distance in cm
  Inch_dis=Sonar.Distance(inch);// To calculate the distance in Inch
  Serial.println("Distance " );
  Serial.print(Inch_dis);
  Serial.println(" inch ");
  Serial.print(cm_dis);
  Serial.println(" cm ");
  delay(250);
}
