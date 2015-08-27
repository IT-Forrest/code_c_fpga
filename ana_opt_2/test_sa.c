/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * Author: Jiafan Wang
 * Date:   03/08/2015
 * ========================================
*/

#include "device.h"
#include <stdint.h>
#include <stdio.h>
#include "scan_chain.h"
#include "serial_port_io.h"
#include "set_config.h"
#include "test_adc.h"

void    PrintXY(uint16 read_len, uint16 *xy_buf)
{
    uint16  rd_val;
    uint16  i;
    if (read_len > MAX_PSOC_BUF)
        PutStr("#CMT\tPrint XY Length> MAX_PSOC_BUF\r\n");

    for (i = 0; i < read_len-1; i++)
    {
        //Since ADC val only has 10bits(1024), so it's always positive for int16
        rd_val = xy_buf[i];
        PutStr("#WSA\tX = ");
        FmtPrint(rd_val & 0x001f);PutStr("\r\n");
        PutStr("#WSA\tY = ");
        FmtPrint(rd_val >> 5);PutStr("\r\n");
    }
    rd_val = xy_buf[i];
    PutStr("#WSA\tX = ");
    FmtPrint(rd_val & 0x001f);PutStr("\r\n");
    PutStr("#WSA\tY = ");
    FmtPrint(rd_val >> 5);PutStr("\r\n");
}

void    JudgePrintXY(uint16 read_len, uint16 *xy_buf)
{
    uint16  rd_val = 0;
    uint8   len = 0;

    // Judge whether we need to print ADC data
    PutStr("\r\n");
    PutStr("#CMT\tPrint XY Data? 1:YES; 0:NO. [1]/0 ");
    rd_val = ReadInput(&len);

    // Continuously Print ADC val MAX_ADC_BUF times
    if (!USE_NEW_VAL || 1 == rd_val)
    {
        PrintXY(read_len, xy_buf);
        PutStr("\r\n#CMT\tPrint XY Data Finished!\r\n");
    }
    PutStr("#CMT\tLen=");FmtPrint(read_len);PutStr("\r\n");
}

//Read each Line from txt file
// uint16  ReadLine(FILE* fp, uint8 *length)
// {
	// char8 ch = ' ';
	// uint16 value = 0;
	// *length = 0;

    // while (ch != EOF)
    // {
        // ch = fgetc(fp);

        // if (ch != '\r')
        // {
			// value = value * 10 + (ch - '0');
            // (*length) ++;
        // }
        // else
        // {
            // ;
        // }
    // }

    // return  value;
// }

// Read the whole Ana_bits from txt file
//uint16  ReadAnaFile(uint16 start_pos, uint16  *xy_buffer)
//{
//    FILE    *fp = NULL;
//    fp = fopen("AnaFile.txt","r");
//    uint16  rd_len = 0;
//    uint16  current_pos = 0;
//    uint16  i = 0;
//    uint8   len = 0;
//
//    current_pos = start_pos;
//    for (i = 0; i < MAX_ANA_BUF; i++)
//    {
//        xy_buffer[current_pos] = ReadLine(fp, &len);
//        current_pos += 1;
//    }
//    rd_len = current_pos - start_pos;
//
//    // Print Just for debug
//    current_pos = start_pos;
//    for (i = 0; i < MAX_ANA_BUF; i++)
//    {
//        FmtPrint(xy_buffer[current_pos]);
//        PutStr("\r\n");
//        current_pos += 1;
//    }
//
//    return  rd_len;
//}

// Just read Tune XY once after Activate SA
uint16  ReadTuneXY()
{
    //uint16  rd_xy = 0;//each variable has 2*5bits
    uint16  rd_val = 0;
    uint16  i = 0;

    //(0) To Reset SA
//    CFG_RSTN_Write(0);

//    //(1) Load Init XY and Ana bits
//    LoadCfg();
//    SetINT_X(init_x);
//    SetINT_Y(init_y);
//    SetAnaBits(ana_bits);
//    SetIterNum(iter_num);
//    TxCfg2ADC();

    //(2) Activate SA for one time and then stop
    WSA_STA_Write(0);
    while(WSA_RDY_Read());

    //CFG_RSTN_Write(1);

    WSA_STA_Write(1);
    while(!WSA_RDY_Read());

    WSA_STA_Write(0);
    while(WSA_RDY_Read());

    //(3) Read Tune_X, Tune_Y
    CFG_STA_Write(0);
	while (CFG_RDY_Read()){;}

    CFG_SEL_Write(1);

    CFG_STA_Write(1);
    while (!CFG_RDY_Read());

    //BIT_SET(rd_val, ANA_So_Read());
    //FmtPrint(ANA_So_Read());

    CFG_STA_Write(0);
    while (CFG_RDY_Read());

    CFG_SEL_Write(0);

    // Skip the 2~35th Period and , The rest part of Init Temperature
    for (i=1; i<=11; i++)//47
    {
        // Create SCK1 and SCK2 clock signal, one time;
    	CFG_STA_Write(1);
    	while (!CFG_RDY_Read()){;}

        //rd_val = rd_val << 1;
        //BIT_SET(rd_val, ANA_So_Read());
        //FmtPrint(ANA_So_Read());

        CFG_STA_Write(0);
		while (CFG_RDY_Read()){;}
    }

    // Fetch Tune X and Tune Y
    for (i=1; i<=10; i++)//47
    {
        // Create SCK1 and SCK2 clock signal, one time;
    	CFG_STA_Write(1);
    	while (!CFG_RDY_Read()){;}

        rd_val = rd_val << 1;
        BIT_SET(rd_val, MSMH_So_Read());//ANA_So_Read()
        //FmtPrint(ANA_So_Read());

        CFG_STA_Write(0);
		while (CFG_RDY_Read()){;}
    }

    //CFG_RSTN_Write(0);

    //PutStr("(b), ");FmtPrint(rd_val);PutStr("(d)\r\n");
    return  rd_val;
}

