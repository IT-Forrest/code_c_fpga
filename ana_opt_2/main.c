#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "psoc_port.h"

#include "device.h"
#include "scan_chain.h"
#include "set_config.h"
#include "serial_port_io.h"
#include "test_adc.h"

void Initialize() // Global Initilization
{
	CFG_STA_Write(0);
}

int main() {
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    uint16  i;//,j;
    uint16  freq;
    //uint8   fsel_phs = 0;
    //uint8   cal_src = 0;
    uint16  amp_val = MAX_IQ_AMPL;
    uint8   tst_type = 0;
    uint16  rd_val = 0;
    uint8   len = 0;
    uint16  read_len = MAX_AVG_NUM;
    //uint16  adc_buf[MAX_PSOC_BUF] = {0};
    //uint8   tune_X, tune_Y = 0;
    IQ_ELEMENT  adc_buf[MAX_PSOC_BUF] = {0};
    TUNE_ELEMENT  *xy_buf = adc_buf;

    uint8   cap2_F[3] = {0};
//printf("short=%d\n",sizeof(int)/sizeof(uint8));
    //CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */

    uint16  flist[3] = {48,60,72};

	/* Init USB UART interface */
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);//!!NOTE!! Make sure this matches your board voltage!
	//while(!USBUART_1_GetConfiguration());
	USBUART_1_CDC_Init();

//    SET_SEL_Write(0);
    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);
    //printf("0x%x, stat\n", *stat_addr);
printf("ctrl_addr=0x%x\n",local_ctrl_addr);
    //*ctrl_addr = 0;
    //*ctrl_addr = 0xffffffff;//init_reg();
    //avs_wait();
    //while((*stat_addr & 1) == 0);

