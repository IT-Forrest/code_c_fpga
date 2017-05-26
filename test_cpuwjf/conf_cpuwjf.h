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

#define     CHIP4_GET_ADC_BUF(bgn_line)     ((read_buf[bgn_line*2+1] << 8) | (read_buf[bgn_line*2]))

int16   rd_bfile_to_mem_buf(FILE *fd, uint8 *sram_buf, uint16 reserve_len);
int16   rd_bfile_to_adc_buf(FILE *fd, uint16 *adc_buf, bool is_osc);
void    dec2bin(int c, int strlen);
void    send_clk_cycles(int clk_cnt);
void    conf_ctrl_flag_value(int mode_type);
void    wait_ctrl_flag_clean();
void    chg_fpga_clk_freq(uint16 cnt_clk);
void    write_insts_to_sram(uint8 *sram_buf, int16 inst_num, uint16 reserve_len);
void    chg_clk_and_start_cpu();
void    get_IQ_data_to_cpu(uint16 addr_adc, uint16 *adcs_buf);
void    read_data_from_sram(uint8 *read_buf, uint16 bgn_line, uint16 num_line);
#endif//_SERIAL_PORT_IO_H_

/* [] END OF FILE */
