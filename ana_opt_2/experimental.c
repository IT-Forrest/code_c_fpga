#include <stdlib.h>
#include "scan_chain.h"
#include "set_config.h"
#include "test_adc.h"

#define VOFFSET 508
#define AVG 16

/* Improved cost function (#1, 5 freq points) for 4D optimization */

uint16 CF1(uint16 x1, uint16 x2, uint16 x3, uint16 x4)
{
    if (x1 < 3 || x2 < 3 || x3 < 3 || x4 < 3) return 255;
    /* ========== ADC Measurement ========== */
    uint16  i = 0, j = 0, k = 0;
    uint16  adc_val;
    uint16  fno = 5;
    uint16  adc_buf[IQ_GRP_OFFSET + fno*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT)];
    uint16  flist[5] = {50, 56, 60, 64, 71};

    for (i = 0; i < IQ_GRP_OFFSET + fno*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT); i++)
    {
        adc_buf[i] = 0;
    }

    // Config
    SetTune_X1(x1);
    SetTune_X2(x2);
    SetTune_X3(x3);
    SetTune_X4(x4);

    SetFreq(flist[0]);

    for (j = 0; j < 2; j++)
    {
        for (i = 0; i < AVG; i++)
        {
            adc_val = ReadCfgADC(0,j,0,1);//in current version, to get ADC needs to send config to the scan chain
            adc_buf[j] += adc_val;
        }
    }
    adc_buf[0] /= AVG;
    adc_buf[1] /= AVG;

    // wait CF begin 4X frequency measurment
    for (i = 0; i < fno; i++)
    {
        // change FS frequency
        SetFreq(flist[i]);
        usleep(300);

        // wait till 4X signals' measurment is done
        for (j = 0; j < 4; j++)
        {
            for (k=0; k < AVG; k++)
            {
                switch (j)
                {
                case 0:
                    adc_val = ReadCfgADC(0,0,0,0); break;
                case 1:
                    adc_val = ReadCfgADC(0,1,0,0); break;
                case 2:
                    adc_val = ReadCfgADC(0,0,1,0); break;
                case 3:
                    adc_val = ReadCfgADC(0,1,1,0); break;
                }
                adc_buf[IQ_GRP_OFFSET + i*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT) + j] += adc_val;
            }

            adc_buf[IQ_GRP_OFFSET + i*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT) + j] /= AVG;

            if ((0 == i) && (1 == j))
            {
                if (abs(adc_buf[0] - adc_buf[2])+abs(adc_buf[1] - adc_buf[3]) > OSC_TOL)
                {
                    return 255;
                }
            }
        }
    }

    /* ========== Calculate Cost Function ========== */
    FSEL_ELEMENT *A0, *A1, *A2, *A3, *A4;

    A0 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 3*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A4 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 4*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

    uint32  F0 = Amp_Division(Amp_Cordic(A0->Iout - VOFFSET, A0->Qout - VOFFSET),
                              Amp_Cordic(A0->Iinn - VOFFSET, A0->Qinn - VOFFSET), 1);
    uint32  F1 = Amp_Division(Amp_Cordic(A1->Iout - VOFFSET, A1->Qout - VOFFSET),
                              Amp_Cordic(A1->Iinn - VOFFSET, A1->Qinn - VOFFSET), 1);
    uint32  F2 = Amp_Division(Amp_Cordic(A2->Iout - VOFFSET, A2->Qout - VOFFSET),
                              Amp_Cordic(A2->Iinn - VOFFSET, A2->Qinn - VOFFSET), 1);
    uint32  F3 = Amp_Division(Amp_Cordic(A3->Iout - VOFFSET, A3->Qout - VOFFSET),
                              Amp_Cordic(A3->Iinn - VOFFSET, A3->Qinn - VOFFSET), 1);
    uint32  F4 = Amp_Division(Amp_Cordic(A4->Iout - VOFFSET, A4->Qout - VOFFSET),
                              Amp_Cordic(A4->Iinn - VOFFSET, A4->Qinn - VOFFSET), 1);

    uint32  F0_tmp, F4_tmp;

    // 1.414*F0
    F0_tmp = F0;
    F0_tmp = F0 + (F0_tmp >> 2);
    F0_tmp = F0 + (F0_tmp >> 2);
    F0_tmp = F0 + (F0_tmp >> 1);
    F0_tmp = F0 + (F0_tmp >> 2);
    // 1.414*F4
    F4_tmp = F4;
    F4_tmp = F4 + (F4_tmp >> 2);
    F4_tmp = F4 + (F4_tmp >> 2);
    F4_tmp = F4 + (F4_tmp >> 1);
    F4_tmp = F4 + (F4_tmp >> 2);

    uint32  cost = 0;
    cost += (abs(F3-F0_tmp) << 1);
    cost += (abs(F1-F4_tmp) << 1);
    cost += (abs(F2-F0_tmp));
    cost += (abs(F2-F4_tmp));
    cost += (abs(F0-F4));
    cost += (abs(F1-F2));
    cost += (abs(F2-F3));
    cost += (abs(F1-F3));
    cost += (F2 > 128) ? 0 : (128-F2);

    cost = (cost >> 1);

    if (cost>255) cost = 255;

    return (uint16) cost;
}

