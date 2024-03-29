// Load Wi-Fi library
#include <WiFi.h>

#define RXD2 16
#define TXD2 17

// Replace with your network credentials
const char* ssid = "Jianning";
const char* password = "67644117";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String response, ip_address;

// Auxiliar variables to store the current output state
String output26State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int wait30 = 30000; // time to reconnect when connection is lost.

// This is your Static IP
IPAddress local_IP(192, 168, 108, 144); 
// Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4); 

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);

  //Configure Static IP
  if(!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("Static IP failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  ip_address = WiFi.localIP().toString();
  Serial.println(ip_address);
  server.begin();
}

void loop() {

// If disconnected, try to reconnect every 30 seconds.
  if ((WiFi.status() != WL_CONNECTED) && (millis() > wait30)) {
    Serial.println("Trying to reconnect WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    wait30 = millis() + 30000;
  } 
  // Check if a client has connected..
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  Serial.print("New client: ");
  Serial.println(client.remoteIP());
   
  // Espera hasta que el cliente envíe datos.
  // while(!client.available()){ delay(1); }

  /////////////////////////////////////////////////////
  // Read the information sent by the client.
  String req = client.readStringUntil('\r');
  Serial.println(req);

  // Make the client's request.
  if(req.indexOf("status") != -1)
  {
    response = "WiFi Connected: " + ip_address;
  }
  if(req.indexOf("stop") != -1)
  {
    response = "STOP";
    Serial2.write(0x10);
  }
  if(req.indexOf("forward") != -1)
  {
    response = "FORWARD";
    Serial2.write(0x11);
  }  
  if(req.indexOf("fl") != -1)
  {
    response = "FL";
    Serial2.write(0x12);
  }
  if(req.indexOf("left") != -1)
  {
    response = "LEFT";
    Serial2.write(0x13);
  }
  if(req.indexOf("fr") != -1)
  {
    response = "FR";
    Serial2.write(0x14);
  }
  if(req.indexOf("right") != -1)
  {
    response = "RIGHT";
    Serial2.write(0x15);
  }
  if(req.indexOf("reverse") != -1)
  {
    response = "REVERSE";
    Serial2.write(0x16);
  }
  if(req.indexOf("rl") != -1)
  {
    response = "RL";
    Serial2.write(0x17);
  }
  if(req.indexOf("rr") != -1)
  {
    response = "RR";
    Serial2.write(0x18);
  }
  if(req.indexOf("boost") != -1)
  {
    response = "BOOST";
    Serial2.write(0x19);
  }
  if(req.indexOf("ifl") != -1)
  {
    response = "+ FL";
    Serial2.write(0x21);
  }
  if(req.indexOf("dfl") != -1)
  {
    response = "- FL";
    Serial2.write(0x22);
  }
  if(req.indexOf("ifr") != -1)
  {
    response = "+ FR";
    Serial2.write(0x23);
  }
  if(req.indexOf("dfr") != -1)
  {
    response = "- FR";
    Serial2.write(0x24);
  }
  if(req.indexOf("ibl") != -1)
  {
    response = "+ BL";
    Serial2.write(0x25);
  }
  if(req.indexOf("dbl") != -1)
  {
    response = "- BL";
    Serial2.write(0x26);
  }
  if(req.indexOf("ibr") != -1)
  {
    response = "+ BR";
    Serial2.write(0x27);
  }
  if(req.indexOf("dbr") != -1)
  {
    response = "- BR";
    Serial2.write(0x28);
  }
  if(req.indexOf("idf") != -1)
  {
    response = "+ DF";
    Serial2.write(0x29);
  }
  if(req.indexOf("ddf") != -1)
  {
    response = "- DF";
    Serial2.write(0x2A);
  }
  if(req.indexOf("iffd") != -1)
  {
    response = "+ D45 ";
    Serial2.write(0x2B);
  }
  if(req.indexOf("dffd") != -1)
  {
    response = "- D45";
    Serial2.write(0x2C);
  }
  if(req.indexOf("ind") != -1)
  {
    response = "+ D90";
    Serial2.write(0x2D);
  }
  if(req.indexOf("dnd") != -1)
  {
    response = "- D90";
    Serial2.write(0x2E);
  }
  if(req.indexOf("testforward") != -1)
  {
    response = "FORWARD";
    Serial2.write(0x31);
  }
  if(req.indexOf("test45left") != -1)
  {
    response = "45 LEFT";
    Serial2.write(0x32);
  }if(req.indexOf("test45right") != -1)
  {
    response = "45 RIGHT";
    Serial2.write(0x33);
  }
  if(req.indexOf("test90left") != -1)
  {
    response = "90 LEFT";
    Serial2.write(0x34);
  }
  if(req.indexOf("test90right") != -1)
  {
    response = "90 RIGHT";
    Serial2.write(0x35);
  }
  if(req.indexOf("testreverse") != -1)
  {
    response = "REVERSE";
    Serial2.write(0x36);
  }
  /*
       if (req.indexOf("on12") != -1) {digitalWrite(LED12, HIGH); estado = "LED12 ON";}
       if (req.indexOf("off12") != -1){digitalWrite(LED12, LOW); estado = "LED12 OFF";}
       if (req.indexOf("on14") != -1) {digitalWrite(LED14, HIGH); estado = "LED14 ON";}
       if (req.indexOf("off14") != -1){digitalWrite(LED14, LOW); estado = "LED14 OFF";}
       if (req.indexOf("consulta") != -1){
           estado ="";
           if (digitalRead(LED12) == HIGH) {estado = "LED12 ON,";} else {estado = "LED12 OFF,";}
           if (digitalRead(LED14) == HIGH) {estado = estado + "LED14 ON";} else {estado = estado + "LED14 OFF";}
           }*/
           

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println(response); //  Return status.

  client.flush();
  client.stop();
  Serial.println("Client disconnected.");
}
