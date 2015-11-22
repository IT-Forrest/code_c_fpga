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

#include    <math.h>
#include    <stdint.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "device.h"
#include    "psoc_port.h"
#include    "set_config.h"
#include    "test_adc.h"

#define PutStr(b) printf(b)

// use for Setup information
uint32    gcfg[MAX_CFG_BITS];       // configuration bits array
uint32    gcfg_bk[MAX_CFG_BITS];    // configuration bits backup
uint32    gcfg_tx[MAX_CFG_BITS];    // configuration bits delivered to FPGA
//uint32    gcfg_rd[MAX_CFG_BITS];
uint32    gscB;
uint32    gscB_bk;

int32   SetFSEL(uint8 rd_val);

typedef struct FREQ_FSEL_M123_STRU
{
    //uint16_t freq;
    uint16_t bs;
    uint16_t m123;
    uint16_t cap1;
    uint16_t cap2;
    uint16_t vres;

}FREQ_FSEL_M123;


FREQ_FSEL_M123 gMapping_Array[MAX_FSEL_LEN] = {
// For amp around 90
{	0	,	32	,	1	,	15	,	71	},
{	0	,	33	,	1	,	15	,	71	},
{	0	,	34	,	1	,	15	,	71	},
{	0	,	35	,	1	,	14	,	71	},
{	0	,	36	,	1	,	14	,	71	},
{	0	,	37	,	1	,	13	,	71	},
{	0	,	38	,	1	,	13	,	71	},
{	0	,	39	,	1	,	13	,	71	},
{	0	,	40	,	1	,	12	,	71	},
{	0	,	41	,	1	,	12	,	51	},
{	0	,	42	,	1	,	12	,	51	},
{	0	,	43	,	1	,	11	,	51	},
{	0	,	44	,	1	,	11	,	51	},
{	1	,	45	,	1	,	11	,	51	},
{	1	,	46	,	1	,	10	,	51	},
{	1	,	47	,	1	,	10	,	51	},
{	1	,	48	,	1	,	10	,	39	},
{	1	,	49	,	1	,	10	,	39	},
{	1	,	50	,	1	,	9	,	39	},
{	1	,	51	,	1	,	9	,	39	},
{	1	,	52	,	1	,	9	,	39	},
{	1	,	53	,	1	,	9	,	39	},
{	1	,	54	,	1	,	9	,	39	},
{	1	,	55	,	1	,	8	,	39	},
{	1	,	56	,	1	,	8	,	39	},
{	1	,	57	,	1	,	8	,	39	},
{	1	,	58	,	1	,	8	,	31	},
{	1	,	59	,	1	,	8	,	31	},
{	1	,	60	,	1	,	8	,	31	},
{	3	,	61	,	1	,	7	,	31	},
{	3	,	62	,	1	,	7	,	31	},
{	3	,	63	,	1	,	7	,	31	},
{	3	,	64	,	1	,	7	,	31	},
{	3	,	65	,	1	,	7	,	31	},
{	3	,	66	,	1	,	7	,	31	},
{	3	,	67	,	1	,	7	,	23	},
{	3	,	68	,	1	,	7	,	23	},
{	3	,	69	,	1	,	6	,	23	},
{	3	,	70	,	1	,	6	,	23	},
{	3	,	71	,	1	,	6	,	23	},
{	3	,	72	,	1	,	6	,	23	},
{	3	,	73	,	1	,	6	,	23	},
{	3	,	74	,	1	,	6	,	23	},
{	3	,	75	,	1	,	6	,	23	},
{	3	,	76	,	1	,	6	,	23	},
{	3	,	77	,	1	,	6	,	23	},
{	3	,	78	,	1	,	5	,	23	},
{	3	,	79	,	1	,	5	,	19	},
{	3	,	80	,	1	,	5	,	19	},
{	7	,	81	,	1	,	5	,	19	},
{	7	,	82	,	1	,	5	,	19	},
{	7	,	83	,	1	,	5	,	19	},
{	7	,	84	,	1	,	5	,	19	},
{	7	,	85	,	1	,	5	,	19	},
{	7	,	86	,	0	,	5	,	19	},
{	7	,	87	,	0	,	5	,	19	},
{	7	,	88	,	0	,	5	,	19	},
{	7	,	89	,	0	,	5	,	19	},
{	7	,	90	,	0	,	5	,	19	},
{	7	,	91	,	0	,	5	,	15	},
{	7	,	92	,	1	,	4	,	15	},
{	7	,	93	,	1	,	4	,	15	},
{	7	,	94	,	1	,	4	,	15	},
{	7	,	95	,	1	,	4	,	15	},
{	7	,	96	,	1	,	4	,	15	},
{	7	,	97	,	1	,	4	,	15	},
{	7	,	98	,	0	,	4	,	15	},
{	7	,	99	,	0	,	4	,	15	},
{	7	,	100	,	0	,	4	,	15	},
{	7	,	101	,	0	,	4	,	15	},
{	7	,	102	,	0	,	4	,	15	},
{	7	,	103	,	0	,	4	,	15	},
{	7	,	104	,	1	,	3	,	15	},
{	7	,	105	,	1	,	3	,	15	},
{	15	,	106	,	1	,	3	,	11	},
{	15	,	107	,	1	,	3	,	11	},
{	15	,	108	,	1	,	3	,	11	},
{	15	,	109	,	1	,	3	,	11	},
{	15	,	110	,	1	,	3	,	11	},
{	15	,	111	,	1	,	3	,	11	},
{	15	,	112	,	1	,	3	,	11	},
{	15	,	113	,	1	,	3	,	11	},
{	15	,	114	,	1	,	3	,	11	},
{	15	,	115	,	0	,	3	,	11	},
{	15	,	116	,	0	,	3	,	11	},
{	15	,	117	,	0	,	3	,	11	},
{	15	,	118	,	0	,	3	,	11	},
{	15	,	119	,	0	,	3	,	11	},
{	15	,	120	,	0	,	3	,	11	},
{	15	,	121	,	0	,	3	,	11	},
{	15	,	122	,	0	,	3	,	11	},
{	15	,	123	,	0	,	3	,	11	},
{	15	,	124	,	0	,	3	,	11	},
{	15	,	125	,	0	,	3	,	11	},
{	15	,	126	,	0	,	3	,	11	},
{	15	,	127	,	0	,	3	,	11	}
};

int init_cfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg[i] = 0;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_bk[i] = 0xffffffff;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_tx[i] = 0;
    //for (i = 0; i<MAX_CFG_BITS; i++) gcfg_rd[i] = 0;
    return (0);
}

