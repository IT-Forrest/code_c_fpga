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
#include    "scan_chain.h"
#include    "serial_port_io.h"
#include    "device.h"
#include    "psoc_port.h"
#include    "set_config.h"
#include    "test_adc.h"

// use for Setup information
uint32    gcfg[MAX_CFG_BITS];       // configuration bits array
uint32    gcfg_bk[MAX_CFG_BITS];    // configuration bits backup
uint32    gcfg_tx[MAX_CFG_BITS];    // configuration bits delivered to FPGA

int32   SetFSEL(uint8 rd_val);

typedef struct FREQ_FSEL_M123_STRU
{
    //uint16_t freq;
    uint16_t fsel;
    uint16_t m123;
    uint16_t cap1;
    uint16_t cap2;
    uint16_t vres;

}FREQ_FSEL_M123;


FREQ_FSEL_M123 gMapping_Array[MAX_FSEL_LEN] = {
{	1	,	32	,	1	,	15	,	71	},
{	1	,	33	,	1	,	15	,	71	},
{	1	,	34	,	1	,	14	,	71	},
{	1	,	35	,	1	,	14	,	71	},
{	1	,	36	,	1	,	13	,	71	},
{	1	,	37	,	1	,	13	,	71	},
{	1	,	38	,	1	,	12	,	71	},
{	1	,	39	,	1	,	12	,	71	},
{	1	,	40	,	1	,	12	,	71	},
{	1	,	41	,	1	,	11	,	51	},
{	1	,	42	,	1	,	11	,	51	},
{	1	,	43	,	1	,	11	,	51	},
{	1	,	44	,	1	,	11	,	51	},
{	1	,	45	,	1	,	10	,	51	},
{	1	,	46	,	1	,	10	,	51	},
{	2	,	47	,	1	,	10	,	51	},
{	2	,	48	,	1	,	10	,	39	},
{	2	,	49	,	1	,	9	,	39	},
{	2	,	50	,	1	,	9	,	39	},
{	2	,	51	,	1	,	9	,	39	},
{	2	,	52	,	1	,	9	,	39	},
{	2	,	53	,	1	,	9	,	39	},
{	2	,	54	,	1	,	8	,	39	},
{	2	,	55	,	1	,	8	,	39	},
{	2	,	56	,	1	,	8	,	39	},
{	2	,	57	,	1	,	8	,	39	},
{	2	,	58	,	1	,	7	,	31	},
{	2	,	59	,	1	,	7	,	31	},
{	2	,	60	,	1	,	7	,	31	},
{	2	,	61	,	1	,	7	,	31	},
{	2	,	62	,	1	,	7	,	31	},
{	2	,	63	,	1	,	7	,	31	},
{	3	,	64	,	1	,	7	,	31	},
{	3	,	65	,	1	,	7	,	31	},
{	3	,	66	,	1	,	7	,	31	},
{	3	,	67	,	1	,	6	,	23	},
{	3	,	68	,	1	,	6	,	23	},
{	3	,	69	,	1	,	6	,	23	},
{	3	,	70	,	1	,	6	,	23	},
{	3	,	71	,	1	,	6	,	23	},
{	3	,	72	,	1	,	6	,	23	},
{	3	,	73	,	1	,	6	,	23	},
{	3	,	74	,	1	,	5	,	23	},
{	3	,	75	,	1	,	5	,	23	},
{	3	,	76	,	1	,	5	,	23	},
{	3	,	77	,	1	,	5	,	23	},
{	3	,	78	,	1	,	5	,	23	},
{	3	,	79	,	1	,	5	,	19	},
{	3	,	80	,	1	,	5	,	19	},
{	3	,	81	,	1	,	4	,	19	},
{	3	,	82	,	1	,	4	,	19	},
{	3	,	83	,	1	,	4	,	19	},
{	3	,	84	,	1	,	4	,	19	},
{	3	,	85	,	1	,	4	,	19	},
{	3	,	86	,	1	,	4	,	19	},
{	3	,	87	,	1	,	4	,	19	},
{	3	,	88	,	1	,	4	,	19	},
{	3	,	89	,	1	,	4	,	19	},
{	3	,	90	,	1	,	4	,	19	},
{	3	,	91	,	1	,	3	,	15	},
{	3	,	92	,	1	,	3	,	15	},
{	3	,	93	,	1	,	3	,	15	},
{	3	,	94	,	1	,	3	,	15	},
{	3	,	95	,	1	,	3	,	15	},
{	3	,	96	,	1	,	3	,	15	},
{	3	,	97	,	1	,	3	,	15	},
{	3	,	98	,	1	,	3	,	15	},
{	3	,	99	,	0	,	4	,	15	},
{	3	,	100	,	0	,	4	,	15	},
{	3	,	101	,	1	,	3	,	15	},
{	3	,	102	,	1	,	3	,	15	},
{	3	,	103	,	1	,	3	,	15	},
{	3	,	104	,	1	,	3	,	15	},
{	3	,	105	,	0	,	3	,	15	},
{	3	,	106	,	0	,	3	,	11	},
{	3	,	107	,	0	,	3	,	11	},
{	3	,	108	,	0	,	3	,	11	},
{	3	,	109	,	0	,	3	,	11	},
{	3	,	110	,	0	,	3	,	11	},
{	3	,	111	,	0	,	3	,	11	},
{	3	,	112	,	0	,	3	,	11	},
{	3	,	113	,	0	,	3	,	11	},
{	3	,	114	,	0	,	3	,	11	},
{	3	,	115	,	0	,	3	,	11	},
{	3	,	116	,	0	,	3	,	11	},
{	3	,	117	,	0	,	3	,	11	},
{	3	,	118	,	0	,	3	,	11	},
{	3	,	119	,	0	,	3	,	11	},
{	3	,	120	,	0	,	3	,	11	},
{	3	,	121	,	0	,	3	,	11	},
{	3	,	122	,	0	,	3	,	11	},
{	3	,	123	,	0	,	3	,	11	},
{	3	,	124	,	0	,	3	,	11	},
{	3	,	125	,	0	,	3	,	11	},
{	3	,	126	,	0	,	3	,	11	},
{	3	,	127	,	0	,	3	,	11	}
};

