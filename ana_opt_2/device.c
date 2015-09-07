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

//#include    "hps_0.h"

#include "device.h"
#include "psoc_port.h"
//#include "scan_chain.h"

#define BIT_SET(F, B)           ((F) |= (B))
#define BIT_RESET(F, B)         ((F) &= ~(B))
#define BIT_ON(F, B)            BIT_SET((F), (1<<(B)))
#define BIT_OFF(F, B)           BIT_RESET((F), (1<<(B)))

int USBUART_1_CDCIsReady() { return 1; }
int USBUART_1_DataIsReady() { return 1; }
void USBUART_1_Start(int x, int y) { ; }
void USBUART_1_CDC_Init() { ; }


// toggle certain bit: activate the Scan chain
void CFG_STA_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_STA):BIT_OFF(local_ctrl_addr, IDX_CTRL_STA);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// change SEL bit: make scan chain transparent
void CFG_SEL_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_SEL):BIT_OFF(local_ctrl_addr, IDX_CTRL_SEL);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}/**/

// change SIN bit: input data to scan chain
void CFG_DAT_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_SIN):BIT_OFF(local_ctrl_addr, IDX_CTRL_SIN);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// toggle certain bit: activate the ADC measurment
void ADC_STA_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_MST):BIT_OFF(local_ctrl_addr, IDX_CTRL_MST);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// change SRC bit: manually change this bit since it's not on Scan chain
void CFG_SRC_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_SRC):BIT_OFF(local_ctrl_addr, IDX_CTRL_SRC);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// change CAL bit: manually change this bit since it's not on Scan chain
void CFG_CAL_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_CAL):BIT_OFF(local_ctrl_addr, IDX_CTRL_CAL);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// change PHS bit: manually change this bit since it's not on Scan chain
void CFG_PHS_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_PHS):BIT_OFF(local_ctrl_addr, IDX_CTRL_PHS);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// change FSEL bits: manually change this bit since it's not on Scan chain
void CFG_FSEL_Write(int rd_val)
{
    (rd_val & 0x2)? BIT_ON(local_ctrl_addr, IDX_CTRL_FS1):BIT_OFF(local_ctrl_addr, IDX_CTRL_FS1);
    (rd_val & 0x1)? BIT_ON(local_ctrl_addr, IDX_CTRL_FS0):BIT_OFF(local_ctrl_addr, IDX_CTRL_FS0);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// change RSTN bit: to reset all the devices
void Global_RSTN_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_RSN):BIT_OFF(local_ctrl_addr, IDX_CTRL_RSN);
    *ctrl_addr = local_ctrl_addr;
    //avs_wait();
    if (bit_val == 1)
        usleep(70);//wait 50 us
}

// change IDX_CTRL_OSC bits:
void CFG_OSCD_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_ctrl_addr, IDX_CTRL_OSC):BIT_OFF(local_ctrl_addr, IDX_CTRL_OSC);
    *ctrl_addr = local_ctrl_addr;
    avs_wait();
}

// disable all RSTN bit in the old version
//void CFG_RSTN_Write(int bit_val){;}


// read Scan chain status:
int CFG_RDY_Read()
{
    return ((*stat_addr >> IDX_STAT_SCRDY) & 1);  // Poll the ready signal
}

// read Ana bits from the middle pin:
int ANA_So_Read()
{
    return ((*stat_addr >> IDX_STAT_SOANA) & 1);  // Get the Ana bits
}

// read Multi-start Meta-heuristic bits from the end pin:
int MSMH_So_Read()
{
    return ((*stat_addr >> IDX_STAT_SODIG) & 1);  // Get the Ana bits
}

// Ready signal from ADC measurment
int ADC_RDY_Read()
{
    return ((*stat_addr >> IDX_STAT_MSRDY) & 1);  // the Ready signal of ADC
}

/////////////////// 4 SA test later  ////////////////////////
int WSA_RDY_Read()
{
    return ((*sastat_addr >> IDX_STAT_WSARDY) & 1);
}

unsigned int SA_TUNEX1_Read() {return (*tunex1_addr);}
unsigned int SA_TUNEX2_Read() {return (*tunex2_addr);}
unsigned int SA_TUNEX3_Read() {return (*tunex3_addr);}
unsigned int SA_TUNEX4_Read() {return (*tunex4_addr);}

void SA_INITX1_Write(int bit_val)
{
    //.... do something here
    *initx1_addr = bit_val;
    avs_wait();
}

