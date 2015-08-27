#ifndef _SET_CONFIG_H_
#define _SET_CONFIG_H_

///////////////////////////////////////////////////////////////
// this .h file is only useful for the 1st. tapeout chip.

#include "socal/hps.h"
#include "socal/alt_gpio.h"


#define     MAX_CFG_BITS    5   // in terms of uint32_t
#define     CFG_A           MAX_CFG_BITS-1  // Appendix bits
#define     CFG_S           3   // most significant bits for scan chain
#define     CFG_M           2
#define     CFG_H           1
#define     CFG_L           0   // least significant bits for scan chain

#define     MAX_SC_BITS     127//total scan chain length for version 2
#define     MAX_FSEL_LEN    (127-32+1)

extern uint32_t    gcfg[MAX_CFG_BITS];       // configuration bits array
extern uint32_t    gcfg_bk[MAX_CFG_BITS];    // configuration bits backup
extern uint32_t    gcfg_tx[MAX_CFG_BITS];    // configuration bits delivered to FPGA

/* Initialization */
int     init_cfg();

/* Config Scan Chain */
void    SetCfg();// just config the Analog control signal
void    LoadCfg();
void    debugCfg();
void    TranxCfg();
void    BackupCfg();
void    RShiftCfg();
int     LoadCfgFile(char* file_name, uint16* fno, uint16* flist);

/* Check Control Signal */
uint8   RtnFSEL();
uint8   RtnCK_PHS();
uint8   RtnCK_SRC();
uint8   RtnCK_CAL();
uint8   RtnCK_OSCD();
uint8   RtnSgen_CAP2();

/* Set Control Variables */
int32   SetCB1(uint8);
int32   SetCB2(uint8);
int32   SetCB3(uint8);
int32   SetCB4(uint8);
int32   SetCB_SW(uint8);
int32   SetFSEL(uint8 rd_val);
int32   SetCK_PHS(uint8 rd_val);
int32   SetCK_CAL(uint8 rd_val);
int32   SetCK_SRC(uint8 rd_val);
int32   SetCK_OSCD(uint8 rd_val);
int32   SetTune_X1(uint8 rd_val);
int32   SetTune_X2(uint8 rd_val);
int32   SetTune_X3(uint8 rd_val);
int32   SetTune_X4(uint8 rd_val);
int32   SetAnaBits(uint8 rd_val);
int32   SetINT_X1(uint8 rd_val);
int32   SetINT_X2(uint8 rd_val);
int32   SetINT_X3(uint8 rd_val);
int32   SetINT_X4(uint8 rd_val);
int32   SetIterNum(uint8 rd_val);
int32   SetSgen_CAP1(uint8 rd_val);
int32   SetSgen_CAP2(uint8 rd_val);
void    SetFreq(uint16);

/****************************************************/
/*  MSB                               */
/*  ________|_______|_______|________ */
/*  |       |       |       |       | */
/*  | cfg_s | cfg_m | cfg_h | cfg_l | */
/*  |_______|_______|_______|_______| */
/*          |       |       |         */
/*      3.      2.      1.      0.    */
/*                                127 */
/****************************************************/

#define BIT_SET(F, B)           ((F) |= (B))
#define BIT_RESET(F, B)         ((F) &= ~(B))
#define BIT_TEST(F, B)          ((F) & (B))

/////////// Set of Analog Circuit ////////////////
#define SET_FSEL_1       BIT_SET(gcfg[CFG_A], 0x00000001)
#define SET_FSEL_0       BIT_SET(gcfg[CFG_A], 0x00000002)
#define SET_CAL_1        BIT_SET(gcfg[CFG_A], 0x00000004)
#define SET_SRC_1        BIT_SET(gcfg[CFG_A], 0x00000008)
#define SET_CLRN_1       BIT_SET(gcfg[CFG_A], 0x00000010)
#define SET_CK_PHS       BIT_SET(gcfg[CFG_A], 0x00000020)
#define SET_OSCD         BIT_SET(gcfg[CFG_A], 0x00000040)

#define SET_TUNEX4_9     BIT_SET(gcfg[CFG_L], 0x00000200)
#define SET_TUNEX4_8     BIT_SET(gcfg[CFG_L], 0x00000400)
#define SET_TUNEX4_7     BIT_SET(gcfg[CFG_L], 0x00000800)
#define SET_TUNEX4_6     BIT_SET(gcfg[CFG_L], 0x00001000)
#define SET_TUNEX4_5     BIT_SET(gcfg[CFG_L], 0x00002000)
#define SET_TUNEX4_4     BIT_SET(gcfg[CFG_L], 0x00004000)
#define SET_TUNEX4_3     BIT_SET(gcfg[CFG_L], 0x00008000)
#define SET_TUNEX4_2     BIT_SET(gcfg[CFG_L], 0x00010000)
#define SET_TUNEX4_1     BIT_SET(gcfg[CFG_L], 0x00020000)
#define SET_TUNEX4_0     BIT_SET(gcfg[CFG_L], 0x00040000)

