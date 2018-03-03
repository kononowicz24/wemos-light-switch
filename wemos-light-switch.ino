
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "passwords.h"

ESP8266WebServer server ( 81 );

void switchOutput(int a, int value) {
  switch (value) {
    case 0:
    case 1:
      switch (a) {
        case 1: digitalWrite(D3, value);
            server.send (200, "text/html", "OK <a href='/1/on'>sw ON</a> <a href='/1/off'>sw OFF</a>");
            break;
        case 2: digitalWrite(D4, value);
            server.send (200, "text/html", "OK <a href='/2/on'>sw ON</a> <a href='/2/off'>sw OFF</a>");
            break;
      }
      break;
    //case 2:
    //  switch (a) {
    //    case 1: digitalWrite(D3, value); delay(1000); digitalWrite(D3, !value);  server.send (200, "text/html", "OK"); break;
    //    case 2: digitalWrite(D4, value); delay(1000); digitalWrite(D4, !value);  server.send (200, "text/html", "OK"); break;
    //  }
    //  break;
  }
 
}

void setup ( void ) {
  pinMode (D3, OUTPUT);
  pinMode (D4, OUTPUT);
  
	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "gniazdko01" ) ) {
		Serial.println ( "MDNS responder started" );
	}
 
  server.on ("/2/off", []() {switchOutput(2,0);});
  server.on ("/2/on", []() {switchOutput(2,1);});
  server.on ("/1/off", []() {switchOutput(1,0);});
  server.on ("/1/on", []() {switchOutput(1,1);});
  
	server.begin();
	Serial.println ( "HTTP server started" );
}

void loop ( void ) {
	server.handleClient();
}