void SA_INITX2_Write(int bit_val)
{
    //.... do something here
    *initx2_addr = bit_val;
    avs_wait();
}

void SA_INITX3_Write(int bit_val)
{
    //.... do something here
    *initx3_addr = bit_val;
    avs_wait();
}

void SA_INITX4_Write(int bit_val)
{
    //.... do something here
    *initx4_addr = bit_val;
    avs_wait();
}

void SA_ITERNUM_Write(int bit_val)
{
    //.... do something here
    *iternum_addr = bit_val;
    avs_wait();
}

void SA_ANAIN_Write(int bit_val)
{
    //.... do something here
    *anain_addr = bit_val;
    avs_wait();
}

void WSA_STA_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_sactrl_addr, IDX_CTRL_NXT):BIT_OFF(local_sactrl_addr, IDX_CTRL_NXT);
    *sactrl_addr = local_sactrl_addr;
    avs_wait();
}

void WSA_4DY_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_sactrl_addr, IDX_CTRL_4DY):BIT_OFF(local_sactrl_addr, IDX_CTRL_4DY);
    *sactrl_addr = local_sactrl_addr;
    avs_wait();
}

void WSA_RSN_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_sactrl_addr, IDX_CTRL_SARN):BIT_OFF(local_sactrl_addr, IDX_CTRL_SARN);
    *sactrl_addr = local_sactrl_addr;
    avs_wait();
}

///////////////////////////////////////////////
// the AMM I/O of sa_4D_logic module
void SA4D_STA_Write(int bit_val)
{
    (bit_val == 1)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DNXT):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DNXT);
    *sa4Dctrl_addr = local_sa4Dctrl_addr;
    avs_wait();
}

void SA4D_4DY_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4D4DY):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4D4DY);
    *sa4Dctrl_addr = local_sa4Dctrl_addr;
    avs_wait();
}

void SA4D_RSN_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DSARN):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DSARN);
    *sa4Dctrl_addr = local_sa4Dctrl_addr;
    avs_wait();
}

// change Mode value to switch from different algorithms
void SA4D_MOD_Write(int rd_val)
{
    (rd_val & 0x4)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DMOD2):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DMOD2);
    (rd_val & 0x2)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DMOD1):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DMOD1);
    (rd_val & 0x1)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DMOD0):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DMOD0);
    *sa4Dctrl_addr = local_sa4Dctrl_addr;
    avs_wait();
}

// change STEPT value to adjust to different T step
void SA4D_STEPT_Write(int rd_val)
{
    (rd_val & 0x4)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DSTEPT2):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DSTEPT2);
    (rd_val & 0x2)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DSTEPT1):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DSTEPT1);
    (rd_val & 0x1)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DSTEPT0):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DSTEPT0);
    *sa4Dctrl_addr = local_sa4Dctrl_addr;
    avs_wait();
}

int SA4D_RDY_Read()
{
    return ((*sa4Dstat_addr >> IDX_STAT_SA4DRDY) & 1);
}

void SA_INIT4DX1_Write(int bit_val)
{
    //.... do something here
    *init4Dx1_addr = bit_val;
    avs_wait();
}

void SA_INIT4DX2_Write(int bit_val)
{
    //.... do something here
    *init4Dx2_addr = bit_val;
    avs_wait();
}

void SA_INIT4DX3_Write(int bit_val)
{
    //.... do something here
    *init4Dx3_addr = bit_val;
    avs_wait();
}

void SA_INIT4DX4_Write(int bit_val)
{
    //.... do something here
    *init4Dx4_addr = bit_val;
    avs_wait();
}

void SA_ITER4DSA_Write(int bit_val)
{
    //.... do something here
    *iter4Dsa_addr = bit_val;
    avs_wait();
}

void SA_ITER4DSS_Write(int bit_val)
{
    //.... do something here
    *iter4Dss_addr = bit_val;
    avs_wait();
}

void SA_INITT4D_Write(int bit_val)
{
    //.... do something here
    *initT4D_addr = bit_val;
    avs_wait();
}

void SA_ANAIN4D_Write(int bit_val)
{
    //.... do something here
    *anain4D_addr = bit_val;
    avs_wait();
}

unsigned int SA_TUNE4DX1_Read() {return (*tune4Dx1_addr);}
unsigned int SA_TUNE4DX2_Read() {return (*tune4Dx2_addr);}
unsigned int SA_TUNE4DX3_Read() {return (*tune4Dx3_addr);}
unsigned int SA_TUNE4DX4_Read() {return (*tune4Dx4_addr);}


