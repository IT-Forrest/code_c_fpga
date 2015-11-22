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

        BackupCfg();
        Chip3_Send_Cfg_To_SCA();

        Chip3_Idx_Ctrl_Rst_Ana_Write(1);
        usleep(500);

        *pll_tune_ctrl_addr = 1;
        avs_wait();

        usleep(4200);
        printf("Reference Count: %u\n", (uint32_t)(*pll_tune_cntr_addr));
        printf("Feedback Count: %u\n", (uint32_t)(*pll_tune_cntf_addr));

        *pll_tune_ctrl_addr = 0;
        avs_wait();
    }
    else
    {
        int bs[5] = {0,1,3,7,15};
        uint16_t mdiv, i;
        bool flag;

        int a, b;
        printf("Freq,\tBs\n");
        for (mdiv = 32; mdiv <=127; mdiv++)
        {
            flag = false;
            Chip3_Set_Mdiv0(mdiv);
            for (i = 0; i <= 4; i++)
            {
                Chip3_Set_Bs0(bs[i]);

                Chip3_Idx_Ctrl_Rst_Ana_Write(0);
                Chip3_Send_Cfg_To_SCA();
                Chip3_Idx_Ctrl_Rst_Ana_Write(1);
                usleep(500);

                *pll_tune_ctrl_addr = 1;
                avs_wait();
                usleep(4200);

                a = (int)(*pll_tune_cntr_addr);
                b = (int)(*pll_tune_cntf_addr);

                if (a>=b && a-b<=2)
                { flag = true; break; }

                *pll_tune_ctrl_addr = 0;
                avs_wait();
            }

            *pll_tune_ctrl_addr = 0;
            avs_wait();

            if (flag)
                printf("%u,\t%u\n", mdiv, bs[i]);
            else
                printf("%u,\t-1\n", mdiv);
        }
    }

    return( clean_mem() );
}
