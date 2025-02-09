#include "wificontrol.h"
#include "nodemcu.h"

/**
 * @brief Construct a new Wifi Control:: Wifi Control object
 * @param nodeMCU instance to the NodeMCU singleton
 * @param maxClients maximum allowed client connections
 */
WifiControl::WifiControl( NodeMCU *nodeMCU, ConfigControl *configControl )
: m_NodeMCU( nodeMCU ) 
, m_ConfigControl( configControl )
, m_ConnectRetries( 0 )
{}

/**
 * @brief Destroy the Wifi Control:: Wifi Control object
 */
WifiControl::~WifiControl(){
}

/**
 * @brief Connect to wifi.
 * will skip if already connected
 * 
 * @return uint16 result code
 */
uint16 WifiControl::connect(){
    // Check if MAX retries has been reached to skipp connecting to wifi (so that erial communication wont be delayed with connecting to wifi)
    if( m_ConnectRetries >= MAX_RETRY ) return ERROR_WIFI_CONNECTION;

    if( WiFi.status() != WL_CONNECTED ){
        if( !WiFi.config( m_ConfigControl->StaticIP, m_ConfigControl->Gateway, m_ConfigControl->Subnet, m_ConfigControl->DnsPrimary, m_ConfigControl->DnsSecundary ) ){
            Serial.println( "WifiControl::connect: STA configuration failed" );
            Serial.printf( "\tStatic IP: %s\n\tSubnet: %s\n\tGateway: %s\n\tPrimary DNS: %s\n\tSecundary DNS: %s\n"
                , m_ConfigControl->StaticIP.toString().c_str()
                , m_ConfigControl->Gateway.toString().c_str()
                , m_ConfigControl->Subnet.toString().c_str()
                , m_ConfigControl->DnsPrimary.toString().c_str()
                , m_ConfigControl->DnsSecundary.toString().c_str() );
            m_ConnectRetries = MAX_RETRY;
            return ERROR_WIFI_CONFIG;
        }

        Serial.print( "WifiControl::connect Connecting to " );
        Serial.print( m_ConfigControl->SSID );
        WiFi.begin( m_ConfigControl->SSID, m_ConfigControl->PWD );
        int attempt = 0;
        while( attempt < 10 ){
            if( WiFi.status() == WL_CONNECTED ) break;
            delay(500);
            Serial.print(".");
            attempt++;
        }
        Serial.println("");
        if( attempt == 10 ) {
            Serial.printf( "WifiControl::connect: Connecting to %s failed!\n", m_ConfigControl->SSID.c_str() );
            m_ConnectRetries++;
            return ERROR_WIFI_CONNECTION;
        }
        Serial.println( "WifiControl::connect: Connected to wifi successfully!" );
    }
    m_ConnectRetries = 0;
    return SUCCESS;
}

/**
 * @brief Listen for incomming client connections and save the commands
 * and start the TCP server if it has not been started already 
 * 
 * @return uint16 result code
 */
uint16 WifiControl::updateTcpServer(){
    // Create and start TCP server
    if( !m_TcpServerStarted ){
        if( !m_TcpServer ) m_TcpServer = new WiFiServer( m_ConfigControl->PortTCP );
        m_TcpServer->begin();
        m_TcpServerStarted = true;
        Serial.println( "WifiControl::updateTcpServer: TCP server started." );
    }

    // Listen for incomming clients
    if( ( m_TcpClients.size() < m_ConfigControl->MaxClients ) && m_TcpServer->hasClient() ) {
        m_TcpClients.emplace_back( TcpClient( m_ConfigControl, m_TcpServer->accept() ) );
    }
    for( uint i = 0; i < m_TcpClients.size(); i++ ) {
        if( m_TcpClients[i].handleCommand( m_NodeMCU ) == ERROR_CLIENT_DISCONNECTED ) {
            m_TcpClients.erase( m_TcpClients.begin() + i );
        }

    }
    return SUCCESS;
}

/**
 * @brief Handle incomming request of HTTP clients
 * and start the HTTP server if it has not been started already
 * 
 * @return uint16 result code
 */
