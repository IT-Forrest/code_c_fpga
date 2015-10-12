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

//#define DEBUG_ON

int main(int argc, char** argv) {

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);
    init_sc();

    ///////////////////////////////////////////////
    /// To set analog configuration: e.g. Mdiv, BS, etc

    Chip3_Set_Mdiv0(79);
    Chip3_Set_Bs0(3);
    Chip3_Set_Cap0((1<<4)+5);

    Chip3_Set_Mdiv1(86);
    Chip3_Set_Bs1(7);
    Chip3_Set_Cap1((0<<4)+5);

    Chip3_Set_Mdiv2(86);
    Chip3_Set_Bs2(7);
    Chip3_Set_Cap2((0<<4)+5);

    Chip3_Set_Mdiv3(93);
    Chip3_Set_Bs3(7);
    Chip3_Set_Cap3((1<<4)+4);

    Chip3_Set_Cb2(6);
    Chip3_Set_Cb3(6);
    Chip3_Set_Cb1(6);
    Chip3_Set_Cb4(6);

    Chip3_Set_Sw(1);

    Chip3_Set_Test_Mux(0);

    BackupCfg();
    Chip3_Send_Cfg_To_SCA();

    Chip3_Idx_Ctrl_Rst_Ana_Write(1);
    usleep(100);

    Chip3_Idx_Ctrl_Sel_A_Write(1);
    Chip3_Idx_Ctrl_Sel_B_Write(1);

    Chip3_Idx_Ctrl_Rst_N_Write(1);
    usleep(20);

    Chip3_Idx_Ctrl_Sta_Clk_Write(1);

    /// wait the CFSA process is over
    sleep(3);

    Chip3_Idx_Ctrl_Sta_Clk_Write(0);

    /// since LAT_A is already HIGH, no need to
    Chip3_Idx_Ctrl_Sta_Sc_Write(1);

    Chip3_Idx_Ctrl_Flag_A_Write(1);
    while(1 != Chip3_Idx_Stat_Scrdy_Read());

    Chip3_Idx_Ctrl_Flag_A_Write(0);
    while(0 != Chip3_Idx_Stat_Scrdy_Read());

    Chip3_Clear_Scan_Chain(1);
    BIT_SET(GSCA_HIGHEST, ((Chip3_Idx_Stat_Scso_A_Read())<< MAX_SC_A_HIGH));
    #ifdef  DEBUG_ON
    printf("SCB BIT = %d\n", Chip3_Idx_Stat_Scso_A_Read());
    #endif

    Chip3_Idx_Ctrl_Sel_A_Write(0);

    int i;
    for (i = 0; i< MAX_SC_BITS_A-1; i++)
    {
        //gscB = (gscB >> 1);
        RShiftCfg();
        Chip3_Idx_Ctrl_Flag_A_Write(1);
        while(1 != Chip3_Idx_Stat_Scrdy_Read());

        Chip3_Idx_Ctrl_Flag_A_Write(0);
        while(0 != Chip3_Idx_Stat_Scrdy_Read());

        BIT_SET(GSCA_HIGHEST, ((Chip3_Idx_Stat_Scso_A_Read())<< MAX_SC_A_HIGH));
        #ifdef DEBUG_ON
        printf("SCB BIT = %d\n", Chip3_Idx_Stat_Scso_A_Read());
        #endif
    }

    Chip3_Idx_Ctrl_Sta_Sc_Write(0);

    /// make sure to make it passby after serial
    Chip3_Idx_Ctrl_Sel_A_Write(1);

    uint32 rd_Tx1 = Chip3_Rtn_Tx1();
    uint32 rd_Ty1 = Chip3_Rtn_Ty1();
    uint32 rd_Tx2 = Chip3_Rtn_Tx2();
    uint32 rd_Ty2 = Chip3_Rtn_Ty2();

    printf("Tx2_H = %d, Tx2_L = %d\n", ((rd_Tx2&0x3e0)>>5), (rd_Tx2&0x1f));
    printf("Ty2_H = %d, Ty2_L = %d\n", ((rd_Ty2&0x3e0)>>5), (rd_Ty2&0x1f));
    printf("Tx1_H = %d, Tx1_L = %d\n", ((rd_Tx1&0x3e0)>>5), (rd_Tx1&0x1f));
    printf("Ty1_H = %d, Ty1_L = %d\n", ((rd_Ty1&0x3e0)>>5), (rd_Ty1&0x1f));

    return( clean_mem() );
}