int init_cfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg[i] = 0;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_bk[i] = 0xffffffff;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_tx[i] = 0;
    return (0);
}

/* ==================== Initialize Filter Configuration ====================== */
void    InitCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg[i] = 0;
    //    cfg_m = 0;//cfg_m_bk;
    //    cfg_h = 0;//cfg_h_bk;
    //    cfg_l = 0;//cfg_l_bk;
}

/* ==================== Load Old Filter Configuration ====================== */
void    LoadCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg[i] = gcfg_bk[i];
    //    cfg_m = cfg_m_bk;
    //    cfg_h = cfg_h_bk;
    //    cfg_l = cfg_l_bk;
}

/* ==================== Backup Filter Configuration ====================== */
void    BackupCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_bk[i] = gcfg[i];
    //    cfg_m_bk = cfg_m;
    //    cfg_h_bk = cfg_h;
    //    cfg_l_bk = cfg_l;
}

//void    ClearBkCfg()
//{
//    cfg_m_bk = 0;
//    cfg_h_bk = 0;
//    cfg_l_bk = 0;
//}

void    TranxCfg()
{
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++) gcfg_tx[i] = gcfg[i];
    //    cfg_m_tx = cfg_m;
    //    cfg_h_tx = cfg_h;
    //    cfg_l_tx = cfg_l;
}

void    RShiftCfg()
{
    gcfg_tx[CFG_L] = gcfg_tx[CFG_L] >> 1;
    gcfg_tx[CFG_L] = gcfg_tx[CFG_L] | ((gcfg_tx[CFG_H] & 0x1) << 31);
    gcfg_tx[CFG_H] = gcfg_tx[CFG_H] >> 1;
    gcfg_tx[CFG_H] = gcfg_tx[CFG_H] | ((gcfg_tx[CFG_M] & 0x1) << 31);
    gcfg_tx[CFG_M] = gcfg_tx[CFG_M] >> 1;
    gcfg_tx[CFG_M] = gcfg_tx[CFG_M] | ((gcfg_tx[CFG_S] & 0x1) << 31);
    gcfg_tx[CFG_S] = gcfg_tx[CFG_S] >> 1;

    //cfg_l_tx = cfg_l_tx >> 1;
    //cfg_l_tx = cfg_l_tx + ((cfg_h_tx & 0x1) << 31);
    //cfg_h_tx = cfg_h_tx >> 1;
    //cfg_h_tx = cfg_h_tx + ((cfg_m_tx & 0x1) << 31);
    //cfg_m_tx = cfg_m_tx >> 1;
}


/* ==================== Judge if old Configuration exist ====================== */
int32   IsBkCfgValid()
{
    int32   rtn = 0;
    int i;
    for (i = 0; i<MAX_CFG_BITS; i++)
    if (gcfg_bk[i] != 0xffffffff)
        rtn = 1;
    //    if (cfg_m_bk != 0xffffffff || cfg_h_bk != 0xffffffff || cfg_l_bk != 0xffffffff)
    //        rtn = 1;
    //    else
    //        rtn = 0;

    return rtn;
}


// (1) Test for Analog Circuit
int32   SetFSEL(uint8 rd_val)
{
    if (rd_val < 0x01 || rd_val > 0x03)
    {
        PutStr("\t\tSetFSEL value invalid!\r\n");
        return -1;
    }

    // Version 2: FSEL are no longer on scan chain;
    // we have to set FSEL by verilog
    (rd_val & 0x01)?SET_FSEL_0:RESET_FSEL_0;
    (rd_val & 0x02)?SET_FSEL_1:RESET_FSEL_1;
    return 0;
}

uint8   RtnFSEL()
{
    uint8   rd_val = 0;

    if (TEST_FSEL_0) BIT_SET(rd_val,0x01);
    if (TEST_FSEL_1) BIT_SET(rd_val,0x02);

    return  rd_val;
}

