/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * Author: Jiafan Wang
 * Date:   03/08/2015
 * ========================================
*/

#include "device.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "set_config.h"
#include "test_adc.h"
//#define DEBUG_ON

/* =================== Program for Test Chip3 =========================*/
/// Send Configuration to Scan chain A
void    Chip3_Send_Cfg_To_SCA()
{
    Chip3_Idx_Ctrl_Sel_A_Write(0);

    Chip3_Idx_Ctrl_Lat_A_Write(0);
    Chip3_Idx_Ctrl_Sta_Sc_Write(1);

    ///////////////////// Scan chain input /////////////////////
    //// input Scan chain A
    int i;
    for (i = 0; i < MAX_SC_BITS_A; i++)
    {
        Chip3_Idx_Ctrl_Sin_Ab_Write(gcfg[0]);
        RShiftCfg();
        Chip3_Idx_Ctrl_Flag_A_Write(1);
        while(1 != Chip3_Idx_Stat_Scrdy_Read());

        Chip3_Idx_Ctrl_Flag_A_Write(0);
        while(0 != Chip3_Idx_Stat_Scrdy_Read());
    }

    /// when set the latch signal, no need to send clock
    Chip3_Idx_Ctrl_Lat_A_Write(1);
    usleep(2);
    Chip3_Idx_Ctrl_Lat_A_Write(0);

    Chip3_Idx_Ctrl_Sta_Sc_Write(0);

    return;
}

/// After invoke the Control Logic, wait 500us and then Read ADC
uint16  Chip3_Read_AdC()
{
    uint16  rd_val = 0;

    Chip3_Idx_Ctrl_Sel_B_Write(1);

    Chip3_Idx_Ctrl_Sta_Clk_Write(1);
    // Waiting ADC ready by usleep since no signal to indicate when is ready
    usleep(15);
    Chip3_Idx_Ctrl_Sta_Clk_Write(0);

    Chip3_Idx_Ctrl_Lat_B_Write(1);
    usleep(1);
    Chip3_Idx_Ctrl_Lat_B_Write(0);

    Chip3_Idx_Ctrl_Sta_Sc_Write(1);

    Chip3_Idx_Ctrl_Flag_B_Write(1);
    while(1 != Chip3_Idx_Stat_Scrdy_Read());

    Chip3_Idx_Ctrl_Flag_B_Write(0);
    while(0 != Chip3_Idx_Stat_Scrdy_Read());

    Chip3_Clear_Scan_Chain(0);
    BIT_SET(gscB, ((Chip3_Idx_Stat_Scso_B_Read())<< (MAX_SC_BITS_B-1)));
    #ifdef  DEBUG_ON
    printf("SCB BIT = %d\n", Chip3_Idx_Stat_Scso_B_Read());
    #endif

    Chip3_Idx_Ctrl_Sel_B_Write(0);

    int i;
    for (i = 0; i< MAX_SC_BITS_B-1; i++)
    {
        gscB = (gscB >> 1);
        Chip3_Idx_Ctrl_Flag_B_Write(1);
        while(1 != Chip3_Idx_Stat_Scrdy_Read());

        Chip3_Idx_Ctrl_Flag_B_Write(0);
        while(0 != Chip3_Idx_Stat_Scrdy_Read());

        BIT_SET(gscB, ((Chip3_Idx_Stat_Scso_B_Read())<< (MAX_SC_BITS_B-1)));
        #ifdef DEBUG_ON
        printf("SCB BIT = %d\n", Chip3_Idx_Stat_Scso_B_Read());
        #endif
    }

    Chip3_Idx_Ctrl_Sta_Sc_Write(0);

    rd_val = Chip3_Rtn_Adc();
#ifdef DEBUG_ON
    printf("ADC = %d\n", rd_val);
    printf("FNL = %d\n", (CHIP3_CHK_ADC_FNL?1:0));
    printf("RSTN_ADC = %d\n", (CHIP3_CHK_RSTN_ADC?1:0));
    printf("CLRN = %d\n", (CHIP3_CHK_CLRN?1:0));
#endif
    return  rd_val;
}