#define SET_TUNEX3_9     BIT_SET(gcfg[CFG_L], 0x00080000)
#define SET_TUNEX3_8     BIT_SET(gcfg[CFG_L], 0x00100000)
#define SET_TUNEX3_7     BIT_SET(gcfg[CFG_L], 0x00200000)
#define SET_TUNEX3_6     BIT_SET(gcfg[CFG_L], 0x00400000)
#define SET_TUNEX3_5     BIT_SET(gcfg[CFG_L], 0x00800000)
#define SET_TUNEX3_4     BIT_SET(gcfg[CFG_L], 0x01000000)
#define SET_TUNEX3_3     BIT_SET(gcfg[CFG_L], 0x02000000)
#define SET_TUNEX3_2     BIT_SET(gcfg[CFG_L], 0x04000000)
#define SET_TUNEX3_1     BIT_SET(gcfg[CFG_L], 0x08000000)
#define SET_TUNEX3_0     BIT_SET(gcfg[CFG_L], 0x10000000)

#define SET_TUNEX2_9     BIT_SET(gcfg[CFG_L], 0x20000000)
#define SET_TUNEX2_8     BIT_SET(gcfg[CFG_L], 0x40000000)
#define SET_TUNEX2_7     BIT_SET(gcfg[CFG_L], 0x80000000)
#define SET_TUNEX2_6     BIT_SET(gcfg[CFG_H], 0x00000001)
#define SET_TUNEX2_5     BIT_SET(gcfg[CFG_H], 0x00000002)
#define SET_TUNEX2_4     BIT_SET(gcfg[CFG_H], 0x00000004)
#define SET_TUNEX2_3     BIT_SET(gcfg[CFG_H], 0x00000008)
#define SET_TUNEX2_2     BIT_SET(gcfg[CFG_H], 0x00000010)
#define SET_TUNEX2_1     BIT_SET(gcfg[CFG_H], 0x00000020)
#define SET_TUNEX2_0     BIT_SET(gcfg[CFG_H], 0x00000040)

#define SET_TUNEX1_9     BIT_SET(gcfg[CFG_H], 0x00000080)
#define SET_TUNEX1_8     BIT_SET(gcfg[CFG_H], 0x00000100)
#define SET_TUNEX1_7     BIT_SET(gcfg[CFG_H], 0x00000200)
#define SET_TUNEX1_6     BIT_SET(gcfg[CFG_H], 0x00000400)
#define SET_TUNEX1_5     BIT_SET(gcfg[CFG_H], 0x00000800)
#define SET_TUNEX1_4     BIT_SET(gcfg[CFG_H], 0x00001000)
#define SET_TUNEX1_3     BIT_SET(gcfg[CFG_H], 0x00002000)
#define SET_TUNEX1_2     BIT_SET(gcfg[CFG_H], 0x00004000)
#define SET_TUNEX1_1     BIT_SET(gcfg[CFG_H], 0x00008000)
#define SET_TUNEX1_0     BIT_SET(gcfg[CFG_H], 0x00010000)

// #define SET_ADC_0       BIT_SET(gcfg[CFG_H], 0x00400000)
// #define SET_ADC_1       BIT_SET(gcfg[CFG_H], 0x00200000)

#define SET_FNL          BIT_SET(gcfg[CFG_M], 0x00800000)

#define SET_CB4_0        BIT_SET(gcfg[CFG_M], 0x01000000)
#define SET_CB4_1        BIT_SET(gcfg[CFG_M], 0x02000000)
#define SET_CB4_2        BIT_SET(gcfg[CFG_M], 0x04000000)
#define SET_CB3_2        BIT_SET(gcfg[CFG_M], 0x08000000)
#define SET_CB3_1        BIT_SET(gcfg[CFG_M], 0x10000000)
#define SET_CB3_0        BIT_SET(gcfg[CFG_M], 0x20000000)

#define SET_CB_SW        BIT_SET(gcfg[CFG_M], 0x40000000)
#define SET_CB2_0        BIT_SET(gcfg[CFG_M], 0x80000000)
#define SET_CB2_1        BIT_SET(gcfg[CFG_S], 0x00000001)
#define SET_CB2_2        BIT_SET(gcfg[CFG_S], 0x00000002)
#define SET_CB1_2        BIT_SET(gcfg[CFG_S], 0x00000004)
#define SET_CB1_1        BIT_SET(gcfg[CFG_S], 0x00000008)
#define SET_CB1_0        BIT_SET(gcfg[CFG_S], 0x00000010)

#define SET_CCC_0        BIT_SET(gcfg[CFG_S], 0x00000020)
#define SET_CCC_1        BIT_SET(gcfg[CFG_S], 0x00000040)
#define SET_CCC_2        BIT_SET(gcfg[CFG_S], 0x00000080)
#define SET_CCC_3        BIT_SET(gcfg[CFG_S], 0x00000100)
#define SET_CCC_4        BIT_SET(gcfg[CFG_S], 0x00000200)

