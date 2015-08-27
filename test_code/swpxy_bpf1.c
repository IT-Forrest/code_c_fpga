#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
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

#define VOFFSET 504
#define AVG 4
#define IQ_GRP_OFFSET 2

int main() {

    uint16  read_len = 0;
    //uint16  step_pos = 0;
    uint16  fno = 3;
    uint16  flist[3] = {54,66,78};
    uint16  tune_x, tune_y;
    uint16  i;
    FSEL_ELEMENT *A1, *A2, *A3;

    //IQ_ELEMENT  adc_buf[13312] = {0};
    uint16  adc_buf[14] = {0};
    uint16  cost_val[1024] = {0};

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    // For one Frequency, there are only four data
    SetCB1(5);
    SetCB2(5);
    SetCB3(5);
    SetCB4(5);
    SetCB_SW(1);

    BackupCfg();

    A1 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET);
    A2 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 1*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));
    A3 = (FSEL_ELEMENT *) (adc_buf + IQ_GRP_OFFSET + 2*sizeof(FSEL_ELEMENT)/sizeof(IQ_ELEMENT));

#if 1
    for (tune_x = 0; tune_x < MAX_5BIT_VAL; tune_x++)
    {
        for (tune_y = 0; tune_y < MAX_5BIT_VAL; tune_y++)
        {
            SetTune_X1(tune_x);
            SetTune_X2(tune_y);
            SetTune_X3(tune_x);
            SetTune_X4(tune_y);

            //step_pos = FSELIQAvgRead(current_pos, adc_buf, fno, flist, 4);
            //current_pos += step_pos;

            for (i=0; i<14; i++) adc_buf[i] = 0;
            read_len = FSELIQAvgRead(0, adc_buf, fno, flist, AVG);
            for (i=0; i<14; i++) adc_buf[i] /= AVG;

            PrintADC(read_len, adc_buf);

            //cost_val[tune_x*32+tune_y] = GetCost(A1, A2, A3, VOFFSET);
        }
    }
#endif


    //PrintADC(1024, cost_val);

    return( clean_mem() );
}
