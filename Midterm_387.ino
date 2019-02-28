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
    // Test LED 
    digitalWrite(led, HIGH);  
    delay(500); 
    digitalWrite(led, LOW); 
    
    
    Ethernet.begin(mac, ip, gateway, subnet); 
    server.begin(); 
}
void flash(int duration){

  digitalWrite(led, HIGH);
  delay(duration);
  digitalWrite(led,LOW);
  delay(duration);
}
void loop(){ 
    
    EthernetClient client = server.available(); 
    if (client) { 
        while (client.connected()) { 
            if (client.available()) { 
                char t = client.read();

                
                if (readString.length() < 1125) {

                    
                    readString += t; 
                }

                
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
                    client.print("<A HREF=\"http://cdn.playbuzz.com/cdn/facead4d-26cf-4141-b212-8cb2e16702ff/30858099-dbed-474f-b04a-4d5d45246ff0.gif\">Magical Anime gif</A>");
                    client.println("<hr>"); 
                    client.println("<br>");
                    client.println("<H2><a href=\"/?LEDSHORT\"\">Blink LED in SHORT Dash</a><br></H2>");             
                    //client.println("<br>"); 
                    client.println("<H2><a href=\"/?LEDLONG\"\">Blink LED in LONG Dash</a><br></H2>"); 
                    //client.println("<H2><a href=\"/?LEDOFF\"\">Stop LED Morese Code</a><br></H2>");
                    

                    client.println("</BODY>"); 
                    client.println("</HTML>");

                    delay(10); 
                    //stopping client 
                    client.stop();

                    // control arduino pin 
                    if(readString.indexOf("?LEDSHORT") > -1) 
                    { 
                      
                         // flash(225);flash(225); flash(225);
                          //delay(300);
                         // flash(450);flash(450);flash(450);
                         // delay(300);
                          //flash(225);flash(225);flash(225);
                          //delay(900);
                      
                        digitalWrite(led, HIGH);
                        delay(100);
                        digitalWrite(led, LOW);
//                        delay(200);
//                        digitalWrite(led, HIGH);
//                        delay(200);
//                        digitalWrite(led, LOW);
//                        delay(200);
                       // digitalWrite(led, HIGH);
//                        delay(200);
                        
                    } 
                    else{ 
                        if(readString.indexOf("?LEDLONG") > -1) 
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
