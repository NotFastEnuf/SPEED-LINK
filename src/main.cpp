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

float maxSpeed = 0;
float odometer = 0;
float tripDistance = 0;
float maxAltitudeChange = 0;
float initialAltitude = 0;
bool isInitialAltitudeSet = false;

// New variables for distance calculation
static double lastLat = 0;
static double lastLon = 0;
static unsigned long lastUpdateTime = 0;

void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

void handleData() {
    String data = "";
    float currentSpeed = 0;
    float currentAltitude = 0;

    if (gps.location.isValid()) {
        data += "Latitude: " + String(gps.location.lat(), 6) + "<br>";
        data += "Longitude: " + String(gps.location.lng(), 6) + "<br>";
        data += "Satellites: " + String(gps.satellites.value()) + "<br>";
        
        currentSpeed = gps.speed.mph();
        data += "Speed: " + String(currentSpeed, 2) + " mph<br>";
        
        if (currentSpeed > maxSpeed) {
            maxSpeed = currentSpeed;
        }
        data += "Max Speed: " + String(maxSpeed, 2) + " mph<br>";

        // Update odometer and trip distance
        unsigned long currentTime = millis();
        if (lastUpdateTime > 0) {
            float distanceTraveled = TinyGPSPlus::distanceBetween(
                lastLat, lastLon,
                gps.location.lat(), gps.location.lng()
            ) / 1609.344; // Convert meters to miles
            odometer += distanceTraveled;
            tripDistance += distanceTraveled;
        }

        // Update last known position
        lastLat = gps.location.lat();
        lastLon = gps.location.lng();
        lastUpdateTime = currentTime;

        data += "Odometer: " + String(odometer, 2) + " miles<br>";
        data += "Trip: " + String(tripDistance, 2) + " miles<br>";

        // Altitude calculations
        if (gps.altitude.isValid()) { // Check if altitude is valid
            currentAltitude = gps.altitude.feet();
            if (!isInitialAltitudeSet) {
                initialAltitude = currentAltitude;
                isInitialAltitudeSet = true;
            }
            float altitudeChange = abs(currentAltitude - initialAltitude);
            if (altitudeChange > maxAltitudeChange) {
                maxAltitudeChange = altitudeChange;
            }
            data += "Altitude: " + String(currentAltitude, 2) + " ft<br>";
            data += "Max Altitude Change: " + String(maxAltitudeChange, 2) + " ft";
        } else {
            data += "Altitude data not valid<br>";
        }
    } else {
        data += "GPS signal not valid<br>";
    }

    server.send(200, "text/plain", data);
}

void handleClearMaxSpeed() {
    maxSpeed = 0;
    server.send(200, "text/plain", "Max speed cleared");
}

void handleClearTrip() {
    tripDistance = 0;
    server.send(200, "text/plain", "Trip distance cleared");
}

void handleClearMaxAltitudeDelta() {
    maxAltitudeChange = 0;
    initialAltitude = gps.altitude.feet();
    isInitialAltitudeSet = false;
    server.send(200, "text/plain", "Max altitude change cleared");
}

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    #ifdef GPS_DATA_DEBUG
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    #endif
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.on("/clearMaxSpeed", handleClearMaxSpeed);
    server.on("/clearTrip", handleClearTrip);
    server.on("/clearMaxAltitudeDelta", handleClearMaxAltitudeDelta);
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

    yield();
}
