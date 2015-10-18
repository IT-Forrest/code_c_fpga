//+FHDR****************************************************************
// ECE department, TAMU
// --------------------------------------------------------------------
// FILE NAME    : test_chip3_rd_adc.c
// AUTHER       : Jiafan Wang
// DATE         : 09/6/2015
// VERSION      : 1.0
// PURPOSE      : the scan chain test of the tapeout 3 chip
// --------------------------------------------------------------------
// ABSTRACT
//
// This file works for Send Configuration to Scan Chain A and then read ADC from Scan chain B
// --------------------------------------------------------------------

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
    uint16  Midv0 = 0;
    uint16  Bs0 = 0;
    uint16  Cap0 = 0;

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);
    init_sc();

    ///////////////////////////////////////////////
    /// To set analog configuration: e.g. Mdiv, BS, etc
    ///////////////////////////////////////////////
    if (argc == 4)
    {
        Midv0 = atoi(argv[1]);
        Bs0 = atoi(argv[2]);
        Cap0 = atoi(argv[3]);

        Chip3_Set_Mdiv0(Midv0);
        Chip3_Set_Bs0(Bs0);
        Chip3_Set_Cap0(Cap0);
    }
    else
    {
        /// default configuration
        SetFreq(127);
        Chip3_Set_Cap0(31);
    }

    BackupCfg();
    Chip3_Send_Cfg_To_SCA();

    Chip3_Idx_Ctrl_Rst_Ana_Write(1);
    usleep(100);

    uint16 rd_val;
    uint16 i;

    for (i=0; i<32; i++)
    {
        rd_val = Chip3_Cfg_Read_ADC(0, 0, 0, 0);
        usleep(20);
        printf("%u\n", rd_val);
    }

    return( clean_mem() );
}
