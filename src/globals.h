#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

extern ESP8266WebServer server;
extern TinyGPSPlus gps;



extern float currentSpeed;
extern float currentAltitude;
extern float maxSpeed;
extern float odometer;
extern float tripDistance;
extern float maxAltitudeChange;
extern float initialAltitude;
extern bool isInitialAltitudeSet;

#endif // GLOBALS_H
