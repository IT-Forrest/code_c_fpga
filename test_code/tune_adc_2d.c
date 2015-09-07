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

#define ITERNUM 128
#define SSNUM 7
#define VOFFSET 507
#define AVG 8

#define P_OPT
#define P_SWP
#define P_FRP

uint16  fno;
uint16  flist[4] = {0};

void  Test_CFSA_4D(uint16 x1, uint16 x2, uint16 x3, uint16 x4, uint16* adc_buf, uint16* anaz, uint16* cf)
{
    uint16  i = 0, j = 0, k = 0;
    uint16  adc_val;

    for (i = 0; i < 18; i++)
    {
        adc_buf[i] = 0;
    }

    // Config
    SetTune_X1(x1);
    SetTune_X2(x2);
    SetTune_X3(15);
    SetTune_X4(15);

    CFSA4D_MUXTRG_Write(0);
    CFSA4D_MUXTRG_Write(1);

    CFSA4D_MUXRDY_Write(1);
    CFSA4D_MUXRDY_Write(0);

    SetFreq(flist[0]);

    for (j = 0; j < 2; j++)
    {
        for (i = 0; i < AVG; i++)
        {
            adc_val = ReadCfgADC(0,j,0,1);
            adc_buf[j] += adc_val;
            CFSA_ADCIN4D_Write(adc_val);
            CFSA4D_ADCRDY_Write(1);
            CFSA4D_ADCRDY_Write(0);
        }
    }
    adc_buf[0] /= AVG;
    adc_buf[1] /= AVG;

    // wait CF begin 4X frequency measurment
    for (i = 0; i < fno; i++)
    {
        // change FS frequency
        SetFreq(flist[i]);

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

                CFSA_ADCIN4D_Write(adc_val);
                CFSA4D_ADCRDY_Write(1);
                CFSA4D_ADCRDY_Write(0);
            }

            adc_buf[IQ_GRP_OFFSET + i*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT) + j] /= AVG;

            if ((0 == i) && (1 == j))
            {
                if (abs(adc_buf[0] - adc_buf[2])+abs(adc_buf[1] - adc_buf[3]) > OSC_TOL)
                {
                    *anaz = 255;
                    *cf = 255;
                    goto TESTCFSA_END;
                }
            }
        }
    }

    usleep(50);
    *anaz = CFSA_ANABITS_Read();
    *cf = GetCFSA4D(adc_buf, VOFFSET, 0, 1, 0, 0, 1, 1);

TESTCFSA_END:
    usleep(80);

    return;
}

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i = 0, j = 0;
    uint16  tunex1, tunex2, tunex3, tunex4;
    uint16  anaz,cost;
    uint16  gain0, gain1, gain2, gain3;
    uint16  div_accu;
    uint16  adc_buf[384] = {0};

    /* Initialization */
    {
        if (init_mem()) return (1);
        if (init_cfg()) return (1);
        if (syn_ctrl()) return (1);

        if (LoadCfgFile("./case1.cfg", &fno, flist)) return (1);

        tunex1 = 15;
        tunex2 = 15;
        tunex3 = 15;
        tunex4 = 15;

        gain0  = 0;
        gain1  = 0;         // Gain factor for the second term
        gain2  = 1;
        gain3  = 1;
        div_accu = 1;      // Set division accuracy is T27

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

        // TestCost(tunex1, tunex2, tunex1, tunex2, adc_buf, &anaz, &cost);
    }

    /* Find Optimal */
#ifdef P_OPT
    {
        CFSA4D_RSN_Write(1);        // activate the CFSA
        usleep(2);

        printf("*** Optimization, ITERNUM=%d, SSNUM=%d\n", ITERNUM, SSNUM);
        printf("No.\tX\tY\tCFH\tCF\n");

        for (i = 0; i < ITERNUM+SSNUM+1; i++)
        {
            usleep(1);
            tunex1 = CFSA_TUNE4DX1_Read();
            tunex2 = CFSA_TUNE4DX2_Read();

            Test_CFSA_4D(tunex1, tunex2, tunex1, tunex2, adc_buf, &anaz, &cost);
            CFSA4D_ANABITS_Write(anaz);

            CFSA4D_MUXNXT_Write(1);
            CFSA4D_MUXNXT_Write(0);// for safty

            printf("#%d\t%d\t%d\t%d\t%d\n", i+1, tunex1, tunex2, anaz, cost);

            if (anaz<=1)
            {
                i = ITERNUM+1;
            }

        }

        tunex1 = CFSA_TUNE4DX1_Read();
        tunex2 = CFSA_TUNE4DX2_Read();
        Test_CFSA_4D(tunex1, tunex2, tunex1, tunex2, adc_buf, &anaz, &cost);
        //CFSA4D_ANABITS_Write(anaz);

        printf("Optimization Final, X=%d, Y=%d\n", tunex1, tunex2);
    }
#endif // P_OPT

    /* Scan Surface 2D */
#ifdef P_SWP
    {
        printf("*** Surface Scan\n");
        printf("X\tY\tCFH\n");

        for (i=0; i<32; i++)
        {
            for (j=0; j<32; j++)
            {
                Test_CFSA_4D(i, j, i, j, adc_buf, &anaz, &cost);
                // CFSA4D_ANABITS_Write(anaz);
                printf("%d\t%d\t%d\n", i, j, anaz);
            }
        }
    }
#endif // P_SWP

    /* Sweep Frequency Response */
#ifdef P_FRP
    {
        FSEL_ELEMENT *A;

        printf("*** Surface Scan, X=%d, Y=%d\n", tunex1, tunex2);
        printf("Fq\tIo\tIi\tQo\tQi\tAi\tAo\tG\n");

        SetTune_X1(tunex1);
        SetTune_X2(tunex2);
        SetTune_X3(15);
        SetTune_X4(15);
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

    printf("***** Setting X=%d, Y=%d\n", tunex1, tunex2);

    SetTune_X1(tunex1);
    SetTune_X2(tunex2);
    SetTune_X3(15);
    SetTune_X4(15);
    TxCfg2ADC();
    //Global_RSTN_Write(0);

    return( clean_mem() );
}
