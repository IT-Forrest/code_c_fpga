/*******************************************************************************
*
* This file is necessary for code migration from PSoC to DE1-SoC.
* Please do not delete it.
********************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef DEVICE_H
#define DEVICE_H

//#include    "hps_0.h"
//#include    "psoc_port.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "psoc_port.h"


typedef float   float32;
typedef uint8_t uint8;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef int int32;
typedef short  int16;
typedef char char8;


// transfer to Linux system
#define USBUART_1_3V_OPERATION  10
#define USBUART_1_PutString     printf
#define USBUART_1_GetChar       getchar
#define USBUART_1_PutChar       putchar


#define IDX_CTRL_STA    (0)                   // bit position. Scan Chain. STA
#define IDX_CTRL_SEL    (1)                   // bit position. Scan Chain. SEL
#define IDX_CTRL_SIN    (2)                   // bit position. Scan Chain. SIN
#define IDX_CTRL_MST    (3)                   // bit position. Measurement. STA
#define IDX_CTRL_SRC    (4)                   // bit position. Measurement. SRC
#define IDX_CTRL_CAL    (5)                   // bit position. Measurement. CAL
#define IDX_CTRL_PHS    (6)                   // bit position. Measurement. PHS
#define IDX_CTRL_FS0    (7)                   // bit position. Measurement. FS[0]
#define IDX_CTRL_FS1    (8)                   // bit position. Measurement. FS[1]
#define IDX_CTRL_OSC    (9)                   // bit position. Measurement. OSCD
#define IDX_CTRL_EXT    (10)                  // bit position. Global. EXTB
#define IDX_CTRL_RSN    (11)                  // bit position. Global. RSTN

#define IDX_STAT_SCRDY  (0)                   // bit position. Scan Chain. Ready
#define IDX_STAT_SOANA  (1)                   // bit position. Scan Chain. SO_ANA
#define IDX_STAT_SODIG  (2)                   // bit position. Scan Chain. SO_DIG
#define IDX_STAT_MSRDY  (3)                   // bit position. Measurement. Ready

#define IDX_CTRL_NXT    (0)                   // bit position. initiate Simulated annealing
#define IDX_CTRL_4DY    (1)                   // bit position. 4Dimension flag
#define IDX_CTRL_SARN   (2)
#define IDX_STAT_WSARDY (0)                   // bit position. Simmulated Annealing. Ready


#define IDX_CTRL_4DNXT    (0)                   // bit position. initiate Simulated annealing
#define IDX_CTRL_4D4DY    (1)                   // bit position. 4Dimension flag
#define IDX_CTRL_4DSARN   (2)
#define IDX_CTRL_4DMOD0   (3)
#define IDX_CTRL_4DMOD1   (4)
#define IDX_CTRL_4DMOD2   (5)
#define IDX_CTRL_4DSTEPT0 (6)
#define IDX_CTRL_4DSTEPT1 (7)
#define IDX_CTRL_4DSTEPT2 (8)

#define IDX_CTRL_ACCU     (9)
#define IDX_CTRL_GAIN0_0  (10)              // Gain coefficient for Gain 0
#define IDX_CTRL_GAIN0_1  (11)              // Gain coefficient for Gain 0
#define IDX_CTRL_GAIN1_0  (12)              // Gain coefficient for Gain 1
#define IDX_CTRL_GAIN1_1  (13)              // Gain coefficient for Gain 1
#define IDX_CTRL_GAIN2_0  (14)              // Gain coefficient for Gain 2
#define IDX_CTRL_GAIN2_1  (15)              // Gain coefficient for Gain 2
#define IDX_CTRL_GAIN3_0  (16)              // Gain coefficient for Gain 3
#define IDX_CTRL_GAIN3_1  (17)              // Gain coefficient for Gain 3

#define IDX_CTRL_MUXNXT  (21)              // manual control signal of NXT
#define IDX_CTRL_MUXRDY  (19)              // manual control signal of Ready
#define IDX_CTRL_MUXTRG  (20)              // manual control signal of TRG

#define IDX_CFSA_SARAND  (22)               // SA Random group flag;
#define IDX_CFSA_CFAVRG  (23)               // CF Average measurment number

#define IDX_STAT_SA4DRDY (0)

/////////////////
#define IDX_CTRL_STA_SC     (0)
#define IDX_CTRL_SIN_AB     (1)
#define IDX_CTRL_SEL_A      (2)
#define IDX_CTRL_SEL_B      (3)
#define IDX_CTRL_LAT_A      (4)
#define IDX_CTRL_LAT_B      (5)
#define IDX_CTRL_FLAG_A     (6)
#define IDX_CTRL_FLAG_B     (7)
#define IDX_CTRL_STA_CLK    (8)
#define IDX_CTRL_STA_CLK    (8)
#define IDX_CTRL_RST_N      (9)
#define IDX_CTRL_ONE_CLK   (10)

#define IDX_STAT_SCREADY    (0)                   // bit position. Scan Chain. Ready
#define IDX_STAT_SCSO_A     (1)                   // bit position. Scan Chain. SO_ANA
#define IDX_STAT_SCSO_B     (2)                   // bit position. Scan Chain. SO_DIG
#define IDX_STAT_ONERDY     (3)                   // bit position. Measurement. Ready

int USBUART_1_CDCIsReady();
int USBUART_1_DataIsReady();
void CFG_CKADC_Write(int bit_val);
// change RSTN bit: ??? what does this signal use for ???
void CFG_RSTN_Write(int bit_val);
void USBUART_1_Start(int x, int y);
void USBUART_1_CDC_Init();
void CFG_STA_Write(int bit_val);
int CFG_RDY_Read();
int ANA_So_Read();
int MSMH_So_Read();
int ADC_RDY_Read();
int WSA_RDY_Read();
void WSA_STA_Write(int bit_val);
void WSA_4DY_Write(int bit_val);
void WSA_RSN_Write(int bit_val);
unsigned int SA_TUNEX1_Read();
unsigned int SA_TUNEX2_Read();
unsigned int SA_TUNEX3_Read();
unsigned int SA_TUNEX4_Read();
void SA_INITX1_Write(int bit_val);
void SA_INITX2_Write(int bit_val);
void SA_INITX3_Write(int bit_val);
void SA_INITX4_Write(int bit_val);
void SA_ITERNUM_Write(int bit_val);
void SA_ANAIN_Write(int bit_val);
void ADC_STA_Write(int bit_val);
void ADC_STA_Write(int bit_val);
void CFG_SEL_Write(int bit_val);

void CFG_FSEL_Write(int rd_val);
void CFG_PHS_Write(int rd_val);
void CFG_SRC_Write(int rd_val);
void CFG_CAL_Write(int rd_val);
void CFG_OSCD_Write(int bit_val);
void CFG_DAT_Write(int bit_val);
void Global_RSTN_Write(int bit_val);

// AMS I/O for sa_4D_test
void SA4D_STA_Write(int bit_val);
void SA4D_4DY_Write(int bit_val);
void SA4D_RSN_Write(int bit_val);
void SA4D_MOD_Write(int rd_val);
void SA4D_STEPT_Write(int rd_val);
void SA_INIT4DX1_Write(int bit_val);
void SA_INIT4DX2_Write(int bit_val);
void SA_INIT4DX3_Write(int bit_val);
void SA_INIT4DX4_Write(int bit_val);
void SA_ITER4DSA_Write(int bit_val);
void SA_ITER4DSS_Write(int bit_val);
void SA_INITT4D_Write(int bit_val);
void SA_ANAIN4D_Write(int bit_val);
int SA4D_RDY_Read();
unsigned int SA_TUNE4DX1_Read();
unsigned int SA_TUNE4DX2_Read();
unsigned int SA_TUNE4DX3_Read();
unsigned int SA_TUNE4DX4_Read();

// AMS I/O for cfsa_4D_test
void CFSA4D_SARAND_Write(int bit_val);
void CFSA4D_CFAVRG_Write(int bit_val);
void CFSA4D_4DY_Write(int bit_val);
void CFSA4D_RSN_Write(int bit_val);
void CFSA4D_MOD_Write(int rd_val);
void CFSA4D_STEPT_Write(int rd_val);
void CFSA4D_DIVACCU_Write(int rd_val);
void CFSA4D_GAIN0_Write(int rd_val);
void CFSA4D_GAIN1_Write(int rd_val);
void CFSA4D_GAIN2_Write(int rd_val);
void CFSA4D_GAIN3_Write(int rd_val);

void CFSA_INIT4DX1_Write(int bit_val);
void CFSA_INIT4DX2_Write(int bit_val);
void CFSA_INIT4DX3_Write(int bit_val);
void CFSA_INIT4DX4_Write(int bit_val);
void CFSA_ITER4DSA_Write(int bit_val);
void CFSA_ITER4DSS_Write(int bit_val);
void CFSA_INITT4D_Write(int bit_val);
void CFSA_TOLIN4D_Write(int bit_val);
void CFSA_OFFIN4D_Write(int bit_val);
void CFSA_3DBFACTOR0_Write(int bit_val);
void CFSA_3DBFACTOR1_Write(int bit_val);
void CFSA_ADCIN4D_Write(int bit_val);
void CFSA_THRESHOLD_Write(int bit_val);

void CFSA4D_MUXNXT_Write(int rd_val);
void CFSA4D_MUXRDY_Write(int rd_val);
void CFSA4D_MUXTRG_Write(int rd_val);

unsigned int CFSA_TUNE4DX1_Read();
unsigned int CFSA_TUNE4DX2_Read();
unsigned int CFSA_TUNE4DX3_Read();
unsigned int CFSA_TUNE4DX4_Read();


int DEBUG_Read();
float sqrt12( const float N);
//double sqrt12( unsigned long N );

#endif