///////////////////////////////////////////////
// the AMM I/O of cfsa_4D_logic module
//void SA4D_STA_Write(int bit_val)
//{
//    (bit_val == 1)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DNXT):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DNXT);
//    *sa4Dctrl_addr = local_sa4Dctrl_addr;
//    avs_wait();
//}

void CFSA4D_SARAND_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CFSA_SARAND):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CFSA_SARAND);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

void CFSA4D_CFAVRG_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CFSA_CFAVRG):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CFSA_CFAVRG);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

void CFSA4D_4DY_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_4D4DY):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_4D4DY);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

void CFSA4D_RSN_Write(int bit_val)
{
    //.... do something here
    (bit_val == 1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_4DSARN):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_4DSARN);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// change Mode value to switch from different algorithms
void CFSA4D_MOD_Write(int rd_val)
{
//    (rd_val & 0x4)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DMOD2):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DMOD2);
//    (rd_val & 0x2)? BIT_ON(local_sa4Dctrl_addr, IDX_CTRL_4DMOD1):BIT_OFF(local_sa4Dctrl_addr, IDX_CTRL_4DMOD1);
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_4DMOD0):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_4DMOD0);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// change STEPT value to adjust to different T step
void CFSA4D_STEPT_Write(int rd_val)
{
    (rd_val & 0x4)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_4DSTEPT2):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_4DSTEPT2);
    (rd_val & 0x2)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_4DSTEPT1):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_4DSTEPT1);
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_4DSTEPT0):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_4DSTEPT0);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// change qualifying accuracy of the division
void CFSA4D_DIVACCU_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_ACCU):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_ACCU);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// change the gain coefficient of Freq0
void CFSA4D_GAIN0_Write(int rd_val)
{
    (rd_val & 0x2)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN0_1):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN0_1);
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN0_0):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN0_0);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// change the gain coefficient of Freq1
void CFSA4D_GAIN1_Write(int rd_val)
{
    (rd_val & 0x2)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN1_1):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN1_1);
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN1_0):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN1_0);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// change the gain coefficient of Freq0
void CFSA4D_GAIN2_Write(int rd_val)
{
    (rd_val & 0x2)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN2_1):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN2_1);
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN2_0):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN2_0);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// change the gain coefficient of Freq0
void CFSA4D_GAIN3_Write(int rd_val)
{
    (rd_val & 0x2)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN3_1):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN3_1);
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN3_0):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_GAIN3_0);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// set the NXT signal
void CFSA4D_MUXNXT_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_MUXNXT):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_MUXNXT);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// set the Tune_ready signal
void CFSA4D_MUXRDY_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_MUXRDY):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_MUXRDY);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

// set the Ctrl_logic_sta signal
void CFSA4D_MUXTRG_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CTRL_MUXTRG):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CTRL_MUXTRG);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

void CFSA4D_ADCRDY_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_cfsa4Dctrl_addr, IDX_CFSA_ADCRDY):BIT_OFF(local_cfsa4Dctrl_addr, IDX_CFSA_ADCRDY);
    *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;
    avs_wait();
}

//int CFSA4D_RDY_Read()
//{
//    return ((*cfsa4Dstat_addr >> IDX_STAT_SA4DRDY) & 1);
//}

void CFSA_INIT4DX1_Write(int bit_val)
{
    //.... do something here
    *cfsainit4Dx1_addr = bit_val;
    avs_wait();
}

void CFSA_INIT4DX2_Write(int bit_val)
{
    //.... do something here
    *cfsainit4Dx2_addr = bit_val;
    avs_wait();
}

void CFSA_INIT4DX3_Write(int bit_val)
{
    //.... do something here
    *cfsainit4Dx3_addr = bit_val;
    avs_wait();
}

void CFSA_INIT4DX4_Write(int bit_val)
{
    //.... do something here
    *cfsainit4Dx4_addr = bit_val;
    avs_wait();
}

void CFSA_ITER4DSA_Write(int bit_val)
{
    //.... do something here
    *cfsaiter4Dsa_addr = bit_val;
    avs_wait();
}

void CFSA_ITER4DSS_Write(int bit_val)
{
    //.... do something here
    *cfsaiter4Dss_addr = bit_val;
    avs_wait();
}

void CFSA_INITT4D_Write(int bit_val)
{
    //.... do something here
    *cfsainitT4D_addr = bit_val;
    avs_wait();
}

