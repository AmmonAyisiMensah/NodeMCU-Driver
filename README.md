# Overview
A comprehensive C++ library and Arduino project for controlling NodeMCU (V2 & V3) boards over various communication interfaces.

**NodeMCU-Driver** provides a unified API for interacting with NodeMCU boards through HTTP, TCP, Serial communication interfaces, and a user-friendly dashboard for seamless configuration and control.

# Features

* **HTTP**: Establish and manage HTTP connections for NodeMCU V2 & V3 boards.
* **TCP**: Configure and control TCP connections for NodeMCU boards.
* **Serial**: Communicate with NodeMCU boards over serial interfaces (always available).
* **Dashboard**: A built-in control panel for easy configuration and monitoring of the NodeMCU board.

# Installation Steps
The most simple way is to open the project in VSCode with the platformIO extension, then upload the filesystem and project. But you can also use platformIO from the terminal inside of the repository directory:

1. **Flash the Filesystem**
   * Use PlatformIO to flash the filesystem onto the NodeMCU board with the following command:
     ```sh
     pio run -t uploadfs
     ```
2. **Flash the Project**
   * Upload the main firmware to the NodeMCU by pressing up:
     ```sh
     pio run -t upload
     ```
3. **Configure WiFi Credentials**
   * Connect to the NodeMCU via Serial.
   * Set your WiFi SSID and password using the command:
     ```sh
     config ssid <your-SSID>
     config pwd <your-password>
     ```

	By default it will use IP address `192.168.0.222` with subnet mask `255.255.255.0`, if you need an other addresses then configure it, example:
	```sh
	config ip 192.168.1.222
	config subnet 255.255.0.0
	config gateway 192.168.1.1
	``` 

4. **Access the Configuration Dashboard**
   * Once connected to WiFi, access the dashboard via a web browser.
   * The dashboard allows real-time configuration and control.

# Configuration Options

The following settings can be configured using the `config` command:

* **Network Settings**:
  ```sh
  config ip <IP_ADDRESS>
  config subnet <SUBNET_MASK>
  config gateway <GATEWAY>
  config dns1 <DNS1>
  config dns2 <DNS2>
  ```
* **TCP Settings**:
  ```sh
  config tcp-port <PORT>
  config max-clients <NUMBER>
  config timeout <TIMEOUT_MILISECONDS>
  ```
* **HTTP Settings**:
  ```sh
  config http-port <PORT>
  ```
* **Pin Configuration**:
  ```sh
  config pin <PIN_NAME> <MODE>
  ```
  * `PIN_NAME`: D0 to D8
  * `MODE`: `input` or `output`

# Contribution
Contributions to **NodeMCU-Driver** are welcome! If you'd like to contribute to the project, please fork the repository and submit a pull request with your changes.

# Changelog
**Version 1.0.0**
---
- Initial version.

**Version 1.1.0**
---
- Added dashboard for easier configuration.
- Updated installation process for PlatformIO.
- Expanded configuration options.

# Contact
For questions, feedback, or to report issues, please contact the maintainer at ammon.mensah@gmail.com.
