#include "configcontrol.h"

ConfigControl::ConfigControl(){
    LittleFS.begin();
    pinData.emplace( PIN_ANA0, (IO_PIN){ "A0", A0, PIN_INPUT, 0 } );
    pinData.emplace( PIN_DIG0, (IO_PIN){ "D0", D0, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG1, (IO_PIN){ "D1", D1, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG2, (IO_PIN){ "D2", D2, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG3, (IO_PIN){ "D3", D3, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG4, (IO_PIN){ "D4", D4, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG5, (IO_PIN){ "D5", D5, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG6, (IO_PIN){ "D6", D6, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG7, (IO_PIN){ "D7", D7, PIN_NOT_SET, 0 } );
    pinData.emplace( PIN_DIG8, (IO_PIN){ "D8", D8, PIN_NOT_SET, 0 } );
    updated = false;
}

ConfigControl::~ConfigControl()
{}

void ConfigControl::loadConfig(){
    // check if the file exists, load default values if not available.
    if (!LittleFS.exists(CONFIG_FILE)) {
        Serial.println("ConfigControl::loadConfig: Configuration file not found, using defaults.");
        StaticIP.fromString( "192.168.0.222" );
        Subnet.fromString( "255.255.255.0" );
        Gateway.fromString( "192.168.0.1" );
        PortTCP = 333;
        PortHTTP = 80;
        DnsPrimary.fromString( "8.8.8.8" );
        DnsSecundary.fromString( "8.8.4.4" );
        MaxClients = 12;
        InActiveTimeout = 1000*60*2;
        loaded = true;
        return;
    }

    // Open the configuration file for reading
    File configFile = LittleFS.open(CONFIG_FILE, "r"); 
    if( !configFile ){
        Serial.println("ConfigControl::loadConfig: Failed to open the configuraton file.");
        return;
    }

    // Read the io control data
    pinData[PIN_DIG0].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG1].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG2].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG3].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG4].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG5].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG6].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG7].mode = static_cast<PinConfig>( configFile.parseInt() );
    pinData[PIN_DIG8].mode = static_cast<PinConfig>( configFile.parseInt() );

    // consume last before reading string
    configFile.read();

    // Read the wifi control data
    String s;
    SSID = configFile.readStringUntil( '\n' );SSID.trim();
    PWD = configFile.readStringUntil( '\n' );PWD.trim();
    s = configFile.readStringUntil( '\n' ); s.trim();
    StaticIP.fromString( s );
    s = configFile.readStringUntil( '\n' ); s.trim();
    Subnet.fromString( s );
    s = configFile.readStringUntil( '\n' ); s.trim();
    Gateway.fromString( s );
    PortTCP = static_cast<uint16>( configFile.parseInt() );
    PortHTTP = static_cast<uint16>( configFile.parseInt() );

    // consume last \n before reading string
    configFile.read();

    s = configFile.readStringUntil( '\n' ); s.trim();
    DnsPrimary.fromString( s );
    s = configFile.readStringUntil( '\n' ); s.trim();
    DnsSecundary.fromString( s );
    MaxClients = static_cast<uint>( configFile.parseInt() );
    InActiveTimeout = static_cast<uint32>( configFile.parseInt() );

    // Done close the configuration file.
    configFile.close();
    loaded = true;
    Serial.println("ConfigControl::loadConfig: Configuration file has been loaded from flash memory.");
}

void ConfigControl::saveConfig(){
    // skip if no changes have been made
    if( !updated ) return;

    // Open configuration file on flash memory to overwrite it
    File configFile = LittleFS.open(CONFIG_FILE, "w"); 
    if( !configFile ) {
        Serial.println("ConfigControl::saveConfig: Failed to open config file for writing.");
        return;
    }
    
    // Safe io control data
    configFile.printf( "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
        pinData[PIN_DIG0].mode,
        pinData[PIN_DIG1].mode,
        pinData[PIN_DIG2].mode,
        pinData[PIN_DIG3].mode,
        pinData[PIN_DIG4].mode,
        pinData[PIN_DIG5].mode,
        pinData[PIN_DIG6].mode,
        pinData[PIN_DIG7].mode,
        pinData[PIN_DIG8].mode 
    );

    // Safewifi control data
    configFile.printf( "%s\n%s\n%s\n%s\n%s\n%d\n%d\n%s\n%s\n%d\n%d\n",
        SSID.c_str(),
        PWD.c_str(),
        StaticIP.toString().c_str(),
        Subnet.toString().c_str(),
        Gateway.toString().c_str(),
        PortTCP,
        PortHTTP,
        DnsPrimary.toString().c_str(),
        DnsSecundary.toString().c_str(),
        MaxClients,
        InActiveTimeout
    );

    // Done close the configuration file
    configFile.close();
    Serial.println("ConfigControl::saveConfig: saved configuration to flash memory.");
    updated = false;
}

/**
 * @brief Show the contents of the config file in the serial monitor
 */
void ConfigControl::printConfig(){
    // check if the file exists, load default values if not available.
    if (!LittleFS.exists(CONFIG_FILE)) {
        Serial.println("ConfigControl::loadConfig: Configuration file not found.");
        return;
    }

    // Open the configuration file for reading
    File configFile = LittleFS.open(CONFIG_FILE, "r"); 
    if( !configFile ){
        Serial.println("ConfigControl::loadConfig: Failed to open the configuraton file.");
        configFile.close();
        return;
    }

    Serial.println( configFile.readString() );
    configFile.close();
    Serial.println("--------------------------------\nIn memory values:");
    Serial.printf( "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
        pinData[PIN_DIG0].mode,
        pinData[PIN_DIG1].mode,
        pinData[PIN_DIG2].mode,
        pinData[PIN_DIG3].mode,
        pinData[PIN_DIG4].mode,
        pinData[PIN_DIG5].mode,
        pinData[PIN_DIG6].mode,
        pinData[PIN_DIG7].mode,
        pinData[PIN_DIG8].mode 
    );

    // Safewifi control data
    Serial.printf( "%s\n%s\n%s\n%s\n%s\n%d\n%d\n%s\n%s\n%d\n%d\n",
        SSID.c_str(),
        PWD.c_str(),
        StaticIP.toString().c_str(),
        Subnet.toString().c_str(),
        Gateway.toString().c_str(),
        PortTCP,
        PortHTTP,
        DnsPrimary.toString().c_str(),
        DnsSecundary.toString().c_str(),
        MaxClients,
        InActiveTimeout
    );
}

/**
 * @brief Read the contents of the config file as a string
 */
String ConfigControl::readConfig(){
    // check if the file exists, load default values if not available.
    if (!LittleFS.exists(CONFIG_FILE)) {
        Serial.println("ConfigControl::loadConfig: Configuration file not found.");
        return "Configuration file not found.";
    }

    // Open the configuration file for reading
    File configFile = LittleFS.open(CONFIG_FILE, "r"); 
    if( !configFile ){
        Serial.println("ConfigControl::loadConfig: Failed to open the configuraton file.");
        configFile.close();
        return "Failed to open the configuraton file.";
    }

    String result = configFile.readString();
    configFile.close();
    return result;
}