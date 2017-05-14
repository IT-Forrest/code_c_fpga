#include "conf_cpuwjf.h"
#include "../ana_opt_2/device.h"// in order to use functions, i.e., Chip4_Idx_Scpu_Clk_1Time_Write

int16  rd_bfile_to_mem_buf(FILE *fd, uint8 *sram_buf, uint16 reserve_len) {
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
        printf("Addr 0x%.3x\t0x%.2x\r\n", 2*i+1, sram_buf[2*i+1]);
        printf("Addr 0x%.3x\t0x%.2x\r\n", 2*i, sram_buf[2*i]);
        if (0 == i) {
            i= reserve_len;///DEFAULT_PC_ADDR;
        } else {
            i++;
        }
    }
    //printf("Total word(s) = %d\r\n", i-reserve_len+1);
    return (i-reserve_len+1);
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
