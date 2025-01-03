#include <globals.h>

static double lastLat = 0;
static double lastLon = 0;
static unsigned long lastUpdateTime = 0;

float maxSpeed = 0;
float odometer = 0;
float tripDistance = 0;
float maxAltitudeChange = 0;
float initialAltitude = 0;
bool isInitialAltitudeSet = false;
float currentSpeed = 0;
float currentAltitude = 0;

void updateTracking () {

    if (gps.location.isValid()) {

        //Speedometer
        currentSpeed = gps.speed.mph(); 
        if (currentSpeed > maxSpeed) {
            maxSpeed = currentSpeed;
        }

        //Odometer
        if (lastLat == 0 || lastLon == 0) {
            lastLat = gps.location.lat();
            lastLon = gps.location.lng();
        }
        float distanceTraveled = TinyGPSPlus::distanceBetween(
            lastLat, lastLon,
            gps.location.lat(), gps.location.lng()
        ) / 1609.344; // Convert meters to miles
        odometer += distanceTraveled;
        tripDistance += distanceTraveled;
        // Update last known position
        lastLat = gps.location.lat();
        lastLon = gps.location.lng();

        // Altimeter
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
        }
        
    } else {
        //no updates
    }
}