/// First configue the control bits on the Scan chain, then Read ADC
IQ_ELEMENT Chip3_Cfg_Read_ADC(uint8 cal, uint8 phs, uint8 src, uint8 oscd)
{
    //LoadCfg(); //since it's rotation now

    Chip3_Set_Cal(cal);
    Chip3_Set_Phs(phs);
    Chip3_Set_Src(src);
    Chip3_Set_Oscd(oscd);

    //////////////////////////////////////////////
    /// Activate the Scan chain and load Configuration
    ///////////////////////////////////////////////
    //Chip3_Idx_Ctrl_Rst_Ana_Write(0);/// RST_ANA is needed only when MDIV is changed
    Chip3_Set_Trg_Test(0);
    Chip3_Send_Cfg_To_SCA();
    //Chip3_Idx_Ctrl_Rst_Ana_Write(1);

    Chip3_Set_Trg_Test(1);/// no need to Rst Ana since MDIV is not changed
    //BackupCfg();
    Chip3_Send_Cfg_To_SCA();

    ///////////////// Read ADC signals //////////////////
    return  Chip3_Read_AdC();
}

/* ==================== Read Result from ADC ====================== */
uint16  ReadADC()
{
    return Chip3_Read_AdC();
}

/* ==================== Data Collection (Bottom-to-Up) ====================== */

// Level 0: Configure and Read ADC once
IQ_ELEMENT ReadCfgADC(uint8 cal, uint8 phs, uint8 src, uint8 oscd)
{
    return Chip3_Cfg_Read_ADC(cal, phs, src, oscd);
}

// Test Case 3: load configuration and then read IQ sampling data
// Level 1: IQ Data
uint16  IQDataReadAdc(uint16 start_pos, IQ_ELEMENT* adc_buf)
{
    uint16  i = start_pos;;

    adc_buf[i]   += ReadCfgADC(0,0,0,0);
    adc_buf[i+1] += ReadCfgADC(0,1,0,0);
    adc_buf[i+2] += ReadCfgADC(0,0,1,0);
    adc_buf[i+3] += ReadCfgADC(0,1,1,0);
    //adc_buf[i+4] += ReadCfgADC(0,0,0,1);

    return sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT);
}

uint16  IQDataReadAdc2(uint16 start_pos, IQ_ELEMENT* adc_buf)
{
    uint16  i = start_pos;;
    uint16  rd_val;

    rd_val = ReadCfgADC(0,0,0,0);
    if (abs(rd_val) > abs(adc_buf[i]/AVG)) adc_buf[i] = rd_val * AVG;

    rd_val = ReadCfgADC(0,1,0,0);
    if (abs(rd_val) > abs(adc_buf[i+1]/AVG)) adc_buf[i+1] = rd_val * AVG;

    rd_val = ReadCfgADC(0,0,1,0);
    if (abs(rd_val) > abs(adc_buf[i+2]/AVG)) adc_buf[i+2] = rd_val * AVG;

    rd_val = ReadCfgADC(0,1,1,0);
    if (abs(rd_val) > abs(adc_buf[i+3]/AVG)) adc_buf[i+3] = rd_val * AVG;

    return sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT);
}

// Test Case 4: Repeatly read IQ sampling data with one configuration and then do average
// Level 1b: IQ Data Average
uint16  IQAvgReadAdc(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 avg)
{
    uint16  i = 0;

    for (i = 0; i < avg; i++)
    {
        IQDataReadAdc(start_pos, adc_buf);
    }

    return sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT);
}

// Test Case 5: Switch FSEL from 1~3 and then do case 3 for IQ data
// Level 2: IQ Data, Sweep Frequency
uint16  FSELIQDataRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist)
{
    uint16  f = 0;
    uint16  current_pos = 0;
    uint16  step_pos = 0;

    current_pos = start_pos;

    // Oscillation detection
    SetFreq(flist[0]);
    adc_buf[current_pos] += ReadCfgADC(0,0,0,1);
    adc_buf[current_pos+1] += ReadCfgADC(0,1,0,1);
    current_pos += 2;

    // IQ Measurement
    for (f = 0; f < fno; f++)
    {
        SetFreq(flist[f]);
        step_pos = IQDataReadAdc(current_pos, adc_buf);
        current_pos += step_pos;
    }

    return current_pos - start_pos;
}


