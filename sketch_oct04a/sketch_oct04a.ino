/*
 * Still not great.  But consistent every 7 seconds.
 * 
 * In my incarnation - Blue and Black need to be tied high.  Brown in RX, Yellow in TX.  Green is GND.
 */

#include <SoftwareSerial.h>

#define BUFFER_SIZE 512

#define SSID  "Snowbirds"      // change this to match your WiFi SSID
#define PASS  "1=2-30495867"  // change this to match your WiFi password
#define PORT  "80"           // using port 8080 by default
#define DST_IP "192.168.1.4"

char buffer[BUFFER_SIZE];
char OKrn[] = "OK\r\n";

SoftwareSerial wifiSerial(7,8);
#define dbg Serial

byte wait_for_wifiSerial_response(int timeout, char* term=OKrn) {
  unsigned long t=millis();
  memset( buffer, 0x00, BUFFER_SIZE );
  bool found=false;
  int i=0;
  int len=strlen(term);
  // wait for at most timeout milliseconds
  // or if OK\r\n is found

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
  dbg.print("[");
  dbg.print(buffer);
  dbg.print (" - " );
  dbg.print (found);
  dbg.print("]\n");

  return found;
}


boolean send_to_wifi( String data, char* term=OKrn, int delayTime=200, boolean poll=true )
{
    boolean ok = false;

    while( !ok )
    {
      delay( delayTime );  
      dbg.print( ">>" );
      dbg.println( data );
      wifiSerial.print( data );
      wifiSerial.print( "\r\n");
      ok = wait_for_wifiSerial_response(3000, term );

      if( !ok && !poll )
        return false;
    }

    return true;
}

void setup() {
  wifiSerial.begin( 115200 );
  dbg.begin( 115200 );

  setupWiFi();
}

void loop() { 
    dbg.print( "---------------------" );

    send_to_wifi( "AT+CIPCLOSE", OKrn, 1000, false );
    String cmd = "AT+CIPSTART=\"TCP\",\""; //make this command: AT+CPISTART="TCP","192.168.88.35",80
    cmd += DST_IP;
    cmd += "\",80";
    send_to_wifi( cmd, "ECT", 1000 );

   
    //The ESP8266 needs to know the size of the GET request
    //This is our HTTP GET Request change to the page and server you want to load.
    cmd = "GET /test.html HTTP/1.0\r\n";
    cmd += "Host: 192.168.1.103\r\n\r\n";
    
    String msg = "";
    msg = "AT+CIPSEND=";
    msg += cmd.length();
    send_to_wifi( msg, OKrn, 1000, false );
    send_to_wifi( cmd, OKrn, 1000, false );
    
    dbg.print( "done" );
    dbg.print( "+---------------------+" );
}

void setupWiFi() {
  // try empty AT command
  send_to_wifi( "AT" );

  send_to_wifi("AT+RST","ready");
  send_to_wifi("AT+CWMODE=1","change");

  char sendBuffer[80];
  String apJoinCmd = "AT+CWJAP=\"";
  apJoinCmd += SSID;
  apJoinCmd += "\",\"";
  apJoinCmd += PASS;
  apJoinCmd += "\"";

  send_to_wifi( apJoinCmd  );
  apJoinCmd = "Leak";

  send_to_wifi("AT+CIFSR", "43", 1000);

  pinMode( 13, OUTPUT );
  digitalWrite( 13, HIGH );
    
/*
  // start server
  send_to_wifi("AT+CIPMUX=1");
  
  send_to_wifi("AT+CIPSERVER=1,80"); // turn on TCP service
  */  
}