void init_sc()
{
    /// Disable global clock and CFSA at the very beginning
    Chip3_Idx_Ctrl_Sta_Clk_Write(0);
    Chip3_Idx_Ctrl_Rst_Ana_Write(0);
    Chip3_Idx_Ctrl_Rst_N_Write(0);

    /// Frequency
    Chip3_Set_Mdiv1(127);
    Chip3_Set_Bs1(0);
    Chip3_Set_Cap1(0);

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

    Chip3_Set_Sw(1);

    ///////////////////////////////////////////////
    /// To set digital configuration: e.g. SA, SS etc
    ///////////////////////////////////////////////

    Chip3_Set_Dbfactor0(0x100 | 106);// Set the constant for 3dB points;
    Chip3_Set_Dbfactor1(0x100 | 106);
    Chip3_Set_Threshold(100);   // Set the threshold for Multi-start SS; if ANA > threshold, choose a new start
    Chip3_Set_Factors(0x00);    // Set weighting factors; 8'b10_00_11_01;//Gain3-Gain2-Gain1-Gain0

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

    Chip3_Set_Aoff(507);
    Chip3_Set_Tol(32);

    /// Filter Settings
    Chip3_Set_Tx1(31);
    Chip3_Set_Tx2(31);
    Chip3_Set_Ty1(31);
    Chip3_Set_Ty2(31);

    Chip3_Set_Cb1(7);
    Chip3_Set_Cb2(7);
    Chip3_Set_Cb3(7);
    Chip3_Set_Cb4(7);

    Chip3_Set_Sw(1); // 0 - 4D, 1 - 2D

    /// Discrete Test Control
    Chip3_Set_Nxt_Test(0);
    Chip3_Set_Trdy_Test(0);
    Chip3_Set_Trg_Test(0);
    Chip3_Set_Test_Mux(1);  // for debug
}

/* Automatically config PLL bits and Amplitude bits */
void autocfg(int offset, int thres)
{
    int bs[5] = {0,1,3,7,15};
    int amp[2] = {0}; // record amplitude
    uint16_t cap[2] = {0}; // record cap2
    int i, j, k, mdiv;
    bool flag;
    IQ_ELEMENT adc_buf[4] = {0};    // Buffer


    printf("Configuring PLL and Amplitude bits...\n");

    for (mdiv = 32; mdiv <=127; mdiv++)
    {
        flag = false;
        Chip3_Set_Mdiv0(mdiv);

        for (i = 0; i <= 4; i++) // Search good BS
        {
            Chip3_Set_Bs0(bs[i]);

            Chip3_Idx_Ctrl_Rst_Ana_Write(0);
            Chip3_Send_Cfg_To_SCA();
            Chip3_Idx_Ctrl_Rst_Ana_Write(1);
            usleep(500);                            // Wait for PLL settling

            *pll_tune_ctrl_addr = 1; avs_wait();    // Start counting 4096 times
            usleep(4200);

            j = (int)(*pll_tune_cntr_addr);
            k = (int)(*pll_tune_cntf_addr);

            if (j>=k && j-k<=2) { flag = true; break; }

            *pll_tune_ctrl_addr = 0; avs_wait();    // Stop counting
        }

        *pll_tune_ctrl_addr = 0; avs_wait();

        if (!flag)
        {
            printf("%u, Frequency test is failed!\n", mdiv);
            break;
        }

        gMapping_Array[mdiv-32].bs = bs[i];
        printf("#%u, bs = %u, ", mdiv, bs[i]);

        for (i=0; i<2; i++) // CAP1
        {
            for (j=0; j<16; j++) // CAP2
            {
                Chip3_Set_Cap0((i << 4) + j);

                memset(adc_buf,0,sizeof(adc_buf));
                IQAvgReadAdc(0, adc_buf, AVG);      //Measure I/Q data
                for (k=0; k<4; k++)
                    adc_buf[k]=(adc_buf[k]/AVG);
                cap[i] = j;
                amp[i] = sqrt(
                       pow((int)adc_buf[2]-offset,2) +
                       pow((int)adc_buf[3]-offset,2)
                );  // Input amplitude

                if (amp[i]<=thres) break;
            }

            j = abs(amp[0]-thres);
            k = abs(amp[1]-thres);
            if (j<=k)
            {
                gMapping_Array[mdiv-32].cap1 = 0;
                gMapping_Array[mdiv-32].cap2 = cap[0];
                printf("cap1 = 0, cap2 = %u, amp = %d\n", cap[0], amp[0]);
            } else
            {
                gMapping_Array[mdiv-32].cap1 = 1;
                gMapping_Array[mdiv-32].cap2 = cap[1];
                printf("cap1 = 1, cap2 = %u, amp = %d\n", cap[1], amp[1]);
            }
        }

    }

}

/* ==================== Initialize Filter Configuration ====================== */
void    InitCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg[i] = 0;
}

/* ==================== Load Old Filter Configuration ====================== */
void    LoadCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg[i] = gcfg_bk[i];
}

/* ==================== Backup Filter Configuration ====================== */
void    BackupCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_bk[i] = gcfg[i];
}

void    TranxCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_tx[i] = gcfg[i];
}

void    RShiftCfg()
{
    int i;
    uint32  tmp;
    uint32  loop = gcfg[0];
    for (i = 0; i < MAX_CFG_BITS-1; i++)
    {
        tmp = (gcfg[i] >> 1);
        gcfg[i] = tmp | ((gcfg[i+1] & 0x1) << 31);
    }
    /// during the last time, only do right shift and rotate
    tmp = (gcfg[i] >> 1);
    gcfg[i] = tmp | ((loop & 0x1) << MAX_SC_A_HIGH);
}

/* ==================== Functions for tapeout Chip3 ====================== */
void    Chip3_Clear_Scan_Chain(uint8 sc_type)
{
    /// sc_type: 1: ScanChain_A, 0: ScanChain_B
    if (sc_type == 1) InitCfg();
    else if (sc_type == 0) gscB = 0;
}

/* ==================== Filter Configuration ====================== */

void SetFreq(uint16 rd_val)
{
    //uint8   i;
    //uint8   dat[12];
    uint16_t  bs = 1;
    uint16_t  cap1, cap2;
    uint16_t  M123 = 60;

    {
        M123 = rd_val;//*3
        if (M123 < 32)  M123 = 32;
        if (M123 > 127) M123 = 127;

        bs   = gMapping_Array[M123-32].bs;
        cap1 = gMapping_Array[M123-32].cap1;
        cap2 = gMapping_Array[M123-32].cap2;
    }

    Chip3_Set_Bs0(bs);
    Chip3_Set_Mdiv0(M123);
    Chip3_Set_Cap0((cap1 << 4) + cap2);
    Chip3_Set_Fs(0);

    return;
}

/// TODO: change to Chip3_
//int LoadCfgFile(char* file_name, uint16* fno, uint16* flist)
//{
//    /* Load Config File, Format:
//        SW Freq fno f0 f1 f2 ... fn CB1 CB2 CB3 CB4 X1 X2 X3 X4
//    */
//    FILE * fd;
//    int val;
//    int i;
//
//    fd = fopen(file_name, "r");
//    if (NULL == fd) return 1;
//
//    if(fscanf(fd, "%d", &val)) {SetCB_SW(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetFreq(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val))
//    {
//        *fno = val;
//        for (i=0; i<*fno; i++)
//        {
//            if(fscanf(fd, "%d", &val))
//            {
//                flist[i] = val;
//            } else {break;}
//        }
//    } else {return 1;}
//
//    if(fscanf(fd, "%d", &val)) {SetCB1(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetCB2(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetCB3(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetCB4(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetTune_X1(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetTune_X2(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetTune_X3(val);} else {return 1;}
//    if(fscanf(fd, "%d", &val)) {SetTune_X4(val);} else {return 1;}
//    BackupCfg();
//
//    fclose(fd);
//
//    return 0;
//}

