#include "tcpclient.h"
#include "command.h"
#include "nodemcu.h"

/**
 * @brief Construct a new Tcp Client object
 * 
 * @param m_WifiClient The ESP8266 WiFiClient retrieed from a WiFiSerer
 */
TcpClient::TcpClient( ConfigControl *configControl, WiFiClient wificlient )
: m_ConfigControl( configControl )
, m_WifiClient( wificlient )
, m_InActiveTime( 0 )
{
    Serial.println( "TcpClient: A TCP connection has been esthablished" );
    m_ActiveTime = millis();
}

/**
 * @brief Destroy the Tcp Client object
 */
TcpClient::~TcpClient()
{}

/**
 * @brief Read client data and execute the command
 * 
 * @return uint16 result code
 */
uint16 TcpClient::handleCommand( NodeMCU *nodeMCU ){
    // Check if connection has data
    if( !m_WifiClient.available() ) {
        m_InActiveTime = millis() - m_ActiveTime;
        if( !isActive() ){
            m_WifiClient.stop();
            Serial.println( "TcpClient::handleCommand: Connection timed out and terminated." );
            return ERROR_CLIENT_DISCONNECTED;
        }
        return SUCCESS;
    }

    // Read the incoming command from the client
    std::vector<String> command;
    String receivedData = m_WifiClient.readStringUntil('\n');

    // Check if there is data else increase the inactive time or terminate the connection if timed out
    if( !receivedData.length() ){
        m_InActiveTime = millis() - m_ActiveTime;
        if( !isActive() ){
            m_WifiClient.stop();
            Serial.println( "TcpClient::handleCommand: Connection timed out and terminated." );
            return ERROR_CLIENT_DISCONNECTED;
        }
        return SUCCESS;
    }

    // Convert the receive data in to a vector of command and arguments 
    while( receivedData.length() > 0 ) {
        int spaceIndex = receivedData.indexOf(' ');
        if( spaceIndex == -1 ) {
            receivedData.trim();
            command.push_back( receivedData );
            receivedData = "";
        } else {
            command.push_back( receivedData.substring( 0, spaceIndex ) );
            receivedData = receivedData.substring( spaceIndex + 1 );
        }
    }
    // Execute the command
    m_ActiveTime = millis();
    return nodeMCU->execute_command( command );
    
}

/**
 * @brief Return the active state of the connection
 * 
 * @return true if last command was not longer then 5 minutes in the past
 * @return false if last command was longer then 5 minutes in the past
 */
bool TcpClient::isActive(){
    return m_InActiveTime < m_ConfigControl->InActiveTimeout;
}
