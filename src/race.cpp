#include <globals.h> 

unsigned long lastMovementTime = 0;
unsigned long stationaryStartTime = 0;
double lastLat = 0, lastLon = 0;
float lastSpeed = 0;
float trapSpeed = 0;
float elapsedTime = 0;
float reactionTime = 0;
float raceDistance = 0; // Set this based on race type

enum RaceState { 
    INACTIVE, 
    PRE_STAGE, 
    STAGE, 
    YELLOW1, 
    YELLOW2, 
    YELLOW3, 
    GREEN_LIGHT, 
    RED_LIGHT,
    VEHICLE_START, 
    RACING, 
    FINISHED 
};
RaceState currentRaceState = INACTIVE;
unsigned long greenLightTime = 0;
unsigned long yellowStartTime = 0;
unsigned long vehicleStartTime = 0;
unsigned long raceFinishTime = 0;


static bool raceRequested = false;
static bool raceCancelled = false;





void handleRaceLogic() {
  if (!raceRequested) return;

  if (raceCancelled) {
    raceCancelled = false;
    raceRequested = false; 
    currentRaceState = INACTIVE;
    return;
  }

  switch(currentRaceState) {
    case INACTIVE:
      // No Activity
      break;

    case PRE_STAGE:
      if (gps.location.isValid()) {
        if (lastLat == 0 && lastLon == 0) {
          lastLat = gps.location.lat();
          lastLon = gps.location.lng();
          stationaryStartTime = millis();
        } else if (TinyGPSPlus::distanceBetween(lastLat, lastLon, gps.location.lat(), gps.location.lng()) < 0.5) {
          if (millis() - stationaryStartTime >= 3000) {
            currentRaceState = STAGE;
          }
        } else {
          lastLat = gps.location.lat();
          lastLon = gps.location.lng();
          stationaryStartTime = millis();
        }
      }
      break;

    case STAGE:
      if (gps.location.isValid()) {
        if (TinyGPSPlus::distanceBetween(lastLat, lastLon, gps.location.lat(), gps.location.lng()) > 0.5) {
          lastMovementTime = millis();
        } else if (millis() - lastMovementTime >= 3000) {
          currentRaceState = YELLOW1;
          yellowStartTime = millis();
        }
      }
      break;

    case YELLOW1:
      if (gps.speed.isValid() && gps.speed.mph() > 1) {
          reactionTime = (millis() - greenLightTime) / 1000.0;
          currentRaceState = RED_LIGHT;
      }
      if (millis() - yellowStartTime >= 500) {
        currentRaceState = YELLOW2;
      }
      break;

    case YELLOW2:
      if (gps.speed.isValid() && gps.speed.mph() > 1) {
          reactionTime = (millis() - greenLightTime) / 1000.0;
          currentRaceState = RED_LIGHT;
      }
      if (millis() - yellowStartTime >= 1000) {
        currentRaceState = YELLOW3;
      }
      break;

    case YELLOW3:
      if (gps.speed.isValid() && gps.speed.mph() > 1) {
          reactionTime = (millis() - greenLightTime) / 1000.0;
          currentRaceState = RED_LIGHT;
      }
      if (millis() - yellowStartTime >= 1500) {
        currentRaceState = GREEN_LIGHT;
        greenLightTime = millis();
      }
      break;

    case GREEN_LIGHT:
      if (gps.speed.isValid() && gps.speed.mph() > 1) {
        reactionTime = (millis() - greenLightTime) / 1000.0;
        currentRaceState = VEHICLE_START;
      }
      break;

    case RED_LIGHT:
      // Handle red light condition (false start)
      currentRaceState = VEHICLE_START;
      // You might want to add a delay here before resetting to INACTIVE
      // delay(3000);
      // currentRaceState = INACTIVE;
      break;

    case VEHICLE_START:
      vehicleStartTime = millis();
      currentRaceState = RACING;
      break;

    case RACING:
      if (gps.location.isValid() && gps.speed.isValid()) {
        float distanceTraveled = TinyGPSPlus::distanceBetween(
          lastLat, lastLon, gps.location.lat(), gps.location.lng());
        
        if (distanceTraveled >= raceDistance) {
          currentRaceState = FINISHED;
        } else {
          lastLat = gps.location.lat();
          lastLon = gps.location.lng();
          lastSpeed = gps.speed.mph();
        }
      }
      break;

    case FINISHED:
      // Interpolate for more accurate ET and Trap Speed
      float overshoot = TinyGPSPlus::distanceBetween(lastLat, lastLon, gps.location.lat(), gps.location.lng()) - raceDistance;
      float totalDistance = TinyGPSPlus::distanceBetween(lastLat, lastLon, gps.location.lat(), gps.location.lng());
      float interpolationFactor = 1 - (overshoot / totalDistance);
      
      trapSpeed = lastSpeed + (gps.speed.mph() - lastSpeed) * interpolationFactor;
      unsigned long finishTime = millis();
      elapsedTime = ((finishTime - vehicleStartTime)/1000) * interpolationFactor;
      
      // Record final results
      // Reset race state
      raceRequested = false;
      currentRaceState = INACTIVE;
      break;
  }
}






void handleStartRace() {
  currentRaceState = PRE_STAGE;
  // Initialize race variables
  //set race distance here in meters **************  NOTHING IS GOING TO WORK WITHOUT HANDLING THIS
  server.send(200, "text/plain", "Race started");
}

void handleCancelRace() {
  currentRaceState = INACTIVE;
  // Reset race variables
  server.send(200, "text/plain", "Race cancelled");
}

// Helper function to convert RaceState enum to string
String getRaceStateName(RaceState state) {
    switch(state) {
        case INACTIVE: return "Inactive";
        case PRE_STAGE: return "Pre-Stage";
        case STAGE: return "Stage";
        case GREEN_LIGHT: return "Green Light";
        case VEHICLE_START: return "Vehicle Start Detected";
        case RACING: return "Racing";
        case FINISHED: return "Finished";
        default: return "Unknown";
    }
}

void handleRaceDisplay() {
    String data = "";
    if (gps.location.isValid()) {
        data += "Race State: " + String(getRaceStateName(currentRaceState)) + "<br>";
        data += "Latitude: " + String(gps.location.lat(), 6) + "<br>";
        data += "Longitude: " + String(gps.location.lng(), 6) + "<br>";
        data += "Speed: " + String(gps.speed.mph()) + " mph<br>";
        
        switch(currentRaceState) {
            case PRE_STAGE:
                data += "Waiting for vehicle to be stationary...<br>";
                break;
            case STAGE:
                data += "Vehicle staged. Waiting for final positioning...<br>";
                break;
            case GREEN_LIGHT:
                data += "GREEN LIGHT! GO!<br>";
                break;
            case VEHICLE_START:
                data += "Reaction time: " + String(reactionTime) + " seconds<br>";
                break;
            case RACING:
                data += "Distance traveled: " + String(TinyGPSPlus::distanceBetween(lastLat, lastLon, gps.location.lat(), gps.location.lng())) + " meters<br>";
                data += "Elapsed time: " + String((millis() - vehicleStartTime) / 1000.0) + " seconds<br>";
                break;
            case FINISHED:
                data += "Race finished!<br>";
                data += "Final E/T: " + String(elapsedTime) + " seconds<br>";
                data += "Trap speed: " + String(trapSpeed) + " mph<br>";
                break;
            default:
                data += "Waiting for race to start...<br>";
        }
    } else {
        data += "GPS signal not valid<br>";
    }

    server.send(200, "text/plain", data);
}


