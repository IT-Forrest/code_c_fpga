#include <stdio.h>
#include <stdlib.h>
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
#include "../ana_opt_2/set_config.h"
#include "../ana_opt_2/test_adc.h"

// Use get_offset first, then set VOFF here
#define VOFFSET 517
#define MAX_READ_ITER  1024

void Chip3_Sweep_fixfreq(uint16 Fq, uint16 read_iter, IQ_ELEMENT *adc_buf)
{
    uint16  i;
    FSEL_ELEMENT *A;

    for (i=0; i< read_iter; i++)
    {
        /// clean and read
        adc_buf[0] = 0;
        adc_buf[1] = 0;
        adc_buf[2] = 0;
        adc_buf[3] = 0;
        IQAvgReadAdc(0, adc_buf, AVG);

        /// average and print
        adc_buf[0] /= AVG;
        adc_buf[1] /= AVG;
        adc_buf[2] /= AVG;
        adc_buf[3] /= AVG;

        printf("%d", Fq);
        A = (FSEL_ELEMENT *)(adc_buf);
        printf("\t%d\t%d\t%d\t%d",
               A->Iinn-VOFFSET,
               A->Qinn-VOFFSET,
               A->Iout-VOFFSET,
               A->Qout-VOFFSET
               );
        printf("\t%d\t%d",
               (int)MAG(A->Iinn-VOFFSET, A->Qinn-VOFFSET),
               (int)MAG(A->Iout-VOFFSET, A->Qout-VOFFSET));
        printf("\t%2.4f\n",
               MAG(A->Iout-VOFFSET, A->Qout-VOFFSET)/
               MAG(A->Iinn-VOFFSET, A->Qinn-VOFFSET));
    }
}


int main(int argc, char** argv)
{
    uint8 test_type = 0;
    uint16  i;
    IQ_ELEMENT adc_buf[384] = {0};

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    Chip3_Idx_Ctrl_Sel_A_Write(0);

    init_sc();
    Chip3_Set_Sw(1);

    if (argc == 4) // Normal
    {
        Chip3_Set_Tx2(atoi(argv[1]));
        //Chip3_Set_Tx2_H4bits(atoi(argv[1]));
        //Chip3_Set_Tx2_L4bits(31);
        Chip3_Set_Ty2(atoi(argv[2]));
        Chip3_Set_Tx1(15);
        Chip3_Set_Ty1(15);
        Chip3_Set_Cb1(7);
        Chip3_Set_Cb2(7);
        Chip3_Set_Cb3(atoi(argv[3]));
        Chip3_Set_Cb4(atoi(argv[3]));
    }
    else if (argc == 5) // Variable Gain
    {
        Chip3_Set_Tx2_H4bits(atoi(argv[1]));
        Chip3_Set_Tx2_L4bits(atoi(argv[2]));
        Chip3_Set_Ty2(atoi(argv[3]));
        Chip3_Set_Tx1(15);
        Chip3_Set_Ty1(15);
        Chip3_Set_Cb1(7);
        Chip3_Set_Cb2(7);
        Chip3_Set_Cb3(atoi(argv[4]));
        Chip3_Set_Cb4(atoi(argv[4]));
    }
    else if (argc == 6) // Two stage
    {
        Chip3_Set_Tx2_H4bits(atoi(argv[1]));
        Chip3_Set_Tx2_L4bits(atoi(argv[2]));
        Chip3_Set_Ty2(atoi(argv[3]));
        Chip3_Set_Tx1(15);
        Chip3_Set_Ty1(15);
        Chip3_Set_Cb1(7);
        Chip3_Set_Cb2(7);
        Chip3_Set_Cb3(atoi(argv[4]));
        Chip3_Set_Cb4(atoi(argv[4]));
        spi_set_res(atoi(argv[5]));
    }
    else
    {
        /// default configuration
        Chip3_Set_Tx2(15);
        Chip3_Set_Tx2(15);
        Chip3_Set_Tx1(15);
        Chip3_Set_Ty1(15);
        Chip3_Set_Cb1(7);
        Chip3_Set_Cb2(7);
        Chip3_Set_Cb3(7);
        Chip3_Set_Cb4(7);

        if (argc >= 2) test_type = atoi(argv[1]);
    }

    BackupCfg();
    Chip3_Send_Cfg_To_SCA();

    Chip3_Idx_Ctrl_Rst_Ana_Write(1);
    Chip3_Idx_Ctrl_Sta_Clk_Write(1);

    /// Sweep
    printf("Fq\tIi\tQi\tIo\tQo\tAi\tAo\tG\n");

    if (test_type == 0)
    {
        /// for Congyin bro' test
        Chip3_Sweep_fixfreq(255, MAX_READ_ITER, adc_buf);
    }
    else
    {
        /// for my own test
        uint16  freq_list[4] = {60, 71, 71, 84};
        for (i = 0; i < 4; i++)
        {
            SetFreq(freq_list[i]);
            Chip3_Send_Cfg_To_SCA();
            usleep(70);

            Chip3_Sweep_fixfreq(freq_list[i], 10240, adc_buf);
        }
    }

    Chip3_Idx_Ctrl_Sta_Clk_Write(0);
    return 0;
}