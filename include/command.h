#ifndef COMMAND_H
#define COMMAND_H

#include <WString.h>

/**
 * @brief The main commands with numeric values.
 */
enum Command{
    COMMAND_ERROR = 0xFFFF,
    COMMAND_RESET = 0xA000,
    COMMAND_CONFIG = 0x1000,
    COMMAND_READ = 0x2000,
    COMMAND_WRITE = 0x3000,
    COMMAND_SUCCESS = 0x0000
};

/**
 * @brief The configuration commands with numeric values.
 */
enum ConfigCommand{
    CONFIG_SHOW = 0x0000,
    CONFIG_ERROR = 0xF000,
    CONFIG_PIN = 0x0100,
    CONFIG_SSID = 0x0200,
    CONFIG_PWD = 0x0300,
    CONFIG_IP = 0x0400,
    CONFIG_SUBNET = 0x0500,
    CONFIG_GATEWAY = 0x0600,
    CONFIG_DNS1 = 0x700,
    CONFIG_DNS2 = 0x800,
    CONFIG_PORT_TCP = 0x0900,
    CONFIG_PORT_HTTP = 0x0A00,
    CONFIG_MAX_CLIENTS = 0x0B00,
    CONFIG_INACTIVE_TIMEOUT = 0x0C00
};

/**
 * @brief The pin mode configuration with numeric values.
 */
enum PinConfig{
    PIN_NOT_SET = 0x0000,
    PIN_INPUT = 0x0010,
    PIN_OUTPUT = 0x0020
};

/**
 * @brief The supported pins that can be used safely (also during boot) with numeric values.
 */
enum PinId{
    PIN_ERROR = 0xFA00,
    PIN_ANA0 = 0x000A,
    PIN_DIG0 = 0x0000,
    PIN_DIG1 = 0x0001,
    PIN_DIG2 = 0x0002,
    PIN_DIG3 = 0x0003,
    PIN_DIG4 = 0x0004,
    PIN_DIG5 = 0x0005,
    PIN_DIG6 = 0x0006,
    PIN_DIG7 = 0x0007,
    PIN_DIG8 = 0x0008
};

/**
 * @brief The supported protocol configurations with numeric values.
 */
enum Protocol{
    PROTOCOL_ERROR = 0xFC00,
    PROTOCOL_HTTP = 0x00C1,
    PROTOCOL_TCP = 0x00C2,
    PROTOCOL_SERIAL = 0x00C3
};

/**
 * @brief The result codes we can have, may be used for error handling aswell
 */
enum ErrorCodes{
    SUCCESS = 0x0000,
    FAILED = 0xFFFF,
    ERROR_CONFIG = CONFIG_ERROR,
    ERROR_CONFIG_PIN = CONFIG_ERROR | CONFIG_PIN,
    ERROR_CONFIG_SSID = CONFIG_ERROR | CONFIG_SSID,
    ERROR_CONFIG_PWD = CONFIG_ERROR | CONFIG_PWD,
    ERROR_CONFIG_IP = CONFIG_ERROR | CONFIG_IP,
    ERROR_CONFIG_SUBNET = CONFIG_ERROR | CONFIG_SUBNET,
    ERROR_CONFIG_GATEWAY = CONFIG_ERROR | CONFIG_GATEWAY,
    ERROR_CONFIG_DNS1 = CONFIG_ERROR | CONFIG_DNS1,
    ERROR_CONFIG_DNS2 = CONFIG_ERROR | CONFIG_DNS2,
    ERROR_CONFIG_PORT_TCP = CONFIG_ERROR | CONFIG_PORT_TCP,
    ERROR_CONFIG_PORT_HTTP = CONFIG_ERROR | CONFIG_PORT_HTTP,
    ERROR_HTTP = PROTOCOL_ERROR | PROTOCOL_HTTP,
    ERROR_TCP = PROTOCOL_ERROR | PROTOCOL_TCP,
    ERROR_SERIAL = PROTOCOL_ERROR | PROTOCOL_SERIAL,
    ERROR_WIFI_CONFIG = PROTOCOL_ERROR | 0x00C4,
    ERROR_WIFI_CONNECTION = PROTOCOL_ERROR | 0x00C5,
    ERROR_CLIENT_DISCONNECTED = PROTOCOL_ERROR | 0x00CD,
    ERROR_READ = COMMAND_READ | 0x0F00,
    ERROR_WRITE = COMMAND_WRITE | 0x0F00
};

/**
 * @brief This functon is called to convert a string commands into an enumerator value.
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
Command parseCommand( const String &command );

/**
 * @brief This functon is called to convert a string commands into an enumerator value.
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
ConfigCommand parseConfigCommand( const String &command );

/**
 * @brief This functon is called to convert a string commands into an enumerator value.
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
PinConfig parsePinConfigCommand( const String &command );

/**
 * @brief This functon is called to convert a string commands into an enumerator value.
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
PinId parsePinCommand( const String &command );

/**
 * @brief This functon is called to convert a string commands into an enumerator value.
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
Protocol parseProtocolCommand( const String &command );

#endif