#define SET_M3_20        BIT_SET(gcfg[CFG_S], 0x00000400)
#define SET_M3_19        BIT_SET(gcfg[CFG_S], 0x00000800)
#define SET_M3_18        BIT_SET(gcfg[CFG_S], 0x00001000)
#define SET_M3_17        BIT_SET(gcfg[CFG_S], 0x00002000)
#define SET_M3_16        BIT_SET(gcfg[CFG_S], 0x00004000)
#define SET_M3_15        BIT_SET(gcfg[CFG_S], 0x00008000)
#define SET_M3_14        BIT_SET(gcfg[CFG_S], 0x00010000)

#define SET_M2_13        BIT_SET(gcfg[CFG_S], 0x00020000)
#define SET_M2_12        BIT_SET(gcfg[CFG_S], 0x00040000)
#define SET_M2_11        BIT_SET(gcfg[CFG_S], 0x00080000)
#define SET_M2_10        BIT_SET(gcfg[CFG_S], 0x00100000)
#define SET_M2_09        BIT_SET(gcfg[CFG_S], 0x00200000)
#define SET_M2_08        BIT_SET(gcfg[CFG_S], 0x00400000)
#define SET_M2_07        BIT_SET(gcfg[CFG_S], 0x00800000)

#define SET_M1_06        BIT_SET(gcfg[CFG_S], 0x01000000)
#define SET_M1_05        BIT_SET(gcfg[CFG_S], 0x02000000)
#define SET_M1_04        BIT_SET(gcfg[CFG_S], 0x04000000)
#define SET_M1_03        BIT_SET(gcfg[CFG_S], 0x08000000)
#define SET_M1_02        BIT_SET(gcfg[CFG_S], 0x10000000)
#define SET_M1_01        BIT_SET(gcfg[CFG_S], 0x20000000)
#define SET_M1_00        BIT_SET(gcfg[CFG_S], 0x40000000)

// SET of Optimization
#define SET_DIMEN_4D     BIT_SET(gcfg[CFG_L], 0x00000001)

#define SET_ITRNUM_7     BIT_SET(gcfg[CFG_L], 0x00000002)
#define SET_ITRNUM_6     BIT_SET(gcfg[CFG_L], 0x00000004)
#define SET_ITRNUM_5     BIT_SET(gcfg[CFG_L], 0x00000008)
#define SET_ITRNUM_4     BIT_SET(gcfg[CFG_L], 0x00000010)
#define SET_ITRNUM_3     BIT_SET(gcfg[CFG_L], 0x00000020)
#define SET_ITRNUM_2     BIT_SET(gcfg[CFG_L], 0x00000040)
#define SET_ITRNUM_1     BIT_SET(gcfg[CFG_L], 0x00000080)
#define SET_ITRNUM_0     BIT_SET(gcfg[CFG_L], 0x00000100)

#define SET_INTX4_4      BIT_SET(gcfg[CFG_H], 0x00020000)
#define SET_INTX4_3      BIT_SET(gcfg[CFG_H], 0x00040000)
#define SET_INTX4_2      BIT_SET(gcfg[CFG_H], 0x00080000)
#define SET_INTX4_1      BIT_SET(gcfg[CFG_H], 0x00100000)
#define SET_INTX4_0      BIT_SET(gcfg[CFG_H], 0x00200000)

#define SET_INTX3_4      BIT_SET(gcfg[CFG_H], 0x00400000)
#define SET_INTX3_3      BIT_SET(gcfg[CFG_H], 0x00800000)
#define SET_INTX3_2      BIT_SET(gcfg[CFG_H], 0x01000000)
#define SET_INTX3_1      BIT_SET(gcfg[CFG_H], 0x02000000)
#define SET_INTX3_0      BIT_SET(gcfg[CFG_H], 0x04000000)

#define SET_INTX2_4      BIT_SET(gcfg[CFG_H], 0x08000000)
#define SET_INTX2_3      BIT_SET(gcfg[CFG_H], 0x10000000)
#define SET_INTX2_2      BIT_SET(gcfg[CFG_H], 0x20000000)
#define SET_INTX2_1      BIT_SET(gcfg[CFG_H], 0x40000000)
#define SET_INTX2_0      BIT_SET(gcfg[CFG_H], 0x80000000)

#define SET_INTX1_4      BIT_SET(gcfg[CFG_M], 0x00000001)
#define SET_INTX1_3      BIT_SET(gcfg[CFG_M], 0x00000002)
#define SET_INTX1_2      BIT_SET(gcfg[CFG_M], 0x00000004)
#define SET_INTX1_1      BIT_SET(gcfg[CFG_M], 0x00000008)
#define SET_INTX1_0      BIT_SET(gcfg[CFG_M], 0x00000010)

#define SET_ANAB_7       BIT_SET(gcfg[CFG_M], 0x00000020)
#define SET_ANAB_6       BIT_SET(gcfg[CFG_M], 0x00000040)
#define SET_ANAB_5       BIT_SET(gcfg[CFG_M], 0x00000080)
#define SET_ANAB_4       BIT_SET(gcfg[CFG_M], 0x00000100)
#define SET_ANAB_3       BIT_SET(gcfg[CFG_M], 0x00000200)
#define SET_ANAB_2       BIT_SET(gcfg[CFG_M], 0x00000400)
#define SET_ANAB_1       BIT_SET(gcfg[CFG_M], 0x00000800)
#define SET_ANAB_0       BIT_SET(gcfg[CFG_M], 0x00001000)