// Test 9: Read the first Tune_X/Y after give an ANA value
uint16  SimpleReadXY(uint8 init_x, uint8 init_y, uint8 ana_bits, uint8 iter_num)
{
    uint16  xy_val;
//    uint8   init_x = 15;
//    uint8   init_y = 15;
//    uint8   ana_bits = 0;
//    uint8   iter_num = 99;

    SetINT_X1(init_x); //SetINT_X(init_x);
    SetINT_X2(init_y); //SetINT_Y(init_y);
    SetAnaBits(ana_bits);//i
    SetIterNum(iter_num);
    debugCfg();
    TxCfg2ADC();
    Global_RSTN_Write(1);

    //TuneY[4,3,2,1,0]+TuneX[4,3,2,1,0]
    xy_val = ReadTuneXY();
    return xy_val;
}


// Test 10: change Anabits and then iteratively read XY
uint16  IterReadXY(uint16 start_pos, TUNE_ELEMENT* xy_buf)
{
    uint16  rd_val = 0;
    uint16  current_pos = 0;
    uint16  i;
    uint8   init_x = 15;
    uint8   init_y = 15;
    //uint8   ana_bits = 0;
    uint8   iter_num = 99;

    current_pos = start_pos;
    for (i = 0; i < MAX_TUNE_NUM; i++)
    {
        //CFG_RSTN_Write(0);
        //(1) Set Init XY and Ana bits
//        SetINT_X(init_x);
//        SetINT_Y(init_y);
//        SetAnaBits(i);//ana_bits
//        SetIterNum(iter_num);
//        TxCfg2ADC();
//
//        //TuneY[4,3,2,1,0]+TuneX[4,3,2,1,0]
        rd_val = SimpleReadXY(init_x, init_y, i, iter_num);
        xy_buf[current_pos] = rd_val;
        current_pos += 1;
    }

    return  current_pos-start_pos;
}

// Test11: Read the first two TuneXY
uint16  DeepReadXY(uint16 start_pos, TUNE_ELEMENT* xy_buf, uint8 init_x, uint8 init_y, uint8 ana_bits, uint8 iter_num)
{
    uint16  rd_val = 0;
    uint16  current_pos = 0;
    uint16  i;
    //uint8   init_x = 15;
    //uint8   init_y = 15;
    //uint8   ana_bits = 0;
    //uint8   iter_num = 99;
    uint8   local_ana;

    current_pos = start_pos;
    for (i = 0; i < 5; i++)
    {
        local_ana = ana_bits+(i+1)*100;
        //(1) Set Init XY and Ana bits
//        SetINT_X(init_x);
//        SetINT_Y(init_y);
//        SetAnaBits(local_ana);//ana_bits
//        SetIterNum(iter_num);
//        TxCfg2ADC();

        //TuneY[4,3,2,1,0]+TuneX[4,3,2,1,0]
        rd_val = SimpleReadXY(init_x, init_y, local_ana, iter_num);
        xy_buf[current_pos] = rd_val;
        current_pos += 1;
    }

    return  current_pos - start_pos;
}


// Test 12:  Iteratively Read the first two TuneXY
uint16  IterDeepXY(uint16 start_pos, TUNE_ELEMENT* xy_buf)
{
    uint16  rd_val = 0;
    uint16  current_pos = 0;
    uint16  i;
    uint8   init_x = 15;
    uint8   init_y = 15;
    //uint8   ana_bits = 0;
    uint8   iter_num = 99;

    current_pos = start_pos;
    for (i = 0; i < MAX_TUNE_NUM; i++)
    {
        //CFG_RSTN_Write(0);
        rd_val = DeepReadXY(current_pos, xy_buf, init_x, init_y, i, iter_num);
        current_pos += rd_val;
    }

    return  current_pos-start_pos;
}

/* [] END OF FILE */
