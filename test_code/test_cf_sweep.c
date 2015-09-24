
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/scan_chain.h"
#include "../ana_opt_2/set_config.h"
#include "../ana_opt_2/serial_port_io.h"
#include "../ana_opt_2/test_adc.h"

//#define DEBUG_PRINT
//#define DEBUG_CORDIC
//#define DEBUG_MULTIP

#define ITERNUM 512
#define SSNUM 16
#define VOFFSET 0
#define AVG 8

#define P_OPT
//#define P_SWP
#define P_FRP

uint16  fno;
uint16  flist[4] = {0};

uint16 Amp_Cordic_Sweep(int16_t x, int16_t y)
{
    int16_t  i, sig;
    int16_t  xn, yn, zn, xold;

    /* BEGIN OF VERILOG */
    xn = abs(x);
    yn = abs(y);

    if (xn<yn) {zn = xn; xn = yn; yn = zn;}

    xn = (xn << 2);///4
    yn = (yn << 2);///4

    for (i=1; i<=8; i++)
    {
        //printf("xtemp=%d, ytemp=%d\n",xn,yn);
        sig = ((xn>0 && yn>0)||
               (xn<0 && yn<0)) ?
               -1 : 1;
        zn = xn;
        //xn -= sig * (yn>>i);
        //yn += sig * (zn>>i);

        xn -= sig * ((yn>=0) ? (yn>>i) : -((-yn)>>i));
        yn += sig * ((zn>=0) ? (zn>>i) : -((-zn)>>i));
    }
//    /* END OF VERILOG */
//
//    for (i=1; i<8; i++)
//    {
//        sig = (yn<0)? 1 : 0;
//        zn = sig? (-yn):yn;
//        zn = (yn>>i);
//        xold = xn;
//        xn = xn + zn;
//        zn = (xold>>i);
//        zn = sig? zn:(-zn);
//        yn = yn + zn;
//    }
    //printf("Finish Cordic\n");

    return (uint16)(xn>>2);
    //return (uint16)(xn);
}

uint16 Amp_Div_Sweep(uint16 x, uint16 y, uint16 div_accu)
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

uint16 Amp_Mult_Sweep(uint8 mod, uint16 multiplicand, uint16 multiplior)
{
    uint16  multiplication = 0;
    uint16  multiplier_tmp = multiplior;
    int16_t  i=0;
    int16_t  j=0;

    if ((mod == 0) || (mod == 1))
    {
        for (i=0; i<8; i++)
        {
            if (multiplier_tmp & 0x0001)
            {
                multiplication += multiplicand;
            }
            multiplier_tmp = (multiplier_tmp >> 1);
            multiplication = (multiplication >> 1);
            #ifdef DEBUG_MULTIP
            printf("multiplication_tmp=%d\n",multiplication);
            #endif
        }

        if (mod == 1)
            multiplication += multiplicand;
        #ifdef  DEBUG_MULTIP
        printf("multiplication_tmp=%d\n",multiplication);
        #endif
    }
    else
    {
        multiplication = (multiplicand<<1);
    }

    return  multiplication;
}