uint16 WifiControl::updateHttpSerer(){
    if( !m_HttpServerStarted ){
        if( !m_HttpServer ) m_HttpServer = new ESP8266WebServer( m_ConfigControl->PortHTTP );

        m_HttpServer->on( "/", HTTP_GET, [ this ](){ handleFileRequest( "/index.html" ); });

        m_HttpServer->onNotFound( [ this ]() { handleFileRequest (m_HttpServer->uri() ); });

        m_HttpServer->on( "/configure/show", HTTP_GET, [ this ](){
            m_HttpServer->send( 200, "text/plain", m_ConfigControl->readConfig() );
        });

        m_HttpServer->on( "/configure", HTTP_POST, [ this ](){
            std::vector<String> command = { "config" };
            if( m_HttpServer->hasArg( "arg1" ) ) command.push_back( m_HttpServer->arg( "arg1" ) );
            if( m_HttpServer->hasArg( "arg2" ) ) command.push_back( m_HttpServer->arg( "arg2" ) );
            if( m_HttpServer->hasArg( "arg3" ) ) command.push_back( m_HttpServer->arg( "arg3" ) );
            
            m_HttpServer->send( 200, "text/plain", String( m_NodeMCU->execute_command( command ) ) );
        });

        m_HttpServer->on( "/read", HTTP_GET, [ this ](){
            if( m_HttpServer->hasArg( "pin" ) ) {
                m_NodeMCU->execute_command( { "read", m_HttpServer->arg( "pin" ) } );
                m_HttpServer->send( 200, "text/plain", String( m_ConfigControl->pinData[ parsePinCommand( m_HttpServer->arg( "pin" ) ) ].value ) );
            }
        });

        m_HttpServer->on( "/read_all", HTTP_GET, [ this ](){
            m_NodeMCU->execute_command( { "read",  "A0" } );
            m_NodeMCU->execute_command( { "read",  "D0" } );
            m_NodeMCU->execute_command( { "read",  "D1" } );
            m_NodeMCU->execute_command( { "read",  "D3" } );
            m_NodeMCU->execute_command( { "read",  "D4" } );
            m_NodeMCU->execute_command( { "read",  "D5" } );
            m_NodeMCU->execute_command( { "read",  "D6" } );
            m_NodeMCU->execute_command( { "read",  "D7" } );
            m_NodeMCU->execute_command( { "read",  "D8" } );
            m_NodeMCU->execute_command( { "read",  "D9" } );
            m_HttpServer->send( 200, "text/plain", String( m_ConfigControl->pinData[ PIN_ANA0 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG0 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG1 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG2 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG3 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG4 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG5 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG6 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG7 ].value )
                    + "," + String( m_ConfigControl->pinData[ PIN_DIG8 ].value )
                );
        });
        
        m_HttpServer->on( "/write", HTTP_POST, [ this ](){
            if( m_HttpServer->hasArg( "pin" ) && m_HttpServer->hasArg( "value" ) ){
                m_NodeMCU->execute_command( { "write", m_HttpServer->arg( "pin" ), m_HttpServer->arg( "value" ) } );
                m_HttpServer->send( 200, "text/plain", "OK" );
            }
        });

        // Start the HTTP server
        m_HttpServer->begin();
        m_HttpServerStarted = true;
        Serial.println( "WifiControl::updateHttpSerer: HTTP server started." );
    }

    // Handle a client request
    m_HttpServer->handleClient();
    return SUCCESS;
}

/**
 * @brief Configure the server settings.
 * 
 * @param command indicates which setting will be configured
 * @param value the new value of the server setting
 * @return uint16 result code
 */
