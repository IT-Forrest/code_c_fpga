
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
    uint8   low_value = 0;
    uint8   high_value = 0;
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
    SWEEP_LOWDATA_Write(low_value);
    SWEEP_HIGHDATA_Write(high_value);

    for(;;)
    {
        printf("#CMT\r\n");
        printf("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        printf("#CMT\t$$$$ Welcome to Jiafan's World. Have fun $$$$\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t1 :Set value to lower 3 bits (0~7)\r\n");
        printf("#CMT\t2 :Set value to higher 5 bits (0~31)\r\n");
        printf("#CMT\t3 :Sweep value from 0~255, and then become 0\r\n");
        printf("#CMT\r\n");
        printf("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        printf("#CMT\tWhich Test next(");FmtPrint(tst_type);printf(")? ");
        rd_val = ReadInput(&len);
        if (USE_NEW_VAL && rd_val < 4)    tst_type = rd_val;

        printf("#CMT\t---------------------\r\n");
        //CFG_RSTN_Write(1);
        //Global_RSTN_Write(0);

        if (1 == tst_type)
        {
            printf("#CMT\t\tSet value to lower 3 bits (0~7)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    low_value = rd_val;
            else {printf("#DLC\tPrevious 3bit value = ");FmtPrint(low_value);printf(")\r\n");}

            //clear sweep flag set flag and value
            SWEEP_START_Write(0);
            SWEEP_LOWDATA_Write(low_value);
            SWEEP_SETLOW_Write(1);
            //SWEEP_SETLOW_Write(0); since we need to keep the output

            printf("#DLC\tSet 3bit value=%d done\n\n", low_value);
        }
        else if (2 == tst_type)
        {
            printf("#CMT\t\tSet value to higher 5 bits (0~31)\r\n");
            printf("#DLC\tValue = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    high_value = rd_val;
            else {printf("#DLC\tPrevious 5bit value = ");FmtPrint(high_value);printf(")\r\n");}

            //clear sweep flag set flag and value
            SWEEP_START_Write(0);
            SWEEP_HIGHDATA_Write(high_value);
            SWEEP_SETHIGH_Write(1);
            //SWEEP_SETHIGH_Write(0); since we need to keep the output

            printf("#DLC\tSet 5bit value=%d done\n\n", high_value);
        }
        else if (3 == tst_type)
        {
            printf("#CMT\t\tSweep value from 0~255, and then become 0\r\n\r\n");

            //clear low/high values and clear LOW/HIGH flag
            low_value = 0;
            high_value = 0;
            SWEEP_SETLOW_Write(0);
            SWEEP_SETHIGH_Write(0);
            SWEEP_LOWDATA_Write(low_value);
            SWEEP_HIGHDATA_Write(high_value);

            //set flag and then configuration
            SWEEP_CNTSCLK_Write(adc_buf[0]);//the interval value
            SWEEP_START_Write(1);
            while(!SWEEP_RDY_Read());
            SWEEP_START_Write(0);

            printf("#DLC\tSweep done\n\n");
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
