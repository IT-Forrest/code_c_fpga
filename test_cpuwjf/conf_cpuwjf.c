#include "conf_cpuwjf.h"
#include "../ana_opt_2/device.h"// in order to use functions, i.e., Chip4_Idx_Scpu_Clk_1Time_Write

//#define     CPU_GLOBAL_ON
//#define     CPU_DETAIL_ON
#define     DEBUG_BIT_ERROR
#define     DEBUG_BIT_AND(F)           ((F) &= ~(0x0025))

int16  rd_bfile_to_mem_buf(FILE *fd, uint8 *sram_buf, uint16 config_len, uint16 reserve_len) {
    int16   i;
    uint32  inst_val = 0;
    char8   line[18];//2 bits more than instruction binary arrays

    for (i=0; i<MAX_SRAM_WORD; ++i) {
        sram_buf[2*i] = 0x00;
        sram_buf[2*i+1] = 0x00;
    }

    i = 0;
    while (fgets(line, sizeof(line), fd)) { //strcmp(line, "")
        if ('\n'==line[0]) continue;/// skip the empty line derived by using fgets

        //printf("Retrive str = %s", line);
        inst_val = strtol(line,0,2);//just for compiling test
        sram_buf[2*i] = inst_val & 0x00ff;
        sram_buf[2*i+1] = (inst_val>>8) & 0x00ff;
        //printf("ADDR 0x%.3x-%.3x:\t 0x%.2x 0x%.2x\n", 2*i+1, 2*i, sram_buf[2*i+1], sram_buf[2*i]);
#ifdef CPU_GLOBAL_ON
        printf("Addr 0x%.3x\t0x%.2x\r\n", 2*i+1, sram_buf[2*i+1]);
        printf("Addr 0x%.3x\t0x%.2x\r\n", 2*i, sram_buf[2*i]);
#endif
        if ((config_len - 1) == i) {
            i= reserve_len;///DEFAULT_PC_ADDR;
        } else {
            i++;
        }
    }
    //printf("Total word(s) = %d\r\n", i-reserve_len+1);
    return (i-reserve_len+1);
}

int16   rd_bfile_to_adc_buf(FILE *fd, uint16 *adc_buf, bool is_osc) {
    int16   i;
    uint16  inst_val = 0;
    char8   line[12];//2 bits more than the ADC binary arrays
    uint16  max_io_data;
    //FSEL_ELEMENT    *A;

    max_io_data = (is_osc)? 18 : 16;
    max_io_data *= MAX_IQDATA_GRP;

    for (i=0; i<max_io_data; ++i) {
        adc_buf[i] = 0;
    }

    i = 0;
    while (fgets(line, sizeof(line), fd)) {
        if ('\n'==line[0]) continue;/// skip the empty line derived by using fgets

        inst_val = strtol(line,0,2);
        adc_buf[i] = inst_val;
#ifdef CPU_GLOBAL_ON
        printf("IQ Data = %d, id=%d\r\n", adc_buf[i], i);
#endif
        ++i;
    }
    return  i;
}


//strlen <= 31
void dec2bin(int c, int strlen)
{
   int i = 0;
   for(i = strlen-1; i >= 0; i--){
     if((c & (1 << i)) != 0){
       printf("1");
     }else{
       printf("0");
     }
   }
}

void send_clk_cycles(int clk_cnt) {
    int i;
    for (i=0; i<clk_cnt; ++i) {
        Chip4_Idx_Scpu_Clk_1Time_Write(1);
        Chip4_Idx_Scpu_Clk_1Time_Write(0);
    }
}

