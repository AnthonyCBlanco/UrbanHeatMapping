// Initialize Map
const map = L.map('map', {
    zoomControl: false // Hide default zoom, we can add it elsewhere if needed
}).setView([37.7749, -122.4194], 14); // Default center, will bounds to data

// Add Light Matter CartoDB Basemap
L.tileLayer('https://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}{r}.png', {
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors &copy; <a href="https://carto.com/attributions">CARTO</a>',
    subdomains: 'abcd',
    maxZoom: 20
}).addTo(map);

// We will define getColor inside loadData so it has access to min/max

// Global variables to hold our layers
let geoJsonLayer;
let heatMapLayer;

// Function to load and render data
async function loadData() {
    try {
        // Use the global variable defined in data.js to bypass local CORS restrictions
        const data = geojsonData;
        
        let maxTemp = -999;
        let minTemp = 999;
        let totalHum = 0;
        let pointCount = data.features.length;

        // Check for NASA Baseline
        const nasaBaseline = data.nasa_baseline_c;
        if (nasaBaseline !== undefined && nasaBaseline !== null) {
            document.getElementById('nasa-baseline').innerText = nasaBaseline.toFixed(1) + " °C";
        } else {
            document.getElementById('nasa-baseline').innerText = "Not Available";
        }

        let heatPoints = [];

        // First pass: Find min and max temperatures and collect heat points
        data.features.forEach(feature => {
            const irTemp = feature.properties.CenterIRTemp_C;
            const hum = feature.properties.Humidity_Pct;
            const lat = feature.geometry.coordinates[1];
            const lon = feature.geometry.coordinates[0];
            
            if (irTemp > maxTemp) maxTemp = irTemp;
            if (irTemp < minTemp) minTemp = irTemp;
            totalHum += hum;
            
            // We will normalize the intensity between 0 and 1 later
            heatPoints.push([lat, lon, irTemp]);
        });
        
        // Normalize heat points intensity (0 to 1) based on our dynamic range
        const tempRange = maxTemp - minTemp;
        heatPoints = heatPoints.map(p => {
            let intensity = tempRange === 0 ? 0.5 : (p[2] - minTemp) / tempRange;
            return [p[0], p[1], intensity];
        });

        // Dynamic color scale function for the UI popups (matches the heatmap gradient)
        function getDynamicColor(temp) {
            const range = maxTemp - minTemp;
            if (temp > minTemp + (range * 0.8)) return '#ef4444'; // Red
            if (temp > minTemp + (range * 0.6)) return '#f97316'; // Orange
            if (temp > minTemp + (range * 0.4)) return '#f59e0b'; // Yellow
            if (temp > minTemp + (range * 0.2)) return '#10b981'; // Green
            return '#3b82f6'; // Blue
        }

        // Clear existing layers if syncing
        if (geoJsonLayer) map.removeLayer(geoJsonLayer);
        if (heatMapLayer) map.removeLayer(heatMapLayer);

        // Add the fluid Heatmap Layer
        heatMapLayer = L.heatLayer(heatPoints, {
            radius: 35,
            blur: 25,
            maxZoom: 17,
            gradient: {
                0.2: '#3b82f6', // Blue
                0.4: '#10b981', // Green
                0.6: '#f59e0b', // Yellow
                0.8: '#f97316', // Orange
                1.0: '#ef4444'  // Red
            }
        }).addTo(map);

        // Add Data points as invisible hover hitboxes for popups
        geoJsonLayer = L.geoJSON(data, {
            pointToLayer: function (feature, latlng) {
                // Create invisible circle marker
                return L.circleMarker(latlng, {
                    radius: 10, // Generous hitbox for hovering
                    fillOpacity: 0,
                    opacity: 0,
                    weight: 2,
                    color: '#ffffff'
                });
            },
            onEachFeature: function (feature, layer) {
                // Build Premium Popup
                const p = feature.properties;
                // Format Delta if available
                let deltaHtml = "";
                if (p.Delta_C !== undefined) {
                    const sign = p.Delta_C > 0 ? "+" : "";
                    const color = p.Delta_C > 0 ? "var(--hot-color)" : "var(--cold-color)";
                    deltaHtml = `<div class="popup-row"><span>NASA Delta:</span> <span class="popup-val" style="color: ${color}">${sign}${p.Delta_C.toFixed(1)}°C</span></div>`;
                }

                const popupContent = `
                    <div class="popup-title">Scan Record</div>
                    <div class="popup-row"><span>Time:</span> <span class="popup-val">${p.Timestamp}</span></div>
                    <div class="popup-row"><span>Surface IR:</span> <span class="popup-val" style="color: ${getDynamicColor(p.CenterIRTemp_C)}">${p.CenterIRTemp_C.toFixed(1)}°C</span></div>
                    ${deltaHtml}
                    <div class="popup-row"><span>Ambient:</span> <span class="popup-val">${p.AmbientTemp_C.toFixed(1)}°C</span></div>
                    <div class="popup-row"><span>Humidity:</span> <span class="popup-val">${p.Humidity_Pct.toFixed(1)}%</span></div>
                `;
                layer.bindPopup(popupContent);
                
                // Add hover effect to reveal the exact point
                layer.on('mouseover', function() {
                    this.setStyle({ opacity: 1, fillOpacity: 0.2 });
                    this.bringToFront();
                });
                layer.on('mouseout', function() {
                    this.setStyle({ opacity: 0, fillOpacity: 0 });
                });
            }
        }).addTo(map);

        // Fit map bounds to our data
        if (pointCount > 0) {
            map.fitBounds(geoJsonLayer.getBounds(), { padding: [50, 50] });
            
            // Update UI Sidebar Stats
            document.getElementById('max-temp').innerText = maxTemp.toFixed(1) + '°C';
            document.getElementById('min-temp').innerText = minTemp.toFixed(1) + '°C';
            document.getElementById('point-count').innerText = pointCount;
            document.getElementById('avg-hum').innerText = (totalHum / pointCount).toFixed(1) + '%';
        }

    } catch (error) {
        console.error("Error loading GeoJSON data:", error);
        alert("Failed to load map data. Ensure process_data.py has been run.");
    }
}

// Initial Load
loadData();

// Sync Button Event Listener
document.getElementById('refresh-btn').addEventListener('click', () => {
    // Add a quick visual flash to button
    const btn = document.getElementById('refresh-btn');
    const originalText = btn.innerText;
    btn.innerText = "Syncing...";
    
    setTimeout(() => {
        loadData();
        btn.innerText = originalText;
    }, 500);
});