//////// Config function for tapeout3
int32    Chip3_Set_Fs(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x03)
    {
        PutStr("\t\tChip3_Set_Fs value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_FS_0: CHIP3_RST_FS_0;
    (rd_val & 0x00000002)? CHIP3_SET_FS_1: CHIP3_RST_FS_1;

    return 0;
}


int32    Chip3_Set_Ana_Bits(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0xff)
    {
        PutStr("\t\tChip3_Set_Ana_Bits value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_ANA_BITS_0: CHIP3_RST_ANA_BITS_0;
    (rd_val & 0x00000002)? CHIP3_SET_ANA_BITS_1: CHIP3_RST_ANA_BITS_1;
    (rd_val & 0x00000004)? CHIP3_SET_ANA_BITS_2: CHIP3_RST_ANA_BITS_2;
    (rd_val & 0x00000008)? CHIP3_SET_ANA_BITS_3: CHIP3_RST_ANA_BITS_3;
    (rd_val & 0x00000010)? CHIP3_SET_ANA_BITS_4: CHIP3_RST_ANA_BITS_4;
    (rd_val & 0x00000020)? CHIP3_SET_ANA_BITS_5: CHIP3_RST_ANA_BITS_5;
    (rd_val & 0x00000040)? CHIP3_SET_ANA_BITS_6: CHIP3_RST_ANA_BITS_6;
    (rd_val & 0x00000080)? CHIP3_SET_ANA_BITS_7: CHIP3_RST_ANA_BITS_7;

    return 0;
}


int32    Chip3_Set_Dbfactor1(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x03ff)
    {
        PutStr("\t\tChip3_Set_Dbfactor1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_DBFACTOR1_0: CHIP3_RST_DBFACTOR1_0;
    (rd_val & 0x00000002)? CHIP3_SET_DBFACTOR1_1: CHIP3_RST_DBFACTOR1_1;
    (rd_val & 0x00000004)? CHIP3_SET_DBFACTOR1_2: CHIP3_RST_DBFACTOR1_2;
    (rd_val & 0x00000008)? CHIP3_SET_DBFACTOR1_3: CHIP3_RST_DBFACTOR1_3;
    (rd_val & 0x00000010)? CHIP3_SET_DBFACTOR1_4: CHIP3_RST_DBFACTOR1_4;
    (rd_val & 0x00000020)? CHIP3_SET_DBFACTOR1_5: CHIP3_RST_DBFACTOR1_5;
    (rd_val & 0x00000040)? CHIP3_SET_DBFACTOR1_6: CHIP3_RST_DBFACTOR1_6;
    (rd_val & 0x00000080)? CHIP3_SET_DBFACTOR1_7: CHIP3_RST_DBFACTOR1_7;
    (rd_val & 0x00000100)? CHIP3_SET_DBFACTOR1_8: CHIP3_RST_DBFACTOR1_8;
    (rd_val & 0x00000200)? CHIP3_SET_DBFACTOR1_9: CHIP3_RST_DBFACTOR1_9;

    return 0;
}


int32    Chip3_Set_Dbfactor0(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x03ff)
    {
        PutStr("\t\tChip3_Set_Dbfactor0 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_DBFACTOR0_0: CHIP3_RST_DBFACTOR0_0;
    (rd_val & 0x00000002)? CHIP3_SET_DBFACTOR0_1: CHIP3_RST_DBFACTOR0_1;
    (rd_val & 0x00000004)? CHIP3_SET_DBFACTOR0_2: CHIP3_RST_DBFACTOR0_2;
    (rd_val & 0x00000008)? CHIP3_SET_DBFACTOR0_3: CHIP3_RST_DBFACTOR0_3;
    (rd_val & 0x00000010)? CHIP3_SET_DBFACTOR0_4: CHIP3_RST_DBFACTOR0_4;
    (rd_val & 0x00000020)? CHIP3_SET_DBFACTOR0_5: CHIP3_RST_DBFACTOR0_5;
    (rd_val & 0x00000040)? CHIP3_SET_DBFACTOR0_6: CHIP3_RST_DBFACTOR0_6;
    (rd_val & 0x00000080)? CHIP3_SET_DBFACTOR0_7: CHIP3_RST_DBFACTOR0_7;
    (rd_val & 0x00000100)? CHIP3_SET_DBFACTOR0_8: CHIP3_RST_DBFACTOR0_8;
    (rd_val & 0x00000200)? CHIP3_SET_DBFACTOR0_9: CHIP3_RST_DBFACTOR0_9;

    return 0;
}


int32    Chip3_Set_Threshold(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0xff)
    {
        PutStr("\t\tChip3_Set_Threshold value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_THRESHOLD_0: CHIP3_RST_THRESHOLD_0;
    (rd_val & 0x00000002)? CHIP3_SET_THRESHOLD_1: CHIP3_RST_THRESHOLD_1;
    (rd_val & 0x00000004)? CHIP3_SET_THRESHOLD_2: CHIP3_RST_THRESHOLD_2;
    (rd_val & 0x00000008)? CHIP3_SET_THRESHOLD_3: CHIP3_RST_THRESHOLD_3;
    (rd_val & 0x00000010)? CHIP3_SET_THRESHOLD_4: CHIP3_RST_THRESHOLD_4;
    (rd_val & 0x00000020)? CHIP3_SET_THRESHOLD_5: CHIP3_RST_THRESHOLD_5;
    (rd_val & 0x00000040)? CHIP3_SET_THRESHOLD_6: CHIP3_RST_THRESHOLD_6;
    (rd_val & 0x00000080)? CHIP3_SET_THRESHOLD_7: CHIP3_RST_THRESHOLD_7;

    return 0;
}


int32    Chip3_Set_Factors(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0xff)
    {
        PutStr("\t\tChip3_Set_Factors value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_FACTORS_0: CHIP3_RST_FACTORS_0;
    (rd_val & 0x00000002)? CHIP3_SET_FACTORS_1: CHIP3_RST_FACTORS_1;
    (rd_val & 0x00000004)? CHIP3_SET_FACTORS_2: CHIP3_RST_FACTORS_2;
    (rd_val & 0x00000008)? CHIP3_SET_FACTORS_3: CHIP3_RST_FACTORS_3;
    (rd_val & 0x00000010)? CHIP3_SET_FACTORS_4: CHIP3_RST_FACTORS_4;
    (rd_val & 0x00000020)? CHIP3_SET_FACTORS_5: CHIP3_RST_FACTORS_5;
    (rd_val & 0x00000040)? CHIP3_SET_FACTORS_6: CHIP3_RST_FACTORS_6;
    (rd_val & 0x00000080)? CHIP3_SET_FACTORS_7: CHIP3_RST_FACTORS_7;

    return 0;
}


int32    Chip3_Set_Avrg_Num(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Avrg_Num value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_AVRG_NUM: CHIP3_RST_AVRG_NUM;

    return 0;
}


int32    Chip3_Set_Rand_Group(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Rand_Group value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_RAND_GROUP: CHIP3_RST_RAND_GROUP;

    return 0;
}


int32    Chip3_Set_Div_Accu(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Div_Accu value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_DIV_ACCU: CHIP3_RST_DIV_ACCU;

    return 0;
}


int32    Chip3_Set_Flag4D(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Flag4D value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_FLAG4D: CHIP3_RST_FLAG4D;

    return 0;
}


int32    Chip3_Set_Opt_Mode(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Opt_Mode value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_OPT_MODE: CHIP3_RST_OPT_MODE;

    return 0;
}


int32    Chip3_Set_Step_T(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tChip3_Set_Step_T value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_STEP_T_0: CHIP3_RST_STEP_T_0;
    (rd_val & 0x00000002)? CHIP3_SET_STEP_T_1: CHIP3_RST_STEP_T_1;
    (rd_val & 0x00000004)? CHIP3_SET_STEP_T_2: CHIP3_RST_STEP_T_2;

    return 0;
}


int32    Chip3_Set_Init_T(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x3f)
    {
        PutStr("\t\tChip3_Set_Init_T value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_INIT_T_0: CHIP3_RST_INIT_T_0;
    (rd_val & 0x00000002)? CHIP3_SET_INIT_T_1: CHIP3_RST_INIT_T_1;
    (rd_val & 0x00000004)? CHIP3_SET_INIT_T_2: CHIP3_RST_INIT_T_2;
    (rd_val & 0x00000008)? CHIP3_SET_INIT_T_3: CHIP3_RST_INIT_T_3;
    (rd_val & 0x00000010)? CHIP3_SET_INIT_T_4: CHIP3_RST_INIT_T_4;
    (rd_val & 0x00000020)? CHIP3_SET_INIT_T_5: CHIP3_RST_INIT_T_5;

    return 0;
}


int32    Chip3_Set_Ss_Iter(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x07ff)
    {
        PutStr("\t\tChip3_Set_Ss_Iter value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_SS_ITER_0: CHIP3_RST_SS_ITER_0;
    (rd_val & 0x00000002)? CHIP3_SET_SS_ITER_1: CHIP3_RST_SS_ITER_1;
    (rd_val & 0x00000004)? CHIP3_SET_SS_ITER_2: CHIP3_RST_SS_ITER_2;
    (rd_val & 0x00000008)? CHIP3_SET_SS_ITER_3: CHIP3_RST_SS_ITER_3;
    (rd_val & 0x00000010)? CHIP3_SET_SS_ITER_4: CHIP3_RST_SS_ITER_4;
    (rd_val & 0x00000020)? CHIP3_SET_SS_ITER_5: CHIP3_RST_SS_ITER_5;
    (rd_val & 0x00000040)? CHIP3_SET_SS_ITER_6: CHIP3_RST_SS_ITER_6;
    (rd_val & 0x00000080)? CHIP3_SET_SS_ITER_7: CHIP3_RST_SS_ITER_7;
    (rd_val & 0x00000100)? CHIP3_SET_SS_ITER_8: CHIP3_RST_SS_ITER_8;
    (rd_val & 0x00000200)? CHIP3_SET_SS_ITER_9: CHIP3_RST_SS_ITER_9;
    (rd_val & 0x00000400)? CHIP3_SET_SS_ITER_10: CHIP3_RST_SS_ITER_10;

    return 0;
}


int32    Chip3_Set_Sa_Iter(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x07ff)
    {
        PutStr("\t\tChip3_Set_Sa_Iter value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_SA_ITER_0: CHIP3_RST_SA_ITER_0;
    (rd_val & 0x00000002)? CHIP3_SET_SA_ITER_1: CHIP3_RST_SA_ITER_1;
    (rd_val & 0x00000004)? CHIP3_SET_SA_ITER_2: CHIP3_RST_SA_ITER_2;
    (rd_val & 0x00000008)? CHIP3_SET_SA_ITER_3: CHIP3_RST_SA_ITER_3;
    (rd_val & 0x00000010)? CHIP3_SET_SA_ITER_4: CHIP3_RST_SA_ITER_4;
    (rd_val & 0x00000020)? CHIP3_SET_SA_ITER_5: CHIP3_RST_SA_ITER_5;
    (rd_val & 0x00000040)? CHIP3_SET_SA_ITER_6: CHIP3_RST_SA_ITER_6;
    (rd_val & 0x00000080)? CHIP3_SET_SA_ITER_7: CHIP3_RST_SA_ITER_7;
    (rd_val & 0x00000100)? CHIP3_SET_SA_ITER_8: CHIP3_RST_SA_ITER_8;
    (rd_val & 0x00000200)? CHIP3_SET_SA_ITER_9: CHIP3_RST_SA_ITER_9;
    (rd_val & 0x00000400)? CHIP3_SET_SA_ITER_10: CHIP3_RST_SA_ITER_10;

    return 0;
}


int32    Chip3_Set_Init_Y1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Init_Y1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_INIT_Y1_0: CHIP3_RST_INIT_Y1_0;
    (rd_val & 0x00000002)? CHIP3_SET_INIT_Y1_1: CHIP3_RST_INIT_Y1_1;
    (rd_val & 0x00000004)? CHIP3_SET_INIT_Y1_2: CHIP3_RST_INIT_Y1_2;
    (rd_val & 0x00000008)? CHIP3_SET_INIT_Y1_3: CHIP3_RST_INIT_Y1_3;
    (rd_val & 0x00000010)? CHIP3_SET_INIT_Y1_4: CHIP3_RST_INIT_Y1_4;

    return 0;
}


int32    Chip3_Set_Init_X1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Init_X1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_INIT_X1_0: CHIP3_RST_INIT_X1_0;
    (rd_val & 0x00000002)? CHIP3_SET_INIT_X1_1: CHIP3_RST_INIT_X1_1;
    (rd_val & 0x00000004)? CHIP3_SET_INIT_X1_2: CHIP3_RST_INIT_X1_2;
    (rd_val & 0x00000008)? CHIP3_SET_INIT_X1_3: CHIP3_RST_INIT_X1_3;
    (rd_val & 0x00000010)? CHIP3_SET_INIT_X1_4: CHIP3_RST_INIT_X1_4;

    return 0;
}


int32    Chip3_Set_Init_Y2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Init_Y2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_INIT_Y2_0: CHIP3_RST_INIT_Y2_0;
    (rd_val & 0x00000002)? CHIP3_SET_INIT_Y2_1: CHIP3_RST_INIT_Y2_1;
    (rd_val & 0x00000004)? CHIP3_SET_INIT_Y2_2: CHIP3_RST_INIT_Y2_2;
    (rd_val & 0x00000008)? CHIP3_SET_INIT_Y2_3: CHIP3_RST_INIT_Y2_3;
    (rd_val & 0x00000010)? CHIP3_SET_INIT_Y2_4: CHIP3_RST_INIT_Y2_4;

    return 0;
}


int32    Chip3_Set_Init_X2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Init_X2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_INIT_X2_0: CHIP3_RST_INIT_X2_0;
    (rd_val & 0x00000002)? CHIP3_SET_INIT_X2_1: CHIP3_RST_INIT_X2_1;
    (rd_val & 0x00000004)? CHIP3_SET_INIT_X2_2: CHIP3_RST_INIT_X2_2;
    (rd_val & 0x00000008)? CHIP3_SET_INIT_X2_3: CHIP3_RST_INIT_X2_3;
    (rd_val & 0x00000010)? CHIP3_SET_INIT_X2_4: CHIP3_RST_INIT_X2_4;

    return 0;
}


int32    Chip3_Set_Nxt_Test(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Nxt_Test value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_NXT_TEST: CHIP3_RST_NXT_TEST;

    return 0;
}


int32    Chip3_Set_Trdy_Test(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Trdy_Test value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_TRDY_TEST: CHIP3_RST_TRDY_TEST;

    return 0;
}


int32    Chip3_Set_Aoff(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x03ff)
    {
        PutStr("\t\tChip3_Set_Aoff value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_AOFF_0: CHIP3_RST_AOFF_0;
    (rd_val & 0x00000002)? CHIP3_SET_AOFF_1: CHIP3_RST_AOFF_1;
    (rd_val & 0x00000004)? CHIP3_SET_AOFF_2: CHIP3_RST_AOFF_2;
    (rd_val & 0x00000008)? CHIP3_SET_AOFF_3: CHIP3_RST_AOFF_3;
    (rd_val & 0x00000010)? CHIP3_SET_AOFF_4: CHIP3_RST_AOFF_4;
    (rd_val & 0x00000020)? CHIP3_SET_AOFF_5: CHIP3_RST_AOFF_5;
    (rd_val & 0x00000040)? CHIP3_SET_AOFF_6: CHIP3_RST_AOFF_6;
    (rd_val & 0x00000080)? CHIP3_SET_AOFF_7: CHIP3_RST_AOFF_7;
    (rd_val & 0x00000100)? CHIP3_SET_AOFF_8: CHIP3_RST_AOFF_8;
    (rd_val & 0x00000200)? CHIP3_SET_AOFF_9: CHIP3_RST_AOFF_9;

    return 0;
}


int32    Chip3_Set_Tol(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x3f)
    {
        PutStr("\t\tChip3_Set_Tol value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_TOL_0: CHIP3_RST_TOL_0;
    (rd_val & 0x00000002)? CHIP3_SET_TOL_1: CHIP3_RST_TOL_1;
    (rd_val & 0x00000004)? CHIP3_SET_TOL_2: CHIP3_RST_TOL_2;
    (rd_val & 0x00000008)? CHIP3_SET_TOL_3: CHIP3_RST_TOL_3;
    (rd_val & 0x00000010)? CHIP3_SET_TOL_4: CHIP3_RST_TOL_4;
    (rd_val & 0x00000020)? CHIP3_SET_TOL_5: CHIP3_RST_TOL_5;

    return 0;
}


int32    Chip3_Set_Trg_Test(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Trg_Test value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_TRG_TEST: CHIP3_RST_TRG_TEST;

    return 0;
}


int32    Chip3_Set_Test_Mux(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Test_Mux value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_TEST_MUX: CHIP3_RST_TEST_MUX;

    return 0;
}


int32    Chip3_Set_Cal(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Cal value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CAL: CHIP3_RST_CAL;

    return 0;
}


int32    Chip3_Set_Src(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Src value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_SRC: CHIP3_RST_SRC;

    return 0;
}


int32    Chip3_Set_Oscd(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Oscd value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_OSCD: CHIP3_RST_OSCD;

    return 0;
}


int32    Chip3_Set_Sw(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Sw value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_SW: CHIP3_RST_SW;

    return 0;
}

void    Chip3_Set_Tx2_L4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TX2_0: CHIP3_RST_TX2_0;
    (rd_val & 0x00000002)? CHIP3_SET_TX2_1: CHIP3_RST_TX2_1;
    (rd_val & 0x00000004)? CHIP3_SET_TX2_2: CHIP3_RST_TX2_2;
    (rd_val & 0x00000008)? CHIP3_SET_TX2_3: CHIP3_RST_TX2_3;
    (rd_val & 0x00000010)? CHIP3_SET_TX2_4: CHIP3_RST_TX2_4;
}

void    Chip3_Set_Tx2_H4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TX2_5: CHIP3_RST_TX2_5;
    (rd_val & 0x00000002)? CHIP3_SET_TX2_6: CHIP3_RST_TX2_6;
    (rd_val & 0x00000004)? CHIP3_SET_TX2_7: CHIP3_RST_TX2_7;
    (rd_val & 0x00000008)? CHIP3_SET_TX2_8: CHIP3_RST_TX2_8;
    (rd_val & 0x00000010)? CHIP3_SET_TX2_9: CHIP3_RST_TX2_9;
}

uint32  Chip3_Rtn_Tx2()
{
    uint32  rd_val = 0;

    if (CHIP3_CHK_TX2_0) BIT_SET(rd_val, 0x00000001);
    if (CHIP3_CHK_TX2_1) BIT_SET(rd_val, 0x00000002);
    if (CHIP3_CHK_TX2_2) BIT_SET(rd_val, 0x00000004);
    if (CHIP3_CHK_TX2_3) BIT_SET(rd_val, 0x00000008);
    if (CHIP3_CHK_TX2_4) BIT_SET(rd_val, 0x00000010);

    if (CHIP3_CHK_TX2_5) BIT_SET(rd_val, 0x00000020);
    if (CHIP3_CHK_TX2_6) BIT_SET(rd_val, 0x00000040);
    if (CHIP3_CHK_TX2_7) BIT_SET(rd_val, 0x00000080);
    if (CHIP3_CHK_TX2_8) BIT_SET(rd_val, 0x00000100);
    if (CHIP3_CHK_TX2_9) BIT_SET(rd_val, 0x00000200);

    return rd_val;
}

int32    Chip3_Set_Tx2(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x001f)
    {
        PutStr("\t\tChip3_Set_Tx2 value invalid!\r\n");
        return -1;
    }

    Chip3_Set_Tx2_L4bits(rd_val);
    Chip3_Set_Tx2_H4bits(rd_val);

    return 0;
}

void    Chip3_Set_Ty2_L4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TY2_0: CHIP3_RST_TY2_0;
    (rd_val & 0x00000002)? CHIP3_SET_TY2_1: CHIP3_RST_TY2_1;
    (rd_val & 0x00000004)? CHIP3_SET_TY2_2: CHIP3_RST_TY2_2;
    (rd_val & 0x00000008)? CHIP3_SET_TY2_3: CHIP3_RST_TY2_3;
    (rd_val & 0x00000010)? CHIP3_SET_TY2_4: CHIP3_RST_TY2_4;
}

void    Chip3_Set_Ty2_H4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TY2_5: CHIP3_RST_TY2_5;
    (rd_val & 0x00000002)? CHIP3_SET_TY2_6: CHIP3_RST_TY2_6;
    (rd_val & 0x00000004)? CHIP3_SET_TY2_7: CHIP3_RST_TY2_7;
    (rd_val & 0x00000008)? CHIP3_SET_TY2_8: CHIP3_RST_TY2_8;
    (rd_val & 0x00000010)? CHIP3_SET_TY2_9: CHIP3_RST_TY2_9;
}

uint32  Chip3_Rtn_Ty2()
{
    uint32  rd_val = 0;

    if (CHIP3_CHK_TY2_0) BIT_SET(rd_val, 0x00000001);
    if (CHIP3_CHK_TY2_1) BIT_SET(rd_val, 0x00000002);
    if (CHIP3_CHK_TY2_2) BIT_SET(rd_val, 0x00000004);
    if (CHIP3_CHK_TY2_3) BIT_SET(rd_val, 0x00000008);
    if (CHIP3_CHK_TY2_4) BIT_SET(rd_val, 0x00000010);

    if (CHIP3_CHK_TY2_5) BIT_SET(rd_val, 0x00000020);
    if (CHIP3_CHK_TY2_6) BIT_SET(rd_val, 0x00000040);
    if (CHIP3_CHK_TY2_7) BIT_SET(rd_val, 0x00000080);
    if (CHIP3_CHK_TY2_8) BIT_SET(rd_val, 0x00000100);
    if (CHIP3_CHK_TY2_9) BIT_SET(rd_val, 0x00000200);

    return rd_val;
}

int32    Chip3_Set_Ty2(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x001f)
    {
        PutStr("\t\tChip3_Set_Ty2 value invalid!\r\n");
        return -1;
    }

    Chip3_Set_Ty2_L4bits(rd_val);
    Chip3_Set_Ty2_H4bits(rd_val);

    return 0;
}


int32    Chip3_Set_Cb4(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tChip3_Set_Cb4 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CB4_0: CHIP3_RST_CB4_0;
    (rd_val & 0x00000002)? CHIP3_SET_CB4_1: CHIP3_RST_CB4_1;
    (rd_val & 0x00000004)? CHIP3_SET_CB4_2: CHIP3_RST_CB4_2;

    return 0;
}


int32    Chip3_Set_Cb3(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tChip3_Set_Cb3 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CB3_0: CHIP3_RST_CB3_0;
    (rd_val & 0x00000002)? CHIP3_SET_CB3_1: CHIP3_RST_CB3_1;
    (rd_val & 0x00000004)? CHIP3_SET_CB3_2: CHIP3_RST_CB3_2;

    return 0;
}

void    Chip3_Set_Tx1_L4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TX1_0: CHIP3_RST_TX1_0;
    (rd_val & 0x00000002)? CHIP3_SET_TX1_1: CHIP3_RST_TX1_1;
    (rd_val & 0x00000004)? CHIP3_SET_TX1_2: CHIP3_RST_TX1_2;
    (rd_val & 0x00000008)? CHIP3_SET_TX1_3: CHIP3_RST_TX1_3;
    (rd_val & 0x00000010)? CHIP3_SET_TX1_4: CHIP3_RST_TX1_4;
}