// Test Case 6: Switch FSEL from 1~3 and then do case 4 for IQ data
// Level 2b: IQ Data Average, Sweep Frequency
uint16  FSELIQAvgRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist, uint16 avg)
{
    uint16  f = 0;
    uint16  current_pos = 0;
    uint16  step_pos = 0;

    current_pos = start_pos;

    // Oscillation detection
    SetFreq(flist[0]);
    for (f = 0; f < avg; f++)
    {
        adc_buf[current_pos] += ReadCfgADC(0,0,0,1);
        adc_buf[current_pos+1] += ReadCfgADC(0,1,0,1);
    }
    current_pos += 2;

    // IQ measurement
    for (f = 0; f < fno; f++)
    {
        SetFreq(flist[f]);
        step_pos = IQAvgReadAdc(current_pos, adc_buf, avg);
        current_pos += step_pos;
    }

    return current_pos - start_pos;
}

/* ==================== Non-UI Test Functions ====================== */
// Sweep frequency and measure the frequency response (No average)
uint16 SweepFreqResp(uint16 stNum, uint16 endNum, uint16 step, IQ_ELEMENT* adc_buf)
{
    uint16 i;
    uint16 idx = 0;
    uint16 stp = 0;

    LoadCfg();
    for (i = stNum; i <= endNum; i += step)
    {
        SetFreq(i);
        stp = IQDataReadAdc(idx, adc_buf);
        idx += stp;
    }

    return (((endNum-stNum)/step+1)*(sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT)));
}

// Sweep frequency and measure the frequency response (Averaged)
uint16 SweepFreqRespAvg(uint16 stNum, uint16 endNum, uint16 step, IQ_ELEMENT* adc_buf, uint16 avg)
{
    uint16 i;
    uint16 idx = 0;
    uint16 stp = 0;

    LoadCfg();
    for (i = stNum; i <= endNum; i += step)
    {
        SetFreq(i);
        Chip3_Send_Cfg_To_SCA();
        usleep(70);

        stp = IQAvgReadAdc(idx, adc_buf, avg);
        idx += stp;
    }

    return (((endNum-stNum)/step+1)*(sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT)));
}

/* ==================== Cost Functions ====================== */
uint16 GetCost2D(uint16* adc_buf, uint16 offset)
{
    FSEL_ELEMENT *A1, *A2, *A3;

    uint16 Is, Qs;  // Vars for Oscillation Detection
    Is = adc_buf[0];
    Qs = adc_buf[1];

    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

    //printf("S|%d|%d|%d|%d\t", Is, A1->Iout, Qs, A1->Qout);
    if (abs(Is - A1->Iout)+abs(Qs - A1->Qout) > OSC_TOL)
    {
        return 255; // Oscillation Detection
    } else
    {
        float F1 = 128*MAG((float)(A1->Iout-offset), (float)(A1->Qout-offset))/
                      MAG((float)(A1->Iinn-offset), (float)(A1->Qinn-offset));
        float F2 = 128*MAG((float)(A2->Iout-offset), (float)(A2->Qout-offset))/
                      MAG((float)(A2->Iinn-offset), (float)(A2->Qinn-offset));
        float F3 = 128*MAG((float)(A3->Iout-offset), (float)(A3->Qout-offset))/
                      MAG((float)(A3->Iinn-offset), (float)(A3->Qinn-offset));

        float cost = abs(F2-1.414*F1) + abs(F2-1.414*F3) + abs(F1-F3);

        if (F2<128) cost += (128-F2);

        if (cost>255) cost = 255;

        return (uint16)cost;
    }
}