/////////// Test of Analog Circuit ////////////////
#define TEST_FSEL_1       BIT_TEST(gcfg[CFG_A], 0x00000001)
#define TEST_FSEL_0       BIT_TEST(gcfg[CFG_A], 0x00000002)
#define TEST_CAL_1        BIT_TEST(gcfg[CFG_A], 0x00000004)
#define TEST_SRC_1        BIT_TEST(gcfg[CFG_A], 0x00000008)
#define TEST_CLRN_1       BIT_TEST(gcfg[CFG_A], 0x00000010)
#define TEST_CK_PHS       BIT_TEST(gcfg[CFG_A], 0x00000020)
#define TEST_OSCD         BIT_TEST(gcfg[CFG_A], 0x00000040)

#define TEST_TUNEX4_9     BIT_TEST(gcfg[CFG_L], 0x00000200)
#define TEST_TUNEX4_8     BIT_TEST(gcfg[CFG_L], 0x00000400)
#define TEST_TUNEX4_7     BIT_TEST(gcfg[CFG_L], 0x00000800)
#define TEST_TUNEX4_6     BIT_TEST(gcfg[CFG_L], 0x00001000)
#define TEST_TUNEX4_5     BIT_TEST(gcfg[CFG_L], 0x00002000)
#define TEST_TUNEX4_4     BIT_TEST(gcfg[CFG_L], 0x00004000)
#define TEST_TUNEX4_3     BIT_TEST(gcfg[CFG_L], 0x00008000)
#define TEST_TUNEX4_2     BIT_TEST(gcfg[CFG_L], 0x00010000)
#define TEST_TUNEX4_1     BIT_TEST(gcfg[CFG_L], 0x00020000)
#define TEST_TUNEX4_0     BIT_TEST(gcfg[CFG_L], 0x00040000)

#define TEST_TUNEX3_9     BIT_TEST(gcfg[CFG_L], 0x00080000)
#define TEST_TUNEX3_8     BIT_TEST(gcfg[CFG_L], 0x00100000)
#define TEST_TUNEX3_7     BIT_TEST(gcfg[CFG_L], 0x00200000)
#define TEST_TUNEX3_6     BIT_TEST(gcfg[CFG_L], 0x00400000)
#define TEST_TUNEX3_5     BIT_TEST(gcfg[CFG_L], 0x00800000)
#define TEST_TUNEX3_4     BIT_TEST(gcfg[CFG_L], 0x01000000)
#define TEST_TUNEX3_3     BIT_TEST(gcfg[CFG_L], 0x02000000)
#define TEST_TUNEX3_2     BIT_TEST(gcfg[CFG_L], 0x04000000)
#define TEST_TUNEX3_1     BIT_TEST(gcfg[CFG_L], 0x08000000)
#define TEST_TUNEX3_0     BIT_TEST(gcfg[CFG_L], 0x10000000)

#define TEST_TUNEX2_9     BIT_TEST(gcfg[CFG_L], 0x20000000)
#define TEST_TUNEX2_8     BIT_TEST(gcfg[CFG_L], 0x40000000)
#define TEST_TUNEX2_7     BIT_TEST(gcfg[CFG_L], 0x80000000)
#define TEST_TUNEX2_6     BIT_TEST(gcfg[CFG_H], 0x00000001)
#define TEST_TUNEX2_5     BIT_TEST(gcfg[CFG_H], 0x00000002)
#define TEST_TUNEX2_4     BIT_TEST(gcfg[CFG_H], 0x00000004)
#define TEST_TUNEX2_3     BIT_TEST(gcfg[CFG_H], 0x00000008)
#define TEST_TUNEX2_2     BIT_TEST(gcfg[CFG_H], 0x00000010)
#define TEST_TUNEX2_1     BIT_TEST(gcfg[CFG_H], 0x00000020)
#define TEST_TUNEX2_0     BIT_TEST(gcfg[CFG_H], 0x00000040)

#define TEST_TUNEX1_9     BIT_TEST(gcfg[CFG_H], 0x00000080)
#define TEST_TUNEX1_8     BIT_TEST(gcfg[CFG_H], 0x00000100)
#define TEST_TUNEX1_7     BIT_TEST(gcfg[CFG_H], 0x00000200)
#define TEST_TUNEX1_6     BIT_TEST(gcfg[CFG_H], 0x00000400)
#define TEST_TUNEX1_5     BIT_TEST(gcfg[CFG_H], 0x00000800)
#define TEST_TUNEX1_4     BIT_TEST(gcfg[CFG_H], 0x00001000)
#define TEST_TUNEX1_3     BIT_TEST(gcfg[CFG_H], 0x00002000)
#define TEST_TUNEX1_2     BIT_TEST(gcfg[CFG_H], 0x00004000)
#define TEST_TUNEX1_1     BIT_TEST(gcfg[CFG_H], 0x00008000)
#define TEST_TUNEX1_0     BIT_TEST(gcfg[CFG_H], 0x00010000)

// #define TEST_ADC_0       BIT_TEST(gcfg[CFG_H], 0x00400000)
// #define TEST_ADC_1       BIT_TEST(gcfg[CFG_H], 0x00200000)

