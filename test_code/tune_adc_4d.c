
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

#define ITERNUM 512
#define SSNUM 7
#define VOFFSET 508
#define AVG 4

#define NEWD
#define P_OPT
//#define P_SWP
#define P_FRP

uint16  fno;
uint16  flist[4] = {0};

void TestCost(uint16 x1, uint16 x2, uint16 x3, uint16 x4, uint16*  adc_buf, uint16* cfh, uint16* cf)
{
    uint16 i;

    // Config
    SetTune_X1(x1);
    SetTune_X2(x2);
    SetTune_X3(x3);
    SetTune_X4(x4);

    // Get IQs
    for (i=0; i<IQ_GRP_OFFSET+fno*4; i++) adc_buf[i] = 0;
    FSELIQAvgRead(0, adc_buf, fno, flist, AVG);
    for (i=0; i<IQ_GRP_OFFSET+fno*4; i++) adc_buf[i] /= AVG;

    // Calculate Costs
    //*cf = GetCost4D_hw(adc_buf, VOFFSET);
    *cfh = GetCost4D(adc_buf,VOFFSET);
    *cf = *cfh;

}

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i = 0, j = 0;
    uint16  tunex1, tunex2, tunex3, tunex4;
    uint16  anaz,cost;

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
        TestCost(tunex1, tunex2, tunex3, tunex4, adc_buf, &anaz, &cost);


#ifndef NEWD
        SA_INITX3_Write(tunex1);
        SA_INITX4_Write(tunex2);
        SA_ITERNUM_Write(ITERNUM);
        WSA_4DY_Write(0);
        SA_ANAIN_Write(anaz);
        WSA_RSN_Write(1);
#else
        SA_INIT4DX3_Write(tunex1);
        SA_INIT4DX4_Write(tunex2);
        SA_INIT4DX1_Write(tunex3);
        SA_INIT4DX2_Write(tunex4);
        SA_ITER4DSA_Write(ITERNUM); // Set SA iteration = ITERNUM
        SA_ITER4DSS_Write(SSNUM);       // Set Sensitivity time = 4
        SA_INITT4D_Write(32);       // Set initial T = 32
        SA4D_STEPT_Write(4);        // Set T step = 4; each time T=T-T_step;
        SA4D_4DY_Write(1);          // Set 4D test
        SA_ANAIN4D_Write(anaz);
        SA4D_RSN_Write(1);
#endif
    }

    /* Find Optimal */
#ifdef P_OPT
    {
        printf("*** Optimization, ITERNUM=%d, SSNUM=%d\n", ITERNUM, SSNUM);
        printf("No.\tX2\tY2\tX1\tY1\tCFH\tCF\n");
        printf("#0\t%d\t%d\t%d\t%d\t%d\t%d\n", tunex1, tunex2, tunex3, tunex4, anaz, cost);

        for (i = 0; i < ITERNUM+SSNUM; i++)
        {
#ifndef NEWD
            SA_ANAIN_Write(anaz);
            WSA_STA_Write(1);
            WSA_STA_Write(0);
            while(!WSA_RDY_Read());
            tunex1 = SA_TUNEX3_Read();
            tunex2 = SA_TUNEX4_Read();
#else
            SA_ANAIN4D_Write(anaz);
            SA4D_STA_Write(1);
            SA4D_STA_Write(0);
            if (i<ITERNUM)
            {
                while(!SA4D_RDY_Read());
            }
            else
            {
                usleep(300);
            }
            tunex1 = SA_TUNE4DX3_Read();
            tunex2 = SA_TUNE4DX4_Read();
            tunex3 = SA_TUNE4DX1_Read();
            tunex4 = SA_TUNE4DX2_Read();
#endif

            TestCost(tunex1, tunex2, tunex3, tunex4, adc_buf, &anaz, &cost);

            printf("#%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1, tunex1, tunex2, tunex3, tunex4, anaz, cost);
            if (anaz<=1)
            {
                i = ITERNUM-1;
            }
        }

#ifndef NEWD
        SA_ANAIN_Write(anaz);
        WSA_STA_Write(1);
        WSA_STA_Write(0);
        tunex1 = SA_TUNEX3_Read();
        tunex2 = SA_TUNEX4_Read();
#else
        SA_ANAIN4D_Write(anaz);
        SA4D_STA_Write(1);
        SA4D_STA_Write(0);
        tunex1 = SA_TUNE4DX3_Read();
        tunex2 = SA_TUNE4DX4_Read();
        tunex3 = SA_TUNE4DX1_Read();
        tunex4 = SA_TUNE4DX2_Read();
#endif

        printf("Optimization Final, X2=%d, Y2=%d, X1=%d, Y1=%d\n", tunex1, tunex2, tunex3, tunex4);
    }
#endif // P_OPT

    /* Scan Surface 2D */
#ifdef P_SWP
    {
        printf("*** Surface Scan\n");
        printf("X\tY\tCFH\tCF\n");

        for (i=0; i<32; i++)
        {
            for (j=0; j<32; j++)
            {
                TestCost(i, j, i, j, adc_buf, &anaz, &cost);
                printf("%d\t%d\t%d\t%d\n", i, j, anaz, cost);
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


        SetFreq(72);
        TxCfg2ADC();

    }
#endif // P_FRP

    return( clean_mem() );
}
