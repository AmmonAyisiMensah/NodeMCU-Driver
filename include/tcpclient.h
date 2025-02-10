/**
 * @file tcpclient.h
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