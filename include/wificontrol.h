#ifndef WIFICONTROL_H
#define WIFICONTROL_H

#include <vector>
#include "tcpclient.h"
#include <ESP8266WebServer.h>

#define MAX_RETRY 10

class NodeMCU;

/**
 * @brief The WifiControl class is used to control the ESP8266 chip.
 * It holds control for an TcpServer that is listning for client commands.
 * And it holds the HTTP server which shows users a small control panel.
 * The HTTP serer will handle client commands recieved through POST request.
 */
class WifiControl {
public:
    /**
     * @brief Construct a new Wifi Control:: Wifi Control object
     * @param nodeMCU instance to the NodeMCU singleton
     * @param configControl instance pointer to the cofiguration control of the flash memory
     */
    WifiControl( NodeMCU *nodeMCU, ConfigControl *configControl );

    /**
     * @brief Destroy the WifiServer object.
     */
    ~WifiControl();
    
    /**
     * @brief Connect to wifi.
     * will skip if already connected
     * 
     * @return uint16 result code
     */
    uint16 connect();

    /**
     * @brief Listen for incomming client connections and save the commands
     * and start the TCP server if it has not been started already.
     * 
     * @return uint16 result code
     */
    uint16 updateTcpServer();

    /**
     * @brief Handle incomming request of HTTP clients
     * and start the HTTP server if it has not been started already
     * 
     * @return uint16 result code
     */
    uint16 updateHttpSerer();

    /**
     * @brief Configure the server settings.
     * 
     * @param command indicates which setting will be configured
     * @param value the new value of the server setting
     * @return uint16 result code
     */
    uint16 configure( const ConfigCommand &command, const String &value );

private:
    /**
     * @brief Load index.html or the corresponding CSS, JS or Font files from the flash memory.
     * 
     * @param path file path to load
     */
    void handleFileRequest( String path );

    /**
     * @brief The singleton NodeMCU instance
     */
    NodeMCU *m_NodeMCU;

    /**
     * @brief Vector containing the current connected clients
     */
    std::vector<TcpClient> m_TcpClients;
    
    /**
     * @brief The ESP8266 wifi server instance
    */
    WiFiServer *m_TcpServer;

    /**
     * @brief The ESP8266 web server instance
     */
    ESP8266WebServer *m_HttpServer;

    /**
     * @brief Flag which is set to true when the TCP server has started
     */
    bool m_TcpServerStarted;

    /**
     * @brief Flag which is set to true when the HTTP server has started
     */
    bool m_HttpServerStarted;
    
    /**
     * @brief Instance poiner of the configuration data in the flash memory of the NodeMCU.
     */
    ConfigControl *m_ConfigControl;

    /**
     * @brief The amount of times the wifi connection has tried to be established but failed.
     */
    uint m_ConnectRetries;
};


#endif