uint16 GetCost4D(uint16* adc_buf, uint16 offset)
{
    FSEL_ELEMENT *A0, *A1, *A2, *A3;

    uint16 Is, Qs;  // Vars for Oscillation Detection
    Is = adc_buf[0];
    Qs = adc_buf[1];

    A0 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 3*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

    //printf("S|%d|%d|%d|%d\t", Is, A1->Iout, Qs, A1->Qout);
    if (abs(Is - A0->Iout)+abs(Qs - A0->Qout) > OSC_TOL)
    {
        return 255; // Oscillation Detection
    } else
    {
        float F0 = 128*MAG((float)(A0->Iout-offset), (float)(A0->Qout-offset))/
                      MAG((float)(A0->Iinn-offset), (float)(A0->Qinn-offset));
        float F1 = 128*MAG((float)(A1->Iout-offset), (float)(A1->Qout-offset))/
                      MAG((float)(A1->Iinn-offset), (float)(A1->Qinn-offset));
        float F2 = 128*MAG((float)(A2->Iout-offset), (float)(A2->Qout-offset))/
                      MAG((float)(A2->Iinn-offset), (float)(A2->Qinn-offset));
        float F3 = 128*MAG((float)(A3->Iout-offset), (float)(A3->Qout-offset))/
                      MAG((float)(A3->Iinn-offset), (float)(A3->Qinn-offset));

        //float cost = abs(F1-2*F0) + abs(F2-2*F3) + abs(F0-F3) + abs(F1-F2);
        float cost = abs(F1-1.414*F0) + abs(F2-1.414*F3) + (abs(F0-F3)>>1) + (abs(F1-F2)>>1);

        if (F2<128) cost += (128-F2);

        if (cost>255) cost = 255;

        return (uint16)cost;
    }
}

uint16 GetCFSA4D(uint16* adc_buf, uint16 offset, uint8 is_4D, uint16 div_accu,
                 uint16 gain0, uint16 gain1, uint16 gain2, uint16 gain3)
{
#define     FREQ_FACTOR(y,x)  ((x==0)?(y):(x==2)?((y)<<1):(x==1)?((y)>>1):0)

    FSEL_ELEMENT *A0, *A1, *A2, *A3;
    uint32  F0_tmp, F3_tmp;
    uint16  f2_compen;

    uint16 Is, Qs;  // Vars for Oscillation Detection
    Is = adc_buf[0];
    Qs = adc_buf[1];

    A0 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 3*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

    //printf("S|%d|%d|%d|%d\t", Is, A1->Iout, Qs, A1->Qout);
    if (abs(Is - A0->Iout)+abs(Qs - A0->Qout) > OSC_TOL)
    {
        return 255; // Oscillation Detection
    } else
    {
        //            Aout = Amp_Cordic(adc_buf[4], adc_buf[3]) << 7;
        //    Ainn = Amp_Cordic(adc_buf[2], adc_buf[1]);
//        uint32  F0 = (Amp_Cordic(A0->Iout - offset, A0->Qout - offset) << 7)/
//                     (Amp_Cordic(A0->Iinn - offset, A0->Qinn - offset));
//        uint32  F1 = (Amp_Cordic(A1->Iout - offset, A1->Qout - offset) << 7)/
//                     (Amp_Cordic(A1->Iinn - offset, A1->Qinn - offset));
//        uint32  F2 = (Amp_Cordic(A2->Iout - offset, A2->Qout - offset) << 7)/
//                     (Amp_Cordic(A2->Iinn - offset, A2->Qinn - offset));
//        uint32  F3 = (Amp_Cordic(A3->Iout - offset, A3->Qout - offset) << 7)/
//                     (Amp_Cordic(A3->Iinn - offset, A3->Qinn - offset));
//
//        if (F0 >= 0x1FF)    F0 = 0x1FF;
//        if (F1 >= 0x1FF)    F1 = 0x1FF;
//        if (F2 >= 0x1FF)    F2 = 0x1FF;
//        if (F3 >= 0x1FF)    F3 = 0x1FF;

        f2_compen = (div_accu)? 128 : 64;

        uint32  F0 = Amp_Division(Amp_Cordic(A0->Iout - offset, A0->Qout - offset),
                                  Amp_Cordic(A0->Iinn - offset, A0->Qinn - offset), div_accu);
        uint32  F1 = Amp_Division(Amp_Cordic(A1->Iout - offset, A1->Qout - offset),
                                  Amp_Cordic(A1->Iinn - offset, A1->Qinn - offset), div_accu);
        uint32  F2 = Amp_Division(Amp_Cordic(A2->Iout - offset, A2->Qout - offset),
                                  Amp_Cordic(A2->Iinn - offset, A2->Qinn - offset), div_accu);
        uint32  F3 = Amp_Division(Amp_Cordic(A3->Iout - offset, A3->Qout - offset),
                                  Amp_Cordic(A3->Iinn - offset, A3->Qinn - offset), div_accu);

        if (!is_4D)
        {
            // 1.414*F0
            F0_tmp = F0;
            F0_tmp = F0 + (F0_tmp >> 2);
            F0_tmp = F0 + (F0_tmp >> 2);
            F0_tmp = F0 + (F0_tmp >> 1);
            F0_tmp = F0 + (F0_tmp >> 2);
            // 1.414*F3
            F3_tmp = F3;
            F3_tmp = F3 + (F3_tmp >> 2);
            F3_tmp = F3 + (F3_tmp >> 2);
            F3_tmp = F3 + (F3_tmp >> 1);
            F3_tmp = F3 + (F3_tmp >> 2);
        }
        else
        {
            // F0 * 2
            F0_tmp = F0 << 1;
            F3_tmp = F3 << 1;
        }

        //uint32  cost = abs(F1-F0_tmp) + abs(F2-F3_tmp) + abs(F0-F3) + abs(F1-F2);
        uint32  cost = 0;
        cost += FREQ_FACTOR(abs(F1-F0_tmp),gain0);
        cost += FREQ_FACTOR(abs(F2-F3_tmp),gain1);
        cost += FREQ_FACTOR(abs(F0-F3),gain2);
        cost += FREQ_FACTOR(abs(F1-F2),gain3);

        //if (F2<128) cost += (128-F2);
        if (F2<f2_compen) cost += (f2_compen-F2);

        if (cost>255) cost = 255;

        return (uint16)cost;
    }
}

