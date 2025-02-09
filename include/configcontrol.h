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