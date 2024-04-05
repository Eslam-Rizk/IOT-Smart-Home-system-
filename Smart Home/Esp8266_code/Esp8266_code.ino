//libraries part

#include <ESP8266WiFi.h>

#include <WiFiUdp.h>

#include <WiFiClient.h>

#include <ESP8266WebServer.h>

#include <EEPROM.h>

#include <Ticker.h>



WiFiUDP Udp;



char* ssid = "Esp8266_ssid";                  // enter your wifi ssid here

char* password = "Esp8266_password";          // enter your wifi password here

unsigned int localUdpPort = 4210;                   // local port to listen on

char incomingPacket[UDP_TX_PACKET_MAX_SIZE];        // buffer for incoming packets



// Set web server port number to 80

WiFiServer server(80);



// Variable to store the HTTP request

String header;



IPAddress ip( xx , xx , xx , xx );                      //enter static ip  here

IPAddress gateway( xx , xx , xx , xx );                    //enter static ip gateway here

IPAddress subnet(255, 255, 255, 0);

IPAddress dns(8, 8, 8, 8);

IPAddress IP_Remote2(192, 168, 1, 12);                  //esp No. 2



int add0 = 0, add1 = 1, add2 = 2, val0, val1, val2;     //variables to store pins states to eeprom



void setup(){

  Serial.begin(115200);             //start seial comm. at baud rate 115200

  pinMode(4, OUTPUT);                // set pin 4 as ouput

  digitalWrite(4, LOW);               //clear pin 4

  pinMode(5, OUTPUT);                // set pin 5 as ouput

  digitalWrite(5, LOW);               //clear pin 5

  pinMode(10, OUTPUT);                // set pin 10 as ouput

  digitalWrite(10, LOW);               //clear pin 10

  pinMode(13, OUTPUT);                // set pin 13 as ouput ( buzzer )

  digitalWrite(13, LOW);              // clear pin 13

  EEPROM.begin(512);                //start EEPROM

  delay(100);



  val0 = EEPROM.read(add0);        // read EEPROM value into variable

  delay(10);

  val1 = EEPROM.read(add1);

  delay(10);

  val2 = EEPROM.read(add2);

  delay(10);



  digitalWrite(4, val0);            //assign EEPROM values to pins

  digitalWrite(5, val1);

  digitalWrite(10, val2);



  WiFi.mode(WIFI_STA);                                           //begin station mode

  Serial.println(); Serial.print("connecting to "); Serial.println(ssid);

  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);

  

  while (WiFi.status() != WL_CONNECTED ) {

    Serial.println(".");

    digitalWrite(13, HIGH);

    delay(100);

    digitalWrite(13, LOW);

    delay(400);

  }



  Udp.begin(localUdpPort);                                   //start udp protocol

  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);      // print current ip address and port to serial port

  delay(500);

  server.begin();

}



