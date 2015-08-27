/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/


#ifndef _SERIAL_PORT_IO_H_
#define _SERIAL_PORT_IO_H_

#include    "device.h"
#include    <stdint.h>
#include    <stdlib.h>

void    ReadArray(uint8 *dat, uint8 *length); // Read number from console
uint16  ReadInput(uint8 *length);
void    PutStr(char *str); // Output string to terminal
void    FmtPrint(int32 val); // Format and printer value
void    PutWait();// Wait for any key

#endif//_SERIAL_PORT_IO_H_

/* [] END OF FILE */