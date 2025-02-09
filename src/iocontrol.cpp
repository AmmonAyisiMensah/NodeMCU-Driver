#include "iocontrol.h"

/**
 * @brief Construct a new NodeMCU object
 */
IOControl::IOControl( ConfigControl *configControl )
: m_ConfigControl(configControl)
{
    
}

/**
 * @brief Perform a reset of the NodeMCU
 */
void IOControl::reset(){
    //resetFunc();
}

/**
 * @brief Load the pin mode from the flash memory confiuration
 */
void IOControl::load(){
    configurePin( PIN_ANA0, m_ConfigControl->pinData[PIN_ANA0].mode );
    configurePin( PIN_DIG0, m_ConfigControl->pinData[PIN_DIG0].mode );
    configurePin( PIN_DIG1, m_ConfigControl->pinData[PIN_DIG1].mode );
    configurePin( PIN_DIG2, m_ConfigControl->pinData[PIN_DIG2].mode );
    configurePin( PIN_DIG3, m_ConfigControl->pinData[PIN_DIG3].mode );
    configurePin( PIN_DIG4, m_ConfigControl->pinData[PIN_DIG4].mode );
    configurePin( PIN_DIG5, m_ConfigControl->pinData[PIN_DIG5].mode );
    configurePin( PIN_DIG6, m_ConfigControl->pinData[PIN_DIG6].mode );
    configurePin( PIN_DIG7, m_ConfigControl->pinData[PIN_DIG7].mode );
    configurePin( PIN_DIG8, m_ConfigControl->pinData[PIN_DIG8].mode );
    m_ConfigControl->updated = false;
}

/**
 * @brief Execute a configuration command for a pin on the board.
 * 
 * @param mode the direction of the pin
 * @param pin the pin to configure
 * @return result code 
 */
uint16 IOControl::configurePin(const PinId &pin, const uint16 &mode){
    if( m_ConfigControl->pinData.count( pin ) == 0 ) return PIN_ERROR;

    switch( mode ){
    case PIN_INPUT:
        pinMode( m_ConfigControl->pinData[pin].gpio, INPUT );
        m_ConfigControl->pinData[pin].mode = PIN_INPUT;
        Serial.printf( "IOControl::configurePin: %s (GPIO%d) as INPUT\n", m_ConfigControl->pinData[pin].name.c_str(), m_ConfigControl->pinData[pin].gpio) ;
        break;
    case PIN_OUTPUT:
        pinMode( m_ConfigControl->pinData[pin].gpio, OUTPUT );
        m_ConfigControl->pinData[pin].mode = PIN_OUTPUT;
        Serial.printf( "IOControl::configurePin: %s (GPIO%d) as OUTPUT\n", m_ConfigControl->pinData[pin].name.c_str(), m_ConfigControl->pinData[pin].gpio );
        break;
    case PIN_NOT_SET:
        return PIN_ERROR;
        break;
    }
    m_ConfigControl->updated = true;
    return COMMAND_SUCCESS;
}

/**
 * @brief Execute a read command on the board.
 * 
 * @param pin th pin to read the value of
 * @param value output buffer for the value of the pin
 * @return uint16 result code
 */
uint16 IOControl::read(const PinId &pin, int &value){
    if( m_ConfigControl->pinData.count( pin ) == 0 ) return PIN_ERROR;

    value = pin == PIN_ANA0 ? analogRead( m_ConfigControl->pinData[pin].gpio ) : digitalRead( m_ConfigControl->pinData[pin].gpio );
    m_ConfigControl->pinData[pin].value = value;
    Serial.printf("IOControl::Read: %s (GPIO%d) = %d\n", m_ConfigControl->pinData[pin].name.c_str(), m_ConfigControl->pinData[pin].gpio, value);
    return COMMAND_SUCCESS;
}

/**
 * @brief Execute a write command on the board.
 * 
 * @param pin th pin to write the value to
 * @param value the value to be written
 * @return uint16 result code
 */
uint16 IOControl::write(const PinId &pin, int value){
    if( m_ConfigControl->pinData.count( pin ) == 0 ) return PIN_ERROR;
    if( pin == PIN_ANA0 ) return PIN_ERROR | PIN_ANA0;

    digitalWrite( m_ConfigControl->pinData[pin].gpio, value );
    Serial.printf("IOControl::Write: %s (GPIO%d) = %d\n", m_ConfigControl->pinData[pin].name.c_str(), m_ConfigControl->pinData[pin].gpio, value);
    return COMMAND_SUCCESS;
}