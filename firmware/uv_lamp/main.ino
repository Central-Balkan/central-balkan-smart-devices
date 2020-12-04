#include <ESP8266WebServer.h>


IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

const char* ssid     = "UVLampCentralBalkan";
const char* password = "centralbalkan";

// Set web server port number to 80
/* WiFiServer server(80); */
ESP8266WebServer server(80); //creating the server at port 80

// Variable to store the HTTP request
String header;

// State
unsigned long lastChangeMadeAt = millis();
const int output12 = 12;
unsigned int workTime = 5000; // 5 seconds by default
unsigned int restTime = 5000; // 5 seconds by default
bool isCurrentlyWorking = true;  // Start working by default


void handleRoot() {
    Serial.println("HTTP: / request");
    server.send(200, "text/plain", "Central Balkan LTD");
}

void handleSetConfig() {
    Serial.println("HTTP: Setting config");

    String workValue = server.arg("work");
    String restValue = server.arg("rest");

    if (workValue != "" && restValue != "") {
        Serial.print("Updating work and rest to: ");
        Serial.print(workValue);
        Serial.print(" and ");
        Serial.println(restValue);
        workTime = workValue.toInt() * 1000;
        restTime = restValue.toInt() * 1000;
        server.send(200, "text/plain", "Set config successfully");
    } else {
        Serial.println("Bad config: ");
        Serial.println("Work: ");
        Serial.println(workValue);
        Serial.println("Rest: ");
        Serial.println(restValue);

        server.send(400, "text/plain", "Bad config");
    }

}

void handleGetConfig() {
    Serial.println("HTTP: Getting config");
    int isCurrentlyWorkingInt;
    int timeLeft;

    unsigned long currentTime = millis();
    unsigned int currentWorkingTime = (currentTime - lastChangeMadeAt);

    if (isCurrentlyWorking) {
        isCurrentlyWorkingInt = 1;
        timeLeft = workTime - currentWorkingTime;
    } else {
        isCurrentlyWorkingInt = 0;
        timeLeft = restTime - currentWorkingTime;
    }

    const String result = (String)workTime + "/" + (String)restTime + "/" + (String)isCurrentlyWorkingInt + "/" + (String)timeLeft;

    server.send(200, "text/plain", result);
}

void handleNotFound() {
    Serial.println("HTTP: Not found");
    server.send(404, "text/plain", "404: Not found");
}

void setup() {
  // This method does the following:
  // - set the baud rate
  // - set pin 12 (D6) mode to OUTPUT
  // - Inialize wifi network
  // - Notify the networks status via serial port
  Serial.begin(115200);
  Serial.print("Starting access point: ");

  pinMode(output12, OUTPUT);
  digitalWrite(output12, HIGH);

  Serial.print("Setting AP (Access Point)…");
  Serial.println(
      WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!"
  );

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/config/set/", handleSetConfig);
  server.on("/config/get/", handleGetConfig);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void UvLampTurnOff() {
    // Responsible for:
    // - Notify lamp stopping via serial port
    // - Reset the current "lastChangeMadeAt"
    // - Set the 12 pin to LOW
    // - Set the current isCurrentlyWorking state to false
    Serial.println("Stopping the UV Lamp");
    lastChangeMadeAt = millis();
    digitalWrite(output12, LOW);
    isCurrentlyWorking = false;
}


void UvLampTurnOn() {
    // Responsible for:
    // - Notify lamp stopping via serial port
    // - Reset the current "lastChangeMadeAt"
    // - Set the 12 pin to LOW
    // - Set the current isCurrentlyWorking state to false
    Serial.println("Starting the UV Lamp");
    lastChangeMadeAt = millis();
    digitalWrite(output12, HIGH);
    isCurrentlyWorking = true;
}


void manageUVLamp() {
    unsigned long currentTime = millis();
    unsigned int currentWorkingTime = (currentTime - lastChangeMadeAt);

    if (isCurrentlyWorking) {
        // Determine if it should stop working
        if (currentWorkingTime > workTime) UvLampTurnOff();

    } else {
        // Determine if it should start working
        if (currentWorkingTime > restTime) UvLampTurnOn();
    }
}

void loop() {
    manageUVLamp(); // Start/stop lamp if needed
    server.handleClient();  // Listen for HTTP requests
}