void    Chip3_Set_Tx1_H4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TX1_5: CHIP3_RST_TX1_5;
    (rd_val & 0x00000002)? CHIP3_SET_TX1_6: CHIP3_RST_TX1_6;
    (rd_val & 0x00000004)? CHIP3_SET_TX1_7: CHIP3_RST_TX1_7;
    (rd_val & 0x00000008)? CHIP3_SET_TX1_8: CHIP3_RST_TX1_8;
    (rd_val & 0x00000010)? CHIP3_SET_TX1_9: CHIP3_RST_TX1_9;
}

uint32  Chip3_Rtn_Tx1()
{
    uint32  rd_val = 0;

    if (CHIP3_CHK_TX1_0) BIT_SET(rd_val, 0x00000001);
    if (CHIP3_CHK_TX1_1) BIT_SET(rd_val, 0x00000002);
    if (CHIP3_CHK_TX1_2) BIT_SET(rd_val, 0x00000004);
    if (CHIP3_CHK_TX1_3) BIT_SET(rd_val, 0x00000008);
    if (CHIP3_CHK_TX1_4) BIT_SET(rd_val, 0x00000010);

    if (CHIP3_CHK_TX1_5) BIT_SET(rd_val, 0x00000020);
    if (CHIP3_CHK_TX1_6) BIT_SET(rd_val, 0x00000040);
    if (CHIP3_CHK_TX1_7) BIT_SET(rd_val, 0x00000080);
    if (CHIP3_CHK_TX1_8) BIT_SET(rd_val, 0x00000100);
    if (CHIP3_CHK_TX1_9) BIT_SET(rd_val, 0x00000200);

    return rd_val;
}