/* Improved cost function (#2, 6 freq points) for 4D optimization */
uint16 CF2(uint16 x1, uint16 x2, uint16 x3, uint16 x4)
{
#define FR_NO 9

    if (x1 < 3 || x2 < 3 || x3 < 3 || x4 < 3) return 255;
    /* ========== ADC Measurement ========== */
    uint16  i = 0, j = 0, k = 0;
    uint16  adc_val;
    uint16  fno = FR_NO;
    uint16  adc_buf[IQ_GRP_OFFSET + fno*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT)];
    uint16  flist[FR_NO] = {50, 56, 60, 64, 71, 32, 115, 47, 75};

    for (i = 0; i < IQ_GRP_OFFSET + fno*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT); i++)
    {
        adc_buf[i] = 0;
    }

    // Config
    SetTune_X1(x1);
    SetTune_X2(x2);
    SetTune_X3(x3);
    SetTune_X4(x4);

    SetFreq(flist[0]);

    for (j = 0; j < 2; j++)
    {
        for (i = 0; i < AVG; i++)
        {
            adc_val = ReadCfgADC(0,j,0,1);//in current version, to get ADC needs to send config to the scan chain
            adc_buf[j] += adc_val;
        }
    }
    adc_buf[0] /= AVG;
    adc_buf[1] /= AVG;

    // wait CF begin 4X frequency measurment
    for (i = 0; i < fno; i++)
    {
        // change FS frequency
        SetFreq(flist[i]);
        usleep(300);

        // wait till 4X signals' measurment is done
        for (j = 0; j < 4; j++)
        {
            for (k=0; k < AVG; k++)
            {
                switch (j)
                {
                case 0:
                    adc_val = ReadCfgADC(0,0,0,0); break;
                case 1:
                    adc_val = ReadCfgADC(0,1,0,0); break;
                case 2:
                    adc_val = ReadCfgADC(0,0,1,0); break;
                case 3:
                    adc_val = ReadCfgADC(0,1,1,0); break;
                }
                adc_buf[IQ_GRP_OFFSET + i*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT) + j] += adc_val;
            }

            adc_buf[IQ_GRP_OFFSET + i*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT) + j] /= AVG;

            if ((0 == i) && (1 == j))
            {
                if (abs(adc_buf[0] - adc_buf[2])+abs(adc_buf[1] - adc_buf[3]) > OSC_TOL)
                {
                    return 255;
                }
            }
        }
    }

    /* ========== Calculate Cost Function ========== */
    FSEL_ELEMENT* A[6];
    uint32 F[FR_NO];

    for (i = 0; i < FR_NO; i++)
    {
        A[i] = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + i*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
        F[i] = Amp_Division(Amp_Cordic(A[i]->Iout - VOFFSET, A[i]->Qout - VOFFSET),
                            Amp_Cordic(A[i]->Iinn - VOFFSET, A[i]->Qinn - VOFFSET), 1);
    }

    uint32  F0_tmp, F4_tmp;

    // 1.414*F0
    F0_tmp = F[0];
    F0_tmp = F[0] + (F0_tmp >> 2);
    F0_tmp = F[0] + (F0_tmp >> 2);
    F0_tmp = F[0] + (F0_tmp >> 1);
    F0_tmp = F[0] + (F0_tmp >> 2);
    // 1.414*F4
    F4_tmp = F[4];
    F4_tmp = F[4] + (F4_tmp >> 2);
    F4_tmp = F[4] + (F4_tmp >> 2);
    F4_tmp = F[4] + (F4_tmp >> 1);
    F4_tmp = F[4] + (F4_tmp >> 2);

    uint32  cost = 0;

    /* 9 pt */
    cost += (abs(F[3]-F0_tmp));
    cost += (abs(F[1]-F4_tmp));
    //cost += (abs(F[2]-F0_tmp));
    //cost += (abs(F[2]-F4_tmp));
    cost += (abs(F[2]-(F[7] << 1)));
    cost += (abs(F[2]-(F[8] << 1)));
    cost += (abs(F[0]-F[4]));
    cost += (abs(F[1]-F[2]));
    cost += (abs(F[2]-F[3]));
    cost += (abs(F[1]-F[3]) << 1);
    cost += ((F[2] > 128) ? 0 : (128-F[2]));
    cost += ((F[5] <= 12) ? 0 : (F[5]-12)<<3);
    cost += ((F[6] <= 12) ? 0 : (F[6]-12)<<3);

    /* 7pt */
    /*
    cost += (abs(F[3]-(F[7] << 1)));
    cost += (abs(F[1]-(F[8] << 1)));
    cost += (abs(F[2]-(F[7] << 1)));
    cost += (abs(F[2]-(F[8] << 1)));
    cost += (abs(F[7]-F[8]) << 1);
    cost += (abs(F[1]-F[2]));
    cost += (abs(F[2]-F[3]));
    cost += (abs(F[1]-F[3]) << 1);
    cost += ((F[2] > 128) ? 0 : (128-F[2]) << 2);
    cost += ((F[5] > 16) ? 0 : (F[5]-16) << 2);
    cost += ((F[6] > 8) ? 0 : (F[6]-8) << 2);
    */


    cost = (cost >> 1);

    if (cost>255) cost = 255;

    return (uint16) cost;
}
