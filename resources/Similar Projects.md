### **1. Satellite vs. UAV Thermal Imaging Comparison (Malang City Study)**

- **Focus:** Direct comparative analysis between macro-scale satellite data and micro-scale drone data.
    
- **Methodology:** Researchers analyzed the Urban Heat Island (UHI) phenomenon in the Pasar Besar Corridor. They utilized Landsat-8 satellite data to generate an LST map for a macro-scale view. Simultaneously, they flew a UAV equipped with a thermal infrared sensor over the same coordinates to capture micro-scale data.
    
- **Key Finding to Reference:** The study documented a significant difference between the highest temperatures recorded by the drone and the satellite. The satellite averaged out localized heat spikes, whereas the drone successfully identified specific "heat islands" (like asphalt roads reaching 48.62 °C) that the satellite missed due to its lower spatial resolution.
    

### **2. Combating UHI with Drone-Based Thermal Visualization (Ann Arbor Study)**

- **Focus:** Measuring the thermal behavior of common urban materials at solar noon to validate urban revitalization efforts.
    
- **Methodology:** A multi-year study in Michigan mapped a 12-acre urban streetscape using drones equipped with thermal sensors. The flights were conducted at an altitude of 200 feet with 90% image overlap, achieving a resolution of 1.5 centimeters per pixel. This was contrasted with standard satellite thermal sensors, which yield 30-meter pixels.
    
- **Key Finding to Reference:** The project established a methodology for capturing an "urban solar lifecycle" by executing flights at three distinct times of day to reveal diurnal UHI variations. Your team can replicate their 200-foot altitude and timed flight schedules for optimal data collection.
    

### **3. Drone-Based 3D Thermal Mapping for Climate-Responsive Planning**

- **Focus:** High-precision building and surface temperature extraction.
    
- **Methodology:** Researchers developed an approach combining a drone, a thermal infrared camera, and computational algorithms to measure vertical and horizontal surface temperatures with centimeter accuracy.
    
- **Key Finding to Reference:** Real-world validation in this study showed that low-altitude drone thermal mapping could measure surface temperatures within a 5 °C error margin. It also proved that drones are immune to the atmospheric interference that often skews satellite data, making drone telemetry a reliable baseline for calibrating satellite inaccuracies.
    

### **4. NASA ARSET: ECOSTRESS UHI Mapping**

- **Focus:** Utilizing NASA's ECOSTRESS platform specifically for urban heat applications.
    
- **Methodology:** NASA's Applied Remote Sensing Training (ARSET) program outlines the exact framework for using data from the Ecosystem Spaceborne Thermal Radiometer Experiment on Space Station (ECOSTRESS). It involves downscaling native 70-meter ECOSTRESS LST data using Google Earth Engine.
    
- **Key Finding to Reference:** While not a drone project itself, this is the official NASA operational standard for processing satellite thermal data. Your backend pipeline team should reference this training to ensure the satellite data you pull matches NASA's exact formatting before you compare it to your Arduino CSV files.
    
