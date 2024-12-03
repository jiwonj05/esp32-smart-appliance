#include <pgmspace.h>
#include "page_main.h"

const char PAGE_MAIN[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html lang="en" class="js-focus-visible">
<head>
<title>The Cloud Based Smart Switch</title>
  <style>
    table {
      position: relative;
      width:100%;
      border-spacing: 0px;
    }
    tr {
      border: 1px solid white;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
    }
    th {
      height: 20px;
      padding: 3px 15px;
      background-color: #343a40;
      color: #FFFFFF !important;
      }
    td {
      height: 20px;
      padding: 3px 15px;
    }
    .status {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      font-weight: normal;
      color: #000000;
      padding-left: 10px;
      text-align: left;
    }
    .tabledata {
      font-size: 24px;
      position: relative;
      padding-left: 5px;
      padding-top: 5px;
      height:   25px;
      border-radius: 5px;
      color: #FFFFFF;
      line-height: 20px;
      transition: all 200ms ease-in-out;
      background-color: #00AA00;
    }
    .bodytext {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 24px;
      text-align: left;
      font-weight: light;
      border-radius: 5px;
      display:inline;
    }
    .navbar {
      width: 100%;
      height: 50px;
      margin: 0;
      padding: 10px 0px;
      background-color: #FFF;
      color: #000000;
      border-bottom: 5px solid #293578;
    }
    .fixed-top {
      position: fixed;
      top: 0;
      right: 0;
      left: 0;
      z-index: 1030;
    }
    .navtitle {
      float: left;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 50px;
      font-weight: bold;
      line-height: 50px;
      padding-left: 20px;
   }
    .category {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: bold;
      font-size: 32px;
      line-height: 50px;
      padding: 20px 10px 0px 10px;
      color: #000000;
    }
    .heading {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: normal;
      font-size: 28px;
      text-align: left;
    }
    .btn {
      background-color: #444444;
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 10px;
    }
    .foot {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      position: relative;
      height:   30px;
      text-align: center;   
      color: #AAAAAA;
      line-height: 20px;
    }
    .timeTracking {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      font-weight: normal;
      color: #000000;
      padding-left: 10px;
      text-align: left;
      margin-top: 10px;
    }
    table tr:first-child th:first-child {
      border-top-left-radius: 5px;
    }
    table tr:first-child th:last-child {
      border-top-right-radius: 5px;
    }
    table tr:last-child td:first-child {
      border-bottom-left-radius: 5px;
    }
    table tr:last-child td:last-child {
      border-bottom-right-radius: 5px;
    }
  </head>
  </style>
  <body style="background-color: #efefef" onload="initialize()">
  
    <header>
      <div class="navbar fixed-top">
          <div class="container">
            <div class="navtitle">The Cloud Based Smart Switch</div>  
          </div>
      </div>
    </header>
  
  <main class="container" style="margin-top:70px">
  <div class="category">Voltage Readings</div>
  <div style="border-radius: 10px !important;">
    <table style="width:50%">
      <colgroup>
        <col span="1" style="background-color:rgb(230,230,230); width: 20%; color:#000000;">
        <col span="1" style="background-color:rgb(200,200,200); width: 15%; color:#000000;">
      </colgroup>
      <tr>
        <th><div class="heading">Pin</div></th>
        <th><div class="heading">Power (W)</div></th>
      </tr>
      <tr>
        <td><div class="bodytext">Analog pin 32</div></td>
        <td><div class="tabledata" id="v0">Loading...</div></td>
      </tr>
    </table>
  </div>
  <div id="timeTracking" class="timeTracking">Time Tracking: On for 0 hours 0 minutes 0 seconds</div>
</main>
 <div class="category">Control</div>
    <button id="toggleButton" class="btn" onclick="toggleRelay()">Toggle</button>
    <div id="status" class="status">Status: Not connected</div>
  <footer class="foot">ESP32 Controls and Data Updates</footer>
  </body>
  
  <script type = "text/javascript">
  // Varibales for time tracking
    let applianceOnTime = 0;  // in seconds
    let applianceStatus = "not connected";  // Initial appliance status
    let applianceTimer;  // To store the timer reference

  // Initialize the page
    function initialize() {
      updateStatus();  // Set initial status
      setInterval(updateStatus, 1000); // Check status every second
    }

  function toggleRelay() {
  if (applianceStatus === "not connected" || applianceStatus === "off") {
    fetch('/relay/on')
      .then(function(response) {
        applianceStatus = "on";  // Appliance turns on
        updateStatusDisplay();
        startTimer();  // Start the timer when appliance is on
      });
  } else if (applianceStatus === "on") {
    fetch('/relay/off')
      .then(function(response) {
        applianceStatus = "off";  // Appliance turns off
        updateStatusDisplay();
        stopTimer();  // Stop the timer when appliance is off
		fetchCost();  // Fetch and display the cost after appliance turns off
      });
    }
  }
  
    // Start the timer when appliance is on
    function startTimer() {
      applianceTimer = setInterval(function() {
        applianceOnTime++;
        updateTimeTracking();
      }, 1000);  // Update every second
    }

    // Stop the timer when the appliance is off or not connected
    function stopTimer() {
      clearInterval(applianceTimer);  // Stop the time tracking
    }

    // Update the time tracking display
  function updateTimeTracking() {
    let hours = Math.floor(applianceOnTime / 3600);
    let minutes = Math.floor((applianceOnTime % 3600) / 60);
    let seconds = applianceOnTime % 60;
    
    document.getElementById("timeTracking").innerText = 
      `Time Tracking: On for ${hours} hours ${minutes} minutes ${seconds} seconds`;
  }
	
  // Fetch the cost 
  function fetchCost() {
    let cost = (applianceOnTime * costPerSecond).toFixed(2);  // Calculate cost based on time
    document.getElementById("costTracking").innerText = `Cost: $${cost}`;
  }

  // update the status 
  function updateStatus() {
    fetch("/getStatus")
    .then(function(response) {
      return response.text();
    })
    .then(function(status) {
      if (status === "on") {
        applianceStatus = "on";
      } else if (status === "off") {
        applianceStatus = "off";
      } else {
        applianceStatus = "not connected";
      }
      displayStatus();
    });
}

// Display the above on the webpage
function displayStatus() {
  var statusText;
  var statusColor;

  if (applianceStatus === "on") {
    statusText = "Status: On";
    statusColor = "#00aa00"; // Green color for "on"
  } else if (applianceStatus === "off") {
    statusText = "Status: Off";
    statusColor = "#ffcc00"; // Yellow color for "off"
  } else {
    statusText = "Status: Not connected";
    statusColor = "#ff0000"; // Red color for "not connected"
  }

  // Update the text and color on the page
  document.getElementById("status").innerText = statusText;
  document.getElementById("status").style.color = statusColor;
}
setInterval(updateStatus, 1000);

// Run the initialize function when the page is loaded
window.onload = initialize;

/* change to measure power
    function updateVoltage() {
  fetch("/getVoltage")  // HTTP request to fetch voltage
    .then(function(response) { 
      return response.text();  // Parse voltage response
    })
    .then(function(voltage) {
      if (voltage && !isNaN(voltage)) {  // Check if the voltage is a valid number
        document.getElementById("v0").innerText = voltage + " V";  // Update the voltage display
      } else {
        document.getElementById("v0").innerText = "Loading...";  // Keep "Loading..." if voltage is invalid
      }
    });
}

// Call updateVoltage() to get latest voltage every second
setInterval(updateVoltage, 1000);
*/
</script>
<div id="costTracking" class="timeTracking">Cost: $0.00</div>
</body>

</html>

)=====";