void conf_ctrl_flag_value(int mode_type) {
    int clk_cnt = 0;
    if (0 == mode_type) clk_cnt = 23;/// scan data from fpga to I/O need >= 23 clk cycles
    else if (3 == mode_type) clk_cnt = 4;/// write to sram need >= 4 clk cycles
    else if (1 == mode_type) clk_cnt = 4;/// read from sram need >= 4 clk cycles
    else if (2 == mode_type) clk_cnt = 23;/// export data from I/O to fpga need >= 23 clk cycles
    else {
        printf("mode_type ranges from 0 to 3\r\n");
        return;
    }

    Chip4_Idx_Scpu_Ctrl_Bgn_Write(1);
    Chip4_Idx_Scpu_Ctrl_Mod_Write(mode_type);
    Chip4_Idx_Scpu_Ctrl_Load_Write(1);
    send_clk_cycles(clk_cnt);
    while(!Chip4_SCPU_Idx_Ctrl_Rdy());
}

void wait_ctrl_flag_clean() {
    int clk_cnt = 4;///default clk_cnt = 4;
    Chip4_Idx_Scpu_Ctrl_Bgn_Write(0);
    Chip4_Idx_Scpu_Ctrl_Load_Write(0);
    send_clk_cycles(clk_cnt);
    while(Chip4_SCPU_Idx_Ctrl_Rdy());
}

void chg_fpga_clk_freq(uint16  cnt_clk) {
    ///cnt_clk = 1;
    if (cnt_clk) {
        Chip4_SCPU_CNT_SCLK_Write(cnt_clk - 1);
        printf("#DLC\tSRAM data = %d kHz\n\n", 50*1000/(cnt_clk*2));
        Chip4_Idx_Scpu_Clk_Freq_Chg_Write(1);
        //sleep(1);
        Chip4_Idx_Scpu_Clk_Discrt_Write(1);
    } else {
        ;
    }
}

void write_insts_to_sram(uint8 *sram_buf, int16 inst_num, uint16 config_len, uint16 reserve_len) {
    uint16  i,j;
    uint16  addr_tmp = 0;
    //reserve_len = DEFAULT_PC_ADDR;

    /// initialize
    Chip4_Idx_Scpu_Rst_N_Write(0);
    //Chip4_Idx_Scpu_Rst_N_Write(1);

    send_clk_cycles(1);//Initialize one cycle

    for (i=0; i<(inst_num-1)+reserve_len; ) {
        for (j=2; j>=1; --j) {
            addr_tmp = 2*i+j-1;
            Chip4_SCPU_SRAM_ADDR_Write(addr_tmp);
            Chip4_SCPU_SRAM_DATA_Write(sram_buf[addr_tmp]);
#ifdef CPU_GLOBAL_ON
            printf("Addr 0x%.3x\t0x%.2x\r\n", addr_tmp, sram_buf[addr_tmp]);
#endif
            /// swift instructions and address to CTRL module
            conf_ctrl_flag_value(0);
            wait_ctrl_flag_clean();

            /// notify CTRL send instructions to SRAM
            conf_ctrl_flag_value(3);
            wait_ctrl_flag_clean();
        }

        if ((config_len - 1) == i) {
            i = reserve_len;
        } else {
            ++i;
        }
    }
}

void chg_clk_and_start_cpu() {
    Chip4_Idx_Scpu_Clk_Discrt_Write(0);
    if (0) {
        ///to apply default clk freq (50MHz), enable this branch
        Chip4_Idx_Scpu_Clk_Freq_Chg_Write(0);
    }
    Chip4_Idx_Scpu_Rst_N_Write(1);

    Chip4_Idx_Scpu_Ctrl_Bgn_Write(0);
    Chip4_Idx_Scpu_Cpu_Bgn_Write(1);
    Chip4_Idx_Scpu_Cpu_Bgn_Write(0);
}

