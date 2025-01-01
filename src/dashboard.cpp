#include <globals.h>


/* New variables for distance calculation
static double lastLat = 0;
static double lastLon = 0;
static unsigned long lastUpdateTime = 0;

float maxSpeed = 0;
float odometer = 0;
float tripDistance = 0;
float maxAltitudeChange = 0;
float initialAltitude = 0;
bool isInitialAltitudeSet = false;
*/

void handleDashboardDisplay() {
    String data = "";
    if (gps.location.isValid()) {
        data += "Speed: " + String(currentSpeed, 2) + " mph<br>";
        data += "Max Speed: " + String(maxSpeed, 2) + " mph<br>";
        data += "Odometer: " + String(odometer, 2) + " miles<br>";
        data += "Trip: " + String(tripDistance, 2) + " miles<br>";
        if (gps.altitude.isValid()) { // Check if altitude is valid
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