#if 0
uint16 GetCost4D(uint16* adc_buf, uint16 offset)
{
    FSEL_ELEMENT *A1, *A2, *A3;

    uint16 Is, Qs;  // Vars for Oscillation Detection
    Is = adc_buf[0];
    Qs = adc_buf[1];

    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

    //printf("S|%d|%d|%d|%d\t", Is, A1->Iout, Qs, A1->Qout);
    if (abs(Is - A1->Iout)+abs(Qs - A1->Qout) > OSC_TOL)
    {
        return 255; // Oscillation Detection
    } else
    {
        float F1 = 64*MAG((float)(A1->Iout-offset), (float)(A1->Qout-offset))/
                      MAG((float)(A1->Iinn-offset), (float)(A1->Qinn-offset));
        float F2 = 64*MAG((float)(A2->Iout-offset), (float)(A2->Qout-offset))/
                      MAG((float)(A2->Iinn-offset), (float)(A2->Qinn-offset));
        float F3 = 64*MAG((float)(A3->Iout-offset), (float)(A3->Qout-offset))/
                      MAG((float)(A3->Iinn-offset), (float)(A3->Qinn-offset));

        float cost = abs(F2-2*F1) + abs(F2-2*F3) + abs(F1-F3);

        if (F2<64) cost += (64-F2);

        if (cost>255) cost = 255;

        return (uint16)cost;
    }
}
#endif

// Cost function emulating hardware implementation
uint16 Amp_Division(uint16 x, uint16 y, uint16 div_accu)
{
#define DIVIDEND_WIDTH  12
#define QUOTIENT_WIDTH  9

    uint16  divident;
    uint16  dividor;
    uint32  divident_tmp;
    uint32  dividor_tmp;
    int16_t  i;

    divident = x;
    dividor = y;
    divident_tmp = (div_accu)? (divident<<7):(divident<<6);
    dividor_tmp = dividor << QUOTIENT_WIDTH;

    if ((divident >> 2) >= dividor)
    {
        // 511 = 9'b11_1111111; 256 = 9'b100_000000
        return (div_accu)? 511: 256;
    }
    else
    {
        for (i = QUOTIENT_WIDTH; i > 0; i--)
        {
            divident_tmp = (divident_tmp << 1);// left shift 1 bit
            divident_tmp = (divident_tmp & 0x1FFFFF);// remove the leftmost bit

            if (divident_tmp >= dividor_tmp)
            {
                divident_tmp = divident_tmp - dividor_tmp + 1;
            }
        }
        return  (divident_tmp & 0x1FF);
    }
}