void get_IQ_data_to_cpu(uint16 addr_adc, uint16 *adcs_buf, bool is_osc) {
    uint16  k;
    uint16  adc_data;
    uint16  addr_iqs = 0;
    uint16  max_io_data = (is_osc)? 18 : 16;
    uint16  IQ_data[18];
    uint16  IQ_abs[2];

    for (k=0; k < max_io_data; ) {
        addr_iqs = (addr_adc*max_io_data) + k;
        adc_data = adcs_buf[addr_iqs];
        IQ_data[k] = adcs_buf[addr_iqs];
#ifdef  DEBUG_BIT_ERROR
        DEBUG_BIT_AND(adc_data);
        DEBUG_BIT_AND(IQ_data[k]);
#endif
        /// wait for ADC request signal
        while(!Chip4_SCPU_Idx_App_Start());
        //printf("#DLC\tApp Start: get ADC!!\r\n");

        Chip4_ADC_Write(adc_data);

        Chip4_Idx_Scpu_Clk_Discrt_Write(1);
        Chip4_Idx_Scpu_App_Done_Write(1);
        send_clk_cycles(20);

        /// wait for ADC request finish
        //while(Chip4_SCPU_Idx_App_Start());
        Chip4_Idx_Scpu_App_Done_Write(0);
        Chip4_Idx_Scpu_Clk_Discrt_Write(0);

        if (is_osc) {
            if (3 == k) {
                IQ_abs[0] = (IQ_data[0]>IQ_data[2])?(IQ_data[0]-IQ_data[2]):(IQ_data[2]-IQ_data[0]);
                IQ_abs[1] = (IQ_data[1]>IQ_data[3])?(IQ_data[1]-IQ_data[3]):(IQ_data[3]-IQ_data[1]);

                if ((IQ_abs[0] + IQ_abs[1]) < OSCD_TOL) {
                    ++k;
                }
                else {
                    printf("#DLC\tOSCD violated!!!\r\n");
                    break;
                }
            }
            else {
                ++k;
            }
        }
        else {
            ++k;
        }
    }
}

void read_data_from_sram(uint8 *read_buf, uint16 bgn_line, uint16 num_line) {
    uint16  i,j;
    uint16  addr_tmp = 0;
    uint16  end_line = bgn_line + num_line;

    /// fetch result from sram
    Chip4_Idx_Scpu_Rst_N_Write(0);
    Chip4_Idx_Scpu_Clk_Discrt_Write(1);

    //for (i = 2; i <3; ++i) {
    for (i = bgn_line; i <end_line; ++i) {
        //inst_val = 0;
        for (j=2; j >=1; --j) {
            addr_tmp = 2*i+j-1;
            Chip4_SCPU_SRAM_ADDR_Write(addr_tmp);
            Chip4_SCPU_SRAM_DATA_Write(0xff);

            conf_ctrl_flag_value(0);
#ifdef CPU_GLOBAL_ON
            printf("#DLC\tLoop in Ctrl_Rdy=1\r\n");
#endif
            wait_ctrl_flag_clean();

            /// read data from SRAM
            conf_ctrl_flag_value(1);
#ifdef CPU_GLOBAL_ON
            printf("#DLC\tRead Ctrl_Rdy=1\r\n");
#endif
            wait_ctrl_flag_clean();

            /// export sram data to fpga
            Chip4_SCPU_SRAM_ADDR_Write(0x00);
            Chip4_SCPU_SRAM_DATA_Write(0x00);
            conf_ctrl_flag_value(2);
#ifdef CPU_GLOBAL_ON
            printf("#DLC\tExport Ctrl_Rdy=1\r\n");
#endif
            wait_ctrl_flag_clean();

#ifdef CPU_DETAIL_ON
            dec2bin(Chip4_CCT_Sram_Addr_Read(), 10);
            printf(" ");
            dec2bin(Chip4_CCT_Sram_Data_Read(), 8);
            printf(" Loop j=%d\n\n", j);
#endif

//            if (1 == j) {
//                inst_val |= (0x000000ff & Chip4_CCT_Sram_Data_Read());
//                printf("#DLC\tSram Data 0x%x\r\n", Chip4_CCT_Sram_Data_Read());
//            } else { // (2 == j)
//                inst_val |= ((0x000000ff & Chip4_CCT_Sram_Data_Read()) << 8);
//                printf("#DLC\tSram Data 0x%x\r\n", Chip4_CCT_Sram_Data_Read());
//            }
            read_buf[addr_tmp] = (0x000000ff & Chip4_CCT_Sram_Data_Read());
        }
    }
}

