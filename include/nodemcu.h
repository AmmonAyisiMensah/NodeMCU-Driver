#ifndef NODEMCU_H
#define NODEMCU_H

#include "iocontrol.h"
#include "wificontrol.h"


/**
 * @brief This class encapsulates the main goal of not coding the NodeMCU.
 * It loads default configuration values from  @file config.h at startup.
 * And can be configured and operated by communication protocols. (HTTP, TCP, Serial)
 */
class NodeMCU{
public:
    /**
     * @brief Construct a new NodeMCU object.
     * 
     * @param baudRate the communication speed of the serial interface
     */
    NodeMCU( const uint &baudRate = 115200 );

    /**
     * @brief Destroy the Node M C U object.
     */
    ~NodeMCU();
    
    /**
     * @brief The main execution called by main.cpp in the loop section.
     */
    void run();

    /**
     * @brief Execute the received command from one of the communication protocols.
     * 
     * @param command commands and arguments
     * @return uint16 result code
     */
    uint16 execute_command( const std::vector<String> &command );

private:
    /**
     * @brief This will control the configuration data in the flash memory of the NodeMCU.
     */
    ConfigControl *m_ConfigControl;

    /**
     * @brief This will control the IO board.
     */
    IOControl *m_IOControl;

    /**
     * @brief This will control the ESP8266 wifi connectifity.
     */
    WifiControl *m_Server;

    /**
     * @brief The main error handling method
     */
    void handle_error(uint16 errorCode);

    /**
     * @brief Read serial data if available and save the command.
     * When this function is called it will only handle command line.
     * The NodeMCU supports multiple communication protocols but serial will always be available.
     * 
     * @return result code
     */
    uint16 handle_serial();

    /**
     * @brief Execute configuration command
     * 
     * @param command commands and arguments
     * @return uint16 result code
     */
    uint16 configure( const std::vector<String> &command );
};

#endif