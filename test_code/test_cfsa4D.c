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

///ITERNUM=128, SSNUM=22
#define ITERNUM   512
#define SSNUM     7
#define VOFFSET   508
#define AVG       8
#define IS_4D     1
#define DIV_ACCU  1


uint16  fno;
uint16  flist[4] = {0};


void  TestCF_SA(uint16 div_accu, uint16 gain0, uint16 gain1, uint16 gain2, uint16 gain3,
                 uint16 x1, uint16 x2, uint16 x3, uint16 x4, uint16* adc_buf, uint16* anaz)
{
    uint16  i = 0, j = 0, k = 0;
    uint16  adc_val;
    uint16  tmp_buf[7]; //tmp_buf for adc_buf
    FSEL_ELEMENT *tmp_A;

    for (i = 0; i < 18; i++)
    {
        adc_buf[i] = 0;
    }

    // send tune bits
    SetTune_X1(x1);
    SetTune_X2(x2);
    SetTune_X3(x3);
    SetTune_X4(x4);

    CFSA4D_MUXRDY_Write(1);
    CFSA4D_MUXRDY_Write(0);

    // wait CF begin 4X frequency measurment
    for (i=0; i < 4; i++)
    {
        // change FS frequency
        SetFreq(flist[i]);

        // wait till 4X signals' measurment is done
        for (j = (0==i)?6:4; j >= 1; j--)
        {
            tmp_buf[j] = 0;

            for (k=0; k < AVG; k++)
            {
                TxCfg2ADC();
                //debugCfg();

                // CF send control signal to ctrl module;
                CFSA4D_MUXTRG_Write(1);
                usleep(16);

                adc_val = ReadADC();
                tmp_buf[j] += adc_val;

                CFSA_ADCIN4D_Write(adc_val);
                CFSA4D_MUXTRG_Write(0);

                //printf("%d, ", adc_val);
            }
            usleep(2);

            // finish average computation
            tmp_buf[j] = (tmp_buf[j] >> 3);

            //printf("%d\n", tmp_buf[j]);

            if ((0 == i) && (3 == j))
            {
                if (abs(tmp_buf[6] - tmp_buf[4])+abs(tmp_buf[5] - tmp_buf[3]) > OSC_TOL)
                {
                    *anaz = 255;
                    goto TESTCFSA_END;
                }
            }
        }

        tmp_A = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + i*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
        tmp_A->Iout = tmp_buf[4];
        tmp_A->Qout = tmp_buf[3];
        tmp_A->Iinn = tmp_buf[2];
        tmp_A->Qinn = tmp_buf[1];
    }

    adc_buf[0] = tmp_buf[6];
    adc_buf[1] = tmp_buf[5];

    //*anaz = GetCFSA4D(adc_buf, VOFFSET, IS_4D, div_accu, gain0, gain1, gain2, gain3);
    *anaz = CFSA_ANABITS_Read();
    CFSA4D_ANABITS_Write(*anaz);

    for (i = 0; i < 18; i++)
    {
        //printf("%d\t",adc_buf[i]);
    }

TESTCFSA_END:
    usleep(8);

    return;
}


int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i = 0;
    uint16  tunex1, tunex2, tunex3, tunex4;
    uint16  gain0, gain1, gain2, gain3;
    uint16  anaz;
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
        CFSA4D_4DY_Write(IS_4D);          // Set 4D test
        CFSA4D_SARAND_Write(1);
        CFSA4D_CFAVRG_Write(1);
        CFSA4D_DIVACCU_Write(div_accu);
        CFSA4D_MOD_Write(1);          // Optimization type
        CFSA_TOLIN4D_Write(OSC_TOL);       // the OSCD threshold value;
        CFSA_OFFIN4D_Write(VOFFSET);      // Offset used in IQ measurement
    }

    // Find minimum
    {
        CFSA4D_RSN_Write(1);        // activate the CFSA
        usleep(2);

        for (i = 0; i < ITERNUM + SSNUM + 4; i++)
        {
            tunex1 = CFSA_TUNE4DX1_Read();
            tunex2 = CFSA_TUNE4DX2_Read();
            tunex3 = CFSA_TUNE4DX3_Read();
            tunex4 = CFSA_TUNE4DX4_Read();

            printf("#%d\t%d\t%d\t%d\t%d\t", i+1, tunex1, tunex2, tunex3, tunex4);

            TestCF_SA(div_accu, gain0, gain1, gain2, gain3, tunex1, tunex2, tunex3, tunex4, adc_buf, &anaz);

            printf("%d\n", anaz);//cost=

            CFSA4D_MUXNXT_Write(1);
            CFSA4D_MUXNXT_Write(0);// for safty

            if (anaz<=1)
            {
                i = ITERNUM;
            }
        }

        tunex1 = CFSA_TUNE4DX1_Read();
        tunex2 = CFSA_TUNE4DX2_Read();
        tunex3 = CFSA_TUNE4DX3_Read();
        tunex4 = CFSA_TUNE4DX4_Read();

        printf("Optimization Final, X2=%d, Y2=%d, X1=%d, Y1=%d\n", tunex1, tunex2, tunex3, tunex4);
    }

    SetFreq(90);
    SetTune_X1(tunex1);
    SetTune_X2(tunex2);
    SetTune_X3(tunex3);
    SetTune_X4(tunex4);

    CFG_FSEL_Write(RtnFSEL());
    CFG_PHS_Write(0);
    CFG_SRC_Write(0);
    CFG_CAL_Write(0);
    CFG_OSCD_Write(0);

    TxCfg2ADC();
    Global_RSTN_Write(0);



    return 0;
}


