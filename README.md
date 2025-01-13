## Overview

The **ESP32 Smart Switch** system not only lets you control home appliances remotely but also provides important electrical data such as voltage and current readings from an **AC-powered device**. The device includes a **web interface** for remote control and real-time monitoring of power consumption.

**Key Features:**

- **Remote Control**: Control home appliances through a web interface.
- **Voltage and Current Measurement**: Use sensors to monitor AC voltage and current.
- **Real-time Monitoring**: View live data of voltage and current from connected appliances.
- **ESP32 Microcontroller**: Handles Wi-Fi communication and manages relay and sensor data.
- **Expandable**: Can be extended to monitor multiple devices or integrate with voice assistants.

## Components Used

- **ESP32** Microcontroller
- **Relay Module** to control the appliance
- **ZMCT103C** AC current sensor
- **ZMPT101B**
- **Jumper Wires** for connections
- **Power Supply** for ESP32 and the appliance

## Project Setup

### Hardware Setup

1. **Relay Connections**:
    
    - Connect the **Relay Module** to one of the GPIO pins (e.g., GPIO23) of the ESP32 to control the appliance’s power.
2. **Voltage and Current Sensor**:
    
    - Use the **ZMCT103C AC current sensor** to measure the current being drawn by the appliance.
    - For voltage measurement, use the **ZMPT101B AC voltage sensor** that can scale down the voltage to a safe range for the ESP32’s ADC.
3. **Power Supply**:
    
    - Ensure the **ESP32** is powered via a USB cable.
    - The appliance is controlled through the relay, with the sensor measuring voltage and current for monitoring.

### Software Setup

1. **Arduino IDE Setup**:
    
    - Install the **ESP32 board** in Arduino IDE.
    - Install the necessary libraries to interface with the **voltage and current sensors** (e.g., `AnalogRead` or specific libraries for your sensors).
2. **Code**:
    
    - Clone or download the repository.
    - Open the `esp32_smart_switch_with_sensors.ino` file in Arduino IDE.
    - Update the Wi-Fi credentials in the code (SSID and password).
    - Upload the code to your ESP32 board.
3. **Access Web Interface**:
    
    - After uploading, check the **Serial Monitor** for the IP address assigned to the ESP32.
    - Enter the IP address into your browser to access the web interface.
    - Use the interface to control the appliance and view real-time voltage and current data.

## Results

- **Voltage and Current Data**: Real-time voltage and current readings are displayed on the web interface.
- **Remote Control**: The relay allows users to turn the appliance on and off through the web interface.

## Future Improvements

- **Real-Time Power Consumption**: Calculate and display real-time power consumption (Watts).
- **Data Logging**: Add the ability to log voltage, current, and power data over time for further analysis.
- **Voice Control**: Integrate with Amazon Alexa or Google Assistant for voice-based control.
- **Multiple Devices**: Extend the system to monitor and control multiple appliances.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