int32    Chip3_Set_Tx1(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x001f)
    {
        PutStr("\t\tChip3_Set_Tx1 value invalid!\r\n");
        return -1;
    }

    Chip3_Set_Tx1_L4bits(rd_val);
    Chip3_Set_Tx1_H4bits(rd_val);

    return 0;
}

void    Chip3_Set_Ty1_L4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TY1_0: CHIP3_RST_TY1_0;
    (rd_val & 0x00000002)? CHIP3_SET_TY1_1: CHIP3_RST_TY1_1;
    (rd_val & 0x00000004)? CHIP3_SET_TY1_2: CHIP3_RST_TY1_2;
    (rd_val & 0x00000008)? CHIP3_SET_TY1_3: CHIP3_RST_TY1_3;
    (rd_val & 0x00000010)? CHIP3_SET_TY1_4: CHIP3_RST_TY1_4;
}

void    Chip3_Set_Ty1_H4bits(uint16 rd_val)
{
    (rd_val & 0x00000001)? CHIP3_SET_TY1_5: CHIP3_RST_TY1_5;
    (rd_val & 0x00000002)? CHIP3_SET_TY1_6: CHIP3_RST_TY1_6;
    (rd_val & 0x00000004)? CHIP3_SET_TY1_7: CHIP3_RST_TY1_7;
    (rd_val & 0x00000008)? CHIP3_SET_TY1_8: CHIP3_RST_TY1_8;
    (rd_val & 0x00000010)? CHIP3_SET_TY1_9: CHIP3_RST_TY1_9;
}