#define TEST_FNL          BIT_TEST(gcfg[CFG_M], 0x00800000)

#define TEST_CB4_0        BIT_TEST(gcfg[CFG_M], 0x01000000)
#define TEST_CB4_1        BIT_TEST(gcfg[CFG_M], 0x02000000)
#define TEST_CB4_2        BIT_TEST(gcfg[CFG_M], 0x04000000)
#define TEST_CB3_2        BIT_TEST(gcfg[CFG_M], 0x08000000)
#define TEST_CB3_1        BIT_TEST(gcfg[CFG_M], 0x10000000)
#define TEST_CB3_0        BIT_TEST(gcfg[CFG_M], 0x20000000)

#define TEST_CB_SW        BIT_TEST(gcfg[CFG_M], 0x40000000)
#define TEST_CB2_0        BIT_TEST(gcfg[CFG_M], 0x80000000)
#define TEST_CB2_1        BIT_TEST(gcfg[CFG_S], 0x00000001)
#define TEST_CB2_2        BIT_TEST(gcfg[CFG_S], 0x00000002)
#define TEST_CB1_2        BIT_TEST(gcfg[CFG_S], 0x00000004)
#define TEST_CB1_1        BIT_TEST(gcfg[CFG_S], 0x00000008)
#define TEST_CB1_0        BIT_TEST(gcfg[CFG_S], 0x00000010)

#define TEST_CCC_0        BIT_TEST(gcfg[CFG_S], 0x00000020)
#define TEST_CCC_1        BIT_TEST(gcfg[CFG_S], 0x00000040)
#define TEST_CCC_2        BIT_TEST(gcfg[CFG_S], 0x00000080)
#define TEST_CCC_3        BIT_TEST(gcfg[CFG_S], 0x00000100)
#define TEST_CCC_4        BIT_TEST(gcfg[CFG_S], 0x00000200)

#define TEST_M3_20        BIT_TEST(gcfg[CFG_S], 0x00000400)
#define TEST_M3_19        BIT_TEST(gcfg[CFG_S], 0x00000800)
#define TEST_M3_18        BIT_TEST(gcfg[CFG_S], 0x00001000)
#define TEST_M3_17        BIT_TEST(gcfg[CFG_S], 0x00002000)
#define TEST_M3_16        BIT_TEST(gcfg[CFG_S], 0x00004000)
#define TEST_M3_15        BIT_TEST(gcfg[CFG_S], 0x00008000)
#define TEST_M3_14        BIT_TEST(gcfg[CFG_S], 0x00010000)

#define TEST_M2_13        BIT_TEST(gcfg[CFG_S], 0x00020000)
#define TEST_M2_12        BIT_TEST(gcfg[CFG_S], 0x00040000)
#define TEST_M2_11        BIT_TEST(gcfg[CFG_S], 0x00080000)
#define TEST_M2_10        BIT_TEST(gcfg[CFG_S], 0x00100000)
#define TEST_M2_09        BIT_TEST(gcfg[CFG_S], 0x00200000)
#define TEST_M2_08        BIT_TEST(gcfg[CFG_S], 0x00400000)
#define TEST_M2_07        BIT_TEST(gcfg[CFG_S], 0x00800000)

#define TEST_M1_06        BIT_TEST(gcfg[CFG_S], 0x01000000)
#define TEST_M1_05        BIT_TEST(gcfg[CFG_S], 0x02000000)
#define TEST_M1_04        BIT_TEST(gcfg[CFG_S], 0x04000000)
#define TEST_M1_03        BIT_TEST(gcfg[CFG_S], 0x08000000)
#define TEST_M1_02        BIT_TEST(gcfg[CFG_S], 0x10000000)
#define TEST_M1_01        BIT_TEST(gcfg[CFG_S], 0x20000000)
#define TEST_M1_00        BIT_TEST(gcfg[CFG_S], 0x40000000)

// TEST of Optimization
#define TEST_DIMEN_4D     BIT_TEST(gcfg[CFG_L], 0x00000001)

#define TEST_ITRNUM_7     BIT_TEST(gcfg[CFG_L], 0x00000002)
#define TEST_ITRNUM_6     BIT_TEST(gcfg[CFG_L], 0x00000004)
#define TEST_ITRNUM_5     BIT_TEST(gcfg[CFG_L], 0x00000008)
#define TEST_ITRNUM_4     BIT_TEST(gcfg[CFG_L], 0x00000010)
#define TEST_ITRNUM_3     BIT_TEST(gcfg[CFG_L], 0x00000020)
#define TEST_ITRNUM_2     BIT_TEST(gcfg[CFG_L], 0x00000040)
#define TEST_ITRNUM_1     BIT_TEST(gcfg[CFG_L], 0x00000080)
#define TEST_ITRNUM_0     BIT_TEST(gcfg[CFG_L], 0x00000100)

