#include "nodemcu.h"
#include "config.h"
#include <vector>

/**
 * @brief Construct a new NodeMCU object.
 */
NodeMCU::NodeMCU( const uint &baudRate ){
    Serial.begin(baudRate);
    m_ConfigControl = new ConfigControl();
    m_IOControl = new IOControl( m_ConfigControl );
    m_Server = new WifiControl( this, m_ConfigControl );
}

/**
 * @brief Destroy the NodeMCU object
 */
NodeMCU::~NodeMCU(){}

/**
 * @brief The main execution called by main.cpp in the loop section.
 */
void NodeMCU::run(){
    // Load configuration en setup pins
    if( !m_ConfigControl->loaded ){
        // Print the files that are loaded into the flash memory to check if upload was succesfull. 
        Serial.println("\nNodeMCU::run: Listing files in flash memory:");
        Dir dir = LittleFS.openDir("/");
        while (dir.next()) {
            Serial.print("\tFile: ");
            Serial.println(dir.fileName());
        }

        // Load config and pins
        m_ConfigControl->loadConfig();
        m_IOControl->load();
    } 

    // Execute serial communication
    uint16 result = handle_serial();
    handle_error( result );    

    result = m_Server->connect();
    handle_error( result );

    // Only handle TCP and HTTP when connected to wifi
    if( result == SUCCESS ){
        // Execute TCP communication
        result = m_Server->updateTcpServer();
        handle_error( result );
        
        // Execute HTTP communication and control panel
        result = m_Server->updateHttpSerer();
        handle_error( result );
    }

    // Save configuration if an update has occured
    m_ConfigControl->saveConfig();
}

/**
 * @brief The main error handling method
 */
void NodeMCU::handle_error(uint16 errorCode){

}

/**
 * @brief Read serial data if available and save the command.
 * When this function is called it will only handle command line.
 * The NodeMCU supports multiple communication protocols but serial will always be available.
 * 
 * @return result code
 */
uint16 NodeMCU::handle_serial(){
    std::vector<String> command;

    if ( Serial.available() > 0 ){
        String input = Serial.readStringUntil('\n');

        while( input.length() > 0 ) {
            int spaceIndex = input.indexOf(' ');
            if( spaceIndex == -1 ) {
                input.trim();
                command.push_back( input );
                input = "";
            } else {
                command.push_back( input.substring( 0, spaceIndex ) );
                input = input.substring( spaceIndex + 1 );
            }
        }
    }
    return command.size() ? execute_command( command ) : FAILED;
}

/**
 * @brief Execute the received command from one of the communication protocols.
 * 
 * @return uint16 result code
 */
uint16 NodeMCU::execute_command( const std::vector<String> &command ){
    uint16 result = SUCCESS;
    int buffer;

    Serial.printf("NodeMCU::execute_command: ");
        for(String arg: command){
            Serial.printf(arg.c_str());
            Serial.printf(" ");
        }
        Serial.println();

    switch( parseCommand( command[0] ) ){
    case COMMAND_RESET: 
        m_IOControl->reset(); 
        break;
    case COMMAND_CONFIG: 
        if( command.size() < 2 ) return ERROR_CONFIG;
        result = configure( command ); 
        break;
    case COMMAND_READ: 
        if( command.size() < 2 ) return ERROR_READ;
        result = m_IOControl->read(  parsePinCommand( command[1] ), buffer ); 
        break;
    case COMMAND_WRITE: 
        if( command.size() < 3 ) return ERROR_WRITE;
        result = m_IOControl->write(  parsePinCommand( command[1] ), command[2].toInt() ); 
        break;
    default: 
        Serial.printf("NodeMCU::execute_command: error parsing command --> %s)\n",command[0].c_str()); 
        break;
    }

    return result;
}

uint16 NodeMCU::configure(const std::vector<String> &command){
    uint16 result = COMMAND_ERROR;
    
    ConfigCommand config = parseConfigCommand( command [1] );
    switch ( config ){
    case CONFIG_SHOW:
        Serial.println( "NodeMCU::configure: Showing current configuration on the flash memory:" );
        m_ConfigControl->printConfig();
        break;
    case CONFIG_PIN:
        if( command.size() < 4 ) return CONFIG_ERROR;
        result = m_IOControl->configurePin( parsePinCommand( command[2] ), parsePinConfigCommand( command[3] ) ); 
        break;
    case CONFIG_SSID:
    case CONFIG_PWD:
    case CONFIG_IP:
    case CONFIG_SUBNET:
    case CONFIG_GATEWAY:
    case CONFIG_PORT_TCP:
    case CONFIG_PORT_HTTP:
    case CONFIG_DNS1:
    case CONFIG_DNS2:
    case CONFIG_MAX_CLIENTS:
    case CONFIG_INACTIVE_TIMEOUT:
        if( command.size() < 3 ) return ERROR_CONFIG;
        result = m_Server->configure( config, command[2] );
        break;
    case CONFIG_ERROR: /* not possible */ break;
    }
    return result;
}