/*
 * style_css.h
 * -----------
 * Description: Defines the visual styling of the dashboard.
 *
 * STUDENT TASKS:
 * 1. Change colors or fonts to customize the look.
 */

const char style_css[] PROGMEM = R"rawliteral(
body {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  text-align: center;
  margin: 0;
  padding: 0;
  background-color: #f0f2f5;
  color: #333;
}

header {
  background-color: #0078D7;
  color: white;
  padding: 1.5rem;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

#dashboard {
  max-width: 1000px;
  margin: 20px auto;
  padding: 10px;
}

/* Hardware Status Section */
#actuator-status-container {
  display: flex;
  justify-content: center;
  gap: 15px;
  margin-bottom: 20px;
  flex-wrap: wrap;
}

.actuator-badge {
  background: white;
  padding: 8px 16px;
  border-radius: 20px;
  font-size: 0.9rem;
  font-weight: 600;
  border: 1px solid #ddd;
  color: #888;
  display: flex;
  align-items: center;
  gap: 8px;
  transition: all 0.3s ease;
}

.actuator-badge::before {
  content: "";
  display: inline-block;
  width: 10px;
  height: 10px;
  border-radius: 50%;
  background-color: #ccc;
}

/* "ON" State Styling */
.actuator-badge.active {
  color: #28a745;
  border-color: #28a745;
  background-color: #e8f5e9;
  box-shadow: 0 0 10px rgba(40, 167, 69, 0.2);
}

.actuator-badge.active::before {
  background-color: #28a745;
  box-shadow: 0 0 5px #28a745;
}

#dashboard-content {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(450px, 1fr));
  gap: 20px;
  margin-bottom: 20px;
}

.sensor-card {
  background: white;
  padding: 15px;
  border-radius: 12px;
  box-shadow: 0 4px 6px rgba(0,0,0,0.05);
  border: 1px solid #e0e0e0;
}

.sensor-label { font-size: 1.2rem; font-weight: bold; color: #555; }
.data-display { font-size: 2.5rem; font-weight: bold; margin: 10px 0; color: #0078D7; }
.unit { font-size: 1rem; color: #888; margin-left: 5px; }

canvas {
  width: 100%;
  height: 250px;
  background-color: #fafafa;
  border-top: 1px solid #eee;
  margin-top: 15px;
}

.controls {
  background: white;
  padding: 20px;
  border-radius: 12px;
  display: flex;
  justify-content: center;
  gap: 15px;
  box-shadow: 0 4px 6px rgba(0,0,0,0.05);
}

button {
  padding: 12px 24px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  border: none;
  border-radius: 8px;
  color: white;
  transition: all 0.2s;
}

#btn-start { background-color: #28a745; }
#btn-stop { background-color: #dc3545; }
#btn-clear { background-color: #6c757d; }
#btn-download { background-color: #17a2b8; }

button:hover { opacity: 0.9; transform: translateY(-1px); }
button:disabled { background-color: #ccc; cursor: not-allowed; }

.status-bar { margin-top: 15px; font-size: 0.9rem; color: #666; }

footer {
  margin-top: 30px;
  padding-top: 15px;
  border-top: 1px solid #ddd;
  font-size: 0.85rem;
  color: #888;
}
)rawliteral";