#define TEST_INTX4_4      BIT_TEST(gcfg[CFG_H], 0x00020000)
#define TEST_INTX4_3      BIT_TEST(gcfg[CFG_H], 0x00040000)
#define TEST_INTX4_2      BIT_TEST(gcfg[CFG_H], 0x00080000)
#define TEST_INTX4_1      BIT_TEST(gcfg[CFG_H], 0x00100000)
#define TEST_INTX4_0      BIT_TEST(gcfg[CFG_H], 0x00200000)

#define TEST_INTX3_4      BIT_TEST(gcfg[CFG_H], 0x00400000)
#define TEST_INTX3_3      BIT_TEST(gcfg[CFG_H], 0x00800000)
#define TEST_INTX3_2      BIT_TEST(gcfg[CFG_H], 0x01000000)
#define TEST_INTX3_1      BIT_TEST(gcfg[CFG_H], 0x02000000)
#define TEST_INTX3_0      BIT_TEST(gcfg[CFG_H], 0x04000000)

#define TEST_INTX2_4      BIT_TEST(gcfg[CFG_H], 0x08000000)
#define TEST_INTX2_3      BIT_TEST(gcfg[CFG_H], 0x10000000)
#define TEST_INTX2_2      BIT_TEST(gcfg[CFG_H], 0x20000000)
#define TEST_INTX2_1      BIT_TEST(gcfg[CFG_H], 0x40000000)
#define TEST_INTX2_0      BIT_TEST(gcfg[CFG_H], 0x80000000)

#define TEST_INTX1_4      BIT_TEST(gcfg[CFG_M], 0x00000001)
#define TEST_INTX1_3      BIT_TEST(gcfg[CFG_M], 0x00000002)
#define TEST_INTX1_2      BIT_TEST(gcfg[CFG_M], 0x00000004)
#define TEST_INTX1_1      BIT_TEST(gcfg[CFG_M], 0x00000008)
#define TEST_INTX1_0      BIT_TEST(gcfg[CFG_M], 0x00000010)

#define TEST_ANAB_7       BIT_TEST(gcfg[CFG_M], 0x00000020)
#define TEST_ANAB_6       BIT_TEST(gcfg[CFG_M], 0x00000040)
#define TEST_ANAB_5       BIT_TEST(gcfg[CFG_M], 0x00000080)
#define TEST_ANAB_4       BIT_TEST(gcfg[CFG_M], 0x00000100)
#define TEST_ANAB_3       BIT_TEST(gcfg[CFG_M], 0x00000200)
#define TEST_ANAB_2       BIT_TEST(gcfg[CFG_M], 0x00000400)
#define TEST_ANAB_1       BIT_TEST(gcfg[CFG_M], 0x00000800)
#define TEST_ANAB_0       BIT_TEST(gcfg[CFG_M], 0x00001000)


/////////// Reset of Analog Circuit ////////////////
#define RESET_FSEL_1       BIT_RESET(gcfg[CFG_A], 0x00000001)
#define RESET_FSEL_0       BIT_RESET(gcfg[CFG_A], 0x00000002)
#define RESET_CAL_1        BIT_RESET(gcfg[CFG_A], 0x00000004)
#define RESET_SRC_1        BIT_RESET(gcfg[CFG_A], 0x00000008)
#define RESET_CLRN_1       BIT_RESET(gcfg[CFG_A], 0x00000010)
#define RESET_CK_PHS       BIT_RESET(gcfg[CFG_A], 0x00000020)
#define RESET_OSCD         BIT_RESET(gcfg[CFG_A], 0x00000040)

#define RESET_TUNEX4_9     BIT_RESET(gcfg[CFG_L], 0x00000200)
#define RESET_TUNEX4_8     BIT_RESET(gcfg[CFG_L], 0x00000400)
#define RESET_TUNEX4_7     BIT_RESET(gcfg[CFG_L], 0x00000800)
#define RESET_TUNEX4_6     BIT_RESET(gcfg[CFG_L], 0x00001000)
#define RESET_TUNEX4_5     BIT_RESET(gcfg[CFG_L], 0x00002000)
#define RESET_TUNEX4_4     BIT_RESET(gcfg[CFG_L], 0x00004000)
#define RESET_TUNEX4_3     BIT_RESET(gcfg[CFG_L], 0x00008000)
#define RESET_TUNEX4_2     BIT_RESET(gcfg[CFG_L], 0x00010000)
#define RESET_TUNEX4_1     BIT_RESET(gcfg[CFG_L], 0x00020000)
#define RESET_TUNEX4_0     BIT_RESET(gcfg[CFG_L], 0x00040000)

#define RESET_TUNEX3_9     BIT_RESET(gcfg[CFG_L], 0x00080000)
#define RESET_TUNEX3_8     BIT_RESET(gcfg[CFG_L], 0x00100000)
#define RESET_TUNEX3_7     BIT_RESET(gcfg[CFG_L], 0x00200000)
#define RESET_TUNEX3_6     BIT_RESET(gcfg[CFG_L], 0x00400000)
#define RESET_TUNEX3_5     BIT_RESET(gcfg[CFG_L], 0x00800000)
#define RESET_TUNEX3_4     BIT_RESET(gcfg[CFG_L], 0x01000000)
#define RESET_TUNEX3_3     BIT_RESET(gcfg[CFG_L], 0x02000000)
#define RESET_TUNEX3_2     BIT_RESET(gcfg[CFG_L], 0x04000000)
#define RESET_TUNEX3_1     BIT_RESET(gcfg[CFG_L], 0x08000000)
#define RESET_TUNEX3_0     BIT_RESET(gcfg[CFG_L], 0x10000000)

