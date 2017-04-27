
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/serial_port_io.h"
#include "../ana_opt_2/psoc_port.h"

#define ITERNUM 225
#define SSNUM 7
#define USE_NEW_VAL     (0 != len)

int main() {
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i = 0;//,j;
    uint8   tst_type = 1;
    uint16  rd_val = 0;
    uint8   len = 0;
    uint8   clk_stop = 0;// 0: TURN on clk to chip
    uint8   cpu_rst_n = 0;// 0: reset; 1: activate
    //0: No change to the output signals                    1: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=FOUT[5:0]
    //2: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_datainA[5:0]   3: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_datainB[5:0]
    //4: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_status[5:0]    5: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_status[5:0]
    //6: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=pc_pointer[5:0]   7: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=reg_C_reg[5:0]
    uint8   test_mux_chs = 0;// 0~7 debug settings:

    uint8   ctrl_bgn = 0;// 0: CPU working path; 1: SRAM loading path
    //00: serially loop instructions to CCT; 11: CCT send instructions to SRAM parallelly
    //01: CCT fetch contents from SRAM parallelly; 10: nothing happens
    uint8   ctrl_mode = 0;
    uint8   ctrl_load = 0;// 1: Notify SRAM_CTRL to load SRAM parallelly
    uint8   cpu_bgn  = 0;// 1: Activate CPU as a pulse;
    uint8   cpu_wait = 0;// 1: Single instruction stop flag;
    uint8   app_done = 0;// 1: App done signal to unlock chip

//    uint8   low_value = 0;
//    uint8   high_value = 0;
    //int16  read_len;
    uint8   fd_str[10];
    uint16  adc_buf[10] = {0};

    if (init_mem()) return (1);
    //if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    FILE *fd = fopen("sweep_interval.cfg","r");
    if (fd == NULL)
    {
        printf("open file failed!\n");
        return (1);
    }

    //while(fscanf(fd,"%d",&fint))
    while(fgets((char*)fd_str,10,fd))
    {
        ((char*)fd_str)[9] = '\0';
        adc_buf[i] = atoi((char*)fd_str);
        i++;
    }

    //Since FPGA code are still running, registers keep the old value. Thus we clean them.
    //coe_sweep_bits = (reg_low||reg_high)?{reg_high_bits,reg_low_bits}:sweep_cntsclk[7:0];
    //SWEEP_LOWDATA_Write(low_value);
    //SWEEP_HIGHDATA_Write(high_value);

    for(;;)
    {
        printf("#CMT\r\n");
        printf("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        printf("#CMT\t$$$$ Welcome to Jiafan's World. Have fun $$$$\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t1 :Turn ON/OFF clk to chip (0~1)\r\n");
        printf("#CMT\t2 :Reset_N CPU chip (0~1)\r\n");
        printf("#CMT\t3 :Set test mux channels (0~7)\r\n");
        printf("#CMT\t4 :Switch between CPU/SRAM path (0~1)\r\n");
        printf("#CMT\t5 :   SRAM path:  Set contrl mode (0~3)\r\n");
        printf("#CMT\t6 :   SRAM path:  Load contents to SRAM (0~1)\r\n");
        printf("#CMT\t7 :CPU path:  set activation pulse (0~1)\r\n");
        printf("#CMT\t8 :CPU path:  cpu single step wait (0~1)\r\n");
        printf("#CMT\t9 :CPU path:  app_done to unlock chip (0~1)\r\n");
//        printf("#CMT\t1 :Set value to lower 3 bits (0~7)\r\n");
//        printf("#CMT\t2 :Set value to higher 5 bits (0~31)\r\n");
//        printf("#CMT\t3 :Sweep value from 0~255, and then become 0\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        printf("#CMT\tWhich Test next(");FmtPrint(tst_type);printf(")? ");
        rd_val = ReadInput(&len);
        if (USE_NEW_VAL && rd_val < 4)    tst_type = rd_val;

        printf("#CMT\t---------------------\r\n");
        //CFG_RSTN_Write(1);
        //Global_RSTN_Write(0);

//        if (1 == tst_type)
//        {
//            printf("#CMT\t\tSet value to lower 3 bits (0~7)\r\n");
//            printf("#DLC\tValue = ");
//            rd_val = ReadInput(&len);
//            if (USE_NEW_VAL)    low_value = rd_val;
//            else {printf("#DLC\tPrevious 3bit value = ");FmtPrint(low_value);printf(")\r\n");}
//
//            //clear sweep flag set flag and value
//            SWEEP_START_Write(0);
//            SWEEP_LOWDATA_Write(low_value);
//            SWEEP_SETLOW_Write(1);
//            //SWEEP_SETLOW_Write(0); since we need to keep the output
//
//            printf("#DLC\tSet 3bit value=%d done\n\n", low_value);
//        }
//        else if (2 == tst_type)
//        {
//            printf("#CMT\t\tSet value to higher 5 bits (0~31)\r\n");
//            printf("#DLC\tValue = ");
//            rd_val = ReadInput(&len);
//            if (USE_NEW_VAL)    high_value = rd_val;
//            else {printf("#DLC\tPrevious 5bit value = ");FmtPrint(high_value);printf(")\r\n");}
//
//            //clear sweep flag set flag and value
//            SWEEP_START_Write(0);
//            SWEEP_HIGHDATA_Write(high_value);
//            SWEEP_SETHIGH_Write(1);
//            //SWEEP_SETHIGH_Write(0); since we need to keep the output
//
//            printf("#DLC\tSet 5bit value=%d done\n\n", high_value);
//        }
//        else if (3 == tst_type)
//        {
//            printf("#CMT\t\tSweep value from 0~255, and then become 0\r\n\r\n");
//
//            //clear low/high values and clear LOW/HIGH flag
//            low_value = 0;
//            high_value = 0;
//            SWEEP_SETLOW_Write(0);
//            SWEEP_SETHIGH_Write(0);
//            SWEEP_LOWDATA_Write(low_value);
//            SWEEP_HIGHDATA_Write(high_value);
//
//            //set flag and then configuration
//            SWEEP_CNTSCLK_Write(adc_buf[0]);//the interval value
//            SWEEP_START_Write(1);
//            while(!SWEEP_RDY_Read());
//            SWEEP_START_Write(0);
//
//            printf("#DLC\tSweep done\n\n");
//        }
//        else
        if (1 == tst_type) {
            printf("#CMT\t\tTurn ON/OFF clk to chip (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    clk_stop = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(clk_stop);printf(")\r\n");}

            Chip4_Idx_Scpu_Clk_Stop_Write(clk_stop);
            if (clk_stop) printf("#DLC\tTurn OFF clk to chip\n\n");
            else printf("#DLC\tTurn ON clk to chip\n\n");
        }
        else if (2 == tst_type) {
            printf("#CMT\t9 :Reset the CPU at CPU path (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    cpu_rst_n = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(cpu_rst_n);printf(")\r\n");}

            Chip4_Idx_Scpu_Rst_N_Write(cpu_rst_n);
            if (cpu_rst_n) printf("#DLC\tCPU is enable\n\n");
            else printf("#DLC\tCPU is disable\n\n");
        }
        else if (3 == tst_type) {
            printf("#CMT\t3 :Set test mux channels (0~7)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    test_mux_chs = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(test_mux_chs);printf(")\r\n");}

            Chip4_Idx_Scpu_Test_Mux_Write(test_mux_chs);
            if (0 == test_mux_chs) printf("#DLC\tNo change to the output signals\n\n");
            else if (1 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO}=FOUT[5:0]\n\n");
            else if (2 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_datainA[5:0]\n\n");
            else if (3 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_datainB[5:0]\n\n");
            else if (4 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_status[5:0]\n\n");
            else if (5 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_status[5:0]\n\n");
            else if (6 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO}=pc_pointer[5:0]\n\n");
            else if (7 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO}=reg_C_reg[5:0]\n\n");
            else printf("#DLC\tOooops, nothing happens...\n\n");
        }
        else if (4 == tst_type) {
            printf("#CMT\t4 :Switch between CPU/SRAM path (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    ctrl_bgn = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(ctrl_bgn);printf(")\r\n");}

            Chip4_Idx_Scpu_Ctrl_Bgn_Write(ctrl_bgn);
            if (ctrl_bgn) printf("#DLC\tTurn on SRAM loading path\n\n");
            else printf("#DLC\tTurn on CPU working path\n\n");
        }
        else if (5 == tst_type) {
            printf("#CMT\t5 :Set contrl mode at SRAM path (0~3)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    ctrl_mode = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(ctrl_mode);printf(")\r\n");}

            Chip4_Idx_Scpu_Ctrl_Mod_Write(ctrl_mode);
            //00: serially loop instructions to CCT; 11: CCT send instructions to SRAM parallelly
            //01: CCT fetch contents from SRAM parallelly; 10: nothing happens
            if (0 == ctrl_mode) printf("#DLC\tserially loop instructions to CCT\n\n");
            else if (1 == ctrl_mode) printf("#DLC\tCCT send instructions to SRAM parallelly\n\n");
            else if (3 == ctrl_mode) printf("#DLC\tCCT fetch contents from SRAM parallelly\n\n");
            else printf("#DLC\tOooops, nothing happens...\n\n");
        }
        else if (6 == tst_type) {
            printf("#CMT\t6 :Notify SRAM_CTRL to load SRAM (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    ctrl_load = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(ctrl_load);printf(")\r\n");}

            Chip4_Idx_Scpu_Ctrl_Load_Write(ctrl_load);
            if (ctrl_load) printf("#DLC\tActivate the SRAM loading\n\n");
            else printf("#DLC\tFinish the SRAM loading\n\n");
        }
        else if (7 == tst_type) {
            printf("#CMT\t7 :CPU path:  set activation pulse (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    cpu_bgn = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(cpu_bgn);printf(")\r\n");}

            Chip4_Idx_Scpu_Cpu_Bgn_Write(cpu_bgn);
            if (cpu_bgn) printf("#DLC\tActivate a pulse to CPU\n\n");
            else printf("#DLC\tFinish a pulse to CPU\n\n");
        }
        else if (8 == tst_type) {
            printf("#CMT\t8 :CPU path:  cpu single step wait (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    cpu_wait = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(cpu_wait);printf(")\r\n");}

            Chip4_Idx_Scpu_Cpu_Wait_Write(cpu_wait);
            if (cpu_wait) printf("#DLC\tActivate a pulse to CPU\n\n");
            else printf("#DLC\tFinish a pulse to CPU\n\n");
        }
        else if (9 == tst_type) {
            printf("#CMT\t9 :CPU path:  app_done to unlock chip (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    app_done = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(app_done);printf(")\r\n");}

            Chip4_Idx_Scpu_App_Done_Write(app_done);
            if (app_done) printf("#DLC\tApp done signal is valid\n\n");
            else printf("#DLC\tApp done signal is invalid\n\n");
        }
        else
        {
            printf("#CMT\t\t No this kind of test right now...(");FmtPrint(tst_type);printf(")\r\n");
        }

        printf("#CMT\t---------- press any key to continue -----------\r\n");

	    PutWait();
    }

    return( clean_mem() );
}
