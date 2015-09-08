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
#include "scan_chain.h"
#include "serial_port_io.h"
#include "set_config.h"
#include "test_adc.h"

/* ==================== Tx Configuration to FPGA ====================== */
void    TxCfg2ADC()
{
    uint8   i = 0;
    uint8   Snd_num = MAX_SC_BITS;

    TranxCfg();//update the transmission data, very important!!

    //manually config these signals
    CFG_FSEL_Write(RtnFSEL());
    CFG_PHS_Write(RtnCK_PHS());
    CFG_SRC_Write(RtnCK_SRC());
    CFG_CAL_Write(RtnCK_CAL());
    CFG_OSCD_Write(RtnCK_OSCD());

    Global_RSTN_Write(0);

    CFG_SEL_Write(0);

    CFG_STA_Write(0);

	for (i=0; i<Snd_num; i++)//47
	{
        CFG_DAT_Write(gcfg_tx[CFG_L] & 0x1);
        RShiftCfg();

		CFG_STA_Write(1);
		while (!CFG_RDY_Read());

		CFG_STA_Write(0);
	}

    //usleep(50);
	Global_RSTN_Write(1);
}

/* ==================== Read Result from ADC ====================== */
uint16  ReadADC()
{
    uint16  rd_val = 0;
    uint8   i = 0;

	ADC_STA_Write(0);
	while (ADC_RDY_Read()){;}

    ADC_STA_Write(1);
    while (!ADC_RDY_Read());

    ADC_STA_Write(0);
	while (ADC_RDY_Read()){;}

    CFG_STA_Write(0);
	while (CFG_RDY_Read()){;}

    CFG_SEL_Write(1);//make scan chain transparent

    CFG_STA_Write(1);
    while (!CFG_RDY_Read());

    BIT_SET(rd_val, ANA_So_Read());

    CFG_STA_Write(0);
    while (CFG_RDY_Read());

    CFG_SEL_Write(0);

    // The 2~9th Period, SEL should be 0
    for (i=1; i<=9; i++)//47
    {
        // Create SCK1 and SCK2 clock signal, one time;
    	CFG_STA_Write(1);
    	while (!CFG_RDY_Read()){;}

        rd_val = rd_val << 1;
        BIT_SET(rd_val, ANA_So_Read());

        CFG_STA_Write(0);
		while (CFG_RDY_Read()){;}
    }

    //CFG_STA_Write(1);
    //while (!CFG_RDY_Read()){;}
    //PutStr("#DEB\tADC 10th bit = ");FmtPrint(ANA_So_Read());PutStr("\r\n");

    //CFG_CKADC_Write(0);

    //PutStr("(b), ");FmtPrint(rd_val);PutStr("(d)\r\n");
    return  rd_val;
}

/* ==================== Data Collection (Bottom-to-Up) ====================== */

// Level 0: Configure and Read ADC once
IQ_ELEMENT ReadCfgADC(uint8 cal, uint8 phs, uint8 src, uint8 oscd)
{
    SetCK_CAL(cal);
    SetCK_PHS(phs);
    SetCK_SRC(src);
    SetCK_OSCD(oscd);
    // OSCD Set
    debugCfg();
    TxCfg2ADC();
    return ReadADC();
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

// Test Case 7: Change XY and then do case 3 for Iout Iin, Qout Qin of N Freq
// Level 3: IQ Data Average, Sweep Frequency, Sweep XY(2D)
uint16  AllXYIQDataRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist)
{
    uint8 tune_x = 0;
    uint8 tune_y = 0;
    uint16  current_pos = 0;
    uint16  step_pos = 0;

    current_pos = start_pos;
    for (tune_x = 0; tune_x < MAX_5BIT_VAL; tune_x++)
    {
        for (tune_y = 0; tune_y < MAX_5BIT_VAL; tune_y++)
        {
            SetTune_X1(tune_x);//SetTune_X(tune_x);
            SetTune_X2(tune_y);//SetTune_Y(tune_y);
            step_pos = FSELIQDataRead(current_pos, adc_buf, fno, flist);
            current_pos += step_pos;
        }
    }

    return current_pos - start_pos;
}

