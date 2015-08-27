#include <stdio.h>
#include <stdlib.h>
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


int main(int argc, char** argv) {

    uint16  read_len = 0;

    IQ_ELEMENT  adc_buf[MAX_PSOC_BUF] = {0};

    if (argc<=1) return 0;

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    int cb = atoi(argv[2]);
    int x  = atoi(argv[3]);
    int y  = atoi(argv[4]);

    int fq = atoi(argv[1]);
    fq = (fq<32 || fq>127) ? 32 : fq;

    // For one Frequency, there are only four data
    SetCB1(cb);
    SetCB2(cb);
    SetCB3(cb);
    SetCB4(cb);
    SetCB_SW(1);
    //SetSgen_CAP1(0);
    //SetSgen_CAP2(6);
    SetTune_X1(x);
    SetTune_X2(y);
    SetTune_X3(x);
    SetTune_X4(y);
    BackupCfg();

    //read_len = SweepFreqResp(36, 90, 3, adc_buf);
#if 1
    read_len = SweepFreqRespAvg(32, 127, 1, adc_buf, 8);
    PrintADC(read_len, adc_buf);
#endif



#if 1
    SetFreq(fq);
    CFG_FSEL_Write(RtnFSEL());
    CFG_PHS_Write(0);
    CFG_SRC_Write(0);
    CFG_CAL_Write(0);

    Global_RSTN_Write(0);

    // OSCD Set
    debugCfg();
    TxCfg2ADC();

    Global_RSTN_Write(1);
#endif

    return( clean_mem() );
}
