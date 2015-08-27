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
#define AVG_NUM 15

// Use get_offset first, then set VOFF here
#define VOFF 508

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
    return 0;

}
