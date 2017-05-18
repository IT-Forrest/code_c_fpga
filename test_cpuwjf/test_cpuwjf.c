
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
#include "conf_cpuwjf.h"

#define ITERNUM 225
#define SSNUM 7
#define USE_NEW_VAL     (0 != len)
#define DEFAULT_PC_ADDR (16)
#define MAX_SRAM_LEN    (1024)

int main() {
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i,j;
    uint16  err_cnt = 0;
    uint8   tst_type = 1;
    uint16  rd_val = 0;
    uint8   len = 0;

    FILE    *fd;
    //char8   line[18];//2 bits more than instruction binary arrays
    uint8   clk_stop = 0;// 0: TURN on clk to chip
    uint8   cpu_rst_n = 0;// 0: reset; 1: activate
    //0: No change to the output signals                    1: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=FOUT[5:0]
    //2: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_datainA[5:0]   3: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_datainB[5:0]
    //4: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_status[5:0]    5: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=io_status[5:0]
    //6: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=pc_pointer[5:0]   7: {NXT, SCLK1, SCLK2, LAT, SPI_SO}=reg_C_reg[5:0]
    uint8   test_mux_chs = 0;// 0~7 debug settings:

    uint8   ctrl_bgn = 0;// 0: CPU working path; 1: RAM loading path
    //00: serially loop instructions to CCT; 11: CCT send instructions to SRAM parallelly
    //01: CCT fetch contents from SRAM parallelly; 10: nothing happens
    uint8   ctrl_mode = 0;
    uint8   ctrl_load = 0;// 1: Notify SRAM_CTRL to load SRAM parallelly
    uint8   cpu_bgn  = 0;// 1: Activate CPU as a pulse;
    uint8   cpu_wait = 0;// 1: Single instruction stop flag;
    uint8   app_done = 0;// 1: App done signal to unlock chip
    uint16  sram_addr = 0;// 0~1023
    uint8   sram_data = 0;// 0~255
    uint16  cnt_clk = 0;// 0,1,2,...256; 0: means no change to the original freq; 1:means 1/2 freq; 2: means 1/4 freq;

//    uint8   low_value = 0;
//    uint8   high_value = 0;
    //int16  read_len;
    uint8   sram_buf[MAX_SRAM_LEN];/// SRAM 1024X8, or say, 512X16
    int16   inst_num = 0;
    uint16  inst_val = 0;
    uint16  addr_tmp = 0;
    //uint16  adc_buf[10] = {0};

    if (init_mem()) return (1);
    if (syn_ctrl()) return (1);

//    FILE *fd = fopen("sweep_interval.cfg","r");
//    if (fd == NULL)
//    {
//        printf("open file failed!\n");
//        return (1);
//    }
//
//    //while(fscanf(fd,"%d",&fint))
//    while(fgets((char*)fd_str,10,fd))
//    {
//        ((char*)fd_str)[9] = '\0';
//        adc_buf[i] = atoi((char*)fd_str);
//        i++;
//    }

    for(;;)
    {
        printf("#CMT\r\n");
        printf("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        printf("#CMT\t$$$$ Welcome to Jiafan's World. Have fun $$$$\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t0 :Read chip status\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t1 :Turn ON/OFF clk to chip (0~1)\r\n");
        printf("#CMT\t2 :Reset_N CPU chip (0~1)\r\n");
        printf("#CMT\t3 :Set test mux channels (0~7)\r\n");
        printf("#CMT\t4 :Switch between CPU/RAM path (0~1)\r\n");
        printf("#CMT\t5 :   RAM path:  Set contrl mode (0~3)\r\n");
        printf("#CMT\t6 :   RAM path:  Load contents to RAM (0~1)\r\n");
        printf("#CMT\t7 :   CPU path:  set activation pulse (0~1)\r\n");
        printf("#CMT\t8 :   CPU path:  cpu single step wait (0~1)\r\n");
        printf("#CMT\t9 :   CPU path:  app_done to unlock chip (0~1)\r\n");
        printf("#CMT\t10:Set SRAM address (0~1023)\r\n");
        printf("#CMT\t11:Set SRAM content (0~255)\r\n");
        printf("#CMT\t12:Bi-split chip freq (0,1,2,3,...255)\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t13:Testcase 1:  instruction to Memory\r\n");
        printf("#CMT\t14:Testcase 2:  instruction to suspend cpu\r\n");
        printf("#CMT\t15:Testcase 3:  instruction for sum_1+2+3+4+...100\r\n");
        printf("#CMT\t16:Testcase 4:  Write/Read many instructions to SRAM\r\n");
        printf("#CMT\t17:Testcase 5:  shift out instruction from CTRL \r\n");
        printf("#CMT\t18:Testcase 6:  Loop in/out Test\r\n");
        printf("#CMT\t19:Testcase 7:  Loop in/Write SRAM/Export out Test\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        printf("#CMT\tWhich Test next(");FmtPrint(tst_type);printf(")? ");
        rd_val = ReadInput(&len);
        if (USE_NEW_VAL)    tst_type = rd_val;

        printf("#CMT\t---------------------\r\n");
        //CFG_RSTN_Write(1);
        //Global_RSTN_Write(0);

        if (0 == tst_type) {
            printf("#CMT\t0 :Read chip status\r\n");
            if (ctrl_bgn) printf("#DLC\tCPU mode\r\n");
            else printf("#DLC\tRAM mode\r\n");
            printf("#DLC\tAddr_rd = 0x%.3x\tAddr_wr = 0x%.3x\r\n", Chip4_CCT_Sram_Addr_Read(), sram_addr);
            printf("#DLC\tData_rd = 0x%.2x\tData_wr = 0x%.2x\r\n", Chip4_CCT_Sram_Data_Read(), sram_data);
            printf("#DLC\tCtrl_mode[1:0] = %d%d\r\n", ctrl_mode & 0x02, ctrl_mode & 0x01);
            printf("#DLC\tCtrl_So = %d\r\n", Chip4_SCPU_Idx_Ctrl_So());
            printf("#DLC\tApp_Start = %d\r\n", Chip4_SCPU_Idx_App_Start());
            printf("#DLC\tNxt_Cont = %d\r\n", Chip4_SCPU_Idx_Nxt_Cont());
            printf("#DLC\tNxt_End = %d\r\n", Chip4_SCPU_Idx_Nxt_End());
            printf("#DLC\tCtrl_Rdy = %d\r\n", Chip4_SCPU_Idx_Ctrl_Rdy());
            printf("#DLC\tCpu_Status = 0x%.2x\r\n", Chip4_Cpu_Stat_Read());
            printf("#DLC\tSC_bits(13) = 0x%.4x\r\n", Chip4_Scan_Chain_Read());
        }
        else if (1 == tst_type) {
            printf("#CMT\t1 :Turn ON/OFF clk to chip (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    clk_stop = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(clk_stop);printf("\r\n");}

            Chip4_Idx_Scpu_Clk_Stop_Write(clk_stop);
            if (clk_stop) printf("#DLC\tTurn OFF clk to chip\n\n");
            else printf("#DLC\tTurn ON clk to chip\n\n");
        }
        else if (2 == tst_type) {
            printf("#CMT\t2 :Reset_N CPU chip (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    cpu_rst_n = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(cpu_rst_n);printf("\r\n");}

            Chip4_Idx_Scpu_Rst_N_Write(cpu_rst_n);
            if (cpu_rst_n) printf("#DLC\tCPU is enable\n\n");
            else printf("#DLC\tCPU is disable\n\n");
        }
        else if (3 == tst_type) {
            printf("#CMT\t3 :Set test mux channels (0~7)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    test_mux_chs = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(test_mux_chs);printf("\r\n");}

            Chip4_Idx_Scpu_Test_Mux_Write(test_mux_chs);
            if (0 == test_mux_chs) printf("#DLC\t{NXT, SCLK1, SCLK2, LAT, SPI_SO} has No change\n\n");
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
            printf("#CMT\t4 :Switch between CPU/RAM path (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    ctrl_bgn = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(ctrl_bgn);printf("\r\n");}

            Chip4_Idx_Scpu_Ctrl_Bgn_Write(ctrl_bgn);
            if (ctrl_bgn) printf("#DLC\tTurn on SRAM loading path\n\n");
            else printf("#DLC\tTurn on CPU working path\n\n");
        }
        else if (5 == tst_type) {
            printf("#CMT\t5 :   RAM path:  Set contrl mode (0~3)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    ctrl_mode = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(ctrl_mode);printf("\r\n");}

            Chip4_Idx_Scpu_Ctrl_Mod_Write(ctrl_mode);
            //00: serially loop instructions to CCT; 11: CCT send instructions to SRAM parallelly
            //01: CCT fetch contents from SRAM parallelly; 10: nothing happens
            if (0 == ctrl_mode) printf("#DLC\tserially loop instructions to CCT\n\n");
            else if (1 == ctrl_mode) printf("#DLC\tCCT send instructions to SRAM parallelly\n\n");
            else if (3 == ctrl_mode) printf("#DLC\tCCT fetch contents from SRAM parallelly\n\n");
            else printf("#DLC\tOooops, nothing happens...\n\n");
        }
        else if (6 == tst_type) {
            printf("#CMT\t6 :   RAM path:  Load contents to RAM (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    ctrl_load = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(ctrl_load);printf("\r\n");}

            Chip4_Idx_Scpu_Ctrl_Load_Write(ctrl_load);
            if (ctrl_load) printf("#DLC\tSRAM loading signal is valid\n\n");
            else printf("#DLC\tSRAM loading signal is invalid\n\n");
        }
        else if (7 == tst_type) {
            printf("#CMT\t7 :   CPU path:  set activation pulse (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    cpu_bgn = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(cpu_bgn);printf("\r\n");}

            Chip4_Idx_Scpu_Cpu_Bgn_Write(cpu_bgn);
            if (cpu_bgn) printf("#DLC\tActivate a pulse to CPU\n\n");
            else printf("#DLC\tFinish a pulse to CPU\n\n");
        }
        else if (8 == tst_type) {
            printf("#CMT\t8 :   CPU path:  cpu single step wait (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    cpu_wait = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(cpu_wait);printf("\r\n");}

            Chip4_Idx_Scpu_Cpu_Wait_Write(cpu_wait);
            if (cpu_wait) printf("#DLC\tActivate a pulse to CPU\n\n");
            else printf("#DLC\tFinish a pulse to CPU\n\n");
        }
        else if (9 == tst_type) {
            printf("#CMT\t9 :   CPU path:  app_done to unlock chip (0~1)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    app_done = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(app_done);printf("\r\n");}

            Chip4_Idx_Scpu_App_Done_Write(app_done);
            if (app_done) printf("#DLC\tApp done signal is valid\n\n");
            else printf("#DLC\tApp done signal is invalid\n\n");
        }
        else if (10 == tst_type) {
            printf("#CMT\t10:Set SRAM address (0~1023)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    sram_addr = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(sram_addr);printf("\r\n");}

            Chip4_SCPU_SRAM_ADDR_Write(sram_addr);
            printf("#DLC\tSRAM address = 0x%.3x\n\n", sram_addr);
        }
        else if (11 == tst_type) {
            printf("#CMT\t11:Set SRAM content (0~255)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    sram_data = rd_val;
            else {printf("#DLC\tPrevious value = ");FmtPrint(sram_data);printf("\r\n");}

            Chip4_SCPU_SRAM_DATA_Write(sram_data);
            printf("#DLC\tSRAM data = 0x%.2x\n\n", sram_data);
        }
        else if (12 == tst_type) {
            printf("#CMT\t12:Bi-split chip freq (0,1,2,3,...255)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    cnt_clk = rd_val;
            else {
                printf("#DLC\tPrevious value = ");
                if (0 == cnt_clk) {
                    FmtPrint(50);
                    printf(" MHz\r\n");
                } else {
                    FmtPrint(50*1000/(cnt_clk*2));//pow(2,cnt_clk)
                    printf(" kHz\r\n");
                }
            }

            if (0 == cnt_clk) {
                Chip4_Idx_Scpu_Clk_Freq_Chg_Write(0);
                printf("#DLC\tClk freq = 50 MHz\n\n");
            } else {
                Chip4_Idx_Scpu_Clk_Freq_Chg_Write(1);
                Chip4_SCPU_CNT_SCLK_Write(cnt_clk - 1);
                printf("#DLC\tSRAM data = %d kHz\n\n", 50*1000/(cnt_clk*2));
            }
        }
        else if (13 == tst_type) {
            printf("#CMT\t13:Testcase 1:  instruction to Memory\r\n");
            printf("#DLC\tRead instructions...\r\n");
            fd = fopen("testcase_1.bin", "r");

            if (fd == NULL) {
                printf("open file failed!\n");
            }
            else {
                inst_num = rd_bfile_to_mem_buf(fd, sram_buf, DEFAULT_PC_ADDR);
                printf("Total word(s) = %d\n", inst_num);
                fclose(fd);
            }
        }
        else if (14 == tst_type) {
            printf("#CMT\t14:Testcase 2:  instruction to suspend cpu\r\n");
            printf("#DLC\tRead instructions...\r\n");
            //fd = fopen("testcase_3.bin", "r");
            sram_buf[0] = 0x80;//10000000
            sram_buf[1] = 0xc8;//10111001
            sram_buf[2] = 0x00;//10000000
            sram_buf[3] = 0xc8;//10111001
            sram_buf[4] = 0x00;//00000000
            sram_buf[5] = 0x08;//00001000

            Chip4_Idx_Scpu_Rst_N_Write(0);
            Chip4_Idx_Scpu_Rst_N_Write(1);

            for (i=0; i<3; ++i) {
                for (j=2; j>=1; --j) {
                    Chip4_Idx_Scpu_Ctrl_Bgn_Write(1);
                    Chip4_Idx_Scpu_Ctrl_Mod_Write(0);

                    addr_tmp = 2*i+j-1;
                    Chip4_SCPU_SRAM_ADDR_Write(addr_tmp);
                    Chip4_SCPU_SRAM_DATA_Write(sram_buf[addr_tmp]);
                    printf("Addr 0x%.3x\t0x%.2x\r\n", addr_tmp, sram_buf[addr_tmp]);
                    /// swift instructions and address to CTRL module
                    Chip4_Idx_Scpu_Ctrl_Load_Write(1);
                    while (!Chip4_SCPU_Idx_Ctrl_Rdy());

                    printf("Clear Ctrl Bgn and Load\r\n");
                    Chip4_Idx_Scpu_Ctrl_Bgn_Write(0);
                    Chip4_Idx_Scpu_Ctrl_Load_Write(0);
                    while (Chip4_SCPU_Idx_Ctrl_Rdy());

                    /// notify CTRL send instructions to SRAM
                    printf("Notify Ctrl Send data to SRAM\r\n");
                    Chip4_Idx_Scpu_Ctrl_Bgn_Write(1);
                    Chip4_Idx_Scpu_Ctrl_Mod_Write(3);
                    Chip4_Idx_Scpu_Ctrl_Load_Write(1);
                    while (!Chip4_SCPU_Idx_Ctrl_Rdy());

                    printf("Clear Ctrl Bgn and Load again\r\n");
                    Chip4_Idx_Scpu_Ctrl_Bgn_Write(0);
                    Chip4_Idx_Scpu_Ctrl_Load_Write(0);
                    while (Chip4_SCPU_Idx_Ctrl_Rdy());
                }
            }

            /// activate the CPU
            Chip4_Idx_Scpu_Ctrl_Bgn_Write(0);
            Chip4_Idx_Scpu_Cpu_Bgn_Write(1);
            Chip4_Idx_Scpu_Cpu_Bgn_Write(0);

            while (!Chip4_SCPU_Idx_App_Start());
            printf("#DLC\tApp_Start = %d\r\n", Chip4_SCPU_Idx_App_Start());
            sleep(10);
            printf("#DLC\tApp_Start = %d\r\n", Chip4_SCPU_Idx_App_Start());
            sleep(10);
            printf("#DLC\tApp_Start = %d\r\n", Chip4_SCPU_Idx_App_Start());

            /// send app done signal and resume CPU
            Chip4_Idx_Scpu_App_Done_Write(1);
            while (Chip4_SCPU_Idx_App_Start());
            Chip4_Idx_Scpu_App_Done_Write(0);

            while(!Chip4_SCPU_Idx_Nxt_End());
            printf("#DLC\tApp_Start = %d\r\n", Chip4_SCPU_Idx_Nxt_Cont());
            sleep(10);
            printf("#DLC\tApp_Start = %d\r\n", Chip4_SCPU_Idx_Nxt_Cont());
            sleep(10);
            printf("#DLC\tApp_Start = %d\r\n", Chip4_SCPU_Idx_Nxt_Cont());
            printf("#DLC\tCPU process finish\r\n");
        }
        else if (15 == tst_type) {
            printf("#CMT\t15:Testcase 3:  instruction for sum_1+2+3+4+...100\r\n");
            printf("#DLC\tRead instructions...\r\n");
            fd = fopen("testcase_3.bin", "r");

            if (fd == NULL) {
                printf("open file failed!\n");
            }
            else {
                inst_num = rd_bfile_to_mem_buf(fd, sram_buf, DEFAULT_PC_ADDR);
                printf("Total instruction(s) = %d DWORD\n", inst_num);

                err_cnt = 0;
                cnt_clk = 1;
                Chip4_SCPU_CNT_SCLK_Write(cnt_clk - 1);
                printf("#DLC\tSRAM data = %d kHz\n\n", 50*1000/(cnt_clk*2));
                Chip4_Idx_Scpu_Clk_Freq_Chg_Write(1);
                //sleep(1);
                Chip4_Idx_Scpu_Clk_Discrt_Write(1);

                /// initialize
                Chip4_Idx_Scpu_Rst_N_Write(0);
                //Chip4_Idx_Scpu_Rst_N_Write(1);

                //Initialize one cycle
                Chip4_Idx_Scpu_Clk_1Time_Write(1);
                Chip4_Idx_Scpu_Clk_1Time_Write(0);
                for (i=0; i<(inst_num-1)+DEFAULT_PC_ADDR; ) {
                    for (j=2; j>=1; --j) {
                        addr_tmp = 2*i+j-1;
                        Chip4_SCPU_SRAM_ADDR_Write(addr_tmp);
                        Chip4_SCPU_SRAM_DATA_Write(sram_buf[addr_tmp]);
                        printf("Addr 0x%.3x\t0x%.2x\r\n", addr_tmp, sram_buf[addr_tmp]);

                        /// swift instructions and address to CTRL module
                        conf_ctrl_flag_value(0);
                        wait_ctrl_flag_clean();

                        /// notify CTRL send instructions to SRAM
                        conf_ctrl_flag_value(3);
                        wait_ctrl_flag_clean();
                    }

                    if (0 == i) {
                        i = DEFAULT_PC_ADDR;
                    } else {
                        ++i;
                    }
                }

                /// activate the CPU
                printf("#DLC\tActivate CPU...\r\n");
                Chip4_Idx_Scpu_Clk_Discrt_Write(0);
                Chip4_Idx_Scpu_Rst_N_Write(1);

                Chip4_Idx_Scpu_Ctrl_Bgn_Write(0);
                Chip4_Idx_Scpu_Cpu_Bgn_Write(1);
                Chip4_Idx_Scpu_Cpu_Bgn_Write(0);

                /// waiting for finish
                while(!Chip4_SCPU_Idx_Nxt_End());
                printf("#DLC\tCPU Process finish!\r\n");

                /// fetch result from sram
                Chip4_Idx_Scpu_Rst_N_Write(0);
                Chip4_Idx_Scpu_Clk_Discrt_Write(1);

                inst_val = 0;
                for (i = 2; i <3; ++i) {
                    for (j=2; j >=1; --j) {
                        addr_tmp = 2*i+j-1;
                        Chip4_SCPU_SRAM_ADDR_Write(addr_tmp);
                        Chip4_SCPU_SRAM_DATA_Write(0xff);

                        conf_ctrl_flag_value(0);
                        printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
                        wait_ctrl_flag_clean();

                        /// read data from SRAM
                        conf_ctrl_flag_value(1);
                        printf("#DLC\tRead Ctrl_Rdy=1\r\n");
                        wait_ctrl_flag_clean();

                        /// export sram data to fpga
                        Chip4_SCPU_SRAM_ADDR_Write(0x00);
                        Chip4_SCPU_SRAM_DATA_Write(0x00);
                        conf_ctrl_flag_value(2);
                        printf("#DLC\tExport Ctrl_Rdy=1\r\n");
                        wait_ctrl_flag_clean();

                        if (1 == j) {
                            inst_val |= (0x000000ff & Chip4_CCT_Sram_Data_Read());
                            printf("#DLC\tSram Data 0x%x\r\n", Chip4_CCT_Sram_Data_Read());
                        } else { // (2 == j)
                            inst_val |= ((0x000000ff & Chip4_CCT_Sram_Data_Read()) << 8);
                            printf("#DLC\tSram Data 0x%x\r\n", Chip4_CCT_Sram_Data_Read());
                        }
                    }
                }
                if (5050 == inst_val) {
                    printf("#DLC\tSum = %d\r\n", inst_val);
                    printf("#DLC\tTest sum_1+2+3+4+...100 passed!\r\n");
                } else {
                    printf("#DLC\tSum = %d\r\n", inst_val);
                    printf("#DLC\tTest sum_1+2+3+4+...100 failed!\r\n");
                }

                fclose(fd);
            }
        }
        else if (16 == tst_type) {
            printf("#CMT\t16:Testcase 4:  Write/Read many instructions to SRAM\r\n");
            printf("#DLC\tRead instructions...\r\n");
            //fd = fopen("testcase_3.bin", "r");
            sram_buf[0 ] = 0x10;//11000000  00010000
            sram_buf[1 ] = 0xc0;
            sram_buf[2 ] = 0x04;//10111011  00000100
            sram_buf[3 ] = 0xbb;
            sram_buf[4 ] = 0x00;//10111001  00000000
            sram_buf[5 ] = 0xb9;
            sram_buf[6 ] = 0x13;//01000001  00010011
            sram_buf[7 ] = 0x41;
            sram_buf[8 ] = 0x01;//01011011  00000001
            sram_buf[9 ] = 0x5b;
            sram_buf[10] = 0x12;//11011000  00010010
            sram_buf[11] = 0xd8;
            sram_buf[12] = 0x02;//00011001  00000010
            sram_buf[13] = 0x19;
            sram_buf[14] = 0x22;//01010010  00100010
            sram_buf[15] = 0x52;
            sram_buf[16] = 0x03;//10111010  00000011
            sram_buf[17] = 0xba;
            sram_buf[18] = 0x33;//01010011  00110011
            sram_buf[19] = 0x53;
            sram_buf[20] = 0x02;//10111011  00000010
            sram_buf[21] = 0xbb;
            sram_buf[22] = 0x40;//10111001  01000000
            sram_buf[23] = 0xb9;
            sram_buf[24] = 0x00;//10111001  00000000
            sram_buf[25] = 0xb9;
            sram_buf[26] = 0x00;//00001000  00000000
            sram_buf[27] = 0x08;

            err_cnt = 0;
            cnt_clk = 1;
            Chip4_SCPU_CNT_SCLK_Write(cnt_clk - 1);
            printf("#DLC\tSRAM data = %d kHz\n\n", 50*1000/(cnt_clk*2));
            Chip4_Idx_Scpu_Clk_Freq_Chg_Write(1);
            //sleep(1);
            Chip4_Idx_Scpu_Clk_Discrt_Write(1);

            /// initialize
            Chip4_Idx_Scpu_Rst_N_Write(0);
            //Chip4_Idx_Scpu_Rst_N_Write(1);

            //Initialize one cycle
            Chip4_Idx_Scpu_Clk_1Time_Write(1);
            Chip4_Idx_Scpu_Clk_1Time_Write(0);
            // write data to SRAM
            for (i=0; i<28; ++i){
               for (j=2; j>1; --j) {
                    Chip4_SCPU_SRAM_ADDR_Write(i);//0x0f1
                    Chip4_SCPU_SRAM_DATA_Write(sram_buf[i]);//0x83

                    conf_ctrl_flag_value(0);
                    printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
                    wait_ctrl_flag_clean();

                    // write data to SRAM
                    conf_ctrl_flag_value(3);
                    printf("#DLC\tWrite Ctrl_Rdy=1\r\n");
                    wait_ctrl_flag_clean();
                    //printf("#DLC\tAddr: 0x%.3x, Data: 0x%.2x, Loop k=%d\r\n", Chip4_CCT_Sram_Addr_Read(), Chip4_CCT_Sram_Data_Read(), j);
                }
            }

            for (i=0; i<28; ++i) {
                /// initialize
                for (j=2; j>1; --j) {
                    Chip4_SCPU_SRAM_ADDR_Write(i);//0x0f1
                    Chip4_SCPU_SRAM_DATA_Write(0xff);

                    conf_ctrl_flag_value(0);
                    printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
                    wait_ctrl_flag_clean();

                    // read data from SRAM
                    conf_ctrl_flag_value(1);
                    printf("#DLC\tRead Ctrl_Rdy=1\r\n");
                    wait_ctrl_flag_clean();

                    // export data to fpga
                    conf_ctrl_flag_value(2);
                    printf("#DLC\tExport Ctrl_Rdy=1\r\n");
                    wait_ctrl_flag_clean();

                    dec2bin(Chip4_CCT_Sram_Addr_Read(), 10);
                    printf(" ");
                    dec2bin(Chip4_CCT_Sram_Data_Read(), 8);
                    printf("\r\n");
                    //printf(" Loop j=%d\n\n", j);
                    //printf("#DLC\tAddr: 0x%.3x, Data: 0x%.2x, Loop k=%d\r\n", Chip4_CCT_Sram_Addr_Read(), Chip4_CCT_Sram_Data_Read(), j);
                }

                if (Chip4_CCT_Sram_Data_Read() == sram_buf[i]) {
                    printf("#DLC\tSRAM Read Correct, addr = 0x%.3x\r\n\n", i);
                } else {
                    err_cnt++;
                    printf("#DLC\tSRAM Read Failed, addr = 0x%.3x\r\n\n", i);
                }
            }

            if (!err_cnt)
                printf("#DLC\tSRAM batch Write/Read Test Correct!\r\n");
            else
                printf("#DLC\tSRAM batch Write/Read Test Failed!\r\n");
        }
        else if (17 == tst_type) {
            printf("#CMT\t17:Testcase 5:  shift out instruction from CTRL \r\n");
            if (Chip4_SCPU_Idx_Ctrl_Rdy()) {
                printf("#DLC\tLoad instructions done\r\n");
            }
            Chip4_Idx_Scpu_Ctrl_Bgn_Write(0);// reset the CTRL
            if (!Chip4_SCPU_Idx_Ctrl_Rdy()) {
                printf("#DLC\tStart load instructions\r\n");
            }

            Chip4_Idx_Scpu_Ctrl_Mod_Write(0);
            Chip4_Idx_Scpu_Ctrl_Bgn_Write(1);

            /// keep the SRAM and DATA value as input

            Chip4_Idx_Scpu_Ctrl_Load_Write(1);
        }
        else if (18 == tst_type) {
            printf("#CMT\t18:Testcase 6:  SRAM all address write/read Test\r\n");
            err_cnt = 0;

            /// initialize
            Chip4_Idx_Scpu_Rst_N_Write(0);
            //Chip4_Idx_Scpu_Rst_N_Write(1);

            cnt_clk = 1;
            Chip4_SCPU_CNT_SCLK_Write(cnt_clk - 1);
            printf("#DLC\tSRAM data = %d kHz\n\n", 50*1000/(cnt_clk*2));
            Chip4_Idx_Scpu_Clk_Freq_Chg_Write(1);
            //sleep(1);
            Chip4_Idx_Scpu_Clk_Discrt_Write(1);

            //Initialize one cycle
            send_clk_cycles(2);
            // write data to sram
            for (j=0; j<MAX_SRAM_LEN; ++j) {
                Chip4_SCPU_SRAM_ADDR_Write(j);
                Chip4_SCPU_SRAM_DATA_Write(j%256);

                conf_ctrl_flag_value(0);
                printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                // Write data to SRAM
                conf_ctrl_flag_value(3);
                printf("#DLC\tWrite Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();
            }

            // read data from SRAM
            for (j=0; j<MAX_SRAM_LEN; ++j) {
                Chip4_SCPU_SRAM_ADDR_Write(j);///0xee + j
                Chip4_SCPU_SRAM_DATA_Write(0xff);

                conf_ctrl_flag_value(0);
                printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                // read data from SRAM
                conf_ctrl_flag_value(1);
                printf("#DLC\tRead Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                // export data to fpga
                Chip4_SCPU_SRAM_ADDR_Write(0);
                Chip4_SCPU_SRAM_DATA_Write(0);

                conf_ctrl_flag_value(2);
                printf("#DLC\tExport Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                dec2bin(Chip4_CCT_Sram_Addr_Read(), 10);
                printf(" ");
                dec2bin(Chip4_CCT_Sram_Data_Read(), 8);
                printf(" Loop j=%d\n\n", j);
                //printf("#DLC\tAddr: 0x%.3x, Data: 0x%.2x, Loop k=%d\r\n", Chip4_CCT_Sram_Addr_Read(), Chip4_CCT_Sram_Data_Read(), j);

                if ((Chip4_CCT_Sram_Addr_Read() == j) && (Chip4_CCT_Sram_Data_Read() == (j%256))) {
                    printf("#DLC\tRound = %d,\tData = 0x%.2x\r\n", j, Chip4_CCT_Sram_Data_Read());
                } else {
                    err_cnt++;
                    printf("#DLC\tRound = %d,\tData = 0x%.2x\r\n", j, Chip4_CCT_Sram_Data_Read());
                }
            }

            if (!err_cnt)
                printf("#DLC\tSRAM all address write/read Test Correct!\r\n");
            else
                printf("#DLC\tSRAM all address write/read Test Failed!\r\n");
        }
        else if (19 == tst_type) {
            printf("#CMT\t19:Testcase 7:  Loop in/Write SRAM/Export out Test\r\n");
            err_cnt = 0;
            sram_data = 0x83;
            sram_addr = 0xf1;

            /// initialize
            Chip4_Idx_Scpu_Rst_N_Write(0);
            //Chip4_Idx_Scpu_Rst_N_Write(1);

            cnt_clk = 1;
            Chip4_SCPU_CNT_SCLK_Write(cnt_clk - 1);
            printf("#DLC\tSRAM data = %d kHz\n\n", 50*1000/(cnt_clk*2));
            Chip4_Idx_Scpu_Clk_Freq_Chg_Write(1);
            //sleep(1);
            Chip4_Idx_Scpu_Clk_Discrt_Write(1);

            //Initialize one cycle
            send_clk_cycles(2);
            // write data to SRAM
            for (j=100; j>1; --j) {
                Chip4_SCPU_SRAM_ADDR_Write(sram_addr);
                Chip4_SCPU_SRAM_DATA_Write(sram_data);

                conf_ctrl_flag_value(0);
                printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                // write data to SRAM
                conf_ctrl_flag_value(3);
                printf("#DLC\tWrite Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();
                //printf("#DLC\tAddr: 0x%.3x, Data: 0x%.2x, Loop k=%d\r\n", Chip4_CCT_Sram_Addr_Read(), Chip4_CCT_Sram_Data_Read(), j);
            }

            // read data from SRAM
            for (j=100; j>1; --j) {
                Chip4_SCPU_SRAM_ADDR_Write(sram_addr);///0xee + j
                Chip4_SCPU_SRAM_DATA_Write(0xff);

                conf_ctrl_flag_value(0);
                printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                // read data from SRAM
                conf_ctrl_flag_value(1);
                printf("#DLC\tRead Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                // export data to fpga
                conf_ctrl_flag_value(2);
                printf("#DLC\tExport Ctrl_Rdy=1\r\n");
                wait_ctrl_flag_clean();

                dec2bin(Chip4_CCT_Sram_Addr_Read(), 10);
                printf(" ");
                dec2bin(Chip4_CCT_Sram_Data_Read(), 8);
                printf(" Loop j=%d\n\n", j);
                //printf("#DLC\tAddr: 0x%.3x, Data: 0x%.2x, Loop k=%d\r\n", Chip4_CCT_Sram_Addr_Read(), Chip4_CCT_Sram_Data_Read(), j);

                if (Chip4_CCT_Sram_Data_Read() == sram_data) {
                    printf("#DLC\tRound = %d,\tData = 0x%.2x\r\n", j, Chip4_CCT_Sram_Data_Read());
                } else {
                    err_cnt++;
                    printf("#DLC\tRound = %d,\tData = 0x%.2x\r\n", j, Chip4_CCT_Sram_Data_Read());
                }
            }

            if (!err_cnt)
                printf("#DLC\tSRAM Write/Read/Export Test Correct!\r\n");
            else
                printf("#DLC\tSRAM Write/Read/Export Test Failed!\r\n");
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
