# ESP32_WiFi_RC_tank
Wi-Fi RC tank firmware that works on a ESP32-C3, controlling one drv8833, one continuous-rotation-mod servo for turret traverse, and one normal servo for gun elevation.

It's created for this 3D printed RC tank I made: https://www.printables.com/model/1488438-135-cm-21-apc-taiwanese-variant-of-m113-apc-an-rc

**Environment:** Arduino IDE  
**Board:** ESP32  
**Libraries to install:**  
- Async TCP by ESP32Async: https://github.com/ESP32Async/AsyncTCP  
- ESP Async WebServer by ESP32Async: https://github.com/ESP32Async/ESPAsyncWebServer  
- ESP32Servo by Kevin Harrington, John K. Bennett: https://madhephaestus.github.io/ESP32Servo/annotated.html  

---

## Schematic  
[![Schematic](https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/circuit.png)](https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/circuit.png)

---

Didn't implement a way to flip drive motor directions in configs, I just flip the connectors.  
Traverse servo direction can be flipped by changing the sign of the **Traverse limit** parameter.  
Elevation servo direction can be flipped by setting **Elevation min** and **Elevation max**.

---

## How to connect

1. Set up your Wi-Fi hotspot to SSID = **Arduino:D**, password = **99999999**  
   <img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094706.png" width="300"/>

2. If your device can figure out the IP of the connected device, enter it in the browser.

3. If your device doesn't show the IP, connect to the ESP32 Wi-Fi (**RC01** default).  
   <img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094518.png" width="300"/>

4. Your device may fight you and try to reconnect to a network with internet — fight back.  
   Then the captive portal will appear.  
   <img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094648.png" width="300"/>

5. Now you can open **192.168.1.1** in your browser.  
   <img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094651.png" width="300"/>

6. If it's connected to your device, it will show its IP. Refresh if needed.  
   <img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094715.png" width="300"/>

7. On Android, long-press the IP to open in Chrome without typing.  
   <img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094722.png" width="300"/>

---

## Web UI

### Control tab  
Round joystick for driving, rectangular joystick for turret.  
<img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094733.png" width="300"/>

### Settings tab  
Change AP (ESP32’s Wi-Fi) and STA (router/device) credentials.  
<img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094740.png" width="300"/>

### Parameters  
At the bottom of the settings tab. Drag left/right to change.  
It flashes cyan when the value is committed.

1. Increase drive deadzone if the drive joystick does nothing near center.  
2. Tune traverse centre if turret spins on its own.  
3. Traverse limit trims max speed for continuous rotation servo.  
4. Elevation min/max constrain the gun elevation motion.

<img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094743.png" width="300"/>

---

## Info tab  
Shows firmware build information.  
<img src="https://github.com/CCCanyon/ESP32_WiFi_RC_tank/blob/main/images/Screenshot_20251202-094747.png" width="300"/>

*Thank you chatGPT for formatting this!*
