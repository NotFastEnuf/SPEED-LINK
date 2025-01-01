#define GPS_DATA_DEBUG

#include <globals.h>
#include <index.h>
#include <dashboard.h>
#include <gps.h>
#include <race.h>
#include <tracking.h>

const char *ssid = "RC_GPS_Tracker";
const char *password = "password123";

ESP8266WebServer server(80);
SoftwareSerial gpsSerial(D4, D1); // RX, TX
TinyGPSPlus gps;

bool newGPSData = false;

void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    
    WiFi.softAP(ssid, password);
    
    server.on("/", handleRoot);
    server.on("/gpsdata", handleGPSDisplay);          // For GPS tab
    server.on("/dashboarddata", handleDashboardDisplay); // For Dashboard tab
    server.on("/clearMaxSpeed", handleClearMaxSpeed);
    server.on("/clearTrip", handleClearTrip);
    server.on("/clearMaxAltitudeDelta", handleClearMaxAltitudeDelta);
    server.on("/raceData", handleRaceDisplay);
    //server.on("/startrace", handleStartRace);
    //server.on("/cancelrace", handleCancelRace);
    server.begin();
    Serial.println("HTTP server started.");
}

void loop() {
    while (gpsSerial.available() > 0) {
        if (gps.encode(gpsSerial.read())) {
            newGPSData = true;
        }
    }

    if (newGPSData) {
        updateTracking();
        newGPSData = false;
    }
    // Race
    handleRaceLogic();

    server.handleClient();
    yield();
}
