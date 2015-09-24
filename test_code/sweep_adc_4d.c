#include <stdio.h>
#include <stdlib.h>
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
#include "../ana_opt_2/experimental.h"

#define ITERNUM 512
#define SSNUM 16
#define VOFFSET 508
#define AVG 8

#define P_OPT
#define P_FRP

uint16 fno;
uint16 flist[4] = {0};

int main()
{
    uint16 tunex1, tunex2, tunex3, tunex4;
    uint16 anaz;
    uint16 i,j;

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    if (LoadCfgFile("./case1.cfg", &fno, flist)) return (1);

    tunex1 = 27;
    tunex2 = 20;
    tunex3 = 22;
    tunex4 = 24;

    printf("*** Surface Scan 1: X2 = %u, Y2 = %u\n", tunex3, tunex4);
    printf("X1\tY1\tCFH\n");

    for (i=0; i<32; i++)
    {
        for (j=0; j<32; j++)
        {
            anaz = CF2(i, j, tunex3, tunex4);
            printf("%d\t%d\t%d\n", i, j, anaz);
        }
    }

    if (LoadCfgFile("./case1.cfg", &fno, flist)) return (1);

    printf("*** Surface Scan 2: X1 = %u, Y1 = %u\n", tunex1, tunex2);
    printf("X2\tY2\tCFH\n");

    for (i=0; i<32; i++)
    {
        for (j=0; j<32; j++)
        {
            anaz = CF2(tunex1, tunex2, i, j);
            printf("%d\t%d\t%d\n", i, j, anaz);
        }
    }

    return( clean_mem() );
}
