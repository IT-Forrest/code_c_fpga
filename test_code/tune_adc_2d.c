#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/set_config.h"
#include "../ana_opt_2/test_adc.h"

#define ITERNUM 128
#define SSNUM 7
#define VOFFSET 512

#define NEWD
#define P_OPT
#define P_SWP
#define P_FRP

uint16  fno = 4;
uint16  flist[4] = {0};

uint16 TestCost(uint16*  adc_buf)
{
    uint16 i;

    // Get IQs
    for (i=0; i<IQ_GRP_OFFSET+fno*4; i++) adc_buf[i] = 0;

    FSELIQAvgRead(0, adc_buf, fno, flist, AVG);

    for (i=0; i<IQ_GRP_OFFSET+fno*4; i++)
    {
        adc_buf[i] /= AVG;
        printf("%d,", adc_buf[i]);
    }

    // Calculate Costs
    return GetCFSA4D(adc_buf, VOFFSET, 0, 1, 0, 0, 1, 1);

}

int main(int argc, char** argv)
{

    if (argc == 4) // Normal
    {
        flist[0] = atoi(argv[1]);
        flist[1] = atoi(argv[2]);
        flist[2] = atoi(argv[2]);
        flist[3] = atoi(argv[3]);
    }

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i = 0, j = 0;

    IQ_ELEMENT adc_buf[384] = {0};

    /* Initialization */
    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    Chip3_Idx_Ctrl_Sel_A_Write(0);
    init_sc();
    Chip3_Set_Sw(1);

    BackupCfg();
    Chip3_Send_Cfg_To_SCA();

    Chip3_Idx_Ctrl_Rst_Ana_Write(1);
    Chip3_Idx_Ctrl_Sta_Clk_Write(1);

    /* Fixed Settings */
    Chip3_Set_Tx1(15);
    Chip3_Set_Ty1(15);
    Chip3_Set_Cb1(7);
    Chip3_Set_Cb2(7);

    Chip3_Set_Tx2(26);
    Chip3_Set_Ty2(8);
    Chip3_Set_Cb3(3);
    Chip3_Set_Cb4(3);

    /* Sweep Frequency Response */
#ifdef P_SWP
    printf("*** SWEEP ***\n");

    for (i=0; i<32; i++)
    {
        Chip3_Set_Tx2(i);
        for (j=0; j<32; j++)
        {
            Chip3_Set_Ty2(j);
            printf("%d,%d,%d\n", i, j, TestCost(adc_buf));
        }
    }
#endif // P_FRP

    return( clean_mem() );
}
