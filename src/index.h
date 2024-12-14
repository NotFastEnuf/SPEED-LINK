// index.h
#ifndef INDEX_H
#define INDEX_H

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>SPEED-LINK Dash</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            text-align: center; 
            margin: 0;
            padding: 0;
            transition: background-color 0.3s, color 0.3s;
            display: flex;
            flex-direction: column;
            min-height: 100vh;
        }
        h1 { font-size: 3em; margin-top: 20px; }
        h2 { font-size: 2em; }
        .page { 
            display: none;
            flex: 1;
            overflow-y: auto;
            padding-bottom: 60px;
        }
        .active { display: block; }
        .nav-buttons {
            display: flex;
            justify-content: space-around;
            background-color: #f0f0f0;
            padding: 10px 0;
            position: fixed;
            bottom: 0;
            left: 0;
            right: 0;
        }
        .nav-buttons button {
            margin: 0;
            padding: 10px 20px;
            font-size: 36px;
            flex: 1;
        }
        .settings-menu {
            position: absolute;
            top: 10px;
            left: 10px;
            cursor: pointer;
            font-size: 24px;
            padding: 5px;
        }
        .settings-overlay {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0,0,0,0.5);
            z-index: 1000;
        }
        .settings-content {
            background: white;
            color: black;
            width: 300px;
            margin: 100px auto;
            padding: 20px;
            border-radius: 10px;
            text-align: center;
        }
        .dark-mode {
            background-color: #121212;
            color: #ffffff;
        }
        .toggle-switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }
        .toggle-switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
            border-radius: 34px;
        }
        .slider::before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }
        input:checked + .slider {
            background-color: #2196F3;
        }
        input:checked + .slider::before {
            transform: translateX(26px);
        }
        .setting-item {
            margin-bottom: 20px;
        }
        #speed { font-size: 24px; font-weight: bold; }
        #maxSpeed { font-size: 36px; font-weight: bold; color:red; }
        #odometer { font-size: 24px; font-weight:bold; }
        #trip { font-size:36px; font-weight:bold; color:red; }
        #altitude { font-size :24px; font-weight:bold; }
        #maxAltitudeChange { font-size :36px; font-weight:bold; color:red; }
        button {
           margin :10px ;
           padding :10px 20px ;
           font-size :16px ;
           cursor :pointer ;
       }
    </style>
    <script>
        function showPage(pageId) {
             var pages = document.getElementsByClassName('page');
             for (var i = 0; i < pages.length; i++) {
                 pages[i].classList.remove('active');
             }
             document.getElementById(pageId).classList.add('active');
         }

         function toggleSettings() {
             var settingsOverlay = document.getElementById('settingsOverlay');
             settingsOverlay.style.display = 
                 settingsOverlay.style.display === 'block' ? 'none' : 'block';
         }

         function toggleDarkMode() {
             document.body.classList.toggle('dark-mode');
             localStorage.setItem('darkMode', document.body.classList.contains('dark-mode'));
         }

         function toggleUnits() {
             var unitsLabel = document.getElementById('unitsLabel');
             if (unitsLabel.textContent === 'Metric') {
                 unitsLabel.textContent = 'Imperial';
                 localStorage.setItem('units', 'imperial');
             } else {
                 unitsLabel.textContent = 'Metric';
                 localStorage.setItem('units', 'metric');
             }
             updateData();
         }

         function updateData() {
             fetch('/data')
                 .then(response => response.text())
                 .then(data => {
                     const lines = data.split('<br>');
                     document.getElementById('latitude').textContent = lines[0];
                     document.getElementById('longitude').textContent = lines[1];
                     document.getElementById('satellites').textContent = lines[2];
                     document.getElementById('speed').textContent = lines[3];
                     document.getElementById('maxSpeed').textContent = lines[4];
                     document.getElementById('odometer').textContent = lines[5];
                     document.getElementById('trip').textContent = lines[6];
                     document.getElementById('altitude').textContent = lines[7];
                     document.getElementById('maxAltitudeChange').textContent = lines[8];
                 })
                 .catch(error => {
                     console.error('Fetch error:', error);
                 });
         }

         function clearMaxSpeed() {
             fetch('/clearMaxSpeed')
                 .then(() => updateData())
                 .catch(error => console.error('Error clearing max speed:', error));
         }

         function clearTrip() {
             fetch('/clearTrip')
                 .then(() => updateData())
                 .catch(error => console.error('Error clearing trip:', error));
         }

         function clearMaxAltitudeDelta() {
             fetch('/clearMaxAltitudeDelta')
                 .then(() => updateData())
                 .catch(error => console.error('Error clearing max altitude change:', error));
         }

         window.onload = function() {
             if (localStorage.getItem('darkMode') === 'true') {
                 document.body.classList.add('dark-mode');
                 document.getElementById('darkModeToggle').checked = true; 
             } 
             if (localStorage.getItem('units') === 'imperial') { 
                 document.getElementById('unitsLabel').textContent = 'Imperial'; 
                 document.getElementById('unitsToggle').checked = true; 
             } 
             setInterval(updateData, 1000);
         }
    </script>
