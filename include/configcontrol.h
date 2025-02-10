/**
 * @file configcontrol.h
 * @author Ammon Ayisi-Mensah (ammon.mensah@gmail.com)
 * @version 1.0.0
 * @date 2025-02-10
 * 
 * @copyright
 * MIT License
 * Copyright (c) 2025 Ammon Ayisi-Mensah
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CONFIGCONTROL_H
#define CONFIGCONTROL_H

#include <map>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include "command.h"

#define CONFIG_FILE "/config.txt"


/**
 * @brief GPIO and status data of a pin.
 */
struct IO_PIN{
    String name;
    uint8_t gpio;
    PinConfig mode;
    int value;
};

/**
 * @brief 
 * 
 */
class ConfigControl {
public:
    /**
     * @brief Construct a new Config Control object
     */
    ConfigControl();
    
    /**
     * @brief Destroy the Config Control object
     */
    ~ConfigControl();

    /**
     * @brief Load the configuration values from the NodeMCU flash memory
     */
    void loadConfig();

    /**
     * @brief Save the configuration values into the NodeMCU flash memory
     */
    void saveConfig();

    /**
     * @brief Show the contents of the config file in the serial monitor
     */
    void printConfig();

    /**
     * @brief Read the contents of the config file as a string
     */
    String readConfig();

    /**
     * @brief Flag that indicates if config values have been loaded
     */
    bool loaded = false;

    /**
     * @brief Flag that indicates if config values have been changed
     */
    bool updated = false;

    /**
     * @brief Contains the GPIO and status data.
     */
    std::map<const PinId, IO_PIN> pinData;
    
    /**
     * @brief The SSID of the access point to connect to
    */
    String SSID;
    
    /**
     * @brief The password of the access point
    */
    String PWD;
    
    /**
     * @brief The static IPv4 address of the NodeMCU
    */
    IPAddress StaticIP;
    
    /**
     * @brief The subnet address
    */
    IPAddress Subnet;
    
    /**
     * @brief The gateway address
    */
    IPAddress Gateway;
    
    /**
     * @brief The port the server will listen on for TCP conenction
 * s
    */
    uint16 PortTCP;
    
    /**
     * @brief The port the server will use for the HTTP web server
    */
    uint16 PortHTTP;
    
    /**
     * @brief The primary domain name service address
    */
    IPAddress DnsPrimary;
    
    /**
     * @brief The secundary domain name service address
    */
    IPAddress DnsSecundary;

    /**
     * @brief Maximum amount of client connections that can be handled at the same time.
     */
    uint MaxClients;

    /**
     * @brief timeout (in ms) for connected clients that dont do anything.
     */
    uint32 InActiveTimeout;
};

#endif