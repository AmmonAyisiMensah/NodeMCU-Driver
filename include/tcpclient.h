#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "configcontrol.h"

/**
 * @brief timeout (in ms) for connected clients that dont do anything.
 */
#define INACTIVE_TIMEOUT 1000*60*2

class NodeMCU;

/**
 * @brief The TcpClient class is a wrapper around the ESP8266 WiFiClient class.
 * And offers the capability to read client data and safe it as a command, 
 * so it can be used to execute an known command.
 * When a TcpClient didnt receie any input for a while they will automatically be terminated and flagged for removal;
 */
class TcpClient {
public:
    /**
     * @brief Construct a new Tcp Client object
     * 
     * @param configControl instance pointer to the cofiguration control of the flash memory
     * @param m_WifiClient The ESP8266 WiFiClient retrieed from a WiFiSerer
     */
    TcpClient( ConfigControl *configControl, WiFiClient wificlient );

    /**
     * @brief Destroy the Tcp Client object
     */
    ~TcpClient();

    /**
     * @brief Read client data and execute the command
     * 
     * @return uint16 result code
     */
    uint16 handleCommand( NodeMCU *nodeMCU );

    /**
     * @brief Return the active state of the connection
     * 
     * @return true if last command was not longer then 5 minutes in the past
     * @return false if last command was longer then 5 minutes in the past
     */
    bool isActive();

private:
    
    /**
     * @brief Instance poiner of the configuration data in the flash memory of the NodeMCU.
     */
    ConfigControl *m_ConfigControl;
    
    /**
     * @brief The ESP8266 wifi client
     */
    WiFiClient m_WifiClient;

    /**
     * @brief The start time of last command
     */
    unsigned long m_ActiveTime;

    /**
     * @brief The milliseconds of no activity since last command
     */
    unsigned long m_InActiveTime;

    /**
     * @brief Flag that connection is still active, when set to false it can be cleaned up
     */
    bool m_Active;
};


#endif