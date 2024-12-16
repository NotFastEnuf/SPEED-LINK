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

void handleGPSData() {
    String data = "";
    
    if (gps.location.isValid()) {
        // Latitude and Longitude
        data += "Latitude: " + String(gps.location.lat(), 6) + "<br>";
        data += "Longitude: " + String(gps.location.lng(), 6) + "<br>";

        // Time and Date
        if (gps.time.isValid()) {
            int hour = gps.time.hour();
            int minute = gps.time.minute();
            int second = gps.time.second();
            data += "Time: " + String(hour) + ":" + String(minute) + ":" + String(second) + "<br>";
        } else {
            data += "Time: Not available<br>";
        }

        if (gps.date.isValid()) {
            int day = gps.date.day();
            int month = gps.date.month();
            int year = gps.date.year();
            data += "Date: " + String(day) + "/" + String(month) + "/" + String(year) + "<br>";
        } else {
            data += "Date: Not available<br>";
        }

        // Speed and Course
        float currentSpeed = gps.speed.mph();
        data += "Speed: " + String(currentSpeed, 2) + " mph<br>";

        if (gps.course.isValid()) {
            float courseValue = gps.course.deg();
            data += "Course: " + String(courseValue, 2) + "°<br>";
        } else {
            data += "Course: Not available<br>";
        }

        // Altitude
        if (gps.altitude.isValid()) {
            float currentAltitude = gps.altitude.feet();
            data += "Altitude: " + String(currentAltitude, 2) + " ft<br>";
        } else {
            data += "Altitude: Not available<br>";
        }

        // Satellites and HDOP
        data += "Satellites: " + String(gps.satellites.value()) + "<br>";

        if (gps.hdop.isValid()) {
            float hdopValue = gps.hdop.hdop();
            data += "HDOP: " + String(hdopValue, 2) + "<br>";
        } else {
            data += "HDOP: Not available<br>";
        }
    } else {
        data += "GPS signal not valid<br>";
    }

    server.send(200, "text/plain", data);
}


void handleDashboardData() {
    String data = "";
    float currentSpeed = 0;
    float currentAltitude = 0;

    if (gps.location.isValid()) {
        currentSpeed = gps.speed.mph();
        data += "Speed: " + String(currentSpeed, 2) + " mph<br>";
        
        if (currentSpeed > maxSpeed) {
            maxSpeed = currentSpeed;
        }
        data += "Max Speed: " + String(maxSpeed, 2) + " mph<br>";

        // Update odometer and trip distance  -  TODO - examine and compare using gps timestamps, arduino time, and ignoring time in odometer calculation
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
    initialAltitude = gps.altitude.feet(); // Reset initial altitude
    isInitialAltitudeSet = false; // Reset flag
    server.send(200, "text/plain", "Max altitude change cleared");
}

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    
    WiFi.softAP(ssid, password);
    
    server.on("/", handleRoot);
    server.on("/gpsdata", handleGPSData);          // For GPS tab
    server.on("/dashboarddata", handleDashboardData); // For Dashboard tab
    server.on("/clearMaxSpeed", handleClearMaxSpeed);
    server.on("/clearTrip", handleClearTrip);
    server.on("/clearMaxAltitudeDelta", handleClearMaxAltitudeDelta);
    
    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    server.handleClient();

    yield();
}
