#ifndef _TEST_ADC_H_
#define _TEST_ADC_H_

#include "device.h"
#include <stdio.h>
#include <stdint.h>

#define IQ_GRP_OFFSET 2
#define OSC_TOL 18

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

/* =================== Program for Test Chip3 =========================*/
void    Chip3_Send_Cfg_To_SCA();
uint16  Chip3_Read_AdC();
IQ_ELEMENT Chip3_Cfg_Read_ADC(uint8 cal, uint8 phs, uint8 src, uint8 oscd);

/* ==================== Data Collection (Bottom-to-Up) ====================== */
uint16  ReadADC();
IQ_ELEMENT ReadCfgADC(uint8 cal, uint8 phs, uint8 src, uint8 oscd);                                     // Level 0
uint16  IQDataReadAdc(uint16 start_pos, IQ_ELEMENT* adc_buf);                                           // Level 1
uint16  IQAvgReadAdc(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 avg);                                // Level 1b
uint16  FSELIQDataRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist);               // Level 2
uint16  FSELIQAvgRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist, uint16 avg);    // Level 2b

uint16  IQDataReadAdc2(uint16 start_pos, IQ_ELEMENT* adc_buf); // Experimental

/* ==================== Non-UI Test Functions ====================== */
uint16  SweepFreqResp(uint16 stNum, uint16 endNum, uint16 step, IQ_ELEMENT* adc_buf);
uint16  SweepFreqRespAvg(uint16 stNum, uint16 endNum, uint16 step, IQ_ELEMENT* adc_buf, uint16 avg);

/* ================== Cost Function ================== */
#define MAG(a,b) sqrt((a)*(a)+(b)*(b))
uint16 Amp_Division(uint16 x, uint16 y, uint16 div_accu);
uint16 Amp_Cordic(int16_t x, int16_t y);
uint16 GetCost2D(uint16* adc_buf, uint16 offset);
uint16 GetCost2D_hw(uint16* adc_buf, uint16 offset);
uint16 GetCost4D(uint16* adc_buf, uint16 offset);
uint16 GetCost4D_hw(uint16* adc_buf, uint16 offset);
uint16 GetCFSA4D(uint16* adc_buf, uint16 offset, uint8 is4D,
                 uint16 div_accu, uint16 gain0, uint16 gain1, uint16 gain2, uint16 gain3);

#endif
