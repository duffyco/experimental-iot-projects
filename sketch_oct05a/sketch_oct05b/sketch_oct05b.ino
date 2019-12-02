/* ====== ESP8266 Demo ======
 * Print out analog values
 * (Updated Dec 14, 2014)
 * ==========================
 *
 * Change SSID and PASS to match your WiFi settings.
 * The IP address is displayed to soft serial upon successful connection.
 *
 * Ray Wang @ Rayshobby LLC
 * http://rayshobby.net/?p=9734
 */

// comment this part out if not using LCD debug

#define BUFFER_SIZE 512

#define SSID  "45jxkclvisdo"      // change this to match your WiFi SSID
#define PASS  "90iojknm12qwaszx"  // change this to match your WiFi password
#define PORT  "80"           // using port 8080 by default

char buffer[BUFFER_SIZE];

/*
// If using Software Serial for debug
// Use the definitions below
//#define esp Serial
*/
#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(7,8);  // use pins 7, 8 for software serial 
#define dbg Serial


// If your MCU has dual USARTs (e.g. ATmega644)
// Use the definitions below

// By default we are looking for OK\r\n
char OKrn[] = "OK\r\n";
byte wait_for_wifiSerial_response(int timeout, char* term=OKrn) {
  unsigned long t=millis();
  bool found=false;
  int i=0;
  int len=strlen(term);
  // wait for at most timeout milliseconds
  // or if OK\r\n is found

/*
  while(wifiSerial.available() > 0) {
    char t = wifiSerial.read();
  }
*/    
  while(millis()<t+timeout) {
    if(wifiSerial.available()) {
      buffer[i++]=wifiSerial.read();
      if(i>=len) {
        if(strncmp(buffer+i-len, term, len)==0) {
          found=true;
          break;
        }
      }
    }
  }
  buffer[i]=0;
  dbg.print(buffer);
  return found;
}

void sendData( 

void setup() {

  // assume esp8266 operates at 115200 baud rate
  // change if necessary to match your modules' baud rate
  wifiSerial.begin(115200);
  
  dbg.begin(9600);
  dbg.println("\nbegin.");
    
  setupWiFi();

  // print device IP address
  //dbg.print("device ip addr:");
  wifiSerial.println("AT+CIFSR");
  wait_for_wifiSerial_response(1000);
  dbg.print("setup done");

}

bool read_till_eol() {
  static int i=0;
  if(wifiSerial.available()) {
    buffer[i++]=wifiSerial.read();
    if(i==BUFFER_SIZE)  i=0;
    if(i>1 && buffer[i-2]==13 && buffer[i-1]==10) {
      buffer[i]=0;
      i=0;
      dbg.print(buffer);
      return true;
    }
  }
  return false;
}

void loop() {
  int ch_id, packet_len;
  char *pb;  
  if(read_till_eol()) {
    if(strncmp(buffer, "+IPD,", 5)==0) {
      // request: +IPD,ch,len:data
      sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
      if (packet_len > 0) {
        // read serial until packet_len character received
        // start from :
        pb = buffer+5;
        while(*pb!=':') pb++;
        pb++;
        if (strncmp(pb, "GET /", 5) == 0) {
          wait_for_wifiSerial_response(1000);
          dbg.println("-> serve homepage");
          serve_homepage(ch_id);
        }
      }
    }
  }
}

void serve_homepage(int ch_id) {
  String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nRefresh: 5\r\n";

  String content="";
  // output the value of each analog input pin
  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    content += "analog input ";
    content += analogChannel;
    content += " is ";
    content += sensorReading;
    content += "<br />\n";       
  }

  header += "Content-Length:";
  header += (int)(content.length());
  header += "\r\n\r\n";
  wifiSerial.print("AT+CIPSEND=");
  wifiSerial.print(ch_id);
  wifiSerial.print(",");
  wifiSerial.println(header.length()+content.length());
  if(wait_for_wifiSerial_response(2000, "> ")) {
    wifiSerial.print(header);
    wifiSerial.print(content);
  } else {
    wifiSerial.print("AT+CIPCLOSE=");
    wifiSerial.println(ch_id);
  }
}


void setupWiFi() {
  // try empty AT command
  wifiSerial.println("AT");
  wait_for_wifiSerial_response(1000);

  // set mode 1 (client)
  wifiSerial.println("AT+CWMODE=1");
  wait_for_wifiSerial_response(1000);  

  // reset WiFi module
 // wifiSerial.print("AT+RST\r\n");
 // wait_for_wifiSerial_response(1500);
 // delay(3000);
 
  // join AP
  wifiSerial.print("AT+CWJAP=\"");
  wifiSerial.print(SSID);
  wifiSerial.print("\",\"");
  wifiSerial.print(PASS);
  wifiSerial.println("\"");
  // this may take a while, so wait for 5 seconds
  wait_for_wifiSerial_response(5000);
  
  wifiSerial.println("AT+CIPSTO=30");  
  wait_for_wifiSerial_response(1000);

  // start server
  wifiSerial.println("AT+CIPMUX=1");
  wait_for_wifiSerial_response(1000);
  
  wifiSerial.print("AT+CIPSERVER=1,"); // turn on TCP service
  wifiSerial.println(PORT);
  wait_for_wifiSerial_response(1000);
  
    
}
