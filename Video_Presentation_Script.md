# NASA Urban Heat Mapping System - Video Presentation Script

**Target Duration:** ~5 Minutes (approx. 700-750 words)
**Pacing:** Conversational, clear, and engaging.

---

### [0:00 - 0:45] Introduction: The Heat Island Problem
**Visual:** Title card with the project name "NASA Urban Heat Mapping System" and your name/team.
**Audio:**
"Hello everyone, and welcome to our project presentation. Today we're tackling a growing environmental issue: the Urban Heat Island effect. As our cities grow, concrete and asphalt absorb and retain heat, creating localized 'heat islands' that can be significantly warmer than surrounding areas. But how do we accurately measure this at a micro-level? 

To answer that question, we built the NASA Urban Heat Mapping System. Our goal was to investigate the urban heat island effect right here at San Bernardino Valley College. We set out to create a system that could collect physical micro-climate surface data across our campus and directly compare it against historical satellite baselines provided by NASA."

### [0:45 - 1:30] The Pivot: From Hardware to Software Engineering
**Visual:** Show the Arduino setup, maybe some early prototype pictures, then transition to screen recordings of code or the architecture diagram.
**Audio:**
"Our original vision involved mounting sensors onto a UAV payload. However, due to hardware constraints, we had to make a crucial pivot. We transitioned from an aerial drone to a handheld scanning system. 

While this seemed like a setback at first, it actually allowed us to focus heavily on what became the core strength of our project: Data Architecture and Software Engineering. We realized that collecting data was only half the battle; the real challenge was fusing physical sensor readings with geospatial data in real-time, processing it, and visualizing it alongside massive NASA datasets. So, we designed a four-part interconnected software architecture to make this happen."

### [1:30 - 2:30] Phase 1 & 2: Data Acquisition & Sensor Fusion
**Visual:** Show the Arduino Mega connected to the laptop. Then cut to a screen recording of the Web Data Acquisition System (the Logger UI) in action while walking.
**Audio:**
"Let’s walk through how the system works, starting with Data Acquisition. At the hardware level, we kept things lightweight. We’re using an Arduino Mega paired with a DHT11 sensor. This acts as a pure sensor node, reading temperature and humidity every two seconds and streaming that telemetry over a USB Serial connection.

But the real magic happens on the laptop. We built a custom browser-based Data Acquisition System using HTML and JavaScript. Using the modern Web Serial API, our web app reads the live stream directly from the Arduino. Simultaneously, it taps into the laptop's HTML5 Geolocation API to track the physical path of the user as they walk across campus. Our software merges these two separate data streams in real-time—pairing a precise temperature reading with an exact GPS coordinate—and compiles it into a downloadable CSV file."

### [2:30 - 3:30] Phase 3: The Data Pipeline & NASA Integration
**Visual:** Show snippets of the Python pipeline (`fetch_nasa_data.py` and `process_data.py`). Show a graphic representing the NASA POWER REST API.
**Audio:**
"Once we have our physical walk data, we move to the processing phase. We wrote a custom Python pipeline to handle the heavy lifting. 

The first step of the pipeline queries the open NASA POWER REST API. We feed it our campus coordinates, and it returns the true regional baseline temperatures based on NASA's historical satellite observations. 

Next, our processing script ingests the CSV data we collected on our walk, formats it into a standardized, web-ready spatial data format called GeoJSON, and fuses our physical on-the-ground readings with the NASA satellite baselines. We now have a clean, combined dataset ready for analysis."

### [3:30 - 4:30] Phase 4: The Interactive Web Dashboard
**Visual:** Screen recording of the interactive dashboard (`dashboard/index.html`). Pan around the map, show the thermal gradient, click on data points, and highlight the comparison with NASA baselines.
**Audio:**
"All of this data architecture culminates in our final piece of software: an Interactive Web Dashboard built with Leaflet.js. 

This modern, fluid interface visualizes the physical data walk as a continuous thermal gradient overlaid on a map of the campus. As you look at the dashboard, you can visually identify 'hotspots' on campus where the micro-climate deviates significantly. More importantly, the dashboard allows for a side-by-side comparison with the NASA dataset. 

By comparing our ground-truth data with the satellite baseline, we can clearly see the localized impact of the urban heat island effect—identifying exactly which concrete structures or asphalt lots are trapping the most heat compared to the regional average."

### [4:30 - 5:00] Conclusion & Future Steps
**Visual:** Final wide shot of the dashboard, fading to a 'Thank You' slide with contact info or project links.
**Audio:**
"In the end, what started as a hardware challenge evolved into a robust software engineering achievement. We built a complete, end-to-end data pipeline: from a physical sensor, to a web-based serial logger, through a Python fusion pipeline querying NASA APIs, and finally into an interactive geospatial dashboard. 

Looking forward, this modular architecture means we can easily scale the system to other campuses, swap in higher-precision sensors, or integrate additional environmental data. 

Thank you for watching, and we'd be happy to answer any questions."
