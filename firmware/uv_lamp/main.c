#include <time.h>
#include <ESP8266WiFi.h>

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

// Replace with your network credentials
// to create its own wifi network
const char* ssid     = "UVLampCentralBalkan";
const char* password = "centralbalkan";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

String output12State = "off";
time_t now = time(nullptr);

String regime = "0";
const int output12 = 12;

void setup() {
  // This method does the following:
  // - set the baud rate
  // - set pin 12 (D6) mode to OUTPUT
  // - Inialize wifi network
  // - Notify the networks status via serial port
  Serial.begin(115200);
  Serial.print("Startging access point: ");

  pinMode(output12, OUTPUT);

  Serial.print("Setting AP (Access Point)…");
  Serial.println(
      WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!"
  );

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop(){
    time_t currentTime = time(nullptr);

    if (regime == "1") {
        bool shouldTrigger  = (currentTime - now > 10);
        if (shouldTrigger) {
            if (output12State == "on") {
                Serial.println("OFF");
                now = time(nullptr);
                digitalWrite(output12, LOW);
                output12State = "off";
            } else {
                Serial.println("ON");
                now = time(nullptr);
                digitalWrite(output12, HIGH);
                output12State = "on";
            }
        }
    } else if  (regime == "2" ) {
        bool shouldTrigger  = (currentTime - now > 20);
        if (shouldTrigger) {
            if (output12State == "on") {
                Serial.println("OFF");
                now = time(nullptr);
                digitalWrite(output12, LOW);
                output12State = "off";
            } else {
                Serial.println("ON");
                now = time(nullptr);
                digitalWrite(output12, HIGH);
                output12State = "on";
            }
        }
    } else if  (regime == "3" ) {
        bool shouldTrigger  = (currentTime - now > 600);
        if (shouldTrigger) {
            if (output12State == "on") {
                Serial.println("OFF");
                now = time(nullptr);
                digitalWrite(output12, LOW);
                output12State = "off";
            } else {
                Serial.println("ON");
                now = time(nullptr);
                digitalWrite(output12, HIGH);
                output12State = "on";
            }
        }
    } else if  (regime == "4" ) {
        bool shouldTrigger  = (currentTime - now > 900);
        if (shouldTrigger) {
            if (output12State == "on") {
                Serial.println("OFF");
                now = time(nullptr);
                digitalWrite(output12, LOW);
                output12State = "off";
            } else {
                Serial.println("ON");
                now = time(nullptr);
                digitalWrite(output12, HIGH);
                output12State = "on";
            }
        }
    } else if  (regime == "5" ) {
        bool shouldTrigger  = (currentTime - now > 1200);
        if (shouldTrigger) {
            if (output12State == "on") {
                Serial.println("OFF");
                now = time(nullptr);
                digitalWrite(output12, LOW);
                output12State = "off";
            } else {
                Serial.println("ON");
                now = time(nullptr);
                digitalWrite(output12, HIGH);
                output12State = "on";
            }
        }
    } else {
        // 0 state
        digitalWrite(output12, LOW);
        output12State = "off";
    }

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /1/on") >= 0)
            {
                Serial.println("Regime 1");
                regime = "1";
            } else if (header.indexOf("GET /2/on") >= 0) {
                Serial.println("Regime 2");
                regime = "2";
            } else if (header.indexOf("GET /3/on") >= 0) {
                Serial.println("Regime 3");
                regime = "3";
            } else if (header.indexOf("GET /4/on") >= 0) {
                Serial.println("Regime 4");
                regime = "4";
            } else if (header.indexOf("GET /5/on") >= 0) {
                Serial.println("Regime 5");
                regime = "5";
            } else if (header.indexOf("GET /off") >= 0) {
                Serial.println("Regime 0");
                regime = "0";
            }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>Central balkan</h1>");

            if (regime == "1") {
                client.println("<h2>Regime: 1 (10 sec / 10 sec)</h2>");
            } else if (regime == "2") {
                client.println("<h2>Regime: 2 (20 sec / 20 sec)</h2>");
            } else if (regime == "3") {
                client.println("<h2>Regime: 3 (10 min / 10 min)</h2>");
            } else if (regime == "4") {
                client.println("<h2>Regime: 4 (15 min / 15 min)</h2>");
            } else if (regime == "5") {
                client.println("<h2>Regime: 5 (20 min / 20 min)</h2>");
            } else if (regime == "0") {
                client.println("<h2>Regime: 0 (OFF)</h2>");
            }

          client.println("<p><a href=\"/1/on\"><button class=\"button\">Regime 1 ( 10 sec / 10 sec )</button></a></p>");

          client.println("<p><a href=\"/2/on\"><button class=\"button\">Regime 2 ( 20 sec / 20 sec )</button></a></p>");

          client.println("<p><a href=\"/3/on\"><button class=\"button\">Regime 3 ( 10 min / 10 min )</button></a></p>");

          client.println("<p><a href=\"/4/on\"><button class=\"button\">Regime 4 ( 15 min / 15 min )</button></a></p>");

          client.println("<p><a href=\"/5/on\"><button class=\"button\">Regime 5 ( 20 min / 20 min )</button></a></p>");

          client.println("<p><a href=\"/off\"><button class=\"button button2\">OFF</button></a></p>");

          client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