uint32  Chip3_Rtn_Ty1()
{
    uint32  rd_val = 0;

    if (CHIP3_CHK_TY1_0) BIT_SET(rd_val, 0x00000001);
    if (CHIP3_CHK_TY1_1) BIT_SET(rd_val, 0x00000002);
    if (CHIP3_CHK_TY1_2) BIT_SET(rd_val, 0x00000004);
    if (CHIP3_CHK_TY1_3) BIT_SET(rd_val, 0x00000008);
    if (CHIP3_CHK_TY1_4) BIT_SET(rd_val, 0x00000010);

    if (CHIP3_CHK_TY1_5) BIT_SET(rd_val, 0x00000020);
    if (CHIP3_CHK_TY1_6) BIT_SET(rd_val, 0x00000040);
    if (CHIP3_CHK_TY1_7) BIT_SET(rd_val, 0x00000080);
    if (CHIP3_CHK_TY1_8) BIT_SET(rd_val, 0x00000100);
    if (CHIP3_CHK_TY1_9) BIT_SET(rd_val, 0x00000200);

    return rd_val;
}

int32    Chip3_Set_Ty1(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x001f)
    {
        PutStr("\t\tChip3_Set_Ty1 value invalid!\r\n");
        return -1;
    }

    Chip3_Set_Ty1_L4bits(rd_val);
    Chip3_Set_Ty1_H4bits(rd_val);

    return 0;
}


