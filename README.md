# ESP32 Firmware - Wi-Fi, HTTP Client, and SPIFFS Storage

This project is an ESP32 firmware built using **ESP-IDF**.  
It demonstrates how to connect the ESP32 to Wi-Fi, make an HTTP GET request to a server, and store the received data in the **SPIFFS filesystem**.  
Additionally, the firmware measures and logs the **write performance** (speed) of SPIFFS while saving the response.

---

## ✨ feature

- Connect ESP32 to Wi-Fi in **station mode**  
- Automatic reconnection if Wi-Fi disconnects  
- Perform **HTTP GET requests** to a given API/URL  
- Store response data in **SPIFFS filesystem**  
- Measure and log **write throughput** (bytes per second)  
