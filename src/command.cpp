/**
 * @file command.cpp
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
#include "command.h"

/**
 * @brief This functon is called to convert a string commands into an enumerator value
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
Command parseCommand(const String &command){
    if( command.equalsIgnoreCase( "reset" ) ) return COMMAND_RESET;
    if( command.equalsIgnoreCase( "config" ) ) return COMMAND_CONFIG;
    if( command.equalsIgnoreCase( "read" ) ) return COMMAND_READ;
    if( command.equalsIgnoreCase( "write" ) ) return COMMAND_WRITE;
    return COMMAND_ERROR;
}

/**
 * @brief This functon is called to convert a string commands into an enumerator value
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
ConfigCommand parseConfigCommand(const String &command){
    if( command.equalsIgnoreCase( "show" )) return CONFIG_SHOW;
    if( command.equalsIgnoreCase( "pin" )) return CONFIG_PIN;
    if( command.equalsIgnoreCase( "ssid" )) return CONFIG_SSID;
    if( command.equalsIgnoreCase( "pwd" )) return CONFIG_PWD;
    if( command.equalsIgnoreCase( "ip" ) ) return CONFIG_IP;    
    if( command.equalsIgnoreCase( "subnet" ) ) return CONFIG_SUBNET;
    if( command.equalsIgnoreCase( "gateway" ) ) return CONFIG_GATEWAY;
    if( command.equalsIgnoreCase( "tcp-port" ) ) return CONFIG_PORT_TCP;
    if( command.equalsIgnoreCase( "http-port" ) ) return CONFIG_PORT_HTTP;
    if( command.equalsIgnoreCase( "dns1" )) return CONFIG_DNS1;
    if( command.equalsIgnoreCase( "dns2" )) return CONFIG_DNS2;
    if( command.equalsIgnoreCase( "max-clients" )) return CONFIG_MAX_CLIENTS;
    if( command.equalsIgnoreCase( "timeout" )) return CONFIG_INACTIVE_TIMEOUT;
    return CONFIG_ERROR;
}

/**
 * @brief This functon is called to convert a string commands into an enumerator value
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
PinConfig parsePinConfigCommand(const String &command){
    if( command.equalsIgnoreCase( "input" ) ) return PIN_INPUT;
    if( command.equalsIgnoreCase( "output" ) ) return PIN_OUTPUT;
    return PIN_NOT_SET;
}

/**
 * @brief This functon is called to convert a string commands into an enumerator value
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
PinId parsePinCommand(const String &command){
    if( command.equalsIgnoreCase( "A0" ) ) return PIN_ANA0;
    if( command.equalsIgnoreCase( "D0" ) ) return PIN_DIG0;
    if( command.equalsIgnoreCase( "D1" ) ) return PIN_DIG1;
    if( command.equalsIgnoreCase( "D2" ) ) return PIN_DIG2;
    if( command.equalsIgnoreCase( "D3" ) ) return PIN_DIG3;
    if( command.equalsIgnoreCase( "D4" ) ) return PIN_DIG4;
    if( command.equalsIgnoreCase( "D5" ) ) return PIN_DIG5;
    if( command.equalsIgnoreCase( "D6" ) ) return PIN_DIG6;
    if( command.equalsIgnoreCase( "D7" ) ) return PIN_DIG7;
    if( command.equalsIgnoreCase( "D8" ) ) return PIN_DIG8;
    return PIN_ERROR;
}

/**
 * @brief This functon is called to convert a string commands into an enumerator value
 * 
 * @param command the provided command string
 * @return an enum value of Command or COMMAND_ERROR
 */
Protocol parseProtocolCommand(const String &command){
    if( command.equalsIgnoreCase( "http" ) ) return PROTOCOL_HTTP;
    if( command.equalsIgnoreCase( "tcp" ) ) return PROTOCOL_TCP;
    if( command.equalsIgnoreCase( "serial" ) ) return PROTOCOL_SERIAL;
    return PROTOCOL_ERROR;
}