// Cost function emulating hardware implementation
uint16 Amp_Cordic(int16_t x, int16_t y)
{
    int16_t  i, sig;
    int16_t  xn, yn, zn;

    /* BEGIN OF VERILOG */
    xn = abs(x);
    yn = abs(y);

    if (xn<yn) {zn = xn; xn = yn; yn = zn;}

    xn = (xn << 4);
    yn = (yn << 4);

    for (i=1; i<=8; i++)
    {
        sig = ((xn>0 && yn>0)||
               (xn<0 && yn<0)) ?
               -1 : 1;
        zn = xn;
        xn -= sig * (yn>>i);
        yn += sig * (zn>>i);
    }
    /* END OF VERILOG */

    return (uint16)(xn>>4);
    //return (uint16)(xn);
}

uint16 GetCost2D_hw(uint16* adc_buf, uint16 offset)
{
    FSEL_ELEMENT *A1, *A2, *A3;

    uint16 Is, Qs;  // Vars for Oscillation Detection
    Is = adc_buf[0];
    Qs = adc_buf[1];

    // Data Preparation
    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

    uint32 F1, F2, F3, F4, F5, F6;
    uint16 cost;

    /* BEGIN OF VERILOG */
    if (abs(Is - A1->Iout)+abs(Qs - A1->Qout) > OSC_TOL)
    {
        return 255; // Oscillation Detection
    } else
    {
        F1 = Amp_Cordic(A1->Iout-offset, A1->Qout-offset); F1 = F1<<7;
        F2 = Amp_Cordic(A2->Iout-offset, A2->Qout-offset); F2 = F2<<7;
        F3 = Amp_Cordic(A3->Iout-offset, A3->Qout-offset); F3 = F3<<7;

        F4 = Amp_Cordic(A1->Iinn-offset, A1->Qinn-offset); F1 = F1/F4;
        F5 = Amp_Cordic(A2->Iinn-offset, A2->Qinn-offset); F2 = F2/F5;
        F6 = Amp_Cordic(A3->Iinn-offset, A3->Qinn-offset); F3 = F3/F6;

        printf("%d\t%d\t%d\n", F1, F2, F3);

        cost = abs(F2-(F1+(F1>>2)+(F1>>3)+(F1>>5)));
        cost += abs(F2-(F3+(F3>>2)+(F3>>3)+(F3>>5)));
        cost += abs(F1-F3);
        if (F2 < (1<<7)) cost += ((1<<7) - F2);
        if (cost>255) cost = 255;

        return (uint16)cost;
    }
    /* END OF VERILOG */
}

uint16 GetCost4D_hw(uint16* adc_buf, uint16 offset)
{
    FSEL_ELEMENT *A1, *A2, *A3;

    uint16 Is, Qs;  // Vars for Oscillation Detection
    Is = adc_buf[0];
    Qs = adc_buf[1];

    // Data Preparation
    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

    uint32 F1, F2, F3, F4, F5, F6;
    uint16 cost;

    /* BEGIN OF VERILOG */
    if (abs(Is - A1->Iout)+abs(Qs - A1->Qout) > OSC_TOL)
    {
        return 255; // Oscillation Detection
    } else
    {
        F1 = Amp_Cordic(A1->Iout-offset, A1->Qout-offset); F1 = F1<<6;
        F2 = Amp_Cordic(A2->Iout-offset, A2->Qout-offset); F2 = F2<<6;
        F3 = Amp_Cordic(A3->Iout-offset, A3->Qout-offset); F3 = F3<<6;

        F4 = Amp_Cordic(A1->Iinn-offset, A1->Qinn-offset); F1 = F1/F4;
        F5 = Amp_Cordic(A2->Iinn-offset, A2->Qinn-offset); F2 = F2/F5;
        F6 = Amp_Cordic(A3->Iinn-offset, A3->Qinn-offset); F3 = F3/F6;

        cost = abs(F2-(F1<<1));
        cost += abs(F2-(F3<<1));
        cost += abs(F1-F3);
        if (F2 < (1<<6)) cost += ((1<<6) - F2);
        if (cost>255) cost = 255;

        return (uint16)cost;
    }
    /* END OF VERILOG */
}

/* [] END OF FILE */

