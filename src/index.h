const char* HTML_CONTENT_GAMEMODE = R"===(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>
/* Ensure full-page layout */
html, body {
  margin: 0;
  padding: 0;
  height: 100%;
  overflow-x: hidden; /* Stops horizontal scroll */
}

.tablink {
  background-color: #555;
  color: white; /* Default text color */
  float: left;
  border: none;
  outline: none;
  cursor: pointer;
  padding: 14px 16px;
  font-size: 17px;
  width: 50%;
  transition: background-color 0.3s, color 0.3s; /* Smooth effect */
}

.tablink:hover {
  background-color: #777;
}

/* Selected tab styling */
.tablink.active {
  background-color: #ddd !important; /* Light background when selected */
  color: black !important; /* Keep text black when selected */
}

/* Ensure the tab content fills the page */
.tabcontent {
  color: black;
  display: none;
  padding: 50px 20px;
  width: 100%;
  height: calc(100vh - 50px); /* Full height minus button space */
  position: absolute;
  top: 50px; /* To prevent overlap with the buttons */
  left: 0;
  overflow: auto;
}

#serialContainer {
  background-color: azure;  /* Dark background like a terminal */
  color: black;         /* Bright text for visibility */
  font-family: "Courier New", monospace;
  padding: 10px;
  border-radius: 10px;
  height: 300px;            /* Fixed height */
  width: 95%;
  overflow-y: auto;         /* Enable vertical scrollbar */
  white-space: pre-wrap;    /* Preserve line breaks */
  border: 2px solid limegreen;
  scrollbar-width: thin;    /* Makes the scrollbar thinner (for Firefox) */
  scrollbar-color: gray; /* Custom scrollbar color */
}

/* Custom Scrollbar for Chrome, Edge, and Safari */
#serialContainer::-webkit-scrollbar {
  width: 8px; /* Scrollbar width */
}

#serialContainer::-webkit-scrollbar-track {
  background: black; /* Track color */
}

#serialContainer::-webkit-scrollbar-thumb {
  background: limegreen; /* Scrollbar color */
  border-radius: 4px;
}

#serialOutput {
  margin: 0;
}

/* Background colors for different tabs */
#GameMode { background-color: azure; }
#SerialMonitor { background-color: aliceblue; }
</style>

<title>Game Mode Selection</title>
</head>
<body>

<!-- Tab buttons -->
<button class="tablink" id="defaultOpen" onclick="openPage('GameMode', this, 'azure')">GameMode Selector</button>
<button class="tablink" onclick="openPage('SerialMonitor', this, 'aliceblue')">Serial Monitor</button>

<!-- Tab content sections -->
<div id="GameMode" class="tabcontent">
  <h3>Game Mode:</h3>
  <form>
    <input type="radio" id="mode1" name="game_mode" value="1" checked>
    <label for="mode1">#1 Quick Draw</label><br>
    
    <input type="radio" id="mode2" name="game_mode" value="2">
    <label for="mode2">#2 Search and Destroy</label><br>
    
    <input type="radio" id="mode3" name="game_mode" value="3">
    <label for="mode3">#3 Blackout</label><br>
    
    <input type="radio" id="mode4" name="game_mode" value="4">
    <label for="mode4">#4 Random</label><br>
    
    <input type="radio" id="mode5" name="game_mode" value="5">
    <label for="mode5">#5 Twin Shot</label><br>
    <br>
    <button onclick="submitGamemode(event)"> Start Game </button>
  </form>
</div>

<div id="SerialMonitor" class="tabcontent">
   <h3>Serial Monitor:</h3>
  <div id="serialContainer">
    <pre id="serialOutput">Waiting for data...</pre>
  </div>
</div>

<script>
function openPage(pageName, element, color) {
  var i, tabcontent, tablink;
  
  // Hide all tab contents
  tabcontent = document.getElementsByClassName('tabcontent');
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = 'none';
  }

  // Reset all button colors and remove "active" class
  tablink = document.getElementsByClassName('tablink');
  for (i = 0; i < tablink.length; i++) {
    tablink[i].style.backgroundColor = ''; // Reset background
    tablink[i].style.color = 'white'; // Reset text color
    tablink[i].classList.remove('active'); // Remove active class
  }

  // Show the selected tab and update button color
  document.getElementById(pageName).style.display = 'block';
  element.style.backgroundColor = color;
  element.style.color = 'black'; // Keep text black when selected
  element.classList.add('active'); // Add active class
}

var socket;
// Set the default open tab
document.addEventListener("DOMContentLoaded", function() {
  var defaultBtn = document.getElementById('defaultOpen');
  if (defaultBtn) {
    defaultBtn.click();
  }
});

function init(){
	socket = new WebSocket("ws://" + window.location.hostname + ":81");
	socket.onmessage = function(event){
		updateSerial(event);
	};
}

function updateSerial(event){
  const serialMonitor = document.getElementById("serialContainer");
	console.log("ESP32 Response: ", event.data);
	const newMessage = document.createElement("p");
  var data = JSON.parse(event.data);
	newMessage.textContent = data.log;
	serialMonitor.appendChild(newMessage);
  serialMonitor.scrollTop = serialMonitor.scrollHeight;
}

function submitGamemode(event){
  event.preventDefault();
  const selectedGamemode = document.querySelector('input[name="game_mode"]:checked');
  if(!selectedGamemode){
    alert("Please selecte a game mode.");
    return;
  }
  const data = JSON.stringify({game_mode: selectedGamemode.value});
  socket.send(data);
  consol.log("Sent: ", data);
}

window.onload = function(event){
	init();
}

</script>

</body>
</html>

)===";