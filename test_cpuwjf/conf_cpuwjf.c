#include "conf_cpuwjf.h"

void rd_bfile_to_sram_buf(FILE *fd, uint8 *sram_buf, uint16 reserve_len) {
    uint16  i;
    uint32  inst_val = 0;
    char8   line[18];//2 bits more than instruction binary arrays

    for (i=0; i<MAX_SRAM_WORD; ++i) {
        sram_buf[2*i] = 0x00;
        sram_buf[2*i+1] = 0x00;
    }

    i = 0;
    while (fgets(line, sizeof(line), fd)) { //strcmp(line, "")
        //printf("Retrive str = %s", line);
        inst_val = strtol(line,0,2);//just for compiling test
        sram_buf[2*i] = inst_val & 0x00ff;
        sram_buf[2*i+1] = (inst_val>>8) & 0x00ff;
        printf("ADDR 0x%.3x-%.3x:\t 0x%.2x 0x%.2x\n", 2*i+1, 2*i, sram_buf[2*i+1], sram_buf[2*i]);
        if (0 == i) {
            i= reserve_len;///DEFAULT_PC_ADDR;
        } else {
            i++;
        }
    }
    printf("Total word(s) = %d\n", i-reserve_len+1);
}