//CFG_STA_Write(1);
    //printf("0x%x\n", *ctrl_addr);
    //CFG_STA_Write(1);
    //printf("0x%x\n", *ctrl_addr);

    //printf("%f\n",sqrt12(125.0));

    //Global_RSTN_Write(0);
    // *ctrl_addr = 1;
    // avs_wait();

    // while (!(*stat_addr & 1));  // Poll the ready signal

    // *ctrl_addr = 0;
    // avs_wait();

	/* Main Test Cycles */
	PutWait();
    for(;;)
    {
        Initialize();

        PutStr("#CMT\r\n");
        PutStr("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        PutStr("#CMT\t$$$$ Welcome to Congyin & Jiafan's 2nd World. Have fun $$$$\r\n");
        PutStr("#CMT\r\n");
        PutStr("#CMT\t0 :Just Typein Configure and then Tx to ADC.\r\n");
        PutStr("#CMT\t1 :Read ADC WITHOUT Tx Configuration\r\n");
        PutStr("#CMT\t2 :Read ADC with Tx Configuration.\r\n");
        PutStr("#CMT\t3 :Load Configuration and then Read IQ data.\r\n");
        PutStr("#CMT\t4 :Repeatly do case 3 16times and then do average.\r\n");
        PutStr("#CMT\t5 :Switch FSEL from 1~3 and then do case 3 for IQ data.\r\n");
        PutStr("#CMT\t6 :Switch FSEL from 1~3 and then do case 4 for IQ data.\r\n");
        PutStr("#CMT\t7 :Change XY from [0,0]~[31,31] and then do case 3 for IQ data.\r\n");
        PutStr("#CMT\t8 :Change XY from [0,0]~[31,31] and then do case 4 for IQ data.\r\n");
        PutStr("#CMT\t-------------------------------------------------------\r\n");
        PutStr("#CMT\t9 :Read the first Tune_X/Y after give an ANA value.\r\n");
        PutStr("#CMT\t10:Read the first Tune_X/Y after go through all ANA values.\r\n");
        PutStr("#CMT\t11:Read the first two Tune_X/Y given ANA values\r\n");
        PutStr("#CMT\t12:Iteratively Read the first two TuneXY \r\n");
        PutStr("#CMT\t-------------------------------------------------------\r\n");
        PutStr("#CMT\t13:Typein the baseline for input amplitude\r\n");
        PutStr("#CMT\t14:Load Configuration and just Read IQ input data\r\n");
        PutStr("#CMT\t15:Repeatly do case 14 16times and then do average.\r\n");
        PutStr("#CMT\t16:Adjust IQ input amplitude to the baseline set in case 13.\r\n");
        PutStr("#CMT\t17:Adjust the average IQ input amplitude to the baseline set in case 13.\r\n");
        PutStr("#CMT\t18:Switch FSEL from 1~3 and then do case 16 for IQ input data.\r\n");
        PutStr("#CMT\t19:Switch FSEL from 1~3 and then do case 17 for IQ input data.\r\n");
        PutStr("#CMT\t20:Change XY from [0,0]~[31,31] and then do case 16 for IQ input data.\r\n");
        PutStr("#CMT\t21:Change XY from [0,0]~[31,31] and then do case 17 for IQ input data.\r\n");
        PutStr("#CMT\t22:Set Frequency from [32~127] MHz.\r\n");

        PutStr("#CMT\r\n");
        PutStr("#CMT\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
        PutStr("#CMT\tWhich Test next(");FmtPrint(tst_type);PutStr(")? ");
        rd_val = ReadInput(&len);
        if (USE_NEW_VAL && rd_val < 23)    tst_type = rd_val;

        PutStr("#CMT\t---------------------\r\n");
        //CFG_RSTN_Write(1);
        //Global_RSTN_Write(0);

        if (0 == tst_type)
        {
            PutStr("#CMT\t\tConfig ADC Test Begin\r\n");

            //configuration and then send
            SetCfg();
            PutStr("#CMT\tConfiguration Finished.\r\n");

            //do you need to write to ADC??
            TxCfg2ADC();
            printf("ctrl=%x", local_ctrl_addr);
        }
        else if (1 == tst_type)
        {
            PutStr("#CMT\t\tRead ADC without Config Test Begin\r\n");

            // Input Read times
            read_len = MAX_ADC_BUF;
            PutStr("#CMT\tInput Rd_ADC times[0~");FmtPrint(read_len);PutStr("]: ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL && (rd_val <= MAX_ADC_BUF)) read_len = rd_val;
            PutStr("#CMT\tSet Rd_ADC times = ");FmtPrint(read_len);PutStr("(d)\r\n");
            PutStr("\r\n");

            // Continuously Read ADC val MAX_ADC_BUF times
            SimpleReadAdc(read_len, adc_buf);
            PutStr("#CMT\tReceive ADC Done!!\r\n");

            JudgePrintADC(read_len, adc_buf);
        }
        else if (2 == tst_type)
        {
            PutStr("#CMT\t\tRead ADC with Config Test Begin\r\n");

            // load last Configuration
            LoadCfg();
            read_len = 1;//MAX_ADC_BUF;

            // Continuously Read ADC val MAX_ADC_BUF times
            CfgAndReadAdc(read_len, adc_buf);
            PutStr("#CMT\tReceive ADC Done!!\r\n");

            JudgePrintADC(read_len, adc_buf);
        }
        else if (3 == tst_type)
        {
            PutStr("#CMT\t\tGet Iout Iin, Qout Qin from ADC Begin\r\n");

            // For one Frequency, there are only four data
            LoadCfg();
            read_len = 4;
            IQDataReadAdc(0, adc_buf);
            PutStr("#CMT\tReceive ADC Done!!\r\n");

            JudgePrintADC(read_len, adc_buf);
        }
        else if (4 == tst_type)
        {
            PutStr("#CMT\t\tGet Average Iout Iin, Qout Qin Test Begin\r\n");

            // Read one Frequency for 16 times
            LoadCfg();
            read_len = 4;
            IQAvgReadAdc(0, adc_buf);
            PutStr("#CMT\tReceive ADC Done!!\r\n");

            JudgePrintADC(read_len, adc_buf);
        }
        else if (5 == tst_type)
        {
            PutStr("#CMT\t\tGet Iout Iin, Qout Qin for N Freq\r\n");

            // Check through 3 Frequencys
            LoadCfg();
            read_len = 0;
            read_len = FSELIQDataRead(0, adc_buf, sizeof(flist)/sizeof(uint16), flist);

            JudgePrintADC(read_len, adc_buf);
        }
        else if (6 == tst_type)
        {
            PutStr("#CMT\t\tGet Average Iout Iin, Qout Qin for N Freq\r\n");

            // Check avarage ADC through 3 frequencies
            LoadCfg();
            read_len = 0;
            read_len = FSELIQAvgRead(0, adc_buf, sizeof(flist)/sizeof(uint16), flist);

            JudgePrintADC(read_len, adc_buf);
        }
        else if (7 == tst_type)
        {
            PutStr("#CMT\t\tChange XY and then do case 3 for IQ data\r\n");

            // Go through XY for all the IQ data
            LoadCfg();
            read_len = 0;
            read_len = AllXYIQDataRead(0, adc_buf, sizeof(flist)/sizeof(uint16), flist);

            JudgePrintADC(read_len, adc_buf);
        }
        else if (8 == tst_type)
        {
            PutStr("#CMT\t\tChange XY and then do case 4 for IQ data\r\n");

            // Go through XY for all the IQ data
            LoadCfg();
            read_len = 0;
            read_len = AllXYIQAvgRead(0, adc_buf, sizeof(flist)/sizeof(uint16), flist);

            JudgePrintADC(read_len, adc_buf);
        }
        else if (9 == tst_type)
        {
            PutStr("#CMT\t\tRead the first Tune_X/Y after given an ANA value\r\n");

            //Read the first Tune_X/Y after given an ANA value
            LoadCfg();
            //CFG_RSTN_Write(0);
            for (i = 0; i<100; i++);

            //TuneY[4,3,2,1,0]+TuneX[4,3,2,1,0]
            rd_val = SimpleReadXY(15,15,200,99);
            xy_buf[0] = rd_val;
            PrintXY(1,xy_buf);
        }
        else if (10 == tst_type)
        {
            PutStr("#CMT\t\tRead the first Tune_X/Y after go through all ANA values\r\n");

            // Read the first Tune_X/Y after give ANA signal
            LoadCfg();
            read_len = 0;
            read_len = IterReadXY(0, xy_buf);

            JudgePrintXY(read_len, xy_buf);
            //read_len = ReadTuneXY(0, xy_buf);
        }
        else if (11 == tst_type)
        {
            PutStr("#CMT\t\tRead the first two Tune_X/Y given ANA values\r\n");

            //Read the the first two Tune_X/Y given ANA values
            LoadCfg();
            //CFG_RSTN_Write(0);
            //for (i = 0; i<100; i++);

            read_len = DeepReadXY(0, xy_buf, 15,15,200,99);
            JudgePrintXY(read_len, xy_buf);
//            rd_val = SimpleReadXY(15,15,200,99);
//            PutStr("#WSA\tX = ");FmtPrint(rd_val & 0x001f);PutStr("\r\n");
//            PutStr("#WSA\tY = ");FmtPrint(rd_val >> 5);PutStr("\r\n");
//
//            rd_val = SimpleReadXY(15,15,204,99);
//            PutStr("#WSA\tX = ");FmtPrint(rd_val & 0x001f);PutStr("\r\n");
//            PutStr("#WSA\tY = ");FmtPrint(rd_val >> 5);PutStr("\r\n");
        }
        else if (12 == tst_type)
        {
            PutStr("#CMT\t\tIteratively Read the first two TuneXY \r\n");

            //Iteratively Read the first two TuneXY
            LoadCfg();
            read_len = 0;
            read_len = IterDeepXY(0, xy_buf);

            JudgePrintXY(read_len, xy_buf);
        }
        else if (13 == tst_type)
        {
            PutStr("#CMT\tTypein the basic input amplitude\r\n");

            //Basic input amplitude
            PutStr("#CMT\tRepeat input amplitude(");FmtPrint(amp_val);PutStr(")?");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    amp_val = rd_val;

            PutStr("#CMT\tIQ input amplitude Finished.\r\n");
        }
        else if (14 == tst_type)
        {
            PutStr("#CMT\t\tJust Get Iin, Qin from ADC Begin\r\n");

            // For one Frequency, there are only four data
            LoadCfg();
            read_len = IQDataInnAdc(0, adc_buf);
            PutStr("#CMT\tReceive Iin/Qin Done!!\r\n");

            JudgePrintADC(read_len, adc_buf);
        }
        else if (15 == tst_type)
        {
            PutStr("#CMT\t\tJust Get Average Iin, Qin Test Begin\r\n");

            // Read one Frequency for 16 times
            LoadCfg();
            read_len = IQAvgInnAdc(0, adc_buf);
            PutStr("#CMT\tReceive Iin/Qin Done!!\r\n");

            JudgePrintADC(read_len, adc_buf);
        }
        else if (16 == tst_type)
        {
            PutStr("#CMT\t\tAdjust the IQ input amplitude to the basic amp\r\n");
            LoadCfg();

            rd_val = AdjustCap2IQInn(amp_val, 0, adc_buf);
            PutStr("#CMT\tCAP2 = ");FmtPrint(rd_val);PutStr("\r\n");
        }
        else if (17 == tst_type)
        {
            PutStr("#CMT\t\tAdjust the average IQ input amplitude to the basic amp\r\n");
            LoadCfg();

            rd_val = AdjustCap2IQAvg(amp_val, 0, adc_buf);
            PutStr("#CMT\tCAP2 = ");FmtPrint(rd_val);PutStr("\r\n");
        }
        else if (18 == tst_type)
        {
            PutStr("#CMT\t\tSwitch FSEL from 1~3 and then do case 16 for IQ input data.\r\n");
            LoadCfg();

            read_len = FSELCap2IQData(amp_val, cap2_F, 0, adc_buf);
            for (i=0; i <3; i++)
            {
                PutStr("#CMT\tCAP2 = (");FmtPrint(cap2_F[i]);PutStr(")\r\n");
            }
        }
        else if (19 == tst_type)
        {
            PutStr("#CMT\t\tSwitch FSEL from 1~3 and then do case 17 for IQ input data.\r\n");
            LoadCfg();

            read_len = FSELCap2IQAvg(amp_val, cap2_F, 0, adc_buf);
            for (i=0; i <3; i++)
            {
                PutStr("#CMT\tCAP2 = (");FmtPrint(cap2_F[i]);PutStr(")\r\n");
            }
        }
        else if (20 == tst_type)
        {
            PutStr("#CMT\t\tChange XY from [0,0]~[31,31] and then do case 16 for IQ input data.\r\n");
            LoadCfg();

            read_len = AllXYAdjustIQInn(amp_val, cap2_F, adc_buf);

            JudgePrintADC(read_len, adc_buf);
        }
        else if (21 == tst_type)
        {
            PutStr("#CMT\t\tChange XY from [0,0]~[31,31] and then do case 17 for IQ input data.\r\n");
            LoadCfg();

            read_len = AllXYAdjustIQAvg(amp_val, cap2_F, adc_buf);

            JudgePrintADC(read_len, adc_buf);
        }
        else if (22 == tst_type)
        {
            freq = 20;
            PutStr("#CMT\t\tSet Frequency from [32~127] MHz.\r\n");
            PutStr("#DLC\t Set Frequency = ");
            rd_val = ReadInput(&len);
            if (USE_NEW_VAL)    freq = rd_val;
            else {PutStr("#DLC\tDefault Frequency = ");FmtPrint(freq);PutStr(")\r\n");}
            SetFreq(freq);
            BackupCfg();
            PutStr("#CMT\t\tSet Frequency finished.\r\n");
        }
        else
        {
            PutStr("#CMT\t\t No this kind of test right now...(");FmtPrint(tst_type);PutStr(")\r\n");
        }

        PutStr("#CMT\t---------------------\r\n");

	    PutWait();
    }

    return( clean_mem() );
}