#define RESET_TUNEX2_9     BIT_RESET(gcfg[CFG_L], 0x20000000)
#define RESET_TUNEX2_8     BIT_RESET(gcfg[CFG_L], 0x40000000)
#define RESET_TUNEX2_7     BIT_RESET(gcfg[CFG_L], 0x80000000)
#define RESET_TUNEX2_6     BIT_RESET(gcfg[CFG_H], 0x00000001)
#define RESET_TUNEX2_5     BIT_RESET(gcfg[CFG_H], 0x00000002)
#define RESET_TUNEX2_4     BIT_RESET(gcfg[CFG_H], 0x00000004)
#define RESET_TUNEX2_3     BIT_RESET(gcfg[CFG_H], 0x00000008)
#define RESET_TUNEX2_2     BIT_RESET(gcfg[CFG_H], 0x00000010)
#define RESET_TUNEX2_1     BIT_RESET(gcfg[CFG_H], 0x00000020)
#define RESET_TUNEX2_0     BIT_RESET(gcfg[CFG_H], 0x00000040)

#define RESET_TUNEX1_9     BIT_RESET(gcfg[CFG_H], 0x00000080)
#define RESET_TUNEX1_8     BIT_RESET(gcfg[CFG_H], 0x00000100)
#define RESET_TUNEX1_7     BIT_RESET(gcfg[CFG_H], 0x00000200)
#define RESET_TUNEX1_6     BIT_RESET(gcfg[CFG_H], 0x00000400)
#define RESET_TUNEX1_5     BIT_RESET(gcfg[CFG_H], 0x00000800)
#define RESET_TUNEX1_4     BIT_RESET(gcfg[CFG_H], 0x00001000)
#define RESET_TUNEX1_3     BIT_RESET(gcfg[CFG_H], 0x00002000)
#define RESET_TUNEX1_2     BIT_RESET(gcfg[CFG_H], 0x00004000)
#define RESET_TUNEX1_1     BIT_RESET(gcfg[CFG_H], 0x00008000)
#define RESET_TUNEX1_0     BIT_RESET(gcfg[CFG_H], 0x00010000)

// #define RESET_ADC_0       BIT_RESET(gcfg[CFG_H], 0x00400000)
// #define RESET_ADC_1       BIT_RESET(gcfg[CFG_H], 0x00200000)

#define RESET_FNL          BIT_RESET(gcfg[CFG_M], 0x00800000)

#define RESET_CB4_0        BIT_RESET(gcfg[CFG_M], 0x01000000)
#define RESET_CB4_1        BIT_RESET(gcfg[CFG_M], 0x02000000)
#define RESET_CB4_2        BIT_RESET(gcfg[CFG_M], 0x04000000)
#define RESET_CB3_2        BIT_RESET(gcfg[CFG_M], 0x08000000)
#define RESET_CB3_1        BIT_RESET(gcfg[CFG_M], 0x10000000)
#define RESET_CB3_0        BIT_RESET(gcfg[CFG_M], 0x20000000)

#define RESET_CB_SW        BIT_RESET(gcfg[CFG_M], 0x40000000)
#define RESET_CB2_0        BIT_RESET(gcfg[CFG_M], 0x80000000)
#define RESET_CB2_1        BIT_RESET(gcfg[CFG_S], 0x00000001)
#define RESET_CB2_2        BIT_RESET(gcfg[CFG_S], 0x00000002)
#define RESET_CB1_2        BIT_RESET(gcfg[CFG_S], 0x00000004)
#define RESET_CB1_1        BIT_RESET(gcfg[CFG_S], 0x00000008)
#define RESET_CB1_0        BIT_RESET(gcfg[CFG_S], 0x00000010)

#define RESET_CCC_0        BIT_RESET(gcfg[CFG_S], 0x00000020)
#define RESET_CCC_1        BIT_RESET(gcfg[CFG_S], 0x00000040)
#define RESET_CCC_2        BIT_RESET(gcfg[CFG_S], 0x00000080)
#define RESET_CCC_3        BIT_RESET(gcfg[CFG_S], 0x00000100)
#define RESET_CCC_4        BIT_RESET(gcfg[CFG_S], 0x00000200)

#define RESET_M3_20        BIT_RESET(gcfg[CFG_S], 0x00000400)
#define RESET_M3_19        BIT_RESET(gcfg[CFG_S], 0x00000800)
#define RESET_M3_18        BIT_RESET(gcfg[CFG_S], 0x00001000)
#define RESET_M3_17        BIT_RESET(gcfg[CFG_S], 0x00002000)
#define RESET_M3_16        BIT_RESET(gcfg[CFG_S], 0x00004000)
#define RESET_M3_15        BIT_RESET(gcfg[CFG_S], 0x00008000)
#define RESET_M3_14        BIT_RESET(gcfg[CFG_S], 0x00010000)

