let currentLat = null;
let currentLon = null;
let loggedData = [];
let isLogging = false;
let port = null;

// HTML Elements
const btnConnect = document.getElementById('connectBtn');
const btnDownload = document.getElementById('downloadBtn');
const elLat = document.getElementById('val-lat');
const elLon = document.getElementById('val-lon');
const elTemp = document.getElementById('val-temp');
const elHum = document.getElementById('val-hum');
const elCount = document.getElementById('val-count');
const elStatus = document.getElementById('statusText');

// 1. Start GPS Tracking immediately
if ("geolocation" in navigator) {
    elStatus.innerText = "Requesting GPS permissions...";
    navigator.geolocation.watchPosition(
        (position) => {
            currentLat = position.coords.latitude;
            currentLon = position.coords.longitude;
            elLat.innerText = currentLat.toFixed(6);
            elLon.innerText = currentLon.toFixed(6);
            if (!isLogging) elStatus.innerText = "GPS locked. Ready for Arduino.";
        },
        (error) => {
            elStatus.innerText = "GPS Error: " + error.message;
        },
        { enableHighAccuracy: true, maximumAge: 0 }
    );
} else {
    elStatus.innerText = "Geolocation not supported by this browser.";
}

// 2. Web Serial API for Arduino
btnConnect.addEventListener('click', async () => {
    try {
        // Request a port and open a connection.
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 115200 });
        
        elStatus.innerText = "Connected to Arduino. Listening for data...";
        btnConnect.style.display = 'none';
        btnDownload.style.display = 'block';
        isLogging = true;
        
        // Setup the stream reader
        const textDecoder = new TextDecoderStream();
        const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
        const reader = textDecoder.readable.getReader();
        
        let buffer = "";
        
        while (true) {
            const { value, done } = await reader.read();
            if (done) break;
            
            buffer += value;
            let lines = buffer.split('\n');
            
            // Keep the last partial line in the buffer
            buffer = lines.pop();
            
            for (let line of lines) {
                line = line.trim();
                if (line === "SYSTEM_READY" || line.startsWith("ERROR")) {
                    console.log(line);
                    continue;
                }
                
                // Parse "TEMP,HUM"
                let parts = line.split(',');
                if (parts.length === 2 && currentLat !== null) {
                    let temp = parseFloat(parts[0]);
                    let hum = parseFloat(parts[1]);
                    
                    if (!isNaN(temp) && !isNaN(hum)) {
                        elTemp.innerText = temp.toFixed(2) + " °C";
                        elHum.innerText = hum.toFixed(2) + " %";
                        
                        // Get current time HH:MM:SS
                        let now = new Date();
                        let timestamp = now.toTimeString().split(' ')[0];
                        
                        // Because we don't have the IR camera, we use Ambient Temp for the Heat Map
                        let irTemp = temp; 
                        
                        // Log the data point
                        loggedData.push([
                            timestamp,
                            currentLat.toFixed(6),
                            currentLon.toFixed(6),
                            temp.toFixed(2),
                            hum.toFixed(2),
                            irTemp.toFixed(2)
                        ]);
                        
                        elCount.innerText = loggedData.length;
                    }
                }
            }
        }
    } catch (err) {
        elStatus.innerText = "Serial Error: " + err;
    }
});

// 3. Download CSV
btnDownload.addEventListener('click', () => {
    if (loggedData.length === 0) {
        alert("No data logged yet!");
        return;
    }
    
    let csvContent = "Timestamp,Latitude,Longitude,AmbientTemp_C,Humidity_Pct,CenterIRTemp_C\n";
    loggedData.forEach(row => {
        csvContent += row.join(",") + "\n";
    });
    
    // Create a blob and trigger download
    const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
    const url = URL.createObjectURL(blob);
    const link = document.createElement("a");
    link.setAttribute("href", url);
    link.setAttribute("download", "datalog.csv");
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
});
