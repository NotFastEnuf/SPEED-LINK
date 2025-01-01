#include <globals.h>

void handleGPSDisplay() {
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