int32    Chip3_Set_Cb2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tChip3_Set_Cb2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CB2_0: CHIP3_RST_CB2_0;
    (rd_val & 0x00000002)? CHIP3_SET_CB2_1: CHIP3_RST_CB2_1;
    (rd_val & 0x00000004)? CHIP3_SET_CB2_2: CHIP3_RST_CB2_2;

    return 0;
}


int32    Chip3_Set_Cb1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tChip3_Set_Cb1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CB1_0: CHIP3_RST_CB1_0;
    (rd_val & 0x00000002)? CHIP3_SET_CB1_1: CHIP3_RST_CB1_1;
    (rd_val & 0x00000004)? CHIP3_SET_CB1_2: CHIP3_RST_CB1_2;

    return 0;
}


int32    Chip3_Set_Phs(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Phs value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_PHS: CHIP3_RST_PHS;

    return 0;
}


int32    Chip3_Set_Cap3(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Cap3 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CAP3_0: CHIP3_RST_CAP3_0;
    (rd_val & 0x00000002)? CHIP3_SET_CAP3_1: CHIP3_RST_CAP3_1;
    (rd_val & 0x00000004)? CHIP3_SET_CAP3_2: CHIP3_RST_CAP3_2;
    (rd_val & 0x00000008)? CHIP3_SET_CAP3_3: CHIP3_RST_CAP3_3;
    (rd_val & 0x00000010)? CHIP3_SET_CAP3_4: CHIP3_RST_CAP3_4;

    return 0;
}


int32    Chip3_Set_Bs3(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x0f)
    {
        PutStr("\t\tChip3_Set_Bs3 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_BS3_0: CHIP3_RST_BS3_0;
    (rd_val & 0x00000002)? CHIP3_SET_BS3_1: CHIP3_RST_BS3_1;
    (rd_val & 0x00000004)? CHIP3_SET_BS3_2: CHIP3_RST_BS3_2;
    (rd_val & 0x00000008)? CHIP3_SET_BS3_3: CHIP3_RST_BS3_3;

    return 0;
}


int32    Chip3_Set_Mdiv3(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x7f)
    {
        PutStr("\t\tChip3_Set_Mdiv3 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_MDIV3_0: CHIP3_RST_MDIV3_0;
    (rd_val & 0x00000002)? CHIP3_SET_MDIV3_1: CHIP3_RST_MDIV3_1;
    (rd_val & 0x00000004)? CHIP3_SET_MDIV3_2: CHIP3_RST_MDIV3_2;
    (rd_val & 0x00000008)? CHIP3_SET_MDIV3_3: CHIP3_RST_MDIV3_3;
    (rd_val & 0x00000010)? CHIP3_SET_MDIV3_4: CHIP3_RST_MDIV3_4;
    (rd_val & 0x00000020)? CHIP3_SET_MDIV3_5: CHIP3_RST_MDIV3_5;
    (rd_val & 0x00000040)? CHIP3_SET_MDIV3_6: CHIP3_RST_MDIV3_6;

    return 0;
}


int32    Chip3_Set_Cap2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Cap2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CAP2_0: CHIP3_RST_CAP2_0;
    (rd_val & 0x00000002)? CHIP3_SET_CAP2_1: CHIP3_RST_CAP2_1;
    (rd_val & 0x00000004)? CHIP3_SET_CAP2_2: CHIP3_RST_CAP2_2;
    (rd_val & 0x00000008)? CHIP3_SET_CAP2_3: CHIP3_RST_CAP2_3;
    (rd_val & 0x00000010)? CHIP3_SET_CAP2_4: CHIP3_RST_CAP2_4;

    return 0;
}


int32    Chip3_Set_Bs2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x0f)
    {
        PutStr("\t\tChip3_Set_Bs2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_BS2_0: CHIP3_RST_BS2_0;
    (rd_val & 0x00000002)? CHIP3_SET_BS2_1: CHIP3_RST_BS2_1;
    (rd_val & 0x00000004)? CHIP3_SET_BS2_2: CHIP3_RST_BS2_2;
    (rd_val & 0x00000008)? CHIP3_SET_BS2_3: CHIP3_RST_BS2_3;

    return 0;
}


int32    Chip3_Set_Mdiv2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x7f)
    {
        PutStr("\t\tChip3_Set_Mdiv2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_MDIV2_0: CHIP3_RST_MDIV2_0;
    (rd_val & 0x00000002)? CHIP3_SET_MDIV2_1: CHIP3_RST_MDIV2_1;
    (rd_val & 0x00000004)? CHIP3_SET_MDIV2_2: CHIP3_RST_MDIV2_2;
    (rd_val & 0x00000008)? CHIP3_SET_MDIV2_3: CHIP3_RST_MDIV2_3;
    (rd_val & 0x00000010)? CHIP3_SET_MDIV2_4: CHIP3_RST_MDIV2_4;
    (rd_val & 0x00000020)? CHIP3_SET_MDIV2_5: CHIP3_RST_MDIV2_5;
    (rd_val & 0x00000040)? CHIP3_SET_MDIV2_6: CHIP3_RST_MDIV2_6;

    return 0;
}


int32    Chip3_Set_Cap1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Cap1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CAP1_0: CHIP3_RST_CAP1_0;
    (rd_val & 0x00000002)? CHIP3_SET_CAP1_1: CHIP3_RST_CAP1_1;
    (rd_val & 0x00000004)? CHIP3_SET_CAP1_2: CHIP3_RST_CAP1_2;
    (rd_val & 0x00000008)? CHIP3_SET_CAP1_3: CHIP3_RST_CAP1_3;
    (rd_val & 0x00000010)? CHIP3_SET_CAP1_4: CHIP3_RST_CAP1_4;

    return 0;
}