#define RESET_M2_13        BIT_RESET(gcfg[CFG_S], 0x00020000)
#define RESET_M2_12        BIT_RESET(gcfg[CFG_S], 0x00040000)
#define RESET_M2_11        BIT_RESET(gcfg[CFG_S], 0x00080000)
#define RESET_M2_10        BIT_RESET(gcfg[CFG_S], 0x00100000)
#define RESET_M2_09        BIT_RESET(gcfg[CFG_S], 0x00200000)
#define RESET_M2_08        BIT_RESET(gcfg[CFG_S], 0x00400000)
#define RESET_M2_07        BIT_RESET(gcfg[CFG_S], 0x00800000)

#define RESET_M1_06        BIT_RESET(gcfg[CFG_S], 0x01000000)
#define RESET_M1_05        BIT_RESET(gcfg[CFG_S], 0x02000000)
#define RESET_M1_04        BIT_RESET(gcfg[CFG_S], 0x04000000)
#define RESET_M1_03        BIT_RESET(gcfg[CFG_S], 0x08000000)
#define RESET_M1_02        BIT_RESET(gcfg[CFG_S], 0x10000000)
#define RESET_M1_01        BIT_RESET(gcfg[CFG_S], 0x20000000)
#define RESET_M1_00        BIT_RESET(gcfg[CFG_S], 0x40000000)

// RESET of Optimization
#define RESET_DIMEN_4D     BIT_RESET(gcfg[CFG_L], 0x00000001)

#define RESET_ITRNUM_7     BIT_RESET(gcfg[CFG_L], 0x00000002)
#define RESET_ITRNUM_6     BIT_RESET(gcfg[CFG_L], 0x00000004)
#define RESET_ITRNUM_5     BIT_RESET(gcfg[CFG_L], 0x00000008)
#define RESET_ITRNUM_4     BIT_RESET(gcfg[CFG_L], 0x00000010)
#define RESET_ITRNUM_3     BIT_RESET(gcfg[CFG_L], 0x00000020)
#define RESET_ITRNUM_2     BIT_RESET(gcfg[CFG_L], 0x00000040)
#define RESET_ITRNUM_1     BIT_RESET(gcfg[CFG_L], 0x00000080)
#define RESET_ITRNUM_0     BIT_RESET(gcfg[CFG_L], 0x00000100)

#define RESET_INTX4_4      BIT_RESET(gcfg[CFG_H], 0x00020000)
#define RESET_INTX4_3      BIT_RESET(gcfg[CFG_H], 0x00040000)
#define RESET_INTX4_2      BIT_RESET(gcfg[CFG_H], 0x00080000)
#define RESET_INTX4_1      BIT_RESET(gcfg[CFG_H], 0x00100000)
#define RESET_INTX4_0      BIT_RESET(gcfg[CFG_H], 0x00200000)

#define RESET_INTX3_4      BIT_RESET(gcfg[CFG_H], 0x00400000)
#define RESET_INTX3_3      BIT_RESET(gcfg[CFG_H], 0x00800000)
#define RESET_INTX3_2      BIT_RESET(gcfg[CFG_H], 0x01000000)
#define RESET_INTX3_1      BIT_RESET(gcfg[CFG_H], 0x02000000)
#define RESET_INTX3_0      BIT_RESET(gcfg[CFG_H], 0x04000000)

#define RESET_INTX2_4      BIT_RESET(gcfg[CFG_H], 0x08000000)
#define RESET_INTX2_3      BIT_RESET(gcfg[CFG_H], 0x10000000)
#define RESET_INTX2_2      BIT_RESET(gcfg[CFG_H], 0x20000000)
#define RESET_INTX2_1      BIT_RESET(gcfg[CFG_H], 0x40000000)
#define RESET_INTX2_0      BIT_RESET(gcfg[CFG_H], 0x80000000)

#define RESET_INTX1_4      BIT_RESET(gcfg[CFG_M], 0x00000001)
#define RESET_INTX1_3      BIT_RESET(gcfg[CFG_M], 0x00000002)
#define RESET_INTX1_2      BIT_RESET(gcfg[CFG_M], 0x00000004)
#define RESET_INTX1_1      BIT_RESET(gcfg[CFG_M], 0x00000008)
#define RESET_INTX1_0      BIT_RESET(gcfg[CFG_M], 0x00000010)

#define RESET_ANAB_7       BIT_RESET(gcfg[CFG_M], 0x00000020)
#define RESET_ANAB_6       BIT_RESET(gcfg[CFG_M], 0x00000040)
#define RESET_ANAB_5       BIT_RESET(gcfg[CFG_M], 0x00000080)
#define RESET_ANAB_4       BIT_RESET(gcfg[CFG_M], 0x00000100)
#define RESET_ANAB_3       BIT_RESET(gcfg[CFG_M], 0x00000200)
#define RESET_ANAB_2       BIT_RESET(gcfg[CFG_M], 0x00000400)
#define RESET_ANAB_1       BIT_RESET(gcfg[CFG_M], 0x00000800)
#define RESET_ANAB_0       BIT_RESET(gcfg[CFG_M], 0x00001000)


#endif