// Test Case 8: Change XY and then do case 4 for average Iout Iin, Qout Qin of N Freq
// Level 3b: IQ Data Average, Sweep Frequency, Sweep XY(2D)
uint16  AllXYIQAvgRead(uint16 start_pos, IQ_ELEMENT* adc_buf, uint16 fno, uint16* flist, uint16 avg)
{
    uint8 tune_x = 0;
    uint8 tune_y = 0;
    uint16  current_pos = 0;
    uint16  step_pos = 0;

    current_pos = start_pos;
    for (tune_x = 0; tune_x < MAX_5BIT_VAL; tune_x++)
    {
        for (tune_y = 0; tune_y < MAX_5BIT_VAL; tune_y++)
        {
            SetTune_X1(tune_x);//SetTune_X(tune_x);
            SetTune_X2(tune_y);//SetTune_Y(tune_y);
            step_pos = FSELIQAvgRead(current_pos, adc_buf, fno, flist, avg);
            current_pos += step_pos;
        }
    }

    return current_pos - start_pos;
}

// Test Case 1: simplely read ADC without configuration
uint16  SimpleReadAdc(uint16 read_len, IQ_ELEMENT* adc_buf)
{
    uint16 i = 0;

    for (i = 0; i < read_len-1; i++)
    {
        adc_buf[i] = ReadADC();
    }
    adc_buf[i] = ReadADC();

    return read_len;
}

// Test Case 2: read ADC sampling data with configuration
uint16  CfgAndReadAdc(uint16 read_len, IQ_ELEMENT* adc_buf)
{
    // load last Configuration
    debugCfg();
    TxCfg2ADC();

    // Shift register to get ADC val
    SimpleReadAdc(read_len, adc_buf);

    // Set Configuration 2nd time
    debugCfg();
    TxCfg2ADC();

    return read_len;
}


/* ==================== UI-related Functions ====================== */
// Print ADC from Buffer
void    PrintADC(uint16 read_len, uint16 *adc_buf)
{
    uint16  i;
    if (read_len > MAX_PSOC_BUF)
        PutStr("#CMT\tPrint Length> MAX_PSOC_BUF\r\n");

    for (i = 0; i < read_len-1; i++)
    {
        //Since ADC val only has 10bits(1024), so it's always positive for int16
        FmtPrint(adc_buf[i]);
        PutStr("\r\n");
    }
    FmtPrint(adc_buf[i]);
    PutStr("\r\n");
}

void    JudgePrintADC(uint16 read_len, uint16 *adc_buf)
{
    uint16  rd_val = 0;
    uint8   len = 0;

    // Judge whether we need to print ADC data
    PutStr("\r\n");
    PutStr("#CMT\tPrint ADC Data? 1:YES; 0:NO. [1]/0 ");
    rd_val = ReadInput(&len);

    // Continuously Print ADC val MAX_ADC_BUF times
    if (!USE_NEW_VAL || 1 == rd_val)
    {
        PrintADC(read_len, adc_buf);
        PutStr("\r\n#CMT\tPrint ADC Data Finished!\r\n");
    }
    PutStr("#CMT\tLen=");FmtPrint(read_len);PutStr("\r\n");
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
        stp = IQAvgReadAdc(idx, adc_buf, avg);
        idx += stp;
    }

    return (((endNum-stNum)/step+1)*(sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT)));
}

uint16  GetADCOffset(uint16 avg)
{
    uint16 i = 0;
    uint16 offset = 0;

    // Set default value
    SetFreq(108);
    SetCB1(7);
    SetCB2(7);
    SetCB3(7);
    SetCB4(7);
    SetCB_SW(1);
    SetTune_X1(31);
    SetTune_X2(31);
    SetTune_X3(31);
    SetTune_X4(31);
    SetSgen_CAP1(1);
    SetSgen_CAP2(15);
    BackupCfg();
    LoadCfg();

    CFG_PHS_Write(0);
    CFG_SRC_Write(1);
    CFG_CAL_Write(1);
    CFG_OSCD_Write(0);

    for (i=0; i<avg; i++)
    {
        debugCfg();
        TxCfg2ADC();

        usleep(100);

        offset += ReadADC();
    }

    TxCfg2ADC();

    return (offset/avg);

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

        //if (!is_4D)
        if (1)
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