</head>
<body>
    <h1><i>SPEED-LINK</i></h1>

    <div class="settings-menu" onclick="toggleSettings()">
        &#8942;
    </div>

    <div id="settingsOverlay" class="settings-overlay" onclick="toggleSettings()">
        <div class="settings-content" onclick="event.stopPropagation()">
          <h2>Settings</h2>
          <div class="setting-item">
              <label>Dark Mode</label><br>
              <label class="toggle-switch">
                  <input type="checkbox" id="darkModeToggle" onchange="toggleDarkMode()">
                  <span class="slider"></span>
              </label>
          </div>
          <div class="setting-item">
              <label>Units:</label><br>
              <span id="unitsLabel">Metric</span><br>
              <label class="toggle-switch">
                  <input type="checkbox" id="unitsToggle" onchange="toggleUnits()">
                  <span class="slider"></span>
              </label>
          </div>
      </div>
    </div>

    <div id="dashboard" class="page active">
        <h2>Dashboard</h2>
        
       <div id="speed">Current Speed:<span>Loading...</span></div>
       <div id="maxSpeed">Max Speed:<span>Loading...</span></div>
       <button onclick="clearMaxSpeed()">Clear Max Speed</button>

       <div id="odometer">Odometer:<span>Loading...</span></div>
       <div id="trip">Trip:<span>Loading...</span></div>
       <button onclick="clearTrip()">Clear Trip</button>

       <div id="altitude">Altitude:<span>Loading...</span></div>
       <div id="maxAltitudeChange">Max Altitude Change:<span>Loading...</span></div>
       <button onclick="clearMaxAltitudeDelta()">Clear Max Altitude Change</button>

    </div>

    <div id="race" class="page">
        <h2>Race</h2>
        <div id="raceTime">Race Time: <span>00:00:00</span></div>
        <div id="lapTime">Lap Time: <span>00:00:00</span></div>
        <div id="bestLap">Best Lap: <span>--:--:--</span></div>
        <button onclick="startRace()">Start Race</button>
        <button onclick="stopRace()">Stop Race</button>
        <button onclick="newLap()">New Lap</button>
    </div>

    <div id="gps" class="page">
        <h2>GPS Data</h2>
        <div id="latitude">Latitude: <span>Loading...</span></div>
        <div id="longitude">Longitude: <span>Loading...</span></div>
        <div id="satellites">Satellites: <span>Loading...</span></div>
        <div id="hdop">HDOP: <span>Loading...</span></div>
        <div id="altitude">Altitude: <span>Loading...</span></div>
    </div>

    <div class="nav-buttons">
        <button onclick="showPage('dashboard')">DASHBOARD</button>
        <button onclick="showPage('race')">RACE</button>
        <button onclick="showPage('gps')">GPS</button>
    </div>

</body>
</html>
)rawliteral";

#endif // INDEX_H
