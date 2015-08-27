#ifndef _TEST_ADC_H_
#define _TEST_ADC_H_

#include "device.h"
#include <stdio.h>
#include <stdint.h>
#include "scan_chain.h"

#define IQ_GRP_OFFSET 2
#define OSC_TOL 16

uint16  ReadADC();
void    TxCfg2ADC();//uint8 Snd_num

/* ==================== Data Collection (Bottom-to-Up) ====================== */
IQ_ELEMENT ReadCfgADC(uint8 cal, uint8 phs, uint8 src, uint8 oscd);                                     // Level 0
uint16  IQDataReadAdc(uint16 start_pos, IQ_ELEMENT* adc_buf);                                           // Level 1
uint16  IQAvgReadAdc(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 avg);                                // Level 1b
uint16  FSELIQDataRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist);               // Level 2
uint16  FSELIQAvgRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist, uint16 avg);    // Level 2b
uint16  AllXYIQDataRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist);              // Level 3
uint16  AllXYIQAvgRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist, uint16 avg);   // Level 3b

uint16  SimpleReadAdc(uint16 read_len, IQ_ELEMENT* adc_buf);
uint16  CfgAndReadAdc(uint16 read_len, IQ_ELEMENT* adc_buf);

/* ==================== UI-related Functions ====================== */
void    JudgePrintADC(uint16 read_len, uint16 *adc_buf);
void    PrintADC(uint16 read_len, uint16 *adc_buf);

/* ==================== Non-UI Test Functions ====================== */
uint16  SweepFreqResp(uint16 stNum, uint16 endNum, uint16 step, IQ_ELEMENT* adc_buf);
uint16  SweepFreqRespAvg(uint16 stNum, uint16 endNum, uint16 step, IQ_ELEMENT* adc_buf, uint16 avg);
uint16  GetADCOffset(uint16 avg);

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
