# Absolute Beginner's Guide: Urban Heat Mapping

Welcome! If you have never worked with microcontrollers before, this guide will take you from opening your boxes of parts to extracting your first set of flight data.

---

## Step 1: Install the Software
Before you touch any wires, let's get your laptop ready.
1. Download and install the **[Arduino IDE](https://www.arduino.cc/en/software)** (Integrated Development Environment). This is the program you use to write and upload code to the Arduino.
2. Open the Arduino IDE.
3. On the left-hand sidebar, click the **Library Manager** icon (it looks like a stack of books).
4. Search for and install the following three libraries:
   *   Type `Adafruit AMG88xx` and click Install.
   *   Type `DHT sensor library` (look for the one by Adafruit) and click Install. *(If it asks to install dependencies like "Adafruit Unified Sensor", click "Install All".)*
   *   Type `TinyGPSPlus` (by Mikal Hart) and click Install.

---

## Step 2: Wire the Hardware
You will use "jumper wires" to connect the metal pins on your sensors to the black plastic holes on the Arduino Mega 2560. **Make sure the Arduino is unplugged from your computer and has no battery connected while you do this.**

### Powering the Sensors
All three sensors need power. Connect the `VCC` or `VIN` pin on each sensor to a `5V` pin on the Arduino. Connect the `GND` (Ground) pin on each sensor to a `GND` pin on the Arduino. *(You may need a small "breadboard" to share the power pins if you run out of 5V holes on the Arduino).*

### Connecting the Data Lines
*   **DHT11 (Temp/Humidity):** Connect its `DATA` or `OUT` pin to **Digital Pin 5** on the Arduino.
*   **NEO-6M (GPS):** 
    *   Connect the `TX` pin to **Pin 19 (RX1)** on the Arduino.
    *   Connect the `RX` pin to **Pin 18 (TX1)** on the Arduino.
*   **AMG8833 (Thermal Camera):**
    *   Connect the `SDA` pin to **Pin 20 (SDA)** on the Arduino.
    *   Connect the `SCL` pin to **Pin 21 (SCL)** on the Arduino.

---

## Step 3: Upload the "Brain"
Now we need to put our code into the Arduino.
1. Plug the Arduino into your laptop using the blue USB cable.
2. In the Arduino IDE, go to **Tools > Board** and select **Arduino Mega or Mega 2560**.
3. Go to **Tools > Port** and select the port that shows up (it might say "COM3" or something similar).
4. Open our `UrbanHeatMapping.ino` file in the Arduino IDE.
5. Near the very top of the code, look for this line: 
   `#define SIMULATION_MODE true`
   Change the word `true` to `false` so the Arduino knows you are using real hardware.
6. Click the **Upload** button (the right-pointing arrow in the top left corner). Wait for it to say "Done uploading" at the bottom of the screen.

---

## Step 4: Fly the Drone
1. Unplug the Arduino from your laptop. 
2. Plug a battery pack (like a 9V battery or a USB power bank) into the Arduino. The moment it gets power, it will start looking for a GPS signal.
3. Wait about 1-2 minutes for the GPS module to lock onto satellites (you will usually see a blinking light on the GPS module when it has a lock).
4. Take off! Remember, the Arduino will only record for about **14 minutes** before its memory is completely full, so don't fly for too long.
5. Land the drone and unplug the battery immediately to stop the program.

---

## Step 5: Extract the Data
The data is trapped inside the Arduino's memory. Let's get it out.
1. Take the Arduino back to your desk and plug it into your laptop with the USB cable (no external battery needed).
2. Open the Arduino IDE.
3. In the top right corner, click the **Serial Monitor** icon (it looks like a magnifying glass).
4. In the Serial Monitor window, look for a dropdown menu in the bottom right corner and ensure the baud rate is set to **115200 baud**.
5. You should see a message saying "Send 'E' to Export data via USB."
6. Type an uppercase `E` into the message bar at the top of the Serial Monitor and hit Enter.
7. Your flight data will instantly print out on the screen!
8. Highlight all the data with your mouse, copy it, and paste it into a blank Notepad or Excel file, then save it.
9. Type an uppercase `C` into the Serial Monitor and hit Enter to **Clear** the Arduino's memory, getting it ready for your next flight.
