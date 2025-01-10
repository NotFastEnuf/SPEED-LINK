#define GPS_DATA_DEBUG

#include <globals.h>
#include <index.h>
#include <dashboard.h>
#include <gps.h>
#include <race.h>
#include <tracking.h>
#include <mywebsockets.h>

const char *ssid = "RC_GPS_Tracker";
const char *password = "password123";

WebSocketsServer webSocket = WebSocketsServer(81);
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
    server.on("/racedata", HTTP_GET, handleRaceDisplay);
    server.on("/startRace", HTTP_POST, handleStartRace);
    server.on("/clearRace", HTTP_POST, handleCancelRace);
    server.on("/updateRaceDistance", HTTP_POST, handleUpdateRaceDistance);

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

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
    updateClientData();
    webSocket.loop();
    server.handleClient();
    yield();
}
