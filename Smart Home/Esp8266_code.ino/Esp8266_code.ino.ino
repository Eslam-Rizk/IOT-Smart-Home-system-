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

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

IPAddress ip( xx , xx , xx , xx );                      //enter static ip  here
IPAddress gateway( xx , xx , xx , xx );                    //enter static ip gateway here
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);
IPAddress IP_Remote2(192, 168, 1, 12);                  //esp No. 2
Ticker checkconn; Ticker checkingConn;                  // ticker timers

int st_found = 1, conn_not = 1 , count = 0, apCount = 0, intvalue = 0, apORst = 6, stidrom = 7, stpwrom = 8, newap = 9, apidrom = 17, appwrom = 18, add0 = 0, add1 = 1, add2 = 2, add3 = 3, add4 = 4, val0, val1, val2, val3, val4, stidsize, stpwsize, apidsize, appwsize;
String stssid;
String stpassword;
String apssid;
String appassword;
unsigned long time_now = 0, notConnTime = 0;
bool notConn_ap = false;

void milli_delay(int period) {                         //delay function
  time_now = millis();
  while (millis() < time_now + period) {
    //delay=period
  }
}

//int add0=0,add1=1,add2=2,add3=3,add4=4;
//station ssid EEPROM(21:40),station PW EEPROM(41:60),access point ssid EEPROM(61:80),acess point PW EEPROM(81:100)//add0=0,add1=1,add2=2,add3=3,add4=4,add5=5,add10=10,add12=12,add16=16
void setup()
{
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

  if (EEPROM.read(121) == '1') {                                 //check for new ip address
    Serial.print("the new ip is ");
    IPAddress ipaddress;
    String newip = "";
    for (int i = 0; i < EEPROM.read(122); i++) {
      newip += char(EEPROM.read(123 + i));
    }
    int intnewip = newip.toInt();
    if (ipaddress.fromString(newip)) {
      ip = ipaddress;
      Serial.println(ip);
    }
  }
  else {
    IPAddress ip(192, 168, 1, 11);                             //default ip iddress
  }

  if (EEPROM.read(apORst) == '1') {                              //check if station mode is chosen
    stidsize = EEPROM.read(stidrom);                             //read st ssid size
    stpwsize = EEPROM.read(stpwrom);                             //read st pw size
    //create new st ssid char array & read its content from EEPROM
    for (int i = 0; i <= stidsize - 1; i++) {
      stssid += char(EEPROM.read(21 + i));
    }
    //String stpassword;                                         //create new st pw char array & read its content from EEPROM
    for (int i = 0; i <= stpwsize - 1; i++) {
      stpassword += char(EEPROM.read(41 + i));
    }
    WiFi.mode(WIFI_STA);                                           //begin station mode
    Serial.println(); Serial.print("connecting to "); Serial.println(stssid);
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(stssid, stpassword);
  }

  else {
    if (EEPROM.read(newap) == '1') {                                 //check if new AP is entered
      apidsize = int(EEPROM.read(apidrom));                           //read AP ssid size
      appwsize = int(EEPROM.read(appwrom));                           //read AP pw size
      //create new AP ssid char array & read its content from EEPROM
      for (int i = 0; i <= apidsize - 1; i++) {
        apssid += char(EEPROM.read(61 + i));
      }
      //create new AP pw char array & read its content for EEPROM
      for (int i = 0; i <= appwsize - 1; i++) {
        appassword += char(EEPROM.read(81 + i));
      }
      Serial.println(); Serial.print("establishing new "); Serial.print(apssid);
      WiFi.mode(WIFI_AP);                                        //begin AP mode
      WiFi.config(ip, gateway, subnet);
      Serial.println(WiFi.softAP(apssid, appassword, 1, 0, 5) ? " Ready" : " Failed!");
    }
    else {                                                       //set default AP ssid and pw
      Serial.println(); Serial.print("establishing default "); Serial.println(ssid);
      WiFi.mode(WIFI_AP);
      WiFi.config(ip, gateway, subnet);
      Serial.print(WiFi.softAP(ssid, password, 1, 0, 5) ? " Ready" : " Failed!");
      delay(500);
    }
  }

  Udp.begin(localUdpPort);                                   //start udp protocol
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);      // print current ip address and port to serial port
  delay(500);
  server.begin();
}