int32   SetCK_PHS(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        PutStr("\t\tSetCK_PHS value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CK_PHS:RESET_CK_PHS;
    return 0;
}

uint8   RtnCK_PHS()
{
    uint8   rd_val = 0;
    if (TEST_CK_PHS) BIT_SET(rd_val,0x01);
    return  rd_val;
}

int32   SetCK_SRC(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        PutStr("\t\tSetCK_SRC value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_SRC_1:RESET_SRC_1;
    return 0;
}

uint8   RtnCK_SRC()
{
    uint8   rd_val = 0;
    if (TEST_SRC_1) BIT_SET(rd_val,0x01);
    return  rd_val;
}

int32   SetCK_CAL(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        PutStr("\t\tSetCK_CAL value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CAL_1:RESET_CAL_1;
    return 0;
}

uint8   RtnCK_CAL()
{
    uint8   rd_val = 0;
    if (TEST_CAL_1) BIT_SET(rd_val,0x01);
    return  rd_val;
}

int32   SetCK_OSCD(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        PutStr("\t\tSetCK_SRC value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_OSCD:RESET_OSCD;
    return 0;
}

uint8   RtnCK_OSCD()
{
    uint8   rd_val = 0;
    if (TEST_OSCD) BIT_SET(rd_val,0x01);
    return  rd_val;
}

int32   SetCK_CLRN(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        PutStr("\t\tSetCK_CLRN value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CLRN_1:RESET_CLRN_1;
    return 0;
}

uint8   RtnCK_CLRN()
{
    uint8   rd_val = 0;
    if (TEST_CLRN_1) BIT_SET(rd_val,0x01);
    return  rd_val;
}


int32   SetSgen_CAP1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        PutStr("\t\tSetSgen_CAP1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CCC_4:RESET_CCC_4;

    return 0;
}

uint8   RtnSgen_CAP1()
{
    uint8   rd_val = 0;
    if (TEST_CCC_4) BIT_SET(rd_val,0x01);
    return  rd_val;
}

int32   SetSgen_CAP2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x0f)
    {
        PutStr("\t\tSetSgen_CAP2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CCC_0:RESET_CCC_0;
    (rd_val & 0x02)?SET_CCC_1:RESET_CCC_1;
    (rd_val & 0x04)?SET_CCC_2:RESET_CCC_2;
    (rd_val & 0x08)?SET_CCC_3:RESET_CCC_3;

    return 0;
}

uint8   RtnSgen_CAP2()
{
    uint8   rd_val = 0;

    if (TEST_CCC_0) BIT_SET(rd_val,0x01);
    if (TEST_CCC_1) BIT_SET(rd_val,0x02);
    if (TEST_CCC_2) BIT_SET(rd_val,0x04);
    if (TEST_CCC_3) BIT_SET(rd_val,0x08);

    return  rd_val;
}

int32   SetCB1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tSetCB1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CB1_0:RESET_CB1_0;
    (rd_val & 0x02)?SET_CB1_1:RESET_CB1_1;
    (rd_val & 0x04)?SET_CB1_2:RESET_CB1_2;

    return 0;
}

uint8   RtnCB1()
{
    uint8   rd_val = 0;

    if (TEST_CB1_0) BIT_SET(rd_val,0x01);
    if (TEST_CB1_1) BIT_SET(rd_val,0x02);
    if (TEST_CB1_2) BIT_SET(rd_val,0x04);

    return  rd_val;
}

int32   SetCB2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tSetCB2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CB2_0:RESET_CB2_0;
    (rd_val & 0x02)?SET_CB2_1:RESET_CB2_1;
    (rd_val & 0x04)?SET_CB2_2:RESET_CB2_2;

    return 0;
}

uint8   RtnCB2()
{
    uint8   rd_val = 0;

    if (TEST_CB2_0) BIT_SET(rd_val,0x01);
    if (TEST_CB2_1) BIT_SET(rd_val,0x02);
    if (TEST_CB2_2) BIT_SET(rd_val,0x04);

    return  rd_val;
}

int32   SetCB_SW(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        PutStr("\t\tSetCB_SW value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_CB_SW:RESET_CB_SW;

    return 0;
}

uint8   RtnCB_SW()
{
    uint8   rd_val = 0;
    if (TEST_CB_SW) BIT_SET(rd_val,0x01);
    return  rd_val;
}

int32   SetCB3(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tSetCB3 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01) ? SET_CB3_0 : RESET_CB3_0;
    (rd_val & 0x02) ? SET_CB3_1 : RESET_CB3_1;
    (rd_val & 0x04) ? SET_CB3_2 : RESET_CB3_2;

    return 0;
}

uint8   RtnCB3()
{
    uint8   rd_val = 0;

    if (TEST_CB3_0) BIT_SET(rd_val, 0x01);
    if (TEST_CB3_1) BIT_SET(rd_val, 0x02);
    if (TEST_CB3_2) BIT_SET(rd_val, 0x04);

    return  rd_val;
}

int32   SetCB4(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x07)
    {
        PutStr("\t\tSetCB4 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01) ? SET_CB4_0 : RESET_CB4_0;
    (rd_val & 0x02) ? SET_CB4_1 : RESET_CB4_1;
    (rd_val & 0x04) ? SET_CB4_2 : RESET_CB4_2;

    return 0;
}

uint8   RtnCB4()
{
    uint8   rd_val = 0;

    if (TEST_CB4_0) BIT_SET(rd_val, 0x01);
    if (TEST_CB4_1) BIT_SET(rd_val, 0x02);
    if (TEST_CB4_2) BIT_SET(rd_val, 0x04);

    return  rd_val;
}

int32   SetTune_X1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetTune_X1 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_TUNEX1_0:RESET_TUNEX1_0;
    (rd_val & 0x02)?SET_TUNEX1_1:RESET_TUNEX1_1;
    (rd_val & 0x04)?SET_TUNEX1_2:RESET_TUNEX1_2;
    (rd_val & 0x08)?SET_TUNEX1_3:RESET_TUNEX1_3;
    (rd_val & 0x10)?SET_TUNEX1_4:RESET_TUNEX1_4;

    (rd_val & 0x01)?SET_TUNEX1_5:RESET_TUNEX1_5;
    (rd_val & 0x02)?SET_TUNEX1_6:RESET_TUNEX1_6;
    (rd_val & 0x04)?SET_TUNEX1_7:RESET_TUNEX1_7;
    (rd_val & 0x08)?SET_TUNEX1_8:RESET_TUNEX1_8;
    (rd_val & 0x10)?SET_TUNEX1_9:RESET_TUNEX1_9;

    return 0;
}

uint8   RtnTune_X1()
{
    uint8   rd_val = 0;

    if (TEST_TUNEX1_0) BIT_SET(rd_val,0x01);
    if (TEST_TUNEX1_1) BIT_SET(rd_val,0x02);
    if (TEST_TUNEX1_2) BIT_SET(rd_val,0x04);
    if (TEST_TUNEX1_3) BIT_SET(rd_val,0x08);
    if (TEST_TUNEX1_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32   SetTune_X2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetTune_X2 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_TUNEX2_0:RESET_TUNEX2_0;
    (rd_val & 0x02)?SET_TUNEX2_1:RESET_TUNEX2_1;
    (rd_val & 0x04)?SET_TUNEX2_2:RESET_TUNEX2_2;
    (rd_val & 0x08)?SET_TUNEX2_3:RESET_TUNEX2_3;
    (rd_val & 0x10)?SET_TUNEX2_4:RESET_TUNEX2_4;

    (rd_val & 0x01)?SET_TUNEX2_5:RESET_TUNEX2_5;
    (rd_val & 0x02)?SET_TUNEX2_6:RESET_TUNEX2_6;
    (rd_val & 0x04)?SET_TUNEX2_7:RESET_TUNEX2_7;
    (rd_val & 0x08)?SET_TUNEX2_8:RESET_TUNEX2_8;
    (rd_val & 0x10)?SET_TUNEX2_9:RESET_TUNEX2_9;

    return 0;
}

uint8   RtnTune_X2()
{
    uint8   rd_val = 0;

    if (TEST_TUNEX2_0) BIT_SET(rd_val,0x01);
    if (TEST_TUNEX2_1) BIT_SET(rd_val,0x02);
    if (TEST_TUNEX2_2) BIT_SET(rd_val,0x04);
    if (TEST_TUNEX2_3) BIT_SET(rd_val,0x08);
    if (TEST_TUNEX2_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32   SetTune_X3(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetTune_X3 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_TUNEX3_0:RESET_TUNEX3_0;
    (rd_val & 0x02)?SET_TUNEX3_1:RESET_TUNEX3_1;
    (rd_val & 0x04)?SET_TUNEX3_2:RESET_TUNEX3_2;
    (rd_val & 0x08)?SET_TUNEX3_3:RESET_TUNEX3_3;
    (rd_val & 0x10)?SET_TUNEX3_4:RESET_TUNEX3_4;

    (rd_val & 0x01)?SET_TUNEX3_5:RESET_TUNEX3_5;
    (rd_val & 0x02)?SET_TUNEX3_6:RESET_TUNEX3_6;
    (rd_val & 0x04)?SET_TUNEX3_7:RESET_TUNEX3_7;
    (rd_val & 0x08)?SET_TUNEX3_8:RESET_TUNEX3_8;
    (rd_val & 0x10)?SET_TUNEX3_9:RESET_TUNEX3_9;

    return 0;
}

uint8   RtnTune_X3()
{
    uint8   rd_val = 0;

    if (TEST_TUNEX3_0) BIT_SET(rd_val,0x01);
    if (TEST_TUNEX3_1) BIT_SET(rd_val,0x02);
    if (TEST_TUNEX3_2) BIT_SET(rd_val,0x04);
    if (TEST_TUNEX3_3) BIT_SET(rd_val,0x08);
    if (TEST_TUNEX3_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32   SetTune_X4(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetTune_X4 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_TUNEX4_0:RESET_TUNEX4_0;
    (rd_val & 0x02)?SET_TUNEX4_1:RESET_TUNEX4_1;
    (rd_val & 0x04)?SET_TUNEX4_2:RESET_TUNEX4_2;
    (rd_val & 0x08)?SET_TUNEX4_3:RESET_TUNEX4_3;
    (rd_val & 0x10)?SET_TUNEX4_4:RESET_TUNEX4_4;

    (rd_val & 0x01)?SET_TUNEX4_5:RESET_TUNEX4_5;
    (rd_val & 0x02)?SET_TUNEX4_6:RESET_TUNEX4_6;
    (rd_val & 0x04)?SET_TUNEX4_7:RESET_TUNEX4_7;
    (rd_val & 0x08)?SET_TUNEX4_8:RESET_TUNEX4_8;
    (rd_val & 0x10)?SET_TUNEX4_9:RESET_TUNEX4_9;

    return 0;
}

uint8   RtnTune_X4()
{
    uint8   rd_val = 0;

    if (TEST_TUNEX4_0) BIT_SET(rd_val,0x01);
    if (TEST_TUNEX4_1) BIT_SET(rd_val,0x02);
    if (TEST_TUNEX4_2) BIT_SET(rd_val,0x04);
    if (TEST_TUNEX4_3) BIT_SET(rd_val,0x08);
    if (TEST_TUNEX4_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32 SetANA_M1(uint8 rd_val)
{
    if (rd_val > 0x7f)
    {
        PutStr("\t\tSetANA_M1 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_M1_00:RESET_M1_00;
    (rd_val & 0x02)?SET_M1_01:RESET_M1_01;
    (rd_val & 0x04)?SET_M1_02:RESET_M1_02;
    (rd_val & 0x08)?SET_M1_03:RESET_M1_03;
    (rd_val & 0x10)?SET_M1_04:RESET_M1_04;
    (rd_val & 0x20)?SET_M1_05:RESET_M1_05;
    (rd_val & 0x40)?SET_M1_06:RESET_M1_06;

    return 0;
}

uint8   RtnANA_M1()
{
    uint8   rd_val = 0;

    if (TEST_M1_00) BIT_SET(rd_val,0x01);
    if (TEST_M1_01) BIT_SET(rd_val,0x02);
    if (TEST_M1_02) BIT_SET(rd_val,0x04);
    if (TEST_M1_03) BIT_SET(rd_val,0x08);
    if (TEST_M1_04) BIT_SET(rd_val,0x10);
    if (TEST_M1_05) BIT_SET(rd_val,0x20);
    if (TEST_M1_06) BIT_SET(rd_val,0x40);

    return  rd_val;
}


int32 SetANA_M2(uint8 rd_val)
{
    if (rd_val > 0x7f)
    {
        PutStr("\t\tSetANA_M2 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_M2_07:RESET_M2_07;
    (rd_val & 0x02)?SET_M2_08:RESET_M2_08;
    (rd_val & 0x04)?SET_M2_09:RESET_M2_09;
    (rd_val & 0x08)?SET_M2_10:RESET_M2_10;
    (rd_val & 0x10)?SET_M2_11:RESET_M2_11;
    (rd_val & 0x20)?SET_M2_12:RESET_M2_12;
    (rd_val & 0x40)?SET_M2_13:RESET_M2_13;

    return 0;
}

uint8   RtnANA_M2()
{
    uint8   rd_val = 0;

    if (TEST_M2_07) BIT_SET(rd_val,0x01);
    if (TEST_M2_08) BIT_SET(rd_val,0x02);
    if (TEST_M2_09) BIT_SET(rd_val,0x04);
    if (TEST_M2_10) BIT_SET(rd_val,0x08);
    if (TEST_M2_11) BIT_SET(rd_val,0x10);
    if (TEST_M2_12) BIT_SET(rd_val,0x20);
    if (TEST_M2_13) BIT_SET(rd_val,0x40);

    return  rd_val;
}


int32 SetANA_M3(uint8 rd_val)
{
    if (rd_val > 0x7f)
    {
        PutStr("\t\tSetANA_M3 value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_M3_14:RESET_M3_14;
    (rd_val & 0x02)?SET_M3_15:RESET_M3_15;
    (rd_val & 0x04)?SET_M3_16:RESET_M3_16;
    (rd_val & 0x08)?SET_M3_17:RESET_M3_17;
    (rd_val & 0x10)?SET_M3_18:RESET_M3_18;
    (rd_val & 0x20)?SET_M3_19:RESET_M3_19;
    (rd_val & 0x40)?SET_M3_20:RESET_M3_20;

    return 0;
}

uint8   RtnANA_M3()
{
    uint8   rd_val = 0;

    if (TEST_M3_14) BIT_SET(rd_val,0x01);
    if (TEST_M3_15) BIT_SET(rd_val,0x02);
    if (TEST_M3_16) BIT_SET(rd_val,0x04);
    if (TEST_M3_17) BIT_SET(rd_val,0x08);
    if (TEST_M3_18) BIT_SET(rd_val,0x10);
    if (TEST_M3_19) BIT_SET(rd_val,0x20);
    if (TEST_M3_20) BIT_SET(rd_val,0x40);

    return  rd_val;
}


// (2) Test for Optimization
int32   SetDimension4D(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val != 0x01)
    {
        printf("rd_val=%d\n",rd_val);
        PutStr("\t\tSetDimension4D value invalid!\r\n");
        return -1;
    }

    (rd_val & 0x01)?SET_DIMEN_4D:RESET_DIMEN_4D;

    return 0;
}

uint8   RtnDimension4D()
{
    uint8   rd_val = 0;
    if (TEST_DIMEN_4D) BIT_SET(rd_val,0x01);
    return  rd_val;
}

int32   SetAnaBits(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0xff)
    {
        PutStr("\t\tSetAnaBits value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_ANAB_0:RESET_ANAB_0;
    (rd_val & 0x02)?SET_ANAB_1:RESET_ANAB_1;
    (rd_val & 0x04)?SET_ANAB_2:RESET_ANAB_2;
    (rd_val & 0x08)?SET_ANAB_3:RESET_ANAB_3;
    (rd_val & 0x10)?SET_ANAB_4:RESET_ANAB_4;
    (rd_val & 0x20)?SET_ANAB_5:RESET_ANAB_5;
    (rd_val & 0x40)?SET_ANAB_6:RESET_ANAB_6;
    (rd_val & 0x80)?SET_ANAB_7:RESET_ANAB_7;

    return 0;
}

uint8   RtnAnaBits()
{
    uint8   rd_val = 0;

    if (TEST_ANAB_0) BIT_SET(rd_val,0x01);
    if (TEST_ANAB_1) BIT_SET(rd_val,0x02);
    if (TEST_ANAB_2) BIT_SET(rd_val,0x04);
    if (TEST_ANAB_3) BIT_SET(rd_val,0x08);
    if (TEST_ANAB_4) BIT_SET(rd_val,0x10);
    if (TEST_ANAB_5) BIT_SET(rd_val,0x20);
    if (TEST_ANAB_6) BIT_SET(rd_val,0x40);
    if (TEST_ANAB_7) BIT_SET(rd_val,0x80);

    return  rd_val;
}

int32   SetINT_X1(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetInit_X1 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_INTX1_0:RESET_INTX1_0;
    (rd_val & 0x02)?SET_INTX1_1:RESET_INTX1_1;
    (rd_val & 0x04)?SET_INTX1_2:RESET_INTX1_2;
    (rd_val & 0x08)?SET_INTX1_3:RESET_INTX1_3;
    (rd_val & 0x10)?SET_INTX1_4:RESET_INTX1_4;

    return 0;
}

uint8   RtnINT_X1()
{
    uint8   rd_val = 0;

    if (TEST_INTX1_0) BIT_SET(rd_val,0x01);
    if (TEST_INTX1_1) BIT_SET(rd_val,0x02);
    if (TEST_INTX1_2) BIT_SET(rd_val,0x04);
    if (TEST_INTX1_3) BIT_SET(rd_val,0x08);
    if (TEST_INTX1_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32   SetINT_X2(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetInit_X2 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_INTX2_0:RESET_INTX2_0;
    (rd_val & 0x02)?SET_INTX2_1:RESET_INTX2_1;
    (rd_val & 0x04)?SET_INTX2_2:RESET_INTX2_2;
    (rd_val & 0x08)?SET_INTX2_3:RESET_INTX2_3;
    (rd_val & 0x10)?SET_INTX2_4:RESET_INTX2_4;

    return 0;
}

uint8   RtnINT_X2()
{
    uint8   rd_val = 0;

    if (TEST_INTX2_0) BIT_SET(rd_val,0x01);
    if (TEST_INTX2_1) BIT_SET(rd_val,0x02);
    if (TEST_INTX2_2) BIT_SET(rd_val,0x04);
    if (TEST_INTX2_3) BIT_SET(rd_val,0x08);
    if (TEST_INTX2_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32   SetINT_X3(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetInit_X3 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_INTX3_0:RESET_INTX3_0;
    (rd_val & 0x02)?SET_INTX3_1:RESET_INTX3_1;
    (rd_val & 0x04)?SET_INTX3_2:RESET_INTX3_2;
    (rd_val & 0x08)?SET_INTX3_3:RESET_INTX3_3;
    (rd_val & 0x10)?SET_INTX3_4:RESET_INTX3_4;

    return 0;
}

uint8   RtnINT_X3()
{
    uint8   rd_val = 0;

    if (TEST_INTX3_0) BIT_SET(rd_val,0x01);
    if (TEST_INTX3_1) BIT_SET(rd_val,0x02);
    if (TEST_INTX3_2) BIT_SET(rd_val,0x04);
    if (TEST_INTX3_3) BIT_SET(rd_val,0x08);
    if (TEST_INTX3_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32   SetINT_X4(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0x1f)
    {
        PutStr("\t\tSetInit_X4 value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_INTX4_0:RESET_INTX4_0;
    (rd_val & 0x02)?SET_INTX4_1:RESET_INTX4_1;
    (rd_val & 0x04)?SET_INTX4_2:RESET_INTX4_2;
    (rd_val & 0x08)?SET_INTX4_3:RESET_INTX4_3;
    (rd_val & 0x10)?SET_INTX4_4:RESET_INTX4_4;

    return 0;
}

uint8   RtnINT_X4()
{
    uint8   rd_val = 0;

    if (TEST_INTX4_0) BIT_SET(rd_val,0x01);
    if (TEST_INTX4_1) BIT_SET(rd_val,0x02);
    if (TEST_INTX4_2) BIT_SET(rd_val,0x04);
    if (TEST_INTX4_3) BIT_SET(rd_val,0x08);
    if (TEST_INTX4_4) BIT_SET(rd_val,0x10);

    return  rd_val;
}

int32   SetIterNum(uint8 rd_val)
{
    if (rd_val != 0x00 && rd_val > 0xfe)
    {
        PutStr("\t\tSetIter_Num value invalid!\r\n");
        return  -1;
    }

    (rd_val & 0x01)?SET_ITRNUM_0:RESET_ITRNUM_0;
    (rd_val & 0x02)?SET_ITRNUM_1:RESET_ITRNUM_1;
    (rd_val & 0x04)?SET_ITRNUM_2:RESET_ITRNUM_2;
    (rd_val & 0x08)?SET_ITRNUM_3:RESET_ITRNUM_3;
    (rd_val & 0x10)?SET_ITRNUM_4:RESET_ITRNUM_4;
    (rd_val & 0x20)?SET_ITRNUM_5:RESET_ITRNUM_5;
    (rd_val & 0x40)?SET_ITRNUM_6:RESET_ITRNUM_6;
    (rd_val & 0x80)?SET_ITRNUM_7:RESET_ITRNUM_7;

    return 0;
}

uint8   RtnIterNum()
{
    uint8   rd_val = 0;

    if (TEST_ITRNUM_0) BIT_SET(rd_val,0x01);
    if (TEST_ITRNUM_1) BIT_SET(rd_val,0x02);
    if (TEST_ITRNUM_2) BIT_SET(rd_val,0x04);
    if (TEST_ITRNUM_3) BIT_SET(rd_val,0x08);
    if (TEST_ITRNUM_4) BIT_SET(rd_val,0x10);
    if (TEST_ITRNUM_5) BIT_SET(rd_val,0x20);
    if (TEST_ITRNUM_6) BIT_SET(rd_val,0x40);
    if (TEST_ITRNUM_7) BIT_SET(rd_val,0x80);

    return  rd_val;
}

void    debugCfg()
{
    if (0 == DEBUG_Read())
    {
        PutStr("#BUG\tJust for debug\r\n");
        PutStr("#BUG\tDIMEN_4D = "); FmtPrint(TEST_DIMEN_4D ? 1 : 0);PutStr("\r\n");
        PutStr("#BUG\tFSEL = ");FmtPrint(RtnFSEL());PutStr("\r\n");
        PutStr("#BUG\tM1 = "); FmtPrint(RtnANA_M1()); PutStr("\r\n");
        PutStr("#BUG\tM2 = "); FmtPrint(RtnANA_M2()); PutStr("\r\n");
        PutStr("#BUG\tM3 = "); FmtPrint(RtnANA_M3()); PutStr("\r\n");
        PutStr("#BUG\tPHS = ");FmtPrint(RtnCK_PHS()); PutStr("\r\n");
        PutStr("#BUG\tCAL = "); FmtPrint(TEST_CAL_1?1:0);PutStr("\r\n");
        PutStr("#BUG\tSRC = "); FmtPrint(TEST_SRC_1?1:0);PutStr("\r\n");
        PutStr("#BUG\tCLRN = ");FmtPrint(TEST_CLRN_1?1:0);PutStr("\r\n");
        PutStr("#BUG\tCAP2 = ");FmtPrint(RtnSgen_CAP2());PutStr("\r\n");
        PutStr("#BUG\tCAP1 = ");FmtPrint(RtnSgen_CAP1());PutStr("\r\n");
        PutStr("#BUG\tCB_SW = ");FmtPrint(RtnCB_SW());PutStr("\r\n");
        PutStr("#BUG\tCB1 = ");FmtPrint(RtnCB1());PutStr("\r\n");
        PutStr("#BUG\tCB2 = ");FmtPrint(RtnCB2());PutStr("\r\n");
        PutStr("#BUG\tCB3 = ");FmtPrint(RtnCB3());PutStr("\r\n");
        PutStr("#BUG\tCB4 = ");FmtPrint(RtnCB4());PutStr("\r\n");
        PutStr("#BUG\tTuneX1 = ");FmtPrint(RtnTune_X1());PutStr("\r\n");
        PutStr("#BUG\tTuneX2 = ");FmtPrint(RtnTune_X2());PutStr("\r\n");
        PutStr("#BUG\tTuneX3 = ");FmtPrint(RtnTune_X3());PutStr("\r\n");
        PutStr("#BUG\tTuneX4 = ");FmtPrint(RtnTune_X4());PutStr("\r\n");
        PutStr("#BUG\tANA_BITS = "); FmtPrint(RtnAnaBits()); PutStr("\r\n");
        PutStr("#BUG\tINT_X1 = "); FmtPrint(RtnINT_X1());PutStr("\r\n");
        PutStr("#BUG\tINT_X2 = "); FmtPrint(RtnINT_X2());PutStr("\r\n");
        PutStr("#BUG\tINT_X3 = "); FmtPrint(RtnINT_X3()); PutStr("\r\n");
        PutStr("#BUG\tINT_X4 = "); FmtPrint(RtnINT_X4()); PutStr("\r\n");
        PutStr("#BUG\tITER_NUM = ");FmtPrint(RtnIterNum());PutStr("\r\n");
        PutStr("\r\n");
    }
}


/* ==================== Filter Configuration ====================== */
void SetCfg()
{
    /**************************************/
    /*  MSB                               */
    /*  ________|_______|_______|________ */
    /*  |       |       |       |       | */
    /*  | cfg_s | cfg_m | cfg_h | cfg_l | */
    /*  |_______|_______|_______|_______| */
    /*          |       |       |         */
    /*      3.      2.      1.      0.    */
    /*                                127 */
    /**************************************/

	uint16  rd_val;
    uint8   i;
    uint8   dat[12];
    uint8   len;

    InitCfg();

    if (IsBkCfgValid())
    {
        LoadCfg();
        PutStr("#DLC\tRepeat Old Config? 1:Yes; 0:No. [1]/0 ");
        rd_val = ReadInput(&len);
        if ((rd_val & 0x1) || (0 == len))
            goto SEND_COM;
    }
    else
    {
        BackupCfg();//IsBkCfgValid() return zero only one time
    }

    PutStr("#DLC\t0. IS DIMEN_4D("); FmtPrint(TEST_DIMEN_4D ? 1 : 0); PutStr("): [0,1] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetDimension4D(rd_val);

    // (1) Test for Analog Circuit
    // Read FSEL, range [1~3]
    if (0 == RtnANA_M1()) SetANA_M1(60);
    PutStr("#DLC\t1. Set Frequency(");FmtPrint(RtnANA_M1());PutStr("): [0~127] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetFreq(rd_val);

    /*if (0 == RtnFSEL()) SetFSEL(1);
    PutStr("#DLC\t1. FSEL(");FmtPrint(RtnFSEL());PutStr("): [1,2,3] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetFSEL(rd_val);
    //SetFSEL(3);
    //PutStr("#DLC\t\r\n");
    //else SetFSEL(RtnFSEL());

    PutStr("#DLC\t2. M1("); FmtPrint(RtnANA_M1()); PutStr("): [0~127]");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetANA_M1(rd_val);

    PutStr("#DLC\t3. M2("); FmtPrint(RtnANA_M2()); PutStr("): [0~127]");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetANA_M2(rd_val);

    PutStr("#DLC\t4. M3("); FmtPrint(RtnANA_M3()); PutStr("): [0~127]");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetANA_M3(rd_val);*/

    // Read PHS/CAL/SRC
    PutStr("#DLC\t5. Bits for PHS/CAL/SRC(");FmtPrint(TEST_CK_PHS?1:0);
    FmtPrint(TEST_CAL_1?1:0);FmtPrint(TEST_SRC_1?1:0);PutStr("): [0,1] ");
    ReadArray(dat, &len);
    if (USE_NEW_VAL)
        for (i=0; i<len; i++)
        {
            if (0 == i)
                SetCK_PHS((dat[i] & 0x01));
            else if (1 == i)
                SetCK_CAL((dat[i] & 0x01));
            //(dat[i] & 0x01)?SET_CAL_1:RESET_CAL_1;
            else if (2 == i)
                SetCK_SRC((dat[i] & 0x01));
                //(dat[i] & 0x01)?SET_SRC_1:RESET_SRC_1;
        }

    // Read sgen_cap2 range [0,15]
    PutStr("#DLC\t6. CAP2(");FmtPrint(RtnSgen_CAP2());PutStr("): [0~15] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetSgen_CAP2(rd_val);
    //else SetSgen_CAP2(RtnSgen_CAP2());

    // Read sgen_cap1 range 0 or 1
    PutStr("#DLC\t7. CAP1(");FmtPrint(TEST_CCC_4?1:0);PutStr("): [0,1] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetSgen_CAP1(rd_val);
    //else SetSgen_CAP1(TEST_CCC_4?1:0);

    // Read SW value [0, 1]
    //if (0 == RtnCB_SW()) SetCB_SW(1);
    PutStr("#DLC\t8. CB_SW(");FmtPrint(RtnCB_SW());PutStr("): [0,1] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetCB_SW(rd_val);

    // Read CB1 range [0,7]
    if (0 == RtnCB1()) SetCB1(3);
    PutStr("#DLC\t9. CB1(");FmtPrint(RtnCB1());PutStr("): [0~7] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetCB1(rd_val);
    //else SetCB1(RtnCB1());

    // Read CB2 range [0,7]
    if (0 == RtnCB2()) SetCB2(3);
    PutStr("#DLC\t10. CB2(");FmtPrint(RtnCB2());PutStr("): [0~7] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetCB2(rd_val);
    //else SetCB2(RtnCB2());

    // Read CB3 range [0,7]
    if (0 == RtnCB3()) SetCB3(3);
    PutStr("#DLC\t11. CB3("); FmtPrint(RtnCB3()); PutStr("): [0~7] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetCB3(rd_val);
    //else SetCB1(RtnCB1());

    // Read CB4 range [0,7]
    if (0 == RtnCB4()) SetCB4(3);
    PutStr("#DLC\t12. CB4("); FmtPrint(RtnCB4()); PutStr("): [0~7] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetCB4(rd_val);
    //else SetCB2(RtnCB2());


    // Read TUNE_X1 range [0~31]
    if (0 == RtnTune_X1()) SetTune_X1(15);
    PutStr("#DLC\t13. Tune_X1(");FmtPrint(RtnTune_X1());PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetTune_X1(rd_val);
    //else SetTune_X(RtnTune_X());

    // Read TUNE_X2 range [0~31]
    if (0 == RtnTune_X2()) SetTune_X2(15);
    PutStr("#DLC\t14. Tune_X2(");FmtPrint(RtnTune_X2());PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetTune_X2(rd_val);
    //else SetTune_Y(RtnTune_Y());

    // Read TUNE_X3 range [0~31]
    if (0 == RtnTune_X3()) SetTune_X3(15);
    PutStr("#DLC\t15. Tune_X3("); FmtPrint(RtnTune_X3()); PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetTune_X3(rd_val);
    //else SetTune_X(RtnTune_X());

    // Read TUNE_X4 range [0~31]
    if (0 == RtnTune_X4()) SetTune_X4(15);
    PutStr("#DLC\t16. Tune_X4("); FmtPrint(RtnTune_X4()); PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetTune_X4(rd_val);
    //else SetTune_Y(RtnTune_Y());


    // (2) Test for Optimization; available later
#if 0
    // Read ANA_BITS range [0~255]
    PutStr("#DLC\tA. ANA_BITS(");FmtPrint(RtnAnaBits());PutStr("): [0~255] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetAnaBits(rd_val);
    //else SetAnaBits(RtnAnaBits());

    // Read INT_X1 range [0~31]
    PutStr("#DLC\tB. INT_X1(");FmtPrint(RtnINT_X1()); PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetINT_X1(rd_val);
    //else SetINT_X(RtnINT_X());

    // Read INT_X2 range [0~31]
    PutStr("#DLC\tC. INT_X2(");FmtPrint(RtnINT_X2()); PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetINT_X2(rd_val);
    //else SetINT_Y(RtnINT_Y());

    // Read INT_X3 range [0~31]
    PutStr("#DLC\tCa. INT_X3("); FmtPrint(RtnINT_X3()); PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetINT_X3(rd_val);
    //else SetINT_X(RtnINT_X());

    // Read INT_X4 range [0~31]
    PutStr("#DLC\tCb. INT_X4("); FmtPrint(RtnINT_X4()); PutStr("): [0~31] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetINT_X4(rd_val);
    //else SetINT_Y(RtnINT_Y());

    // Read ITER_NUM range [0~127]
    PutStr("#DLC\tD. ITER_NUM(");FmtPrint(RtnIterNum()); PutStr("): [0~255] ");
    rd_val = ReadInput(&len);
    if (USE_NEW_VAL) SetIterNum(rd_val);
    //else SetIterNum(RtnIterNum());
#endif

	// Read inputs and store them in registers
    BackupCfg();

SEND_COM:
    debugCfg();
    // shift register

}

void SetFreq(uint16 rd_val)
{
    //uint8   i;
    //uint8   dat[12];
    uint16_t  fsel = 2;
    uint16_t  cap1, cap2;
    uint16_t  M123 = 60;
    uint16_t  vres;

    {
        M123 = rd_val;//*3
        if (M123 < 32)  M123 = 32;
        if (M123 > 127) M123 = 127;

        fsel = gMapping_Array[M123-32].fsel;
        cap1 = gMapping_Array[M123-32].cap1;
        cap2 = gMapping_Array[M123-32].cap2;
        vres = gMapping_Array[M123-32].vres;
    }

    spi_set_res(vres);

    SetFSEL(fsel);
    SetANA_M1(M123);
    SetANA_M2(M123);
    SetANA_M3(M123);
    SetSgen_CAP1(cap1);
    SetSgen_CAP2(cap2);

    debugCfg();
    return;
}

int LoadCfgFile(char* file_name, uint16* fno, uint16* flist)
{
    /* Load Config File, Format:
        SW Freq fno f0 f1 f2 ... fn CB1 CB2 CB3 CB4 X1 X2 X3 X4
    */
    FILE * fd;
    int val;
    int i;

    fd = fopen(file_name, "r");
    if (NULL == fd) return 1;

    if(fscanf(fd, "%d", &val)) {SetCB_SW(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetFreq(val);} else {return 1;}
    if(fscanf(fd, "%d", &val))
    {
        *fno = val;
        for (i=0; i<*fno; i++)
        {
            if(fscanf(fd, "%d", &val))
            {
                flist[i] = val;
            } else {break;}
        }
    } else {return 1;}

    if(fscanf(fd, "%d", &val)) {SetCB1(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetCB2(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetCB3(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetCB4(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetTune_X1(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetTune_X2(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetTune_X3(val);} else {return 1;}
    if(fscanf(fd, "%d", &val)) {SetTune_X4(val);} else {return 1;}
    BackupCfg();

    fclose(fd);

    return 0;
}


/* [] END OF FILE */
