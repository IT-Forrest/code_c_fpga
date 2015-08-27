
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

uint16 GetAna(uint16 x, uint16 y, uint16*  adc_buf)
{
    return adc_buf[x*32+y];
}

// sweep every pair of XY;
int main() {
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i = 0;//,j;
    //int32  fint;
    //int16  read_len;
    uint8   fd_str[10];
    uint16  tunex1, tunex2;
    uint16  anaz;

    uint16  adc_buf[1024] = {0};
    uint16  res_buf[3][ITERNUM+SSNUM] = {{0}};

    if (init_mem()) return (1);
    //if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    FILE *fd = fopen("BPF_31M_F8.txt","r");
    if (fd == NULL)
    {
        printf("open file failed!\n");
        return (1);
    }

    //while(fscanf(fd,"%d",&fint))
    while(fgets((char*)fd_str,10,fd))
    {
        //fd_rd[9] = '\0';
       // adc_buf[i] = fint;
        ((char*)fd_str)[9] = '\0';
        adc_buf[i] = atoi((char*)fd_str);
        //fd_str[i] = atoi(fd_rd);

        //printf("%d, line = %d, %s\n",fd_str[i],i+1,fd_rd);
        //printf("%d\tn=%d\t",i, adc_buf[i]);
        i++;
    }

    tunex1 = 15; tunex2 = 15;
    SA_INIT4DX3_Write(tunex1);
    SA_INIT4DX4_Write(tunex2);
    SA_ITER4DSA_Write(ITERNUM); // Set SA iteration = ITERNUM
    SA_ITER4DSS_Write(SSNUM);       // Set Sensitivity time = 4
    SA_INITT4D_Write(32);       // Set initial T = 32
    SA4D_STEPT_Write(4);        // Set T step = 4; each time T=T-T_step;
    SA4D_4DY_Write(0);          // Set 2D test
    anaz = GetAna(tunex1,tunex2,adc_buf);
    SA_ANAIN4D_Write(anaz);     // Set Ana bits

    SA4D_RSN_Write(1);

    printf("%d\t(x,y)=(%d,%d)\tAna=%d\n",0,tunex1,tunex2,anaz);

    for (i = 0; i < ITERNUM+SSNUM; i++)
    {
        SA4D_STA_Write(1);
        SA4D_STA_Write(0);
        while(!SA4D_RDY_Read());

        //ReadTuneXY();
        tunex1 = SA_TUNE4DX3_Read();
        tunex2 = SA_TUNE4DX4_Read();

        anaz = GetAna(tunex1,tunex2,adc_buf);
        SA_ANAIN4D_Write(anaz);
        //TxCfg2ADC();

        res_buf[0][i] = tunex1;
        res_buf[1][i] = tunex2;
        res_buf[2][i] = anaz;
    }

    for (i = 0; i < ITERNUM+SSNUM; i++)
    {
        printf("%d\t(x,y)=(%d,%d)\tAna=%d\n",i+1, res_buf[0][i],res_buf[1][i],res_buf[2][i]);
    }

    SA4D_STA_Write(1);
    SA4D_STA_Write(0);

    printf("Final, X=%d, Y=%d", SA_TUNE4DX3_Read(), SA_TUNE4DX4_Read());

    return( clean_mem() );
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
