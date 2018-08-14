#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "passwords.h" //this file contains the name and the password of the local router

ESP8266WebServer server ( 81 );//the port of the server would be 81

void switchOutput(int a, int value) {
  switch (value) {
    case 0:
    case 1:
      switch (a) {
        case 1: digitalWrite(D3, value); //set pin D3 to the state we want
            server.send (200, "text/html", "OK <a href='/1/on'>sw ON</a> <a href='/1/off'>sw OFF</a>");//respond to the server
            break;
        case 2: digitalWrite(D4, value);
            server.send (200, "text/html", "OK <a href='/2/on'>sw ON</a> <a href='/2/off'>sw OFF</a>");
            break;
      }
      break;
  }
 
}

void setup ( void ) {
        pinMode (D3, OUTPUT);
        pinMode (D4, OUTPUT);
  
	Serial.begin ( 115200 );//set serial terminal baud rate to 115200bps
	WiFi.begin ( ssid, password );//start the wireless radio and connect it to the ssid
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) { //while not connected, try to connect
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() ); //here the local IP would show on the serial terminal
	if ( MDNS.begin ( "remoteSocket01" ) ) {//setting the mDNS responder allows not to type in the exact IP address
		Serial.println ( "MDNS responder started" );
	}
 
	//on each connection to the server the function will be called depending on the address typed in
        server.on ("/2/off", []() {switchOutput(2,0);}); //setting output 2 to 0=off
        server.on ("/2/on", []() {switchOutput(2,1);}); // setting output 2 to 1=on
        server.on ("/1/off", []() {switchOutput(1,0);});
        server.on ("/1/on", []() {switchOutput(1,1);});
  
	server.begin();
	Serial.println ( "HTTP server started" );
}

void loop ( void ) {
	server.handleClient();
}
