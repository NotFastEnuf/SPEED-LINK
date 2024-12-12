#define GPS_DATA_DEBUG

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <index.h>

const char *ssid = "RC_GPS_Tracker";
const char *password = "password123";

ESP8266WebServer server(80);
SoftwareSerial gpsSerial(D4, D1); // RX, TX
TinyGPSPlus gps;

void handleRoot() {
    server.send(200, "text/html", htmlPage); // Send the HTML content from the string at index.h
}


float maxSpeed = 0;

void handleData() {
    String data = "";
    float currentSpeed = 0;

    if (gps.location.isValid()) {
        data += "Latitude: " + String(gps.location.lat(), 6) + "<br>";
        data += "Longitude: " + String(gps.location.lng(), 6) + "<br>";
        data += "Satellites: " + String(gps.satellites.value()) + "<br>";
        currentSpeed = gps.speed.mph();
        data += "Speed: " + String(currentSpeed, 2) + " mph<br>";
        
        if (currentSpeed > maxSpeed) {
            maxSpeed = currentSpeed;
        }
        data += "Max Speed: " + String(maxSpeed, 2) + " mph";
    } else {
        data += "GPS signal not valid<br>";
    }

    server.send(200, "text/plain", data);
}

void handleClearMaxSpeed() {
    maxSpeed = 0;
    server.send(200, "text/plain", "Max speed cleared");
}


void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600); // Adjust baud rate if necessary
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    #ifdef GPS_DATA_DEBUG
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    #endif
    server.on("/clearMaxSpeed", handleClearMaxSpeed);
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    #ifdef GPS_DATA_DEBUG
    Serial.println("HTTP server started");
    #endif
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    server.handleClient();

    yield(); // Allow background tasks to run
}
