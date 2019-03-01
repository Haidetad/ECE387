//Original Source code modified and adapted from the following source:  https://alselectro.wordpress.com/2016/10/30/arduino-ethernet-shield-led-onoff-from-webpage/


#include <SPI.h> 
#include <Ethernet.h>
#include "pitches.h"


byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x0C, 0xA7 }; //Arduino MAC adress
byte ip[] = { 192, 168, 1, 44 }; //IP adress arduino is set to, same network as router
byte gateway[] = { 192, 168, 1, 1};  
byte subnet[] = { 255, 255, 255, 0}; 
EthernetServer server(80); 

String readString; 

int led = 8;
int piezo = 2;

int HappyBDay[] = {
                    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
                    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
                    NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_D4,
                    NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4};

int noteLengths[] = {
                     4, 4, 2, 2, 2, 1, 
                     4, 4, 2, 2, 2, 1, 
                     4, 4, 2, 2, 4, 4, 2, 1, 
                     4, 4, 2, 2, 2, 1};

void setup(){

    pinMode(led, OUTPUT); //tests led
    digitalWrite(led, HIGH);  
    delay(500); 
    digitalWrite(led, LOW); 
    
    
    Ethernet.begin(mac, ip, gateway, subnet); 
    server.begin(); 
}
void flash(int duration){//made for SOS blink

  digitalWrite(led, HIGH);
  delay(duration);
  digitalWrite(led,LOW);
  delay(duration);
}

void loop(){ 
    //creates the client connection
    EthernetClient client = server.available(); 
    if (client) { 
        while (client.connected()) { 
            if (client.available()) { 
                char t = client.read();

                //reads the charactor by charactor HTTP requests
                if (readString.length() < 1125) {

                    //stores them to strings
                    readString += t; 
                }

                //0x0D is carraige return in ASCII
                if (t == 0x0D) { 
                    client.println("HTTP/1.1 200 OK"); 
                    client.println("Content-Type: text/html"); 
                    client.println();

                    client.println("<HTML>"); 
                    client.println("<HEAD>"); 
                    client.println("<TITLE> ARDUINO ETHERNET SHIELD</TITLE>"); 
                    client.println("</HEAD>"); 
                    client.println("<BODY>"); 
                    client.println("<hr>"); 
                    client.println("<hr>"); 
                    client.println("<br>"); 
                    client.println("<H1 style=\"color:red;\">ARDUINO ETHERNET SHIELD LED MAGIC</H1>"); 
                    client.println("<hr>"); 
                    client.println("<br>");
                    client.print("<A HREF=\"http://cdn.playbuzz.com/cdn/facead4d-26cf-4141-b212-8cb2e16702ff/30858099-dbed-474f-b04a-4d5d45246ff0.gif\">Magical Anime gif</A>");//Link to an anime Gif
                    client.println("<hr>"); 
                    client.println("<br>");
                    client.println("<H2><a href=\"/LEDSHORT\"\">Blink LED in SHORT Dash</a><br></H2>"); // makes a clickable link for activating pins            
                    //client.println("<br>"); 
                    client.println("<H2><a href=\"/LEDLONG\"\">Blink LED in LONG Dash</a><br></H2>");
                    client.println("<br>"); 
                    client.println("<H2><a href=\"/LEDSOS\"\">LED SOS Morse Code</a><br></H2>");
                    client.println("<br>");
                    client.println("<H2><a href=\"/PIEZOBEEP\"\">Make Pizeo Beep</a><br></H2>");
                    
                    

                    client.println("</BODY>"); 
                    client.println("</HTML>");

                    delay(10); 
                    //stops client 
                    client.stop();

                    // controls arduino pins depending on which link you click on the webpage 
                    if(readString.indexOf("LEDSHORT") > -1) 
                    { 
                                           
                        digitalWrite(led, HIGH);
                        delay(100);
                        digitalWrite(led, LOW);

                    } 
                    //Morse code SOS blink
                    if(readString.indexOf("LEDSOS") > -1){
                      
                          flash(225);flash(225); flash(225);
                          delay(300);
                          flash(450);flash(450);flash(450);
                          delay(300);
                          flash(225);flash(225);flash(225);
                          delay(900);
                                             
                    }
                    //plays Happy Birthday from Piezo 
                    if(readString.indexOf("PIEZOBEEP") > -1){
                        for (int thisNote = 0; thisNote <27 ; thisNote++) {
                     
                        int noteLength = 1000/noteLengths[thisNote];
                        tone(piezo, HappyBDay[thisNote],noteLength);
                        int pauseBetweenNotes = noteLength * 1.30;
                        delay(pauseBetweenNotes);
                        noTone(piezo);
                        }
                    }
                    else{ 
                        if(readString.indexOf("LEDLONG") > -1) 
                        { 
                            
                            digitalWrite(led, HIGH);
                            delay(225);
                            digitalWrite(led, LOW);
                        } 
                    } 
                    //clearing string for next read 
                    readString="";

                } 
            } 
        } 
    } 
}