void loop() {



  WiFiClient client = server.available();                             // Listen for incoming clients on web server



  if (client) {                                                       // If a new client connects,

    Serial.println("New Client.");                                  // print a message out in the serial port

    String currentLine = "";                                        // make a String to hold incoming data from the client

    currentTime = millis();

    previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected

      currentTime = millis();

      if (client.available()) {                                 // if there's bytes to read from the client,

        char c = client.read();                                           // read a byte, then

        Serial.write(c);                                                  // print it out the serial monitor

        header += c;

        if (c == '\n') {                                              // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.

          // that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)

            // and a content-type so the client knows what's coming, then a blank line:

            client.println("HTTP/1.1 200 OK");

            client.println("Content-type:text/html");

            client.println("Connection: close");

            client.println();



            // send data to esp No.2

            if (header.indexOf("GET /D1") >= 0) {

              Serial.println("D1");

              Udp.beginPacket(IP_Remote2, 4210);                      //start udp conection with ip and port

              Udp.write("1");                                           //send "1" through udp

              Udp.endPacket();                                        //end udp connection

              digitalWrite(13, HIGH);                                 // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }

            else if (header.indexOf("GET /D2") >= 0) {                     // send data to esp No.2

              Serial.println("D2");

              Udp.beginPacket(IP_Remote2, 4210);                      //start udp conection with ip and port

              Udp.write("2");                                           //send "2" through udp

              Udp.endPacket();                                       //end udp connection

              digitalWrite(13, HIGH);                                // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }

            else if (header.indexOf("GET /D3") >= 0) {                     // send data to esp No.2

              Serial.println("D3");

              Udp.beginPacket(IP_Remote2, 4210);                      //start udp conection with ip and port

              Udp.write("3");                                           //send "3" through udp

              Udp.endPacket();                                       //end udp connection

              digitalWrite(13, HIGH);                                // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }

            else if (header.indexOf("GET /D4") >= 0) {                     // send data to esp No.2

              Serial.println("D4");

              Udp.beginPacket(IP_Remote2, 4210);                      //start udp conection with ip and port

              Udp.write("4");                                           //send "4" through udp

              Udp.endPacket();                                       //end udp connection

              digitalWrite(13, HIGH);                                // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }

            else if (header.indexOf("GET /D5") >= 0) {                     // send data to esp No.2

              Serial.println("D5");

              Udp.beginPacket(IP_Remote2, 4210);                      //start udp conection with ip and port

              Udp.write("5");                                           //send "5" through udp

              Udp.endPacket();                                       //end udp connection

              digitalWrite(13, HIGH);                                // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }

            else if (header.indexOf("GET /D6") >= 0) {

              Serial.println("D6");

              digitalWrite(4, !digitalRead(4));                     //change pin 4 state

              if (digitalRead(4) == LOW) {                          //save the new state to EEPROM

                EEPROM.write(add0, 0);

              }

              else if (digitalRead(4) == HIGH) {

                EEPROM.write(add0, 1);

              }

              delay(100);

              digitalWrite(13, HIGH);                                // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }

            else if (header.indexOf("GET /D7") >= 0) {

              Serial.println("D7");

              digitalWrite(5, !digitalRead(5));                     //change pin 5 state

              if (digitalRead(5) == LOW) {                          //save the new state to EEPROM

                EEPROM.write(add1, 0);

              }

              else if (digitalRead(5) == HIGH) {

                EEPROM.write(add1, 1);

              }

              delay(100);

              digitalWrite(13, HIGH);                                // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }

            else if (header.indexOf("GET /D8") >= 0) {

              Serial.println("D8");

              digitalWrite(10, !digitalRead(10));                     //change pin 10 state

              if (digitalRead(10) == LOW) {                          //save the new state to EEPROM

                EEPROM.write(add2, 0);

              }

              else if (digitalRead(10) == HIGH) {

                EEPROM.write(add2, 1);

              }

              delay(100);

              digitalWrite(13, HIGH);                                // beep the buzzer

              delay(300);

              digitalWrite(13, LOW);

            }





            // Display the HTML web page

            client.println("<!DOCTYPE html><html>");

            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");

            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS to style the on/off buttons

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}button {color: #ffffff;height:60;width:180px;background-color: #2d63c8;font-size: 19px;border: 1px solid #2d63c8;padding: 15px 50px;cursor: pointer}button:active {color: #2d63c8;background-color: #fff84d;}body{background-color: #fff84d;text-align: center;}</style></head>");



            // Web Page Heading

            client.println("<body><h1>Home</h1>");

            client.println("<p><a href=\"/D1\"><button class=\"button D1\">D1</button></a></p>");

            client.println("<p><a href=\"/D2\"><button class=\"button D2\">D2</button></a></p>");

            client.println("<p><a href=\"/D3\"><button class=\"button D3\">D3</button></a></p>");

            client.println("<p><a href=\"/D4\"><button class=\"button D4\">D4</button></a></p>");

            client.println("<p><a href=\"/D5\"><button class=\"button D5\">D5</button></a></p>");

            client.println("<p><a href=\"/D6\"><button class=\"button D6\">D6</button></a></p>");

            client.println("<p><a href=\"/D7\"><button class=\"button D7\">D7</button></a></p>");

            client.println("<p><a href=\"/D8\"><button class=\"button D8\">D8</button></a></p>");

            client.println("</body></html>");



            // The HTTP response ends with another blank line

            client.println();

            // Break out of the while loop

            break;

          } else { // if you got a newline, then clear currentLine

            currentLine = "";

          }

        } else if (c != '\r') { // if you got anything else but a carriage return character,

          currentLine += c; // add it to the end of the currentLine

        }

      }

    }

    // Clear the header variable

    header = "";

    // Close the connection

    client.stop();

    Serial.println("Client disconnected.");

    Serial.println("");

  }



  int packetSize = Udp.parsePacket();                     //receiving data through udp

  if (packetSize)

  {

    // receive incoming UDP packets

    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());

    int len = Udp.read(incomingPacket, UDP_TX_PACKET_MAX_SIZE);                 //get size of incoming packet

    if (len > 0)

    {

      incomingPacket[len] = 0;

    }

    Serial.printf("UDP packet contents: %s\n", incomingPacket);                 //print the content of the received packet



    if (strcmp(incomingPacket, "reboot") == 0) { //reboot the device

      Udp.beginPacket(Udp.remoteIP(), 4210);         //start a socket with the remote controlling device

      Udp.write("Attempting to reboot... ");         //reply and confirm the packets are received

      Udp.endPacket();                               //close the socket

      delay(100);

      ESP.restart();                                 //reboot the esp

    }



    //control esp1 and esp2 via udp packets

    if (incomingPacket[0] == '1') { //D1

      Serial.println("D1");

      Udp.beginPacket(IP_Remote2, 4210);

      Udp.write("1");

      Udp.endPacket();

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }

    if (incomingPacket[0] == '2') { //D2

      Serial.println("D2");

      Udp.beginPacket(IP_Remote2, 4210);

      Udp.write("2");

      Udp.endPacket();

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }

    if (incomingPacket[0] == '3') { //D3

      Serial.println("D3");

      Udp.beginPacket(IP_Remote2, 4210);

      Udp.write("3");

      Udp.endPacket();

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }

    if (incomingPacket[0] == '4') { //D4

      Serial.println("D4");

      Udp.beginPacket(IP_Remote2, 4210);

      Udp.write("4");

      Udp.endPacket();

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }

    if (incomingPacket[0] == '5') { //D5

      Serial.println("D5");

      Udp.beginPacket(IP_Remote2, 4210);

      Udp.write("5");

      Udp.endPacket();

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }

    if (incomingPacket[0] == '6') { //D6

      Serial.println("D8");

      digitalWrite(4, !digitalRead(4));

      if (digitalRead(4) == LOW) {

        EEPROM.write(add0, 0);

      }

      else if (digitalRead(4) == HIGH) {

        EEPROM.write(add0, 1);

      }

      delay(100);

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }



    if (incomingPacket[0] == '7') { //D7

      Serial.println("D7");

      digitalWrite(5, !digitalRead(5));

      if (digitalRead(5) == LOW) {

        EEPROM.write(add1, 0);

      }

      else if (digitalRead(5) == HIGH) {

        EEPROM.write(add1, 1);

      }

      delay(100);

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }



    if (incomingPacket[0] == '8') { //D8

      Serial.println("D8");

      digitalWrite(10, !digitalRead(10));

      if (digitalRead(10) == LOW) {

        EEPROM.write(add2, 0);

      }

      else if (digitalRead(10) == HIGH) {

        EEPROM.write(add2, 1);

      }

      delay(100);

      digitalWrite(13, HIGH);

      delay(300);

      digitalWrite(13, LOW);

    }



    EEPROM.commit();

  }

}

