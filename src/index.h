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
        #gpsLatitude, #gpsLongitude, #gpsTime, #gpsDate, 
        #gpsSpeed, #gpsCourse, #gpsAltitude, 
        #gpsSatellites, #gpsHdop {
            font-size: 24px; 
            font-weight: bold; 
        }
        button {
           margin :10px ;
           padding :10px 20px ;
           font-size :16px ;
           cursor :pointer ;
       }

        /* New styles for the race tab */
        #race {
            max-width: 600px;
            margin: 0 auto;
            background-color: #f0f0f0;
            padding: 20px;
            border-radius: 10px;
        }
        .light {
            width: 50px;
            height: 50px;
            border-radius: 25px;
            display: inline-block;
            margin: 5px;
        }
        .half-light {
            width: 25px;
            height: 25px;
            border-radius: 12.5px;
            display: inline-block;
            margin: 2.5px;
        }
        .prestage { background-color: #664400; }
        .stage { background-color: #664400; }
        .yellow { background-color: #664400; }
        .green { background-color: #004400; }
        .red { background-color: #660000; }
        .light.active.prestage, .half-light.active.prestage { background-color: #ffaa00; }
        .light.active.stage, .half-light.active.stage { background-color: #ffaa00; }
        .light.active.yellow { background-color: yellow; }
        .light.active.green { background-color: lime; }
        .light.active.red { background-color: red; }
        #raceTree {
            margin: 20px 0;
            display: flex;
            flex-direction: column;
            align-items: center;
        }
        .tree-row {
            display: flex;
            justify-content: center;
            margin-bottom: 10px;
        }
        .label {
            font-weight: bold;
            margin-bottom: 5px;
        }
        .hidden {
            display: none;
        }
        @keyframes flash {
            0%, 100% { opacity: 1; }
            50% { opacity: 0; }
        }
        .flashing {
            animation: flash 0.5s linear infinite;
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
}

function updateData() {
    var activePage = document.querySelector('.page.active');
    var pageId = activePage.id;

    if (pageId === 'dashboard') {
       updateDashboardData();
   } else if (pageId === 'gps') {
       updateGPSData();
   } else if (pageId === 'race') {
       updateRaceData();
   }
}

function updateDashboardData() {
        fetch('/dashboarddata')
            .then(response => response.text())
            .then(data => {
                const lines = data.split('<br>');
                document.getElementById('speed').textContent = lines[0];
                document.getElementById('maxSpeed').textContent = lines[1];
                document.getElementById('odometer').textContent = lines[2];
                document.getElementById('trip').textContent = lines[3];
                document.getElementById('altitude').textContent = lines[4];
                document.getElementById('maxAltitudeChange').textContent = lines[5];
            })
            .catch(error => {
                console.error('Fetch error:', error);
            });
    }

function updateGPSData() {
    fetch('/gpsdata')
        .then(response => response.text())
        .then(data => {
            const lines = data.split('<br>');
            document.getElementById('gpsLatitude').textContent = lines[0];
            document.getElementById('gpsLongitude').textContent = lines[1];
            document.getElementById('gpsTime').textContent = lines[2];
            document.getElementById('gpsDate').textContent = lines[3];
            document.getElementById('gpsSpeed').textContent = lines[4];
            document.getElementById('gpsCourse').textContent = lines[5];
            document.getElementById('gpsAltitude').textContent = lines[6];
            document.getElementById('gpsSatellites').textContent = lines[7];
            document.getElementById('gpsHdop').textContent = lines[8];
        })
    .catch(error => {
        console.error('Fetch error:', error);
    });
}

function updateRaceData() {
   fetch('/racedata')
       .then(response => response.json())
       .then(data => {
           document.getElementById('raceState').innerHTML = `Race State: <span>${data.raceState || 'N/A'}</span>`;
           document.getElementById('raceDistance').innerHTML = `Race Distance: <span>${data.raceDistance || 'N/A'}</span> feet`;

           const currentSpeedElement = document.getElementById('currentSpeed');
           const reactionTimeElement = document.getElementById('reactionTime');
           const elapsedTimeElement = document.getElementById('elapsedTime');
           const trapSpeedElement = document.getElementById('trapSpeed');

           if (data.raceState === 'FINISHED') {
               currentSpeedElement.classList.add('hidden');
               trapSpeedElement.classList.remove('hidden');
               trapSpeedElement.innerHTML = `Trap Speed: <span>${data.trapSpeed ? parseFloat(data.trapSpeed).toFixed(2) : 'N/A'}</span> mph`;
           } else {
               currentSpeedElement.classList.remove('hidden');
               currentSpeedElement.innerHTML = `Current Speed: <span>${data.currentSpeed ? parseFloat(data.currentSpeed).toFixed(2) : 'N/A'}</span> mph`;
               trapSpeedElement.classList.add('hidden');
           }

           if (['RACING', 'FINISHED', 'RED_LIGHT'].includes(data.raceState)) {
               reactionTimeElement.classList.remove('hidden');
               elapsedTimeElement.classList.remove('hidden');
               reactionTimeElement.innerHTML = `Reaction Time: <span>${data.reactionTime ? parseFloat(data.reactionTime).toFixed(3) : 'N/A'}</span> seconds`;
               elapsedTimeElement.innerHTML = `Elapsed Time: <span>${data.elapsedTime ? parseFloat(data.elapsedTime).toFixed(3) : 'N/A'}</span> seconds`;
           } else {
               reactionTimeElement.classList.add('hidden');
               elapsedTimeElement.classList.add('hidden');
           }

           updateRaceTree(data.raceState);
       })
       .catch(error => console.error('Error fetching race data:', error));
}





function updateRaceTree(state) {
   const allLights = document.querySelectorAll('.light, .half-light, .prestage, .stage, .yellow, .green');

   allLights.forEach(light => light.classList.remove('active', 'flashing', 'red'));
   
   // Ensure base classes are present
   const baseLights = document.querySelectorAll('.prestage', '.stage', '.yellow', '.green');
      //baseLights.forEach(light => light.classList.add(light.className));

   baseLights.forEach(light => { // Reset to default color
    light.className.split(' ').forEach(cls => {
        if (cls) light.classList.add(cls);
    });
   });

   if (state === 'INACTIVE') return;

   switch(state) {
       case 'PRE_STAGE':
           document.querySelectorAll('.prestage').forEach(light => light.classList.add('active'));
           break;

       case 'STAGE':
           document.querySelectorAll('.prestage, .stage').forEach(light => light.classList.add('active'));
           break;

       case 'YELLOW1':
           document.querySelectorAll('.prestage, .stage').forEach(light => light.classList.add('active'));
           document.getElementById('yellow1').classList.add('active');
           document.getElementById('yellow2').classList.add('active');
           break;

       case 'YELLOW2':
           document.querySelectorAll('.prestage, .stage').forEach(light => light.classList.add('active'));
           document.getElementById('yellow3').classList.add('active');
           document.getElementById('yellow4').classList.add('active');
           break;

       case 'YELLOW3':
           document.querySelectorAll('.prestage, .stage').forEach(light => light.classList.add('active'));
           document.getElementById('yellow5').classList.add('active');
           document.getElementById('yellow6').classList.add('active');
           break;

       case 'GREEN_LIGHT':
           document.querySelectorAll('.prestage, .stage').forEach(light => light.classList.add('active'));
           document.querySelectorAll('.yellow').forEach(light => light.classList.add('active'));
           document.getElementById('green1').classList.add('active');
           document.getElementById('green2').classList.add('active');
           break;

       case 'RED_LIGHT':
           const mainLights = document.querySelectorAll('#yellow1, #yellow2, #yellow3, #yellow4, #yellow5, #yellow6, #green1, #green2');
           mainLights.forEach(light => light.classList.add('active', 'red', 'flashing'));
           break;
       
       case 'RACING':
           document.querySelectorAll('.prestage, .stage').forEach(light => light.classList.add('active'));
           document.querySelectorAll('.yellow').forEach(light => light.classList.add('active'));
           document.getElementById('green1').classList.add('active');
           document.getElementById('green2').classList.add('active');
           break;

       case 'FINISHED':
           document.querySelectorAll('.prestage, .stage, .yellow').forEach(light => light.classList.add('active'));
           document.querySelectorAll('.yellow').forEach(light => light.classList.add('flashing'));
           document.getElementById('green1').classList.add('active', 'flashing');
           document.getElementById('green2').classList.add('active', 'flashing');
           break;

       default:
          console.warn("Unknown race state:", state);
   }
}

function startRace() {
   console.log("Attempting to start race...");
   fetch('/startRace', { method:'POST' })
       .then(response => {
           console.log("Start race response status:", response.status);
           return response.json();
       })
       .then(data => {
           console.log("Race start response:", data);
           updateRaceData();
       })
       .catch(error => console.error("Error starting race:", error));
}


function clearRace() {
   fetch('/clearRace', { method:'POST' })
       .then(response => response.json())
       .then(data => {
           console.log("Race cancel response:", data);  // Add this line
           updateRaceData();
       })
       .catch(error => console.error("Error clearing race:", error));
}


function updateRaceDistance() {
   const raceType = document.getElementById("raceType").value;
   const customDistanceInput = document.getElementById("customDistance");

   let distance;

   if (raceType === "custom") {
       customDistanceInput.style.display = "inline-block";
       distance = parseFloat(customDistanceInput.value);
       if (isNaN(distance) || distance <= 0) {
           console.error("Invalid custom distance");
           return; // Exit the function if the custom distance is invalid
       }
   } else {
       customDistanceInput.style.display = "none";
       distance = raceType === "eighth" ? 132 : 264; // Feet
   }
   
   fetch("/updateRaceDistance", {
        method: 'POST',
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify({ distance }),
   })
   .then(response => {
       if (!response.ok) {
           throw new Error('Network response was not ok');
       }
       return response.json();
   })
   .then(data => console.log("Race distance updated:", data))
   .catch(error => console.error("Error updating race distance:", error));
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
        setInterval(updateData, 100);
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

    <div id="gps" class="page">
        <h2>GPS Data</h2>
        <p id="gpsLatitude">Latitude: <span>Loading...</span></p>
        <p id="gpsLongitude">Longitude: <span>Loading...</span></p>
        <p id="gpsTime">Time: <span>Loading...</span></p>
        <p id="gpsDate">Date: <span>Loading...</span></p>
        <p id="gpsSpeed">Current Speed: <span>Loading...</span></p>
        <p id="gpsCourse">Course: <span>Loading...</span></p>
        <p id="gpsAltitude">Altitude: <span>Loading...</span></p>
        <p id="gpsSatellites">Satellites: <span>Loading...</span></p>
        <p id="gpsHdop">HDOP: <span>Loading...</span></p>
    </div>


    <div class="nav-buttons">
        <button onclick="showPage('dashboard')">DASHBOARD</button>
        <button onclick="showPage('race')">RACE</button>
        <button onclick="showPage('gps')">GPS</button>
    </div>

    <div id="race" class="page">
        <h2>Race</h2>
        <div id="raceState">Race State: <span>Inactive</span></div>
        <div id="raceDistance">Race Distance: <span>0</span> feet</div>
        <div id="currentSpeed">Current Speed: <span>0</span> mph</div>
        <div id="reactionTime" class="hidden">Reaction Time: <span>0</span> seconds</div>
        <div id="elapsedTime" class="hidden">Elapsed Time: <span>0</span> seconds</div>
        <div id="trapSpeed" class="hidden">Trap Speed: <span>0</span> mph</div>
        
        <div id="raceTree">
            <!-- Pre-Stage Lights -->
            <div class="label">Pre-Stage</div>
            <div class="tree-row">
                <div id="prestage1" class="half-light prestage"></div>
                <div id="prestage2" class="half-light prestage"></div>
                <div id="prestage3" class="half-light prestage"></div>
                <div id="prestage4" class="half-light prestage"></div>
            </div>

            <!-- Stage Lights -->
            <div class="label">Stage</div>
            <div class="tree-row">
                <div id="stage1" class="half-light stage"></div>
                <div id="stage2" class="half-light stage"></div>
                <div id="stage3" class="half-light stage"></div>
                <div id="stage4" class="half-light stage"></div>
            </div>

            <!-- Yellow Lights -->
            <div class="label">Race Lights</div>
            <div class="tree-row">
                <div id="yellow1" class="light yellow"></div>
                <div id="yellow2" class="light yellow"></div>
            </div>
            <div class="tree-row">
                <div id="yellow3" class="light yellow"></div>
                <div id="yellow4" class="light yellow"></div>
            </div>
            <div class="tree-row">
                <div id="yellow5" class="light yellow"></div>
                <div id="yellow6" class="light yellow"></div>
            </div>

            <!-- Green Lights -->
            <div class="tree-row">
                <div id="green1" class="light green"></div>
                <div id="green2" class="light green"></div>
            </div>
        </div>

        <!-- Race Type Selection -->
        <select id='raceType' onchange='updateRaceDistance()'>
            <option value='eighth'>Scale 1/8 Mile</option>
            <option value='quarter'>Scale 1/4 Mile</option>
            <option value='custom'>Custom</option>
        </select>

        <!-- Custom Distance Input -->
        <input type='number' id='customDistance' style='display:none;' placeholder='Enter distance in feet' onchange='updateRaceDistance()'>

        <!-- Control Buttons -->
        <br/>
        <button onclick='startRace()'>Start Race</button>
        <button onclick='clearRace()'>Clear Race</button>

    </div>
</body>
</html>




)rawliteral";

#endif // INDEX_H