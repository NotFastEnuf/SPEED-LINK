#include <mywebsockets.h>

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, 
            webSocket.remoteIP(num)[0], webSocket.remoteIP(num)[1], 
            webSocket.remoteIP(num)[2], webSocket.remoteIP(num)[3], payload);
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
            // Handle incoming messages here
            // Link up old button listeners, etc and remove old server methods
            break;
        default:
            Serial.printf("[%u] Unknown WebSocket event type: %d\n", num, type);
    }
}

void updateClientData (){
//Construct a master data update sheet here
//Simple test for now
String json = "{\"lat\":" + String(gps.location.lat(), 6) + 
              ",\"lon\":" + String(gps.location.lng(), 6) + 
              ",\"speed\":" + String(gps.speed.kmph()) + "}";
webSocket.broadcastTXT(json);
}
