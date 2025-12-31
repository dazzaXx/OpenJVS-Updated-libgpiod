#include "hardware/rotary.h"
#include "hardware/device.h"
#include "console/debug.h"

/**
 * Init Rotary on Raspberry Pi HAT
 * 
 * Inits the rotary controller on the Raspberry Pi HAT
 * to select which map we will use
 * 
 * @returns JVS_ROTARY_STATUS_SUCCESS if it inited correctly.
 */
JVSRotaryStatus initRotary(void)
{
    setupGPIO(18);
    if (!setGPIODirection(18, IN))
    {
        debug(1, "Warning: Failed to set Raspberry Pi GPIO Pin 18\n");
        return JVS_ROTARY_STATUS_ERROR;
    }

    setupGPIO(19);
    if (!setGPIODirection(19, IN))
    {
        debug(1, "Warning: Failed to set Raspberry Pi GPIO Pin 19\n");
        return JVS_ROTARY_STATUS_ERROR;
    }

    setupGPIO(20);
    if (!setGPIODirection(20, IN))
    {
        debug(1, "Warning: Failed to set Raspberry Pi GPIO Pin 20\n");
        return JVS_ROTARY_STATUS_ERROR;
    }

    setupGPIO(21);
    if (!setGPIODirection(21, IN))
    {
        debug(1, "Warning: Failed to set Raspberry Pi GPIO Pin 21\n");
        return JVS_ROTARY_STATUS_ERROR;
    }

    return JVS_ROTARY_STATUS_SUCCESS;
}

/**
 * Get rotary value
 * 
 * Returns the value from 0 to 15 for
 * which map to use.
 * 
 * @returns The value from 0 to 15 on the rotary encoder, or -1 on error
 */
int getRotaryValue(void)
{
    int bit0 = readGPIO(18);
    int bit1 = readGPIO(19);
    int bit2 = readGPIO(20);
    int bit3 = readGPIO(21);
    
    /* Check for GPIO read errors */
    if (bit0 < 0 || bit1 < 0 || bit2 < 0 || bit3 < 0)
    {
        debug(1, "Warning: Failed to read GPIO pins for rotary encoder\n");
        return -1;
    }
    
    int value = 0;
    value = value | (bit0 << 0);
    value = value | (bit1 << 1);
    value = value | (bit2 << 2);
    value = value | (bit3 << 3);

    value = ~value & 0x0F;

    return value;
}
