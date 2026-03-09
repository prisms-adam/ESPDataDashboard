/*
 * script_js.h
 * -----------
 * Description: The brains of the dashboard.
 *
 * STUDENT TASKS:
 * 1. Adjust 'updateInterval' (in milliseconds) if needed.
 */

const char script_js[] PROGMEM = R"rawliteral(
let updateInterval = 1000; // Update this value to change your sample rate in ms
let sensors = [];
let outputs = [];
let charts = {};
let experimentData = [];
let isRecording = false;
let recordingStartTime = 0;

const dashboardContent = document.getElementById('dashboard-content');
const actuatorContainer = document.getElementById('actuator-status-container');
const statusText = document.getElementById('status-text');
const dataCountSpan = document.getElementById('data-count');

class SensorChart {
    constructor(id, label, unit, canvas) {
        this.id = id; this.label = label; this.unit = unit; this.canvas = canvas;
        this.ctx = canvas.getContext('2d');
        this.ySteps = 5; this.xSteps = 5;
        this.syncResolution();
    }
    syncResolution() {
        const dpr = window.devicePixelRatio || 1;
        const rect = this.canvas.getBoundingClientRect();
        this.canvas.width = rect.width * dpr;
        this.canvas.height = rect.height * dpr;
        this.ctx.scale(dpr, dpr);
        this.displayW = rect.width; this.displayH = rect.height;
    }
    draw(dataList) {
        this.syncResolution();
        const ctx = this.ctx;
        const p = { t: 20, r: 30, b: 50, l: 65 };
        const w = this.displayW - p.l - p.r;
        const h = this.displayH - p.t - p.b;
        ctx.clearRect(0, 0, this.displayW, this.displayH);
        ctx.fillStyle = "#333"; ctx.font = "bold 12px sans-serif"; ctx.textAlign = "center";
        ctx.fillText("Time (seconds)", p.l + w/2, this.displayH - 10);
        ctx.save(); ctx.translate(15, p.t + h/2); ctx.rotate(-Math.PI / 2);
        ctx.fillText(`${this.label} (${this.unit})`, 0, 0); ctx.restore();
        if (dataList.length < 2) return;
        let vals = dataList.map(d => d.values[this.id]);
        let yMin = Math.min(...vals), yMax = Math.max(...vals);
        let yR = (yMax - yMin) || 1.0;
        yMin -= yR * 0.15; yMax += yR * 0.15;
        let xMax = dataList[dataList.length - 1].elapsed || 1;
        ctx.strokeStyle = "#e0e0e0"; ctx.fillStyle = "#666"; ctx.font = "10px sans-serif"; ctx.lineWidth = 1;
        for (let i = 0; i <= this.ySteps; i++) {
            let y = p.t + h - (i / this.ySteps) * h;
            ctx.beginPath(); ctx.moveTo(p.l, y); ctx.lineTo(p.l + w, y); ctx.stroke();
            ctx.textAlign = "right"; ctx.fillText((yMin + (yMax - yMin) * (i / this.ySteps)).toFixed(1), p.l - 8, y + 3);
        }
        for (let i = 0; i <= this.xSteps; i++) {
            let x = p.l + (i / this.xSteps) * w;
            ctx.beginPath(); ctx.moveTo(x, p.t); ctx.lineTo(x, p.t + h); ctx.stroke();
            ctx.textAlign = "center"; ctx.fillText(((i / this.xSteps) * xMax).toFixed(1) + "s", x, p.t + h + 15);
        }
        ctx.save(); ctx.beginPath(); ctx.rect(p.l, p.t, w, h); ctx.clip();
        ctx.beginPath(); ctx.strokeStyle = '#0078D7'; ctx.lineWidth = 2;
        dataList.forEach((d, i) => {
            let x = p.l + (d.elapsed / xMax) * w;
            let y = p.t + h - ((d.values[this.id] - yMin) / (yMax - yMin)) * h;
            if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
        });
        ctx.stroke(); ctx.restore();
    }
}

async function initDashboard() {
    try {
        const res = await fetch('/api/config');
        const config = await res.json();
        sensors = config.sensors;
        outputs = config.outputs;

        // Render Outputs
        actuatorContainer.innerHTML = "";
        outputs.forEach(o => {
            const badge = document.createElement('div');
            badge.id = `badge-${o.id}`;
            badge.className = "actuator-badge";
            badge.innerText = o.label;
            actuatorContainer.appendChild(badge);
        });

        // Render Charts
        dashboardContent.innerHTML = "";
        sensors.forEach(s => {
            const card = document.createElement('div');
            card.className = "sensor-card";
            card.innerHTML = `<div class="sensor-label">${s.label}</div><div class="data-display"><span id="val-${s.id}">--</span><span class="unit">${s.unit}</span></div><canvas id="chart-${s.id}"></canvas>`;
            dashboardContent.appendChild(card);
            charts[s.id] = new SensorChart(s.id, s.label, s.unit, document.getElementById(`chart-${s.id}`));
        });

        setInterval(fetchData, updateInterval);
        window.addEventListener('resize', updateUI);
    } catch (err) { statusText.innerText = "Connection Error"; }
}

async function fetchData() {
    try {
        const res = await fetch('/api/data');
        const data = await res.json();

        // Update Values
        sensors.forEach(s => {
            const el = document.getElementById(`val-${s.id}`);
            if (el) el.innerText = data.sensors[s.id].toFixed(2);
        });

        // Update Actuator Badges
        outputs.forEach(o => {
            const badge = document.getElementById(`badge-${o.id}`);
            if (badge) {
                if (data.isRecording) badge.classList.add('active');
                else badge.classList.remove('active');
            }
        });

        if (isRecording) {
            let elapsed = (Date.now() - recordingStartTime) / 1000.0;
            experimentData.push({ date: data.date, time: data.time, elapsed, values: data.sensors });
            updateUI();
        }
    } catch (err) {}
}

function updateUI() {
    dataCountSpan.innerText = experimentData.length;
    sensors.forEach(s => charts[s.id].draw(experimentData));
}

async function startRecording() {
    await fetch('/api/start'); // Trigger hardware ON
    if (experimentData.length === 0) recordingStartTime = Date.now();
    isRecording = true;
    document.getElementById('btn-start').disabled = true;
    document.getElementById('btn-stop').disabled = false;
    statusText.innerText = "Recording...";
}

async function stopRecording() {
    await fetch('/api/stop'); // Trigger hardware OFF
    isRecording = false;
    document.getElementById('btn-start').disabled = false;
    document.getElementById('btn-stop').disabled = true;
    statusText.innerText = "Stopped";
}

function clearData() {
    if (confirm("Clear data?")) {
        experimentData = []; updateUI();
        sensors.forEach(s => document.getElementById(`val-${s.id}`).innerText = "--");
    }
}

function downloadCSV() {
    if (experimentData.length === 0) return alert("No data");
    let csv = "Date,Time,Elapsed(s)," + sensors.map(s => `${s.label} (${s.unit})`).join(",") + "\n";
    experimentData.forEach(d => {
        csv += `${d.date},${d.time},${d.elapsed.toFixed(3)},` + sensors.map(s => d.values[s.id]).join(",") + "\n";
    });
    const blob = new Blob([csv], { type: 'text/csv' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a'); a.href = url; a.download = 'data.csv'; a.click();
}

initDashboard();
)rawliteral";
