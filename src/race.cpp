#include <globals.h> 

unsigned long lastMovementTime = 0;
unsigned long stationaryStartTime = 0;
double lastLat = 0, lastLon = 0;
float lastSpeed = 0;
float trapSpeed = 0;
float elapsedTime = 0;
float reactionTime = 0;
float raceDistance = 132; // Set this based on race type
unsigned long greenLightTime = 0;
unsigned long yellowStartTime = 0;
unsigned long vehicleStartTime = 0;
unsigned long raceFinishTime = 0;

static bool raceRequested = false;

enum RaceState { 
    INACTIVE, 
    PRE_STAGE, 
    STAGE, 
    YELLOW1, 
    YELLOW2, 
    YELLOW3,
    RED_LIGHT, 
    GREEN_LIGHT, 
    VEHICLE_START, 
    RACING, 
    FINISHED 
};
RaceState currentRaceState = INACTIVE;







void handleRaceLogic() {
  if (!raceRequested) return;

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

    case STAGE:  //totally broken
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
          currentRaceState = RED_LIGHT;
      }
      if (millis() - yellowStartTime >= 500) {
        currentRaceState = YELLOW2;
      }
      break;

    case YELLOW2:
      if (gps.speed.isValid() && gps.speed.mph() > 1) {
          currentRaceState = RED_LIGHT;
      }
      if (millis() - yellowStartTime >= 1000) {
        currentRaceState = YELLOW3;
      }
      break;

    case YELLOW3:
      if (gps.speed.isValid() && gps.speed.mph() > 1) {
          currentRaceState = RED_LIGHT;
      }
      if (millis() - yellowStartTime >= 1500) {
        currentRaceState = GREEN_LIGHT;
        greenLightTime = millis();
      }
      break;
    
    case RED_LIGHT:
      // Handle red light condition (false start)
      reactionTime = (millis() - greenLightTime) / 1000.0;
      currentRaceState = VEHICLE_START;
      break;

    case GREEN_LIGHT:
      if (gps.speed.isValid() && gps.speed.mph() > 1) {
        reactionTime = (millis() - greenLightTime) / 1000.0;
        currentRaceState = VEHICLE_START;
      }
      break;

    case VEHICLE_START:
      vehicleStartTime = millis();
      lastLat = gps.location.lat();
      lastLon = gps.location.lng();
      currentRaceState = RACING;
      break;

    case RACING:
      if (gps.location.isValid() && gps.speed.isValid()) {
        float distanceTraveled = TinyGPSPlus::distanceBetween(
          lastLat, lastLon, gps.location.lat(), gps.location.lng());
        
        if (distanceTraveled >= (raceDistance * 0.3048)) {
          currentRaceState = FINISHED;
        } else {
          lastSpeed = gps.speed.mph();
          unsigned long timeNow = millis();
          elapsedTime = (timeNow - vehicleStartTime)/1000;
        }
      }
      //update elapsed time here
      break;

    case FINISHED:
      // Interpolate for more accurate ET and Trap Speed
      float overshoot = TinyGPSPlus::distanceBetween(lastLat, lastLon, gps.location.lat(), gps.location.lng()) - (raceDistance * 0.3048);
      float totalDistance = TinyGPSPlus::distanceBetween(lastLat, lastLon, gps.location.lat(), gps.location.lng());
      float interpolationFactor = 1 - (overshoot / totalDistance);
      // Record final results
      trapSpeed = lastSpeed + (gps.speed.mph() - lastSpeed) * interpolationFactor;
      unsigned long finishTime = millis();
      elapsedTime = ((finishTime - vehicleStartTime)/1000) * interpolationFactor;
      // Reset race state
      raceRequested = false;
      break;
  }
}



void resetRaceGlobals (void) {
// Reset race variables
  lastMovementTime = 0;
  stationaryStartTime = 0;
  lastLat = 0, lastLon = 0;
  lastSpeed = 0;
  trapSpeed = 0;
  elapsedTime = 0;
  reactionTime = 0;
  greenLightTime = 0;
  yellowStartTime = 0;
  vehicleStartTime = 0;
  raceFinishTime = 0;
}


void handleStartRace() {
  raceRequested = true;
  currentRaceState = PRE_STAGE;
  resetRaceGlobals();
  // TODO: Set race distance here in meters
  
  // Create a JSON response with more information
  DynamicJsonDocument doc(256);
  doc["message"] = "Race started";
  doc["raceState"] = "PRE_STAGE";
  doc["raceDistance"] = raceDistance;  // Assuming raceDistance is a global variable
  String response;
  serializeJson(doc, response);
  //Serial.println("Race start request received. Sending response: " + response);  // Log to serial monitor
  server.send(200, "application/json", response);
}



void handleCancelRace() {
  raceRequested = false;
  currentRaceState = INACTIVE;
  resetRaceGlobals();
  server.send(200, "application/json", "{\"message\":\"Race cancelled\"}");
}



void handleUpdateRaceDistance() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, body);
        
        if (error) {
            server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }
        
        if (!doc.containsKey("distance")) {
            server.send(400, "application/json", "{\"error\":\"Missing distance parameter\"}");
            return;
        }
        
        int distance = doc["distance"];
        raceDistance = distance;
        
        server.send(200, "application/json", "{\"message\":\"Race distance updated\",\"distance\":" + String(distance) + "}");
    } else {
        server.send(400, "application/json", "{\"error\":\"Invalid request\"}");
    }
}


// Helper function to convert RaceState enum to string
String getRaceStateName(RaceState state) {
    switch(state) {
        case INACTIVE: return "INACTIVE";
        case PRE_STAGE: return "PRE_STAGE";
        case STAGE: return "STAGE";
        case YELLOW1: return "YELLOW1";
        case YELLOW2: return "YELLOW2";
        case YELLOW3: return "YELLOW3";
        case RED_LIGHT: return "RED_LIGHT";
        case GREEN_LIGHT: return "GREEN_LIGHT";
        case VEHICLE_START: return "VEHICLE_START";
        case RACING: return "RACING";
        case FINISHED: return "FINISHED";
        default: return "Unknown";
    }
}

void handleRaceDisplay() {
    DynamicJsonDocument doc(1024);
    
    if (gps.location.isValid()) {
        doc["raceState"] = getRaceStateName(currentRaceState);
        doc["raceDistance"] = raceDistance;
        doc["currentSpeed"] = gps.speed.mph();
        doc["reactionTime"] = reactionTime;
        doc["elapsedTime"] = elapsedTime;
        doc["trapSpeed"] = trapSpeed; 
        /*
        
        switch(currentRaceState) {
            case INACTIVE:
                break;
            case PRE_STAGE:
            case STAGE:
                // No additional data needed
                break;
            case RED_LIGHT:
                doc["reactionTime"] = reactionTime;
                doc["elapsedTime"] = elapsedTime;
                break;
            case YELLOW1:
            case YELLOW2:
            case YELLOW3:
            case GREEN_LIGHT:
            case VEHICLE_START:
                doc["reactionTime"] = reactionTime;
                doc["elapsedTime"] = elapsedTime;
            case RACING:
                doc["reactionTime"] = reactionTime;
                doc["elapsedTime"] = elapsedTime;
            case FINISHED:
                 doc["reactionTime"] = reactionTime;
                 doc["elapsedTime"] = elapsedTime;
                 doc["trapSpeed"] = trapSpeed;  
        }*/
    } else {
        doc["error"] = "GPS signal not valid";
    }

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}



