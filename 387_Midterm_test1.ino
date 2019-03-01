//Original Source code modified and adapted from the following source:  https://alselectro.wordpress.com/2016/10/30/arduino-ethernet-shield-led-onoff-from-webpage/

#include <SPI.h> 
#include <Ethernet.h>

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x0C, 0xA7 }; //Arduino MAC adress
byte ip[] = { 192, 168, 1, 44 }; 
byte gateway[] = { 192, 168, 1, 1};  
byte subnet[] = { 255, 255, 255, 0}; 
EthernetServer server(80);
String readString; 

int led = 8;

void setup(){

    pinMode(led, OUTPUT); 
    // Tests connection by blinking led 
    digitalWrite(led, HIGH);  
    delay(500); 
    digitalWrite(led, LOW);  
    
    //start Ethernet 
    Ethernet.begin(mac, ip, gateway, subnet); 
    server.begin(); 
}

void loop(){ 

    EthernetClient client = server.available(); 
    if (client) { 
        while (client.connected()) { 
            if (client.available()) { 
                char t = client.read();

                //read char by char HTTP request 
                if (readString.length() < 100) {

                    //store characters to string 
                    readString += t; 
                }

                //if HTTP request has ended– 0x0D is Carriage Return \n ASCII 
                if (t == 0x0D) { 
                    client.println("HTTP/1.1 200 OK"); //send new page 
                    client.println("Content-Type: text/html"); 
                    client.println();

                    client.println("<HTML>"); 
                    client.println("<HEAD>"); 
                    client.println("<TITLE> Arduino Ethernet Shield</TITLE>"); 
                    client.println("</HEAD>"); 
                    client.println("<BODY>"); 
                    client.println("<hr>"); 
                    client.println("<hr>"); 
                    client.println("<br>"); 
                    client.println("<H1 style=\"color:green;\">ARDUINO ETHERNET SHIELD ON/OFF</H1>"); 
                    client.println("<hr>"); 
                    client.println("<br>");

                    client.println("<H2><a href=\"/LEDON\"\">Turn On LED</a><br></H2>"); 
                    client.println("<H2><a href=\"/LEDOFF\"\">Turn Off LED</a><br></H2>");

                    client.println("</BODY>"); 
                    client.println("</HTML>");

                    delay(10); 
                    //stopping client 
                    client.stop();

                    // control arduino pin 
                    if(readString.indexOf("LEDON") > -1) //checks for LEDON 
                    { 
                        digitalWrite(led, HIGH); // set pin high 
                    } 
                    else{ 
                        if(readString.indexOf("LEDOFF") > -1) //checks for LEDOFF 
                        { 
                            digitalWrite(led, LOW); // set pin low 
                        } 
                    } 
                    //clearing string for next read 
                    readString="";

                } 
            } 
        } 
    } 
}
