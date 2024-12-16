# SPEED-LINK Dashboard

SPEED-LINK Dashboard is a GPS-based tracking system for radio controlled vehicles, providing real-time performance data in a modern and accessible interface. This project combines affordable hardware and open source software to create an elite RC tool on your own.

## Features

- Real-time GPS tracking
- Speed monitoring with max speed recording
- Odometer and trip distance tracking
- Altitude tracking with max altitude change - Monitor your max jump or climb height
- Dark mode for better visibility in low-light conditions
- Unit toggle between metric and imperial systems
- Three main views: Dashboard, Race, and GPS Data

## Hardware Requirements

- Wemos D1 Mini development board based on ESP8266 microcontroller
- GPS module (compatible with TinyGPS++ library)
- Misc wire, servo cable, solder, soldering iron, heat-shrink

## Software Requirements

- Visual Studio Code
- PlatformIO extension for VS Code

## Installation

1. Install Visual Studio Code from the official website.
2. Open VS Code and install the PlatformIO extension from the Extensions marketplace.
3. Clone or download this repository to your local machine.

## Setup

1. Open the project folder in VS Code.
2. PlatformIO will automatically detect the project structure and configure the environment.
3. Connect the GPS module to your ESP8266 dev board:
- GPS TX to ESP8266 RX (D4)
- GPS RX to ESP8266 TX (D1) - Optional/Unused at this time 
- GPS VCC to ESP8266 3.3V or Vin as specified by gps module
- GPS GND to ESP8266 GND
4. Connect servo cable to ESP8266 dev board
- Servo Plug VCC to ESP8266 Vin
- Servo Plug GND to ESP8266 GND
5.  Assemble a "backpack" by fixing the gps module and esp8266 together in a stack with double sided foam tape.  Add a velcro strap to the stack and slide the assembly into a section of large heat shrink tubing - leaving the usb port exposed.  Apply heat carefully so as not to damage the velcro strap during shrinking.

## Usage

1. Click the PlatformIO icon in the VS Code sidebar.
2. Use the "Build" button to compile the project.
3. Use the "Upload" button to flash the code to your ESP8266.
4. Open the Serial Monitor in PlatformIO to view debug output and find the IP address of your ESP8266.  (Default is 192.168.4.1)
5. Connect to the WiFi network created by the ESP8266 (default SSID: RC_GPS_Tracker, password: password123).
6. Open a web browser and navigate to the IP address displayed in the Serial Monitor.
7. The dashboard will load, showing real-time GPS data.
8. Use the navigation buttons to switch between Dashboard, Race, and GPS views.
9. Access settings by clicking the three dots in the top-left corner to toggle dark mode or change units.
10.  Install the finished SPEED-LINK GPS module in the rc vehicle by strapping it to the vehicle lipo or other secure position. Plug the servo cable into the an unused port on the vehicle receiver to supply the module with power.
11.  Sendit

## Troubleshooting

- If no GPS data is displayed, ensure the GPS module has a clear view of the sky and check wiring connections.  The D4 pin is connected to the onboard LED of the Wemos D1 Mini, it will flash on and off as serial data is received.
- If the web interface doesn't load, verify you're connected to the correct WiFi network and check the IP address in the Serial Monitor.
- For more accurate altitude readings, consider calibrating the GPS module or using a barometric sensor.

## License

This project is licensed under MIT license.

## Acknowledgments

Special thanks to the rc, open source, and maker communities as well as the libraries that made this project possible.

## Under Development

-race tab ...stage up scale 1/4 or 1/8 mile races.  race head to head using bluetooth.  report ET, trap speed, reaction time, false start
-units switch ...not supported yet
-altitude feature for jump height feature is only operating on raw gps data and needs further development.  Raw gps altitude can not hold a stable floor(slowly floats up and down) but very nicely will pick up jump.
-report trip distance in feet / meters to better evaluate inaccuracy
