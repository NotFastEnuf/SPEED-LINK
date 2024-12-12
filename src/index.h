// index.h
#ifndef INDEX_H
#define INDEX_H

const char* htmlPage = R"rawliteral(

<!DOCTYPE html>
    <html>
    <head>
      <title>RC Car GPS Tracker</title>
      <style>
        body { font-family: Arial, sans-serif; text-align: center; }
        #gpsData { font-size: 18px; margin: 20px; }
        #speed { font-size: 24px; font-weight: bold; }
        #maxSpeed { font-size: 36px; font-weight: bold; color: red; }
        button { font-size: 18px; margin-top: 20px; }
      </style>
      <script>
        function updateData() {
          fetch('/data')
            .then(response => response.text())
            .then(data => {
              const lines = data.split('<br>');
              let latitude = '';
              let longitude = '';
              let satellites = '';
              let speed = '';
              let maxSpeed = '';

              if (lines.length >= 5) {
                latitude = lines[0];
                longitude = lines[1];
                satellites = lines[2];
                speed = lines[3];
                maxSpeed = lines[4];
              }

              document.getElementById('gpsData').innerHTML =
                latitude + '<br>' +
                longitude + '<br>' +
                satellites + '<br>' +
                '<div id="speed">' + speed + '</div>' +
                '<div id="maxSpeed">' + maxSpeed + '</div>';
            })
            .catch(error => {
              console.error('Fetch error:', error);
            });
        }

        function clearMaxSpeed() {
          fetch('/clearMaxSpeed')
            .then(() => {
              updateData(); // Refresh data after clearing max speed
            })
            .catch(error => {
              console.error('Error clearing max speed:', error);
            });
        }

        setInterval(updateData, 1000);
      </script>
    </head>
    <body>
      <h1>RC Car GPS Tracker</h1>
      <div id="gpsData">Loading GPS data...</div>
      <button onclick="clearMaxSpeed()">Clear Max Speed</button>
    </body>
    </html>
    )rawliteral";

#endif // INDEX_H