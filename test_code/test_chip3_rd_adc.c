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
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/set_config.h"
#include "../ana_opt_2/scan_chain.h"
#include "../ana_opt_2/serial_port_io.h"
#include "../ana_opt_2/psoc_port.h"

#define ITERNUM 225
#define SSNUM 7
//#define DEBUG_ON

/// Send Configuration to Scan chain A
void    Chip3_Send_Cfg_To_SCA()
{
    Chip3_Idx_Ctrl_Sel_A_Write(0);

    Chip3_Idx_Ctrl_Lat_A_Write(0);
    Chip3_Idx_Ctrl_Sta_Sc_Write(1);

    ///////////////////// Scan chain input /////////////////////
    //// input Scan chain A
    int i;
    for (i = 0; i < MAX_SC_BITS_A; i++)
    {
        Chip3_Idx_Ctrl_Sin_Ab_Write(gcfg_tx[0]);
        RShiftCfg();
        Chip3_Idx_Ctrl_Flag_A_Write(1);
        while(1 != Chip3_Idx_Stat_Scrdy_Read());

        Chip3_Idx_Ctrl_Flag_A_Write(0);
        while(0 != Chip3_Idx_Stat_Scrdy_Read());
    }

    /// when set the latch signal, no need to send clock
    Chip3_Idx_Ctrl_Lat_A_Write(1);
    usleep(2);
    Chip3_Idx_Ctrl_Lat_A_Write(0);

    Chip3_Idx_Ctrl_Sta_Sc_Write(0);

    return;
}