int32    Chip3_Set_Bs1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x0f)
    {
        PutStr("\t\tChip3_Set_Bs1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_BS1_0: CHIP3_RST_BS1_0;
    (rd_val & 0x00000002)? CHIP3_SET_BS1_1: CHIP3_RST_BS1_1;
    (rd_val & 0x00000004)? CHIP3_SET_BS1_2: CHIP3_RST_BS1_2;
    (rd_val & 0x00000008)? CHIP3_SET_BS1_3: CHIP3_RST_BS1_3;

    return 0;
}


int32    Chip3_Set_Mdiv1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x7f)
    {
        PutStr("\t\tChip3_Set_Mdiv1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_MDIV1_0: CHIP3_RST_MDIV1_0;
    (rd_val & 0x00000002)? CHIP3_SET_MDIV1_1: CHIP3_RST_MDIV1_1;
    (rd_val & 0x00000004)? CHIP3_SET_MDIV1_2: CHIP3_RST_MDIV1_2;
    (rd_val & 0x00000008)? CHIP3_SET_MDIV1_3: CHIP3_RST_MDIV1_3;
    (rd_val & 0x00000010)? CHIP3_SET_MDIV1_4: CHIP3_RST_MDIV1_4;
    (rd_val & 0x00000020)? CHIP3_SET_MDIV1_5: CHIP3_RST_MDIV1_5;
    (rd_val & 0x00000040)? CHIP3_SET_MDIV1_6: CHIP3_RST_MDIV1_6;

    return 0;
}


int32    Chip3_Set_Cap0(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tChip3_Set_Cap0 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CAP0_0: CHIP3_RST_CAP0_0;
    (rd_val & 0x00000002)? CHIP3_SET_CAP0_1: CHIP3_RST_CAP0_1;
    (rd_val & 0x00000004)? CHIP3_SET_CAP0_2: CHIP3_RST_CAP0_2;
    (rd_val & 0x00000008)? CHIP3_SET_CAP0_3: CHIP3_RST_CAP0_3;
    (rd_val & 0x00000010)? CHIP3_SET_CAP0_4: CHIP3_RST_CAP0_4;

    return 0;
}


int32    Chip3_Set_Bs0(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x0f)
    {
        PutStr("\t\tChip3_Set_Bs0 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_BS0_0: CHIP3_RST_BS0_0;
    (rd_val & 0x00000002)? CHIP3_SET_BS0_1: CHIP3_RST_BS0_1;
    (rd_val & 0x00000004)? CHIP3_SET_BS0_2: CHIP3_RST_BS0_2;
    (rd_val & 0x00000008)? CHIP3_SET_BS0_3: CHIP3_RST_BS0_3;

    return 0;
}


int32    Chip3_Set_Mdiv0(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x7f)
    {
        PutStr("\t\tChip3_Set_Mdiv0 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_MDIV0_0: CHIP3_RST_MDIV0_0;
    (rd_val & 0x00000002)? CHIP3_SET_MDIV0_1: CHIP3_RST_MDIV0_1;
    (rd_val & 0x00000004)? CHIP3_SET_MDIV0_2: CHIP3_RST_MDIV0_2;
    (rd_val & 0x00000008)? CHIP3_SET_MDIV0_3: CHIP3_RST_MDIV0_3;
    (rd_val & 0x00000010)? CHIP3_SET_MDIV0_4: CHIP3_RST_MDIV0_4;
    (rd_val & 0x00000020)? CHIP3_SET_MDIV0_5: CHIP3_RST_MDIV0_5;
    (rd_val & 0x00000040)? CHIP3_SET_MDIV0_6: CHIP3_RST_MDIV0_6;

    return 0;
}

/// functions for Scan chain B:
int32    Chip3_Set_Adc(uint16 rd_val)
{
    if (rd_val != 0x0000 && rd_val > 0x03ff)
    {
        PutStr("\t\tChip3_Set_Adc value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_ADC_0: CHIP3_RST_ADC_0;
    (rd_val & 0x00000002)? CHIP3_SET_ADC_1: CHIP3_RST_ADC_1;
    (rd_val & 0x00000004)? CHIP3_SET_ADC_2: CHIP3_RST_ADC_2;
    (rd_val & 0x00000008)? CHIP3_SET_ADC_3: CHIP3_RST_ADC_3;
    (rd_val & 0x00000010)? CHIP3_SET_ADC_4: CHIP3_RST_ADC_4;
    (rd_val & 0x00000020)? CHIP3_SET_ADC_5: CHIP3_RST_ADC_5;
    (rd_val & 0x00000040)? CHIP3_SET_ADC_6: CHIP3_RST_ADC_6;
    (rd_val & 0x00000080)? CHIP3_SET_ADC_7: CHIP3_RST_ADC_7;
    (rd_val & 0x00000100)? CHIP3_SET_ADC_8: CHIP3_RST_ADC_8;
    (rd_val & 0x00000200)? CHIP3_SET_ADC_9: CHIP3_RST_ADC_9;

    return 0;
}

uint32  Chip3_Rtn_Adc()
{
    uint32  rd_val = 0;

    if (CHIP3_CHK_ADC_0) BIT_SET(rd_val, 0x00000001);
    if (CHIP3_CHK_ADC_1) BIT_SET(rd_val, 0x00000002);
    if (CHIP3_CHK_ADC_2) BIT_SET(rd_val, 0x00000004);
    if (CHIP3_CHK_ADC_3) BIT_SET(rd_val, 0x00000008);
    if (CHIP3_CHK_ADC_4) BIT_SET(rd_val, 0x00000010);
    if (CHIP3_CHK_ADC_5) BIT_SET(rd_val, 0x00000020);
    if (CHIP3_CHK_ADC_6) BIT_SET(rd_val, 0x00000040);
    if (CHIP3_CHK_ADC_7) BIT_SET(rd_val, 0x00000080);
    if (CHIP3_CHK_ADC_8) BIT_SET(rd_val, 0x00000100);
    if (CHIP3_CHK_ADC_9) BIT_SET(rd_val, 0x00000200);

    return rd_val;
}

int32    Chip3_Set_Adc_Fnl(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Adc_Fnl value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_ADC_FNL: CHIP3_RST_ADC_FNL;

    return 0;
}


int32    Chip3_Set_Rstn_Adc(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Rstn_Adc value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_RSTN_ADC: CHIP3_RST_RSTN_ADC;

    return 0;
}


int32    Chip3_Set_Clk_Adc(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Clk_Adc value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CLK_ADC: CHIP3_RST_CLK_ADC;

    return 0;
}

int32    Chip3_Set_Clrn(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x01)
    {
        PutStr("\t\tChip3_Set_Clrn value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x00000001)? CHIP3_SET_CLRN: CHIP3_RST_CLRN;

    return 0;
}


/* [] END OF FILE */
