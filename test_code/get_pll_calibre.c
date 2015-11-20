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
#include "../ana_opt_2/set_config.h"
#include "../ana_opt_2/psoc_port.h"
#include "../ana_opt_2/test_adc.h"

int main(int argc, char** argv) {

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);
    init_sc();

    ///////////////////////////////////////////////
    /// To set analog configuration: e.g. Mdiv, BS, etc
    ///////////////////////////////////////////////
    if (argc == 3)
    {
        Chip3_Set_Mdiv0(atoi(argv[1]));
        Chip3_Set_Bs0(atoi(argv[2]));
    }
    else
    {
        printf("Arguments needed: Mdiv Bs\n");
        return 1;
    }

    BackupCfg();
    Chip3_Send_Cfg_To_SCA();

    Chip3_Idx_Ctrl_Rst_Ana_Write(1);
    usleep(70);

    *pll_tune_ctrl_addr = 1;
    avs_wait();

    usleep(500);
    printf("Reference Count: %u\n", (uint32_t)(*pll_tune_cntr_addr));
    printf("Feedback Count: %u\n", (uint32_t)(*pll_tune_cntf_addr));

    *pll_tune_ctrl_addr = 0;
    avs_wait();

    return( clean_mem() );
}