uint16 GetCFSA4D_SWEEP(uint16* adc_buf, uint16 offset, uint8 is_4D, uint16 div_accu,
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
        //            Aout = Amp_Cordic_Sweep(adc_buf[4], adc_buf[3]) << 7;
        //    Ainn = Amp_Cordic_Sweep(adc_buf[2], adc_buf[1]);
//        uint32  F0 = (Amp_Cordic_Sweep(A0->Iout - offset, A0->Qout - offset) << 7)/
//                     (Amp_Cordic_Sweep(A0->Iinn - offset, A0->Qinn - offset));
//        uint32  F1 = (Amp_Cordic_Sweep(A1->Iout - offset, A1->Qout - offset) << 7)/
//                     (Amp_Cordic_Sweep(A1->Iinn - offset, A1->Qinn - offset));
//        uint32  F2 = (Amp_Cordic_Sweep(A2->Iout - offset, A2->Qout - offset) << 7)/
//                     (Amp_Cordic_Sweep(A2->Iinn - offset, A2->Qinn - offset));
//        uint32  F3 = (Amp_Cordic_Sweep(A3->Iout - offset, A3->Qout - offset) << 7)/
//                     (Amp_Cordic_Sweep(A3->Iinn - offset, A3->Qinn - offset));
//
//        if (F0 >= 0x1FF)    F0 = 0x1FF;
//        if (F1 >= 0x1FF)    F1 = 0x1FF;
//        if (F2 >= 0x1FF)    F2 = 0x1FF;
//        if (F3 >= 0x1FF)    F3 = 0x1FF;

        f2_compen = (div_accu)? 128 : 64;

        uint32  F0 = Amp_Div_Sweep(Amp_Cordic_Sweep(A0->Iout - offset, A0->Qout - offset),
                                  Amp_Cordic_Sweep(A0->Iinn - offset, A0->Qinn - offset), div_accu);
        uint32  F1 = Amp_Div_Sweep(Amp_Cordic_Sweep(A1->Iout - offset, A1->Qout - offset),
                                  Amp_Cordic_Sweep(A1->Iinn - offset, A1->Qinn - offset), div_accu);
        uint32  F2 = Amp_Div_Sweep(Amp_Cordic_Sweep(A2->Iout - offset, A2->Qout - offset),
                                  Amp_Cordic_Sweep(A2->Iinn - offset, A2->Qinn - offset), div_accu);
        uint32  F3 = Amp_Div_Sweep(Amp_Cordic_Sweep(A3->Iout - offset, A3->Qout - offset),
                                  Amp_Cordic_Sweep(A3->Iinn - offset, A3->Qinn - offset), div_accu);
        #ifdef  DEBUG_CORDIC
        printf("\t\tF0.Ao=%d, F0.Ai=%d, F0=%d\tF3.Ao=%d, F3.Ai=%d, F3=%d\n",
               Amp_Cordic_Sweep(A0->Iout - offset, A0->Qout - offset),
               Amp_Cordic_Sweep(A0->Iinn - offset, A0->Qinn - offset),
               F0,
               Amp_Cordic_Sweep(A3->Iout - offset, A3->Qout - offset),
               Amp_Cordic_Sweep(A3->Iinn - offset, A3->Qinn - offset),
               F3);
        #endif // DEBUG_CORDIC

        #ifdef  DEBUG_PRINT
        printf("A0\tA1\tA2\tA3\n");
        printf("%d\t%d\t%d\t%d\n", F0, F1, F2, F3);
        #endif

        //if (!is_4D)
        if (1)
        {
            // 1.414*F0
            F0_tmp = Amp_Mult_Sweep(0x01, F0, 106);
//            F0_tmp = F0;
//            F0_tmp = F0 + (F0_tmp >> 2);
//            F0_tmp = F0 + (F0_tmp >> 2);
//            F0_tmp = F0 + (F0_tmp >> 1);
//            F0_tmp = F0 + (F0_tmp >> 2);
            // 1.414*F3
            F3_tmp = Amp_Mult_Sweep(0x01, F3, 106);
//            F3_tmp = F3;
//            printf("\t%d\n", F3_tmp);
//            F3_tmp = F3 + (F3_tmp >> 2);
//            printf("\t%d\n", F3_tmp);
//            F3_tmp = F3 + (F3_tmp >> 2);
//            printf("\t%d\n", F3_tmp);
//            F3_tmp = F3 + (F3_tmp >> 1);
//            printf("\t%d\n", F3_tmp);
//            F3_tmp = F3 + (F3_tmp >> 2);
//            printf("\t%d\n", F3_tmp);

            #ifdef  DEBUG_PRINT
            printf("F0*1.414\tF3*1.414\n");
            printf("%d\t%d\n", F0_tmp, F3_tmp);
            #endif
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

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i = 0, j = 0, k = 0, p = 0;
    uint16  tunex1, tunex2, tunex3, tunex4;
    uint16  anaz,cost;
    uint16  gain0, gain1, gain2, gain3;
    uint16  div_accu;
    uint16  adc_buf[384] = {0};

    /* Initialization */
    {
        //if (init_mem()) return (1);
        //if (init_cfg()) return (1);
        //if (syn_ctrl()) return (1);

        //if (LoadCfgFile("./case1.cfg", &fno, flist)) return (1);

        tunex1 = 15;
        tunex2 = 15;
        tunex3 = 15;
        tunex4 = 15;

        gain0  = 0;
        gain1  = 0;         // Gain factor for the second term
        gain2  = 1;
        gain3  = 1;
        div_accu = 1;      // Set division accuracy is T27
        fno = 4;

#if 0
        CFSA4D_RSN_Write(0);
        CFSA_3DBFACTOR0_Write(0x100|106);    // Set coefficiency for the 1st 3dB frequency point
        CFSA_3DBFACTOR1_Write(0x100|106);    // Set coefficiency for the 2nd 3dB frequency point
        CFSA_THRESHOLD_Write(100);  // Set threshold for multi-start SS
        CFSA4D_GAIN0_Write(gain0);// 2D need to times 2
        CFSA4D_GAIN1_Write(gain1);// 2D need to times 2
        CFSA4D_GAIN2_Write(gain2);
        CFSA4D_GAIN3_Write(gain3);
        CFSA_INIT4DX1_Write(tunex1);
        CFSA_INIT4DX2_Write(tunex2);
        CFSA_INIT4DX3_Write(tunex3);
        CFSA_INIT4DX4_Write(tunex4);
        CFSA_ITER4DSA_Write(ITERNUM); // Set SA iteration = ITERNUM
        CFSA_ITER4DSS_Write(SSNUM);       // SSNUM Set Sensitivity time = 4
        CFSA_INITT4D_Write(32);       // Set initial T = 32
        CFSA4D_STEPT_Write(4);        // Set T step = 4; each time T=T-T_step;
        CFSA4D_4DY_Write(0);          // Set 4D test
        CFSA4D_SARAND_Write(1);
        CFSA4D_CFAVRG_Write(1);
        CFSA4D_DIVACCU_Write(div_accu);
        CFSA4D_MOD_Write(1);          // Optimization type
        CFSA_TOLIN4D_Write(OSC_TOL);       // the OSCD threshold value;
        CFSA_OFFIN4D_Write(VOFFSET);      // Offset used in IQ measurement
#endif
        // TestCost(tunex1, tunex2, tunex1, tunex2, adc_buf, &anaz, &cost);
        for (k = 0; k<1024; k++)
        {
            for (i = 0; i < fno; i++)
            {
                adc_buf[0] = (k+i)%1024;
                adc_buf[1] = (k+i+1)%1024;

                /// Ii Qi and Io Qo have different values
                for (j=0; j<4; j++)
                {
                    adc_buf[2+i*fno+j] = (k+i+j)%1024;
                    if ((0 == i) && (1 == j))
                    {
                        if (abs(adc_buf[0] - adc_buf[2])+abs(adc_buf[1] - adc_buf[3]) > OSC_TOL)
                        {
                            cost = 255;
                            goto SWEEP_END;
                        }
                    }
                }
                #ifdef  DEBUG_PRINT
                printf("Ii\tQi\tIo\tQo\n");
                printf("%d\t%d\t%d\t%d\n", k+i, k+i+1, k+i+2, k+i+3);
                getchar();
                #endif
            }

            SWEEP_END:
            cost = GetCFSA4D_SWEEP(adc_buf, VOFFSET, 0, 1, 0, 0, 1, 1);
            printf("%d\tCFSA_ANA = %d\n", k, cost);
        }
    }
    return 0;

    /* Find Optimal */
#ifdef P_OPT
    {
        CFSA4D_RSN_Write(1);        // activate the CFSA
        usleep(2);

        printf("*** Optimization, ITERNUM=%d, SSNUM=%d\n", ITERNUM, SSNUM);
        printf("No.\tX2\tY2\tX1\tY1\tCFH\tCF\n");

        for (i = 0; i < ITERNUM+SSNUM+1; i++)
        {
            usleep(1);
            tunex1 = CFSA_TUNE4DX1_Read();
            tunex2 = CFSA_TUNE4DX2_Read();
            tunex3 = CFSA_TUNE4DX3_Read();
            tunex4 = CFSA_TUNE4DX4_Read();

            Test_CFSA_4D(tunex1, tunex2, tunex3, tunex4, adc_buf, &anaz, &cost);
            CFSA4D_ANABITS_Write(anaz);// write back to CFSA

            CFSA4D_MUXNXT_Write(1);
            CFSA4D_MUXNXT_Write(0);// for safty

            printf("#%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1, tunex1, tunex2, tunex3, tunex4, anaz, cost);

            if (anaz<=1)
            {
                i = ITERNUM+1;
            }
        }

        /// set the optimal solution to analog circuit
        tunex1 = CFSA_TUNE4DX1_Read();
        tunex2 = CFSA_TUNE4DX2_Read();
        tunex3 = CFSA_TUNE4DX3_Read();
        tunex4 = CFSA_TUNE4DX4_Read();

        Test_CFSA_4D(tunex1, tunex2, tunex3, tunex4, adc_buf, &anaz, &cost);
        //CFSA4D_ANABITS_Write(anaz);

        printf("Optimization Final, X2=%d, Y2=%d, X1=%d, Y1=%d\n", tunex1, tunex2, tunex3, tunex4);
    }
#endif // P_OPT

    /* Scan Surface 4D */
#ifdef P_SWP
    {
        printf("*** Surface Scan\n");
        printf("X2\tY2\tX1\tY1\tCFH\n");

        for (i=0; i<32; i++)
        {
            for (j=0; j<32; j++)
            {
                for (k=0; k<32; k++)
                {
                    for (p=0; p <32; p++)
                    {
                        Test_CFSA_4D(i, j, k, p, adc_buf, &anaz, &cost);
                        // CFSA4D_ANABITS_Write(anaz);
                        printf("%d\t%d\t%d\t%d\t%d\n", i, j, k, p, anaz);
                    }
                }
            }
        }
    }
#endif // P_SWP

    /* Sweep Frequency Response */
#ifdef P_FRP
    {
        FSEL_ELEMENT *A;

        printf("*** Surface Scan, X2=%d, Y2=%d, X1=%d, Y1=%d\n", tunex1, tunex2, tunex3, tunex4);
        printf("Fq\tIo\tIi\tQo\tQi\tAi\tAo\tG\n");

        SetTune_X1(tunex1);
        SetTune_X2(tunex2);
        SetTune_X3(tunex3);
        SetTune_X4(tunex4);
        BackupCfg();

        for (i=0; i<384; i++) adc_buf[i] = 0;
        SweepFreqRespAvg(32, 127, 1, adc_buf, AVG);
        for (i=0; i<384; i++) adc_buf[i] /= AVG;

        for (i=0; i<96; i++)
        {
            printf("%d", i+32);
            A = (FSEL_ELEMENT *)(adc_buf + i*4);
            printf("\t%d\t%d\t%d\t%d", A->Iout, A->Iinn, A->Qout, A->Qinn);
            printf("\t%d\t%d",
                   (int)MAG(A->Iinn-VOFFSET, A->Qinn-VOFFSET),
                   (int)MAG(A->Iout-VOFFSET, A->Qout-VOFFSET));
            printf("\t%2.4f\n",
                   MAG(A->Iout-VOFFSET, A->Qout-VOFFSET)/
                   MAG(A->Iinn-VOFFSET, A->Qinn-VOFFSET));
        }
    }
#endif // P_FRP

    printf("***** Setting X2=%d, Y2=%d, X1=%d, Y1=%d\n", tunex1, tunex2, tunex3, tunex4);

    SetTune_X1(tunex1);
    SetTune_X2(tunex2);
    SetTune_X3(tunex3);
    SetTune_X4(tunex4);
    TxCfg2ADC();
    //Global_RSTN_Write(0);

    return( clean_mem() );
}