int main(int argc, char** argv) {
    uint16  tunex1;
    uint16  tunex2;
    uint16  Midv0 = 0;
    uint16  Bs0 = 0;
    uint16  Cap0 = 0;

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    /// Disable global clock and CFSA at the very beginning
    Chip3_Idx_Ctrl_Sta_Clk_Write(0);
    Chip3_Idx_Ctrl_Rst_N_Write(0);

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
        Chip3_Set_Mdiv0(108);
        Chip3_Set_Bs0(15);
        Chip3_Set_Cap0(31);
    }

    Chip3_Set_Mdiv1(127);
    Chip3_Set_Bs1(0);
    Chip3_Set_Cap1(0);

    Chip3_Set_Mdiv2(127);
    Chip3_Set_Bs2(0);
    Chip3_Set_Cap2(0);

    Chip3_Set_Mdiv3(127);
    Chip3_Set_Bs3(0);
    Chip3_Set_Cap3(0);
    Chip3_Set_Fs(0);

    ///////////////////////////////////////////////
    /// To set digital configuration: e.g. SA, SS etc
    ///////////////////////////////////////////////
    tunex1 = 15;
    tunex2 = 15;

    Chip3_Set_Dbfactor0(0x100 | 106);// Set the constant for 3dB points;
    Chip3_Set_Dbfactor1(0x100 | 106);
    Chip3_Set_Threshold(100);   // Set the threshold for Multi-start SS; if ANA > threshold, choose a new start
    Chip3_Set_Factors(0x8B);    // Set weighting factors; 8'b10_00_11_01;//Gain3-Gain2-Gain1-Gain0

    Chip3_Set_Avrg_Num(1);      // Set # of times when compute the average ADC
    Chip3_Set_Rand_Group(1);
    Chip3_Set_Div_Accu(1);      // Set Franction length in 9-bit Division module. 1: 2:7; 0: 3:6;
    Chip3_Set_Flag4D(0);        // Set 2D/4D test: 1:4D; 0:2D
    Chip3_Set_Opt_Mode(1);      // Set optimization engine mode; 1: Hybrid

    Chip3_Set_Step_T(4);        // Set T step = 4; each time T=T-T_step;
    Chip3_Set_Init_T(32);       // Set initial T = 32

    Chip3_Set_Ss_Iter(SSNUM);   // Set Sensitivity time = 4
    Chip3_Set_Sa_Iter(ITERNUM); // Set SA iteration = ITERNUM
    Chip3_Set_Init_X1(15);
    Chip3_Set_Init_Y1(15);
    Chip3_Set_Init_X2(15);
    Chip3_Set_Init_Y2(15);

    Chip3_Set_Tx1(31);
    Chip3_Set_Tx2(31);
    Chip3_Set_Ty1(31);
    Chip3_Set_Ty2(31);
    Chip3_Set_Cb1(7);
    Chip3_Set_Cb2(7);
    Chip3_Set_Cb3(7);
    Chip3_Set_Cb4(7);
    Chip3_Set_Sw(1);

    Chip3_Set_Nxt_Test(0);
    Chip3_Set_Trdy_Test(0);
    Chip3_Set_Aoff(507);

    Chip3_Set_Tol(63);
    Chip3_Set_Trg_Test(0);
    Chip3_Set_Test_Mux(1);  // for debug

    /// control word configuration
    Chip3_Set_Cal(1);
    Chip3_Set_Phs(0);
    Chip3_Set_Src(0);
    Chip3_Set_Oscd(0);

    BackupCfg();
    TranxCfg();

    //////////////////////////////////////////////
    /// Activate the Scan chain and load Configuration
    ///////////////////////////////////////////////
    Chip3_Idx_Ctrl_Rst_Ana_Write(0);/// RST_ANA is needed only when MDIV is changed
    Chip3_Send_Cfg_To_SCA();
    Chip3_Idx_Ctrl_Rst_Ana_Write(1);

    Chip3_Set_Trg_Test(0);
    BackupCfg();
    TranxCfg();
    Chip3_Send_Cfg_To_SCA();

    ///////////////// Read ADC signals //////////////////

    Chip3_Idx_Ctrl_Sel_B_Write(1);

    Chip3_Idx_Ctrl_Sta_Clk_Write(1);
    // Waiting ADC ready by usleep since no signal to indicate when is ready
    usleep(500);
    Chip3_Idx_Ctrl_Sta_Clk_Write(0);

    Chip3_Idx_Ctrl_Lat_B_Write(1);
    usleep(2);
    Chip3_Idx_Ctrl_Lat_B_Write(0);


    Chip3_Idx_Ctrl_Sta_Sc_Write(1);

    Chip3_Idx_Ctrl_Flag_B_Write(1);
    while(1 != Chip3_Idx_Stat_Scrdy_Read());

    Chip3_Idx_Ctrl_Flag_B_Write(0);
    while(0 != Chip3_Idx_Stat_Scrdy_Read());

    gscB = 0;
    BIT_SET(gscB, ((Chip3_Idx_Stat_Scso_B_Read())<< (MAX_SC_BITS_B-1)));
    #ifdef  DEBUG_ON
    printf("SCB BIT = %d\n", Chip3_Idx_Stat_Scso_B_Read());
    #endif

    Chip3_Idx_Ctrl_Sel_B_Write(0);

    int i;
    for (i = 0; i< MAX_SC_BITS_B-1; i++)
    {
        gscB = (gscB >> 1);
        Chip3_Idx_Ctrl_Flag_B_Write(1);
        while(1 != Chip3_Idx_Stat_Scrdy_Read());

        Chip3_Idx_Ctrl_Flag_B_Write(0);
        while(0 != Chip3_Idx_Stat_Scrdy_Read());

        BIT_SET(gscB, ((Chip3_Idx_Stat_Scso_B_Read())<< (MAX_SC_BITS_B-1)));
        #ifdef DEBUG_ON
        printf("SCB BIT = %d\n", Chip3_Idx_Stat_Scso_B_Read());
        #endif
    }

    Chip3_Idx_Ctrl_Sta_Sc_Write(0);
    printf("ADC = %d\n", Chip3_Rtn_Adc());
    printf("FNL = %d\n", (CHIP3_CHK_ADC_FNL?1:0));
    printf("RSTN_ADC = %d\n", (CHIP3_CHK_RSTN_ADC?1:0));
    printf("CLRN = %d\n", (CHIP3_CHK_CLRN?1:0));

    return( clean_mem() );
}
