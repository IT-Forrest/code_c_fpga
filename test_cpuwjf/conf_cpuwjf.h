/* ========================================
 *
 * Copyright Jiafan Wang, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/


#ifndef _CONF_CPUWJF_H_
#define _CONF_CPUWJF_H_

#include    <stdio.h>
#include    <stdlib.h> ///strtol
#include    <string.h> ///strcmp

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/test_adc.h"

int16   rd_bfile_to_mem_buf(FILE *fd, uint8 *sram_buf, uint16 reserve_len);
int16   rd_bfile_to_adc_buf(FILE *fd, uint16 *adc_buf, bool is_osc);
void    dec2bin(int c, int strlen);
void    send_clk_cycles(int clk_cnt);
void    conf_ctrl_flag_value(int mode_type);
void    wait_ctrl_flag_clean();

#endif//_SERIAL_PORT_IO_H_

/* [] END OF FILE */