uint16 WifiControl::configure( const ConfigCommand &command, const String &value ){
    switch ( command ){
    case CONFIG_SSID:
        m_ConfigControl->SSID = value;
        Serial.printf( "WifiControl::configure: Changed SSID to: %s\n", value.c_str() );
        break;
    case CONFIG_PWD:
        m_ConfigControl->PWD = value; 
        Serial.printf( "WifiControl::configure: Changed wifi password to: %s\n", value.c_str() );
        break;
    case CONFIG_IP:
        if( !IPAddress::isValid( value.c_str() ) ) return ERROR_CONFIG_IP;
        m_ConfigControl->StaticIP.fromString( value );
        Serial.printf( "WifiControl::configure: Changed StaticIP to: %s\n", value.c_str() );
        break;
    case CONFIG_SUBNET:
        if( !IPAddress::isValid( value.c_str() ) ) return ERROR_CONFIG_SUBNET;
        m_ConfigControl->Subnet.fromString( value );
        Serial.printf( "WifiControl::configure: Changed Subnet to: %s\n", value.c_str() );
        break;
    case CONFIG_GATEWAY:
        if( !IPAddress::isValid( value.c_str() ) ) return ERROR_CONFIG_GATEWAY;
        m_ConfigControl->Gateway.fromString( value );
        Serial.printf( "WifiControl::configure: Changed Gateway to: %s\n", value.c_str() );
        break;
    case CONFIG_PORT_TCP:
        if( value.toInt() < 1 ) return ERROR_CONFIG_PORT_TCP;
        m_ConfigControl->PortTCP = value.toInt();
        Serial.printf( "WifiControl::configure: Changed TCP Port to: \n%d", m_ConfigControl->PortTCP );
        break;
    case CONFIG_PORT_HTTP:
        if( value.toInt() < 1 ) return ERROR_CONFIG_PORT_HTTP;
        m_ConfigControl->PortHTTP = value.toInt();
        Serial.printf( "WifiControl::configure: Changed HTTP Port to: \n%d", m_ConfigControl->PortHTTP );
        break;
    case CONFIG_DNS1:
        if( !IPAddress::isValid( value.c_str() ) ) return ERROR_CONFIG_DNS1;
        m_ConfigControl->DnsPrimary.fromString( value );
        Serial.printf( "WifiControl::configure: Changed DnsPrimary to: %s\n", value.c_str() );
        break;
    case CONFIG_DNS2:
        if( !IPAddress::isValid( value.c_str() ) ) return ERROR_CONFIG_DNS2;
        m_ConfigControl->DnsSecundary.fromString( value );
        Serial.printf( "WifiControl::configure: Changed DnsSecundary to: %s\n", value.c_str() );
        break;
    case CONFIG_MAX_CLIENTS:
        if( value.toInt() < 1 ) return ERROR_CONFIG_PORT_TCP;
        m_ConfigControl->MaxClients = value.toInt();
        Serial.printf( "WifiControl::configure: Changed MaxClients to: \n%d", m_ConfigControl->MaxClients );
        break;
    case CONFIG_INACTIVE_TIMEOUT:
        if( value.toInt() < 1 ) return ERROR_CONFIG_PORT_TCP;
        m_ConfigControl->InActiveTimeout = value.toInt();
        Serial.printf( "WifiControl::configure: Changed InActiveTimeout to: \n%d", m_ConfigControl->InActiveTimeout );
        break;
    case CONFIG_SHOW:
    case CONFIG_PIN:
    case CONFIG_ERROR:
        // not possible
        return ERROR_CONFIG;
        break;
    }
    m_ConfigControl->updated = true;
    return SUCCESS;
}

// Serve a file from LittleFS
void WifiControl::handleFileRequest( String path ) {
    if (path.endsWith("/")) path += "index.html";
    String contentType = "text/plain";
    if (path.endsWith(".html")) contentType = "text/html";
    else if (path.endsWith(".css")) contentType = "text/css";
    else if (path.endsWith(".js")) contentType = "application/javascript";
    else if (path.endsWith(".ico")) contentType = "image/x-icon";
    else if (path.endsWith(".woff2")) contentType = "font/woff2";
    
    if (!LittleFS.exists(path)) {
        Serial.println( "WifiControl::handleFileRequest: Could not find file: "+path );
        m_HttpServer->send(404, "text/plain", "Could not find file: "+path);
        return;
    }

    File file = LittleFS.open(path, "r");
    m_HttpServer->streamFile(file, contentType);
    file.close();
}