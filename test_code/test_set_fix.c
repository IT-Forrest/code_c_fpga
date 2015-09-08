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

uint16  fno;
uint16  flist[4] = {0};

int main(int argc, char** argv)
{
    uint16  tunex1, tunex2, tunex3, tunex4;

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    if (LoadCfgFile("./case1.cfg", &fno, flist)) return (1);

    if (argc == 1)
    {
        tunex1 = 11;
        tunex2 = 12;

        printf("***** Setting X=%d, Y=%d\n", tunex1, tunex2);

        SetTune_X1(tunex1);
        SetTune_X2(tunex2);
        SetTune_X3(15);
        SetTune_X4(15);
        TxCfg2ADC();
    }
    else if (argc == 5)
    {
        tunex1 = atoi(argv[1]);
        tunex2 = atoi(argv[2]);
        tunex3 = atoi(argv[3]);
        tunex4 = atoi(argv[4]);

        printf("***** Setting X2=%d, Y2=%d, X1=%d, Y1=%d\n", tunex1, tunex2, tunex3, tunex4);

        SetTune_X1(tunex1);
        SetTune_X2(tunex2);
        SetTune_X3(tunex3);
        SetTune_X4(tunex4);
        TxCfg2ADC();
    }
    else if (argc == 3)
    {
        tunex1 = atoi(argv[1]);
        tunex2 = atoi(argv[2]);

        printf("***** Setting X=%d, Y=%d\n", tunex1, tunex2);

        SetTune_X1(tunex1);
        SetTune_X2(tunex2);
        SetTune_X3(15);
        SetTune_X4(15);
        TxCfg2ADC();
    }

    return( clean_mem() );
}
