# ESP32 SPIFFS HTTP Client 

This project demonstrates how to use ESP32's SPIFFS filesystem to download and store HTTP response data while monitoring write performance in real-time.

## Overview

The application connects to a WiFi network, makes an HTTP GET request to `httpbin.org/get`, and saves the response data to a file in the SPIFFS filesystem. It also calculates and displays the write speed to SPIFFS during the download process.

## Features

- **WiFi Connection Management**: Automatic connection and reconnection handling
- **HTTP Client**: Downloads data from web APIs
- **SPIFFS Integration**: Stores downloaded data in the internal flash filesystem
- **Performance Monitoring**: Real-time calculation of SPIFFS write speed in kBps
- **Event-Driven Architecture**: Uses ESP-IDF event system for clean state management

## Hardware Requirements

- ESP32 development board
- WiFi network access

## Software Requirements

- ESP-IDF (Espressif IoT Development Framework)
- Compatible with ESP-IDF v4.0 and above

## Code Structure

### Main Components

1. **SPIFFS Initialization** (`spiffs_start()`)
   - Configures SPIFFS filesystem with base path `/spiffs`
   - Sets maximum of 5 files
   - Registers the virtual filesystem

2. **WiFi Management** (`wifi_start()`)
   - Initializes WiFi in Station (STA) mode
   - Handles connection events and automatic reconnection
   - Connects to predefined network credentials

3. **HTTP Client** (`api_hit()`)
   - Makes GET request to `http://httpbin.org/get`
   - Streams response data directly to SPIFFS file
   - Triggered automatically when IP address is obtained

4. **Event Handler** (`wifi_event_handler()`)
   - Manages WiFi connection states
   - Triggers HTTP request upon successful IP assignment
   - Handles disconnection and reconnection logic

5. **Performance Monitoring** (`http_event_handler()`)
   - Measures write speed to SPIFFS in real-time
   - Calculates throughput in kBps (kilobytes per second)
   - Logs performance metrics

## Configuration

Before using this code, update the WiFi credentials in the `wifi_start()` function:

```c
wifi_config_t wifi_station = {
    .sta = {
        .ssid = "Your_WiFi_SSID",           // Replace with your WiFi name
        .password = "Your_WiFi_Password",    // Replace with your WiFi password
        .sae_pwe_h2e = WPA3_SAE_PWE_BOTH
    }
};
```

## File Structure

```
project/
├── main/
│   └── main.c                 # Main application code
├── CMakeLists.txt            # Build configuration
└── partitions.csv            # Partition table (if custom SPIFFS size needed)
```

## Build and Flash Instructions

1. **Set up ESP-IDF environment**:
   ```bash
   . $IDF_PATH/export.sh
   ```

2. **Configure project**:
   ```bash
   idf.py menuconfig
   ```

3. **Build project**:
   ```bash
   idf.py build
   ```

4. **Flash to ESP32**:
   ```bash
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

## Expected Output

When running successfully, you should see logs similar to:

```
I (2340) WIFI_HTTP: TOTAL WRITE SPEED IN SPIFFS 45.67 kBps
I (2450) WIFI_HTTP: TOTAL WRITE SPEED IN SPIFFS 48.32 kBps
I (2560) WIFI_HTTP: TOTAL WRITE SPEED IN SPIFFS 46.89 kBps
```

The downloaded response will be saved as `/spiffs/response.txt` in the ESP32's flash memory.

## Key Features Explained

### Performance Monitoring
The code uses `esp_timer_get_time()` to measure the time taken for each write operation and calculates:
- **Write Speed**: Total bytes written ÷ elapsed time
- **Real-time Updates**: Performance metrics updated with each data chunk received

### Event-Driven Design
The application uses ESP32's event system to handle:
- WiFi connection establishment
- IP address assignment
- Automatic HTTP request triggering
- Connection loss recovery

### SPIFFS Benefits
- **Non-volatile Storage**: Data persists across power cycles
- **File System Interface**: Standard file operations (fopen, fwrite, fclose)
- **Wear Leveling**: Built-in flash memory management

## Potential Modifications

- Change the target URL to download different content
- Modify buffer size for different performance characteristics
- Add error handling for network failures
- Implement multiple file downloads
- Add file compression or encryption

## Troubleshooting

1. **WiFi Connection Issues**: Verify SSID and password are correct
2. **SPIFFS Mount Failures**: Check partition table and available flash space
3. **HTTP Errors**: Ensure internet connectivity and target URL accessibility
4. **Performance Issues**: Adjust buffer size or check flash memory health

## License

This example code is in the Public Domain (or CC0 licensed, at your option.)

## Contributing

Feel free to submit issues and enhancement requests!