void CFSA_TOLIN4D_Write(int bit_val)
{
    //.... do something here
    *cfsatolin4D_addr = bit_val;
    avs_wait();
}

void CFSA_OFFIN4D_Write(int bit_val)
{
    //.... do something here
    *cfsaoffin4D_addr = bit_val;
    avs_wait();
}

void CFSA_3DBFACTOR0_Write(int bit_val)
{
    *cfsa3dbfactor0_addr = bit_val;
    avs_wait();
}

void CFSA_3DBFACTOR1_Write(int bit_val)
{
    *cfsa3dbfactor1_addr = bit_val;
    avs_wait();
}

void CFSA_ADCIN4D_Write(int bit_val)
{
    //.... do something here
    *cfsaadcin4D_addr = bit_val;
    avs_wait();
}

void CFSA_THRESHOLD_Write(int bit_val)
{
    //.... do something here
    *cfsathreshold_addr = bit_val;
    avs_wait();
}

void CFSA4D_ANABITS_Write(int bit_val)
{
    *cfsaAnaIn_addr = bit_val;
    avs_wait();
}

unsigned int CFSA_TUNE4DX1_Read() {return (*cfsatune4Dx1_addr);}
unsigned int CFSA_TUNE4DX2_Read() {return (*cfsatune4Dx2_addr);}
unsigned int CFSA_TUNE4DX3_Read() {return (*cfsatune4Dx3_addr);}
unsigned int CFSA_TUNE4DX4_Read() {return (*cfsatune4Dx4_addr);}
unsigned int CFSA_ANABITS_Read()  {return (*cfsaAnaOut_addr);}

/////// tapeout chip 3 //////
void CHIP_TEST_LATA_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_LAT_A):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_LAT_A);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

void CHIP_TEST_LATB_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_LAT_B):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_LAT_B);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

void CHIP_TEST_SELA_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_SEL_A):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_SEL_A);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

void CHIP_TEST_SELB_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_SEL_B):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_SEL_B);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

void CHIP_TEST_FLAGA_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_FLAG_A):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_FLAG_A);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

void CHIP_TEST_FLAGB_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_FLAG_B):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_FLAG_B);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

/// the clock for scan chain, also need FLAGA or FLAGB
void CHIP_TEST_STA_SCCLK_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_STA_SC):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_STA_SC);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

/// the clock for global system
void CHIP_TEST_STA_CLK_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_STA_CLK):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_STA_CLK);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

/// only generate one global clock for debug
void CHIP_TEST_ONE_CLK_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_ONE_CLK):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_ONE_CLK);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

void CHIP_TEST_SINAB_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_SIN_AB):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_SIN_AB);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

void CHIP_TEST_RSTN_Write(int rd_val)
{
    (rd_val & 0x1)? BIT_ON(local_chiptestctrl_addr, IDX_CTRL_RST_N):BIT_OFF(local_chiptestctrl_addr, IDX_CTRL_RST_N);
    *chiptest_ctrl_addr = local_chiptestctrl_addr;
    avs_wait();
}

int CHIP_TEST_SCRDY_Read() {return ((*chiptest_stat_addr >> IDX_STAT_SCREADY) & 1) ;}
int CHIP_TEST_SO_A_Read()  {return ((*chiptest_stat_addr >> IDX_STAT_SCSO_A) & 1) ;}
int CFSA_TEST_SO_B_Read()  {return ((*chiptest_stat_addr >> IDX_STAT_SCSO_B) & 1) ;}
int CFSA_TEST_ONERDY()     {return ((*chiptest_stat_addr >> IDX_STAT_ONERDY) & 1) ;}

// used for debugging
int DEBUG_Read() { return 1;}


// user defined square root
float sqrt12(const float m)
{
   float i=0;
   int j = 0;
   float x1,x2;
   while( (i*i) <= m )
          i+=0.1f;
   x1=i;
   for(j=0;j<10;j++)
   {
       x2=m;
      x2/=x1;
      x2+=x1;
      x2/=2;
      x1=x2;
   }
   return x2;
}

/*
double sqrt12( unsigned long N )
{
    double n, p, low, high;
    if( 2 > N )
        return( N );
    low  = 0;
    high = N;
    while( high > low + 1 )
    {
        n = (high + low) / 2;
        p = n * n;
        if( N < p )
            high = n;
        else if( N > p )
            low = n;
        else
            break;
    }
    return( N == p ? n : low );
}*/


