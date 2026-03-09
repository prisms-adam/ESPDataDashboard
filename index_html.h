/*
 * index_html.h
 * ------------
 * Description: Defines the HTML structure of the dashboard.
 *
 * STUDENT TASKS:
 * 1. (Optional) Change the <h1> title to your project name.
 */

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP Data Dashboard</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <header>
        <h1>ESP Data Dashboard</h1>
    </header>

    <div id="dashboard">
        <!-- New: Hardware Status Panel -->
        <div id="actuator-status-container"></div>

        <div id="dashboard-content"></div>

        <div class="controls">
            <button id="btn-start" onclick="startRecording()">Start Recording</button>
            <button id="btn-stop" onclick="stopRecording()" disabled>Stop Recording</button>
            <button id="btn-clear" onclick="clearData()">Clear Data</button>
            <button id="btn-download" onclick="downloadCSV()">Download CSV</button>
        </div>

        <div class="status-bar">
            Status: <span id="status-text">Ready</span> | Data Points: <span id="data-count">0</span>
        </div>
    </div>

    <script src="script.js"></script>
</body>
</html>
)rawliteral";
