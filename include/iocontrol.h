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