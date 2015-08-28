#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/device.h"
#include "../ana_opt_2/scan_chain.h"
#include "../ana_opt_2/set_config.h"
#include "../ana_opt_2/serial_port_io.h"
#include "../ana_opt_2/test_adc.h"

#define START_FREQ 32
#define STOP_FREQ 127
#define FREQ_STEP 1
#define AVG_NUM 16

// Use get_offset first, then set VOFF here
#define VOFF 507

int main()
{
    uint16  i,j,k,freq;
    int amp;

    IQ_ELEMENT adc_buf[4] = {0};

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    SetCB1(7);
    SetCB2(7);
    SetCB3(7);
    SetCB4(7);
    SetCB_SW(1);
    //SetSgen_CAP1(0);
    //SetSgen_CAP2(6);
    SetTune_X1(31);
    SetTune_X2(31);
    SetTune_X3(31);
    SetTune_X4(31);
    BackupCfg();

    LoadCfg();
    for (freq = START_FREQ; freq <= STOP_FREQ; freq += FREQ_STEP)
    {
        SetFreq(freq);
        for (i=0; i<2; i++) // CAP1
        {
            printf("%d\t%d\t", freq, i);
            SetSgen_CAP1(i);

            for (j=0; j<16; j++) // CAP2
            {
                SetSgen_CAP2(j);
                IQAvgReadAdc(0, adc_buf, AVG_NUM);

                for (k=0; k<4; k++) {adc_buf[k]=(adc_buf[k]/AVG_NUM);}
                amp = sqrt(pow((int)adc_buf[2]-VOFF,2)+pow((int)adc_buf[3]-VOFF,2));
                printf("%d\t", amp);

                memset(adc_buf,0,sizeof(adc_buf));
            }
            printf("\n");
        }
    }

    return 0;

}
