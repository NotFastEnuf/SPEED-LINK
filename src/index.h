// index.h
#ifndef INDEX_H
#define INDEX_H

const char* htmlPage = R"rawliteral(



<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
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
        header {
            background-color: #036eb4;
            color: white;
            text-align: center;
            position: sticky;
            top: 0;
        }
        h1 { font-size: 3em; margin-top: 10px;  margin-bottom: 10px; }
        h2 { font-size: 2em; }
        main {
            flex: 1;
            padding: 0 1rem 1rem 1rem;
            max-width: 600px;
            margin: 0 auto 62px auto;
            background-color: #f0f0f0;
            border-radius: 20px;
        }
        .dark-mode main {
            background-color: #1e1e1e; /* Darker background for main container */
        }
        .page { 
            display: none;
            flex: 1;
            flex-direction: column;
            overflow-y: auto;
        }
        .active { display: block; }
        nav {
            background-color: #34495e;
            position: fixed;
            bottom: 0;
            left: 0;
            right: 0;
            height: 62px; /* Adjust this value based on your button height */
            display: flex;
            justify-content: space-around;
        }
        .nav-buttons {
            display: flex;
            justify-content: space-around;
            width: 100%;
            background-color: #f0f0f0;
        }
        .nav-buttons button {
            margin: 0;
            padding: 10px 20px;
            font-size: 16px;
            font-weight: bold;
            flex: 1;
        }
        .nav-buttons .nav-item.active {
            background-color: #036eb4;
            color: white;
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
        #racerInstructions { font-weight: bold; }
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
            margin: 20px 0 0;
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
        // Hide all pages
        var pages = document.getElementsByClassName('page');
        for (var i = 0; i < pages.length; i++) {
            pages[i].classList.remove('active');
        }
        
        // Show the selected page
        document.getElementById(pageId).classList.add('active');
        
        // Update nav button states
        var navButtons = document.querySelectorAll('.nav-buttons .nav-item');
        navButtons.forEach(button => {
            if (button.getAttribute('onclick').includes(pageId)) {
                button.classList.add('active');
            } else {
                button.classList.remove('active');
            }
        });

        // Store the active tab
        localStorage.setItem('activeTab', pageId);
    }

    // On page load
    document.addEventListener('DOMContentLoaded', function() {
        var activeTab = localStorage.getItem('activeTab');
        if (!activeTab) {
            // Set default tab if none is stored
            activeTab = 'dashboard'; // or whichever tab you want as default
        }
        showPage(activeTab);
    });

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
        updateData(); // Refresh the data with new units
        updateRaceDistance(); // Update race distance display and input placeholder
    }

    function mphToKmh(mph) {
        return (mph * 1.60934).toFixed(2);
    }

    function feetToMeters(feet) {
        return (feet * 0.3048).toFixed(2);
    }

    function milesToKm(miles) {
        return (miles * 1.60934).toFixed(2);
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
                const isMetric = document.getElementById('unitsLabel').textContent === 'Metric';
                
                const speed = parseFloat(lines[0].split(':')[1]);
                const maxSpeed = parseFloat(lines[1].split(':')[1]);
                const odometer = parseFloat(lines[2].split(':')[1]);
                const trip = parseFloat(lines[3].split(':')[1]);
                const altitude = parseFloat(lines[4].split(':')[1]);
                const maxAltitudeChange = parseFloat(lines[5].split(':')[1]);

                document.getElementById('speed').innerHTML = `Current Speed: <span>${isMetric ? mphToKmh(speed) + ' km/h' : speed + ' mph'}</span>`;
                document.getElementById('maxSpeed').innerHTML = `Top Speed: <span>${isMetric ? mphToKmh(maxSpeed) + ' km/h' : maxSpeed + ' mph'}</span>`;
                document.getElementById('odometer').innerHTML = `Odometer: <span>${isMetric ? milesToKm(odometer) + ' km' : odometer + ' miles'}</span>`;
                document.getElementById('trip').innerHTML = `Trip: <span>${isMetric ? milesToKm(trip) + ' km' : trip + ' miles'}</span>`;
                document.getElementById('altitude').innerHTML = `Altitude: <span>${isMetric ? feetToMeters(altitude) + ' m' : altitude + ' ft'}</span>`;
                document.getElementById('maxAltitudeChange').innerHTML = `Max Altitude Change: <span>${isMetric ? feetToMeters(maxAltitudeChange) + ' m' : maxAltitudeChange + ' ft'}</span>`;
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
                const isMetric = document.getElementById('unitsLabel').textContent === 'Metric';
                
                document.getElementById('gpsLatitude').textContent = lines[0];
                document.getElementById('gpsLongitude').textContent = lines[1];
                document.getElementById('gpsTime').textContent = lines[2];
                document.getElementById('gpsDate').textContent = lines[3];
                
                const speed = parseFloat(lines[4].split(':')[1]);
                document.getElementById('gpsSpeed').innerHTML = `Current Speed: <span>${isMetric ? mphToKmh(speed) + ' km/h' : speed + ' mph'}</span>`;
                
                document.getElementById('gpsCourse').textContent = lines[5];
                
                const altitude = parseFloat(lines[6].split(':')[1]);
                document.getElementById('gpsAltitude').innerHTML = `Altitude: <span>${isMetric ? feetToMeters(altitude) + ' m' : altitude + ' ft'}</span>`;
                
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
            const isMetric = document.getElementById('unitsLabel').textContent === 'Metric';
            // Race state is hidden but left in for debugging
            document.getElementById('raceState').innerHTML = `Race State: <span>${data.raceState || 'N/A'}</span>`;
            // Update Racer Instructions based on data.raceState
            //possible states are  INACTIVE, RACE_REQUESTED, PRE_STAGE, STAGE, YELLOW1, YELLOW2, YELLOW3, RED_LIGHT, GREEN_LIGHT, VEHICLE_START, RACING, FINISHED, Unknown

            const racerInstructionsElement = document.getElementById('racerInstructions');
            switch (data.raceState) {
                case 'INACTIVE':
                    racerInstructionsElement.innerHTML = '<span>PRESS START TO RACE!</span>';
                    break;
                case 'RACE_REQUESTED':
                    racerInstructionsElement.innerHTML = '<span>WAIT IN THE STAGING AREA</span>';
                    break;
                case 'PRE_STAGE':
                    racerInstructionsElement.innerHTML = '<span>MOVE TO THE STARTING LINE</span>';
                    break;
                case 'STAGE':
                    racerInstructionsElement.innerHTML = '<span>RACE IS STAGED</span>';
                    break;
                case 'YELLOW1':
                case 'YELLOW2':
                case 'YELLOW3':
                    racerInstructionsElement.innerHTML = '<span>WAIT FOR GREEN</span>';
                    break;
                case 'RED_LIGHT':
                    racerInstructionsElement.innerHTML = '<span>RED LIGHT START - GO! GO! GO!</span>';
                    break;
                case 'GREEN_LIGHT':
                    racerInstructionsElement.innerHTML = '<span>GO! GO! GO!</span>';
                    break;
                case 'VEHICLE_START':
                    racerInstructionsElement.innerHTML = '<span>GO! GO! GO!</span>';
                    break;
                case 'RACING':
                    racerInstructionsElement.innerHTML = '<span>GO! GO! GO!</span>';
                    break;
                case 'FINISHED':
                    racerInstructionsElement.innerHTML = '<span>RACE COMPLETE! CHECK YOUR STATS!</span>';
                    break;
                default:
                    racerInstructionsElement.innerHTML = '<span>LOADING...</span>';
            }

            // Update Race Distance
            document.getElementById('raceDistance').innerHTML = `Race Distance: <span>${isMetric ? feetToMeters(data.raceDistance) + ' m' : data.raceDistance + ' feet'}</span>`;
            // Update the remaining displayed data based on race state
            const currentSpeedElement = document.getElementById('currentSpeed');
            const reactionTimeElement = document.getElementById('reactionTime');
            const elapsedTimeElement = document.getElementById('elapsedTime');
            const trapSpeedElement = document.getElementById('trapSpeed');

            if (data.raceState === 'FINISHED') {
                currentSpeedElement.classList.add('hidden');
                trapSpeedElement.classList.remove('hidden');
                const trapSpeed = data.trapSpeed ? parseFloat(data.trapSpeed).toFixed(2) : 'N/A';
                trapSpeedElement.innerHTML = `Trap Speed: <span>${isMetric ? mphToKmh(trapSpeed) + ' km/h' : trapSpeed + ' mph'}</span>`;
            } else {
                currentSpeedElement.classList.remove('hidden');
                const currentSpeed = data.currentSpeed ? parseFloat(data.currentSpeed).toFixed(2) : 'N/A';
                currentSpeedElement.innerHTML = `Current Speed: <span>${isMetric ? mphToKmh(currentSpeed) + ' km/h' : currentSpeed + ' mph'}</span>`;
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
        case 'RACE_REQUESTED':
            //nothing yet - wait for pre_stage conditions to be met
            break;

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
    const isMetric = document.getElementById('unitsLabel').textContent === 'Metric';

    let distance;

    if (raceType === "custom") {
        customDistanceInput.style.display = "inline-block";
        customDistanceInput.placeholder = isMetric ? "Enter distance in meters" : "Enter distance in feet";
        distance = parseFloat(customDistanceInput.value);
        if (isNaN(distance) || distance <= 0) {
            console.error("Invalid custom distance");
            return; // Exit the function if the custom distance is invalid
        }
        // Convert meters to feet if in metric mode
        if (isMetric) {
            distance = distance / 0.3048; // Convert meters to feet
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
    .then(data => {
        console.log("Race distance updated:", data);
        // Update the displayed race distance
        const displayDistance = isMetric ? feetToMeters(distance) + ' m' : distance + ' feet';
        document.getElementById('raceDistance').innerHTML = `Race Distance: <span>${displayDistance}</span>`;
    })
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
    <header>
        <h1><i>SPEED-LINK</i></h1>
    
        <div class="settings-menu" onclick="toggleSettings()">
        &#8942;
        </div>
    </header>

    <main>
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

        <div id="race" class="page">
            <h2>Race</h2>
            <div id="raceState" class="hidden">Race State: <span>Inactive</span></div>
            <div id="racerInstructions"><span>Loading...</span></div>
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
    </main>

    <nav>
        <div class="nav-buttons">
            <button class="nav-item" onclick="showPage('dashboard')">DASHBOARD</button>
            <button class="nav-item" onclick="showPage('race')">RACE</button>
            <button class="nav-item" onclick="showPage('gps')">GPS</button>
        </div>
    </nav>
</body>
</html>

)rawliteral";

#endif // INDEX_H