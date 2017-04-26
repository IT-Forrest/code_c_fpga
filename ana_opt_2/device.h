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
#define IDX_CTRL_LOW     (1)
#define IDX_CTRL_HIGH    (2)

/// CPU control flags
#define IDX_SCPU_CTRL_BGN   (0)
#define IDX_SCPU_CTRL_LOAD  (1)
#define IDX_SCPU_CTRL_MOD0  (2)
#define IDX_SCPU_CTRL_MOD1  (3)
#define IDX_SCPU_APP_DONE   (4)
#define IDX_SCPU_CPU_BGN    (5)
#define IDX_SCPU_RST_N      (6)
#define IDX_SCPU_CPU_WAIT   (7)
#define IDX_SCPU_TEST_MUX0  (8)
#define IDX_SCPU_TEST_MUX1  (9)
#define IDX_SCPU_TEST_MUX2  (10)
#define IDX_SCPU_CLK_STOP   (11)
#define IDX_SCPU_CLK_CHG    (12)

/// CPU status flags
#define IDX_SCPU_CTRL_RDY   (0)
#define IDX_SCPU_NXT_END    (1)
#define IDX_SCPU_NXT_CONT   (2)
#define IDX_SCPU_APP_START  (3)

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

//// tapeout3
void Chip3_Idx_Ctrl_Sta_Sc_Write(int rd_val);
void Chip3_Idx_Ctrl_Sin_Ab_Write(int rd_val);
void Chip3_Idx_Ctrl_Sel_A_Write(int rd_val);
void Chip3_Idx_Ctrl_Sel_B_Write(int rd_val);
void Chip3_Idx_Ctrl_Lat_A_Write(int rd_val);
void Chip3_Idx_Ctrl_Lat_B_Write(int rd_val);
void Chip3_Idx_Ctrl_Flag_A_Write(int rd_val);
void Chip3_Idx_Ctrl_Flag_B_Write(int rd_val);
void Chip3_Idx_Ctrl_Sta_Clk_Write(int rd_val);
void Chip3_Idx_Ctrl_Rst_N_Write(int rd_val);
void Chip3_Idx_Ctrl_Rst_Ana_Write(int rd_val);

int Chip3_Idx_Stat_Scrdy_Read();
int Chip3_Idx_Stat_Scso_A_Read();
int Chip3_Idx_Stat_Scso_B_Read();

void SWEEP_START_Write(int rd_val);
void SWEEP_SETLOW_Write(int rd_val);
void SWEEP_SETHIGH_Write(int rd_val);
void SWEEP_CNTSCLK_Write(int bit_val);
void SWEEP_LOWDATA_Write(int data_val);
void SWEEP_HIGHDATA_Write(int data_val);
int  SWEEP_RDY_Read();

/// SCPU status flag read functions
int Chip4_SCPU_Idx_Ctrl_Rdy();
int Chip4_SCPU_Idx_Nxt_End();
int Chip4_SCPU_Idx_Nxt_Cont();
int Chip4_SCPU_Idx_App_Start();
int Chip4_SCPU_SRAM_ADDR_Read();
int Chip4_SCPU_SRAM_DATA_Read();
unsigned int    Chip4_Cpu_Stat_Read();
unsigned int    Chip4_CCT_Sram_Addr_Read();
unsigned int    Chip4_CCT_Sram_Data_Read();
unsigned int    Chip4_Scan_Chain_Read();

/// SCPU control bit wrt functions
void Chip4_Idx_Scpu_Ctrl_Bgn_Write(int rd_val);
void Chip4_Idx_Scpu_Ctrl_Load_Write(int rd_val);
void Chip4_Idx_Scpu_Ctrl_Mod_Write(int rd_val);
void Chip4_Idx_Scpu_App_Done_Write(int rd_val);
void Chip4_Idx_Scpu_Cpu_Bgn_Write(int rd_val);
void Chip4_Idx_Scpu_Rst_N_Write(int rd_val);
void Chip4_Idx_Scpu_Cpu_Wait_Write(int rd_val);
void Chip4_Idx_Scpu_Test_Mux_Write(int rd_val);
void Chip4_Idx_Scpu_Clk_Stop_Write(int rd_val);
void Chip4_Idx_Scpu_Clk_Freq_Chg_Write(int rd_val);

/// SCPU parameters configuration functions
void Chip4_SCPU_CNT_SCLK_Write(int rd_val);
void Chip4_SCPU_SRAM_ADDR_Write(int rd_val);
void Chip4_SCPU_SRAM_DATA_Write(int rd_val);
void Chip4_ADC_Write(int data_val);
void Chip4_Cpu_Ctrl_Flags_Write(int data_val);

int DEBUG_Read();
float sqrt12( const float N);
//double sqrt12( unsigned long N );

#endif
