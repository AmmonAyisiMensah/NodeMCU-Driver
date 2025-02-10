/**
 * @file iocontrol.h
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
#ifndef IOCONTROL_H
#define IOCONTROL_H

#include <Arduino.h>
#include "configcontrol.h"


/**
 * @brief Soft reset function for the NodeMCU board.
 */
//void(* resetFunc) (void) = 0;

/**
 * @brief The IOControl class is used to control the actual IO pins of the NodeMCU board.
 * It can be used to configure the pins and to read/write them.
 */
class IOControl{
public:
    /**
     * @brief Construct a new NodeMCU object
     * 
     * @param configControl instance pointer to the cofiguration control of the flash memory
     */
    IOControl( ConfigControl *configControl );

    /**
     * @brief Perform a reset of the NodeMCU
     */
    void reset();

    /**
     * @brief Load the pin mode from the flash memory confiuration
     */
    void load();
    
    /**
     * @brief Execute a configuration command for a pin on the board.
     * 
     * @param mode the direction of the pin
     * @param pin the pin to configure
     * @return result code 
     */
    uint16 configurePin(const PinId &pin, const uint16 &mode);

    /**
     * @brief Execute a read command on the board.
     * 
     * @param pin th pin to read the value of
     * @param value output buffer for the value of the pin
     * @return uint16 result code
     */
    uint16 read(const PinId &pin, int &value);

    /**
     * @brief Execute a write command on the board.
     * 
     * @param pin th pin to write the value to
     * @param value the value to be written
     * @return uint16 result code
     */
    uint16 write(const PinId &pin, int value);

private:
    /**
     * @brief Instance poiner of the configuration data in the flash memory of the NodeMCU.
     */
    ConfigControl *m_ConfigControl;
};

#endif