void loop() {
  if (notConn_ap) {                                        //if not connected to wifi yet, start counter
    if (millis() >= notConnTime + 1000) {
      apCount++;
      notConnTime = millis();
    }
  }

  if (EEPROM.read(apORst) == '1' && WiFi.status() != WL_CONNECTED && conn_not == 1) {       //if in station mode & not connected to wifi & connection variable =1, start counter
    while (WiFi.status() != WL_CONNECTED ) {
      Serial.println(".");
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(400);
      count++;
      if (count >= 120) {                                                                 //if counter exceeds 60 secs, start access point mode
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        notConn_ap = true;
        WiFi.mode(WIFI_AP);
        WiFi.config(ip, gateway, subnet);
        Serial.print(WiFi.softAP(ssid, password, 1, 0, 5) ? "AP Ready.. station not " : "AP Failed!.. station not");
        conn_not = 0;
        count = 0;
      }
    }
    if (WiFi.status() == WL_CONNECTED) {                                       // if connected to wifi, beep the buzzer and print connected to serial port
      digitalWrite(13, HIGH);
      delay(500);
      digitalWrite(13, LOW);
      count = 0;
      Serial.println("connected");
    }
  }

  if (digitalRead(0) == 0) {                         //hard reset
    int buttondelay = millis();
    while (digitalRead(0) == 0) {
      int buttondelay2 = millis();
      if (buttondelay2 == buttondelay + 1500) {
        Serial.println("Back to defaults... ");
        EEPROM.write(apORst, '0');                     //set access point after reboot
        EEPROM.write(newap, '0');                      //set the default ssid and password
        EEPROM.write(121, '0');                        //set the default IP address
        EEPROM.commit();
      }
    }
  }

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

    if (incomingPacket[0] == 'a' && incomingPacket[1] == 'l' && incomingPacket[2] == 't' ) {   // changing mode(ap pr st) or changing ssis & pw or changing IP address
      if (incomingPacket[3] == 'a' && incomingPacket[4] == 'p') {                                 //changing to a new access point
        Udp.beginPacket(Udp.remoteIP(), 4210);                              // send back a reply, to the IP address and port we got the packet from
        Udp.write("Setting new Access point...");
        Udp.endPacket();
        delay(100);
        packetSize -= 5;
        int i = 0, j = 0;
        for (i = 0; i < packetSize; i++) {
          if (incomingPacket[i + 5] == '&' && incomingPacket[i + 6] == '&' && incomingPacket[i + 7] == '&') break;      //break between ssid and pw
          EEPROM.write(61 + i, incomingPacket[i + 5]);                                                   //(i+5)>> 5 is the NO. of a,l,t,a,p
          EEPROM.commit();
          Serial.print(char(EEPROM.read(61 + i)));
        }
        for (j = 0; j < packetSize - i - 3; j++) {
          EEPROM.write(81 + j, incomingPacket[j + 5 + i + 3]);                                           //(j+5+i+3)>> 5 is NO. of a,l,t,a,p >> i is size of id >> 3 is the size of the separator
          EEPROM.commit();
          Serial.print(char(EEPROM.read(81 + j)));
        }
        EEPROM.write(apidrom, i);                                                                        //store AP ssid size
        EEPROM.write(appwrom, j);                                                                        //store AP pw size
        EEPROM.write(apORst, '0');                                                                       //store that it's in AP mode
        EEPROM.write(newap, '1');                                                                        //store that there're new AP credentials
        EEPROM.commit();
        delay(100);
        ESP.restart();
      }
      else if (incomingPacket[3] == 's' && incomingPacket[4] == 't') {                        //changing staion ssid and pw
        Udp.beginPacket(Udp.remoteIP(), 4210);                                                  // send back a reply, to the IP address and port we got the packet from
        Udp.write("Setting new Station...");
        Udp.endPacket();
        delay(100);
        packetSize -= 5;
        int i = 0, j = 0;
        for (i = 0; i < packetSize; i++) {                                                               //store and serial print the station ssid
          if (incomingPacket[i + 5] == '&' && incomingPacket[i + 6] == '&' && incomingPacket[i + 7] == '&') break; //if station ssid is stored the break to store the password
          EEPROM.write(21 + i, incomingPacket[i + 5]);                                                   //(i+5)>> 5 is the NO. of a,l,t,a,p
          EEPROM.commit();
          Serial.print(char(EEPROM.read(21 + i)));
        }
        for (j = 0; j < packetSize - i - 3; j++) {                                                       //store and serial print the station password
          EEPROM.write(41 + j, incomingPacket[j + 5 + i + 3]);                                           //(j+5+i+3)>> 5 is NO. of a,l,t,a,p >> i is size of id >> 3 is the size of the separator
          EEPROM.commit();
          Serial.print(char(EEPROM.read(41 + j)));
        }
        EEPROM.write(stidrom, i);                                                                        //store AP ssid size
        EEPROM.write(stpwrom, j);                                                                        //store AP pw size
        EEPROM.write(apORst, '1');                                                                       //store that it's in station mode
        EEPROM.commit();                                                                                 //svae eeprom changes
        ESP.restart();                                                                                   //reboot the esp
      }
    }

    if (incomingPacket[0] == 'i' && incomingPacket[1] == 'p') {                           //changing IP address
      Serial.println("new ip address:");                                                 //print to the serial monitor
      Udp.beginPacket(Udp.remoteIP(), 4210);                                             //start a socket with the remote controlling device
      Udp.write("Setting new IP Address... ");                                           //reply and confirm the packets are received
      Udp.endPacket();                                                                   //close the socket
      delay(100);
      EEPROM.write(122, packetSize - 2);                                                  //store the new IP number of packet to eeprom
      for (int i = 0; i < packetSize - 2; i++) {
        EEPROM.write(123 + i, incomingPacket[2 + i]);                                        //store the new IP address to eeprom
      }
      EEPROM.write(121, '1');                                                                     //there is a new IP address stored
      EEPROM.commit();                                                                            //save eeprom changes
      ESP.restart();                                                                            //reboot the esp
    }
    else if (strcmp(incomingPacket, "reboot") == 0) { //reboot the device
      Udp.beginPacket(Udp.remoteIP(), 4210);         //start a socket with the remote controlling device
      Udp.write("Attempting to reboot... ");         //reply and confirm the packets are received
      Udp.endPacket();                               //close the socket
      delay(100);
      ESP.restart();                                 //reboot the esp
    }
    else if (strcmp(incomingPacket, "delete") == 0) {//reset to default settings
      Udp.beginPacket(Udp.remoteIP(), 4210);         //start a socket with the remote controlling device
      Udp.write("Back to defaults... ");             //reply and confirm the packets are received
      Udp.endPacket();                               //close the socket
      delay(100);
      EEPROM.write(apORst, '0');                     //set access point after reboot
      EEPROM.write(newap, '0');                      //set the default ssid and password
      EEPROM.write(121, '0');                        //set the default IP address
      EEPROM.commit();                               //save eeprom changes
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
} if (incomingPacket[0] == '8') { //D8
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

//if can't find selected wifi,set default ssid
if (apCount == 60) {
  apCount = 0;
  notConnTime = 0;
  int n = WiFi.scanNetworks();
  Serial.println(n);
  stssid = "";
  for (int i = 0; i < int(EEPROM.read(stidrom)); i++) {
    stssid += char(EEPROM.read(21 + i));
  }
  for (int i = 0; i < n; i++) {
    Serial.println(WiFi.SSID(i));
    if (WiFi.SSID(i) == stssid && EEPROM.read(apORst) == '1') {
      conn_not = 1;
      notConn_ap = false;
      Serial.println(); Serial.print("connecting to "); Serial.println(stssid);
      WiFi.mode(WIFI_STA);                                         //begin station mode
      WiFi.config(ip, gateway, subnet);
      WiFi.begin(stssid, stpassword);
      while (WiFi.status() != WL_CONNECTED) {
        Serial.println(".");
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13, LOW);
        delay(400);
        count++;
        if (count >= 120) {
          digitalWrite(13, HIGH);
          delay(500);
          digitalWrite(13, LOW);
          notConn_ap = true;
          WiFi.mode(WIFI_AP);
          WiFi.config(ip, gateway, subnet);
          Serial.print(WiFi.softAP(ssid, password, 1, 0, 5) ? "AP Ready.. station not " : "AP Failed!.. station not");
          count = 0;
        }
      }
    }
  }
}

EEPROM.commit();
}
