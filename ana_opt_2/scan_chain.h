/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * This file is used to set the configuration bits
 *      and define relative functions.
 * Author: Jiafan Wang
 * Date:   03/08/2015
 * ========================================
*/

#ifndef _SCAN_CHAIN_H_
#define _SCAN_CHAIN_H_

// #include "hps_0.h"
// #include <stdint.h>
#include "device.h"
// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/mman.h>
// #include "hwlib.h"
// #include "socal/socal.h"
// #include "socal/hps.h"
// #include "socal/alt_gpio.h"

//////////////////////////////////////////////////////
// the macro definition
//////////////////////////////////////////////////////
//#define     DEBUG_ON
#define     USE_NEW_VAL     (0 != len)

//#define     MAX_ANA_BUF     10//MAX_ADC_BUF
#define     MAX_PSOC_BUF    18500//>=18*1024+16*4; 16384
#define     MAX_5BIT_VAL    32//maximum value for a 5bit number
//#define     MAX_SC_BITS     73//total scan chain length for version 1

#define     MAX_TUNE_NUM    1024//1024
#define     MAX_DEEP_XY     2//5
#define     MAX_IQ_AMPL     90
#define     MAX_VOFF        512

#define     MAX_FSEL_NUM    1//later this will be 3;
#define     MAX_ADC_BUF     4096//max
#define     MAX_AVG_NUM     16//average of how many times

// structure for a single IQ_ELEMENT
typedef uint16  IQ_ELEMENT;
typedef IQ_ELEMENT  TUNE_ELEMENT;

// structure for a single frequency
typedef struct  FSEL_ELEMENT_STR
{
    //Voff
    IQ_ELEMENT  Iout;
    IQ_ELEMENT  Qout;
    IQ_ELEMENT  Iinn;
    IQ_ELEMENT  Qinn;
    //For oscillation detection
    //IQ_ELEMENT  Iosc;
}FSEL_ELEMENT;

// structure for one XY solution. Namely, 3 frequencies; FSEL:1,2,3
typedef struct  XY_ELEMENT_STR
{
    IQ_ELEMENT  FSEL_list[MAX_FSEL_NUM];
}XY_ELEMENT;


void    PrintXY(uint16 read_len, uint16 *xy_buf);
void    JudgePrintXY(uint16 read_len, uint16 *xy_buf);
uint16  ReadTuneXY();
uint16  SimpleReadXY(uint8 init_x, uint8 init_y, uint8 ana_bits, uint8 iter_num);
uint16  IterReadXY(uint16 start_pos, TUNE_ELEMENT* xy_buf);
uint16  DeepReadXY(uint16 start_pos, TUNE_ELEMENT* xy_buf, uint8 init_x, uint8 init_y, uint8 ana_bits, uint8 iter_num);
uint16  IterDeepXY(uint16 start_pos, TUNE_ELEMENT* xy_buf);

#endif
