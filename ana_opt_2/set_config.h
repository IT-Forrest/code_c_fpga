#ifndef _SET_CONFIG_H_
#define _SET_CONFIG_H_

///////////////////////////////////////////////////////////////
// this .h file is only useful for the 1st. tapeout chip.

#include "socal/hps.h"
#include "socal/alt_gpio.h"


#define     MAX_CFG_BITS    8   // in terms of uint32_t
#define     CFG_A           MAX_CFG_BITS-1  // Appendix bits
#define     CFG_S           3   // most significant bits for scan chain
#define     CFG_M           2
#define     CFG_H           1
#define     CFG_L           0   // least significant bits for scan chain

//#define     MAX_SC_BITS     127//total scan chain length for version 2
#define     MAX_SC_BITS_A   243//total scan chain length for version 3
#define     MAX_SC_BITS_B   14//the length of Scan chain B for version 3
#define     MAX_FSEL_LEN    (127-32+1)

extern uint32_t    gcfg[MAX_CFG_BITS];       // configuration bits array
extern uint32_t    gcfg_bk[MAX_CFG_BITS];    // configuration bits backup
extern uint32_t    gcfg_tx[MAX_CFG_BITS];    // configuration bits delivered to FPGA
//extern uint32_t    gcfg_rd[MAX_CFG_BITS];    // array used to read scan chain A
extern uint32_t    gscB;    // Scan chain B; just for read
extern uint32_t    gscB_bk;

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


/****************************************************/
/// function and macro for chip3
/* Check Control Signal */
//uint8   RtnFSEL();
//uint8   RtnCK_PHS();
//uint8   RtnCK_SRC();
//uint8   RtnCK_CAL();
//uint8   RtnCK_OSCD();
//uint8   RtnSgen_CAP2();

/// manually control bits for the transparent Input
//int32   Chip3_SetTune_X1(uint16 rd_val);
//int32   Chip3_SetTune_X2(uint16 rd_val);
//int32   Chip3_SetTune_X3(uint16 rd_val);
//int32   Chip3_SetTune_X4(uint16 rd_val);
//int32   Chip3_SetAnabits(uint16 rd_val);

/* Set Control Variables */
int32    Chip3_Set_Fs(uint8 rd_val);
int32    Chip3_Set_Ana_Bits(uint8 rd_val);
int32    Chip3_Set_Dbfactor1(uint16 rd_val);
int32    Chip3_Set_Dbfactor0(uint16 rd_val);
int32    Chip3_Set_Threshold(uint8 rd_val);
int32    Chip3_Set_Factors(uint8 rd_val);
int32    Chip3_Set_Avrg_Num(uint8 rd_val);
int32    Chip3_Set_Rand_Group(uint8 rd_val);
int32    Chip3_Set_Div_Accu(uint8 rd_val);
int32    Chip3_Set_Flag4D(uint8 rd_val);
int32    Chip3_Set_Opt_Mode(uint8 rd_val);
int32    Chip3_Set_Step_T(uint8 rd_val);
int32    Chip3_Set_Init_T(uint8 rd_val);
int32    Chip3_Set_Ss_Iter(uint16 rd_val);
int32    Chip3_Set_Sa_Iter(uint16 rd_val);
//int32   Chip3_SetINT_X1(uint16 rd_val);
//int32   Chip3_SetINT_X2(uint16 rd_val);
//int32   Chip3_SetINT_X3(uint16 rd_val);
//int32   Chip3_SetINT_X4(uint16 rd_val);
int32    Chip3_Set_Init_Y1(uint8 rd_val);
int32    Chip3_Set_Init_X1(uint8 rd_val);
int32    Chip3_Set_Init_Y2(uint8 rd_val);
int32    Chip3_Set_Init_X2(uint8 rd_val);
int32    Chip3_Set_Nxt_Test(uint8 rd_val);
int32    Chip3_Set_Trdy_Test(uint8 rd_val);
int32    Chip3_Set_Aoff(uint16 rd_val);
int32    Chip3_Set_Tol(uint8 rd_val);
int32    Chip3_Set_Trg_Test(uint8 rd_val);
int32    Chip3_Set_Test_Mux(uint8 rd_val);
int32    Chip3_Set_Cal(uint8 rd_val);
int32    Chip3_Set_Src(uint8 rd_val);
int32    Chip3_Set_Oscd(uint8 rd_val);
int32    Chip3_Set_Sw(uint8 rd_val);
int32    Chip3_Set_Tx2(uint16 rd_val);
int32    Chip3_Set_Ty2(uint16 rd_val);
int32    Chip3_Set_Cb4(uint8 rd_val);
int32    Chip3_Set_Cb3(uint8 rd_val);
int32    Chip3_Set_Tx1(uint16 rd_val);
int32    Chip3_Set_Ty1(uint16 rd_val);
int32    Chip3_Set_Cb2(uint8 rd_val);
int32    Chip3_Set_Cb1(uint8 rd_val);
int32    Chip3_Set_Phs(uint8 rd_val);
int32    Chip3_Set_Cap3(uint8 rd_val);
int32    Chip3_Set_Bs3(uint8 rd_val);
int32    Chip3_Set_Mdiv3(uint8 rd_val);
int32    Chip3_Set_Cap2(uint8 rd_val);
int32    Chip3_Set_Bs2(uint8 rd_val);
int32    Chip3_Set_Mdiv2(uint8 rd_val);
int32    Chip3_Set_Cap1(uint8 rd_val);
int32    Chip3_Set_Bs1(uint8 rd_val);
int32    Chip3_Set_Mdiv1(uint8 rd_val);
int32    Chip3_Set_Cap0(uint8 rd_val);
int32    Chip3_Set_Bs0(uint8 rd_val);
int32    Chip3_Set_Mdiv0(uint8 rd_val);
/// Scan chain B (ADC)
int32    Chip3_Set_Adc(uint16 rd_val);
int32    Chip3_Set_Adc_Fnl(uint8 rd_val);
int32    Chip3_Set_Rstn_Adc(uint8 rd_val);
int32    Chip3_Set_Clk_Adc(uint8 rd_val);
int32    Chip3_Set_Clrn(uint8 rd_val);
uint32   Chip3_Rtn_Adc();
/*  MSB                                                               */
/*  ________|_______|_______|_______|_______|_______|_______|________ */
/*  |       |       |       |       |       |       |       |       | */
/*  | cfg_7 | cfg_6 | cfg_5 | cfg_4 | cfg_3 | cfg_2 | cfg_1 | cfg_0 | */
/*  |_______|_______|_______|_______|_______|_______|_______|_______| */
/*          |       |       |       |       |       |       |         */
/*      7       6       5       4       3.      2.      1.      0.    */
/*                                64+179 */
/****************************************************/
//#define BIT_SET(F, B)           ((F) |= (B))
#define BIT_RST(F, B)           ((F) &= ~(B))   ///the same as BIT_RESET
#define BIT_CHK(F, B)           ((F) & (B))     ///the same as BIT_TEST

#define		CHIP3_SET_FS_1		BIT_SET	(gcfg[0], 	0x00000001)
#define		CHIP3_SET_FS_0		BIT_SET	(gcfg[0], 	0x00000002)

#define		CHIP3_SET_ANA_BITS_7		BIT_SET	(gcfg[0], 	0x00000004)
#define		CHIP3_SET_ANA_BITS_6		BIT_SET	(gcfg[0], 	0x00000008)
#define		CHIP3_SET_ANA_BITS_5		BIT_SET	(gcfg[0], 	0x00000010)
#define		CHIP3_SET_ANA_BITS_4		BIT_SET	(gcfg[0], 	0x00000020)
#define		CHIP3_SET_ANA_BITS_3		BIT_SET	(gcfg[0], 	0x00000040)
#define		CHIP3_SET_ANA_BITS_2		BIT_SET	(gcfg[0], 	0x00000080)
#define		CHIP3_SET_ANA_BITS_1		BIT_SET	(gcfg[0], 	0x00000100)
#define		CHIP3_SET_ANA_BITS_0		BIT_SET	(gcfg[0], 	0x00000200)

#define		CHIP3_SET_DBFACTOR1_9		BIT_SET	(gcfg[0], 	0x00000400)
#define		CHIP3_SET_DBFACTOR1_8		BIT_SET	(gcfg[0], 	0x00000800)
#define		CHIP3_SET_DBFACTOR1_7		BIT_SET	(gcfg[0], 	0x00001000)
#define		CHIP3_SET_DBFACTOR1_6		BIT_SET	(gcfg[0], 	0x00002000)
#define		CHIP3_SET_DBFACTOR1_5		BIT_SET	(gcfg[0], 	0x00004000)
#define		CHIP3_SET_DBFACTOR1_4		BIT_SET	(gcfg[0], 	0x00008000)
#define		CHIP3_SET_DBFACTOR1_3		BIT_SET	(gcfg[0], 	0x00010000)
#define		CHIP3_SET_DBFACTOR1_2		BIT_SET	(gcfg[0], 	0x00020000)
#define		CHIP3_SET_DBFACTOR1_1		BIT_SET	(gcfg[0], 	0x00040000)
#define		CHIP3_SET_DBFACTOR1_0		BIT_SET	(gcfg[0], 	0x00080000)

#define		CHIP3_SET_DBFACTOR0_9		BIT_SET	(gcfg[0], 	0x00100000)
#define		CHIP3_SET_DBFACTOR0_8		BIT_SET	(gcfg[0], 	0x00200000)
#define		CHIP3_SET_DBFACTOR0_7		BIT_SET	(gcfg[0], 	0x00400000)
#define		CHIP3_SET_DBFACTOR0_6		BIT_SET	(gcfg[0], 	0x00800000)
#define		CHIP3_SET_DBFACTOR0_5		BIT_SET	(gcfg[0], 	0x01000000)
#define		CHIP3_SET_DBFACTOR0_4		BIT_SET	(gcfg[0], 	0x02000000)
#define		CHIP3_SET_DBFACTOR0_3		BIT_SET	(gcfg[0], 	0x04000000)
#define		CHIP3_SET_DBFACTOR0_2		BIT_SET	(gcfg[0], 	0x08000000)
#define		CHIP3_SET_DBFACTOR0_1		BIT_SET	(gcfg[0], 	0x10000000)
#define		CHIP3_SET_DBFACTOR0_0		BIT_SET	(gcfg[0], 	0x20000000)

#define		CHIP3_SET_THRESHOLD_7		BIT_SET	(gcfg[0], 	0x40000000)
#define		CHIP3_SET_THRESHOLD_6		BIT_SET	(gcfg[0], 	0x80000000)
#define		CHIP3_SET_THRESHOLD_5		BIT_SET	(gcfg[1], 	0x00000001)
#define		CHIP3_SET_THRESHOLD_4		BIT_SET	(gcfg[1], 	0x00000002)
#define		CHIP3_SET_THRESHOLD_3		BIT_SET	(gcfg[1], 	0x00000004)
#define		CHIP3_SET_THRESHOLD_2		BIT_SET	(gcfg[1], 	0x00000008)
#define		CHIP3_SET_THRESHOLD_1		BIT_SET	(gcfg[1], 	0x00000010)
#define		CHIP3_SET_THRESHOLD_0		BIT_SET	(gcfg[1], 	0x00000020)

#define		CHIP3_SET_FACTORS_7		BIT_SET	(gcfg[1], 	0x00000040)
#define		CHIP3_SET_FACTORS_6		BIT_SET	(gcfg[1], 	0x00000080)
#define		CHIP3_SET_FACTORS_5		BIT_SET	(gcfg[1], 	0x00000100)
#define		CHIP3_SET_FACTORS_4		BIT_SET	(gcfg[1], 	0x00000200)
#define		CHIP3_SET_FACTORS_3		BIT_SET	(gcfg[1], 	0x00000400)
#define		CHIP3_SET_FACTORS_2		BIT_SET	(gcfg[1], 	0x00000800)
#define		CHIP3_SET_FACTORS_1		BIT_SET	(gcfg[1], 	0x00001000)
#define		CHIP3_SET_FACTORS_0		BIT_SET	(gcfg[1], 	0x00002000)

#define		CHIP3_SET_AVRG_NUM			BIT_SET	(gcfg[1], 	0x00004000)
#define		CHIP3_SET_RAND_GROUP			BIT_SET	(gcfg[1], 	0x00008000)
#define		CHIP3_SET_DIV_ACCU			BIT_SET	(gcfg[1], 	0x00010000)
#define		CHIP3_SET_FLAG4D			BIT_SET	(gcfg[1], 	0x00020000)
#define		CHIP3_SET_OPT_MODE			BIT_SET	(gcfg[1], 	0x00040000)

#define		CHIP3_SET_STEP_T_2		BIT_SET	(gcfg[1], 	0x00080000)
#define		CHIP3_SET_STEP_T_1		BIT_SET	(gcfg[1], 	0x00100000)
#define		CHIP3_SET_STEP_T_0		BIT_SET	(gcfg[1], 	0x00200000)

#define		CHIP3_SET_INIT_T_5		BIT_SET	(gcfg[1], 	0x00400000)
#define		CHIP3_SET_INIT_T_4		BIT_SET	(gcfg[1], 	0x00800000)
#define		CHIP3_SET_INIT_T_3		BIT_SET	(gcfg[1], 	0x01000000)
#define		CHIP3_SET_INIT_T_2		BIT_SET	(gcfg[1], 	0x02000000)
#define		CHIP3_SET_INIT_T_1		BIT_SET	(gcfg[1], 	0x04000000)
#define		CHIP3_SET_INIT_T_0		BIT_SET	(gcfg[1], 	0x08000000)

#define		CHIP3_SET_SS_ITER_10		BIT_SET	(gcfg[1], 	0x10000000)
#define		CHIP3_SET_SS_ITER_9		BIT_SET	(gcfg[1], 	0x20000000)
#define		CHIP3_SET_SS_ITER_8		BIT_SET	(gcfg[1], 	0x40000000)
#define		CHIP3_SET_SS_ITER_7		BIT_SET	(gcfg[1], 	0x80000000)
#define		CHIP3_SET_SS_ITER_6		BIT_SET	(gcfg[2], 	0x00000001)
#define		CHIP3_SET_SS_ITER_5		BIT_SET	(gcfg[2], 	0x00000002)
#define		CHIP3_SET_SS_ITER_4		BIT_SET	(gcfg[2], 	0x00000004)
#define		CHIP3_SET_SS_ITER_3		BIT_SET	(gcfg[2], 	0x00000008)
#define		CHIP3_SET_SS_ITER_2		BIT_SET	(gcfg[2], 	0x00000010)
#define		CHIP3_SET_SS_ITER_1		BIT_SET	(gcfg[2], 	0x00000020)
#define		CHIP3_SET_SS_ITER_0		BIT_SET	(gcfg[2], 	0x00000040)

#define		CHIP3_SET_SA_ITER_10		BIT_SET	(gcfg[2], 	0x00000080)
#define		CHIP3_SET_SA_ITER_9		BIT_SET	(gcfg[2], 	0x00000100)
#define		CHIP3_SET_SA_ITER_8		BIT_SET	(gcfg[2], 	0x00000200)
#define		CHIP3_SET_SA_ITER_7		BIT_SET	(gcfg[2], 	0x00000400)
#define		CHIP3_SET_SA_ITER_6		BIT_SET	(gcfg[2], 	0x00000800)
#define		CHIP3_SET_SA_ITER_5		BIT_SET	(gcfg[2], 	0x00001000)
#define		CHIP3_SET_SA_ITER_4		BIT_SET	(gcfg[2], 	0x00002000)
#define		CHIP3_SET_SA_ITER_3		BIT_SET	(gcfg[2], 	0x00004000)
#define		CHIP3_SET_SA_ITER_2		BIT_SET	(gcfg[2], 	0x00008000)
#define		CHIP3_SET_SA_ITER_1		BIT_SET	(gcfg[2], 	0x00010000)
#define		CHIP3_SET_SA_ITER_0		BIT_SET	(gcfg[2], 	0x00020000)

#define		CHIP3_SET_INIT_Y1_4		BIT_SET	(gcfg[2], 	0x00040000)
#define		CHIP3_SET_INIT_Y1_3		BIT_SET	(gcfg[2], 	0x00080000)
#define		CHIP3_SET_INIT_Y1_2		BIT_SET	(gcfg[2], 	0x00100000)
#define		CHIP3_SET_INIT_Y1_1		BIT_SET	(gcfg[2], 	0x00200000)
#define		CHIP3_SET_INIT_Y1_0		BIT_SET	(gcfg[2], 	0x00400000)
#define		CHIP3_SET_INIT_X1_4		BIT_SET	(gcfg[2], 	0x00800000)
#define		CHIP3_SET_INIT_X1_3		BIT_SET	(gcfg[2], 	0x01000000)
#define		CHIP3_SET_INIT_X1_2		BIT_SET	(gcfg[2], 	0x02000000)
#define		CHIP3_SET_INIT_X1_1		BIT_SET	(gcfg[2], 	0x04000000)
#define		CHIP3_SET_INIT_X1_0		BIT_SET	(gcfg[2], 	0x08000000)
#define		CHIP3_SET_INIT_Y2_4		BIT_SET	(gcfg[2], 	0x10000000)
#define		CHIP3_SET_INIT_Y2_3		BIT_SET	(gcfg[2], 	0x20000000)
#define		CHIP3_SET_INIT_Y2_2		BIT_SET	(gcfg[2], 	0x40000000)
#define		CHIP3_SET_INIT_Y2_1		BIT_SET	(gcfg[2], 	0x80000000)
#define		CHIP3_SET_INIT_Y2_0		BIT_SET	(gcfg[3], 	0x00000001)
#define		CHIP3_SET_INIT_X2_4		BIT_SET	(gcfg[3], 	0x00000002)
#define		CHIP3_SET_INIT_X2_3		BIT_SET	(gcfg[3], 	0x00000004)
#define		CHIP3_SET_INIT_X2_2		BIT_SET	(gcfg[3], 	0x00000008)
#define		CHIP3_SET_INIT_X2_1		BIT_SET	(gcfg[3], 	0x00000010)
#define		CHIP3_SET_INIT_X2_0		BIT_SET	(gcfg[3], 	0x00000020)

#define		CHIP3_SET_NXT_TEST			BIT_SET	(gcfg[3], 	0x00000040)
#define		CHIP3_SET_TRDY_TEST			BIT_SET	(gcfg[3], 	0x00000080)

#define		CHIP3_SET_AOFF_9		BIT_SET	(gcfg[3], 	0x00000100)
#define		CHIP3_SET_AOFF_8		BIT_SET	(gcfg[3], 	0x00000200)
#define		CHIP3_SET_AOFF_7		BIT_SET	(gcfg[3], 	0x00000400)
#define		CHIP3_SET_AOFF_6		BIT_SET	(gcfg[3], 	0x00000800)
#define		CHIP3_SET_AOFF_5		BIT_SET	(gcfg[3], 	0x00001000)
#define		CHIP3_SET_AOFF_4		BIT_SET	(gcfg[3], 	0x00002000)
#define		CHIP3_SET_AOFF_3		BIT_SET	(gcfg[3], 	0x00004000)
#define		CHIP3_SET_AOFF_2		BIT_SET	(gcfg[3], 	0x00008000)
#define		CHIP3_SET_AOFF_1		BIT_SET	(gcfg[3], 	0x00010000)
#define		CHIP3_SET_AOFF_0		BIT_SET	(gcfg[3], 	0x00020000)

#define		CHIP3_SET_TOL_5		BIT_SET	(gcfg[3], 	0x00040000)
#define		CHIP3_SET_TOL_4		BIT_SET	(gcfg[3], 	0x00080000)
#define		CHIP3_SET_TOL_3		BIT_SET	(gcfg[3], 	0x00100000)
#define		CHIP3_SET_TOL_2		BIT_SET	(gcfg[3], 	0x00200000)
#define		CHIP3_SET_TOL_1		BIT_SET	(gcfg[3], 	0x00400000)
#define		CHIP3_SET_TOL_0		BIT_SET	(gcfg[3], 	0x00800000)

#define		CHIP3_SET_TRG_TEST			BIT_SET	(gcfg[3], 	0x01000000)
#define		CHIP3_SET_TEST_MUX			BIT_SET	(gcfg[3], 	0x02000000)
#define		CHIP3_SET_CAL			BIT_SET	(gcfg[3], 	0x04000000)

#define		CHIP3_SET_SRC			BIT_SET	(gcfg[3], 	0x08000000)
#define		CHIP3_SET_OSCD			BIT_SET	(gcfg[3], 	0x10000000)

#define		CHIP3_SET_SW			BIT_SET	(gcfg[3], 	0x20000000)

#define		CHIP3_SET_TX2_9		BIT_SET	(gcfg[3], 	0x40000000)
#define		CHIP3_SET_TX2_8		BIT_SET	(gcfg[3], 	0x80000000)
#define		CHIP3_SET_TX2_7		BIT_SET	(gcfg[4], 	0x00000001)
#define		CHIP3_SET_TX2_6		BIT_SET	(gcfg[4], 	0x00000002)
#define		CHIP3_SET_TX2_5		BIT_SET	(gcfg[4], 	0x00000004)
#define		CHIP3_SET_TX2_4		BIT_SET	(gcfg[4], 	0x00000008)
#define		CHIP3_SET_TX2_3		BIT_SET	(gcfg[4], 	0x00000010)
#define		CHIP3_SET_TX2_2		BIT_SET	(gcfg[4], 	0x00000020)
#define		CHIP3_SET_TX2_1		BIT_SET	(gcfg[4], 	0x00000040)
#define		CHIP3_SET_TX2_0		BIT_SET	(gcfg[4], 	0x00000080)

#define		CHIP3_SET_TY2_9		BIT_SET	(gcfg[4], 	0x00000100)
#define		CHIP3_SET_TY2_8		BIT_SET	(gcfg[4], 	0x00000200)
#define		CHIP3_SET_TY2_7		BIT_SET	(gcfg[4], 	0x00000400)
#define		CHIP3_SET_TY2_6		BIT_SET	(gcfg[4], 	0x00000800)
#define		CHIP3_SET_TY2_5		BIT_SET	(gcfg[4], 	0x00001000)
#define		CHIP3_SET_TY2_4		BIT_SET	(gcfg[4], 	0x00002000)
#define		CHIP3_SET_TY2_3		BIT_SET	(gcfg[4], 	0x00004000)
#define		CHIP3_SET_TY2_2		BIT_SET	(gcfg[4], 	0x00008000)
#define		CHIP3_SET_TY2_1		BIT_SET	(gcfg[4], 	0x00010000)
#define		CHIP3_SET_TY2_0		BIT_SET	(gcfg[4], 	0x00020000)

#define		CHIP3_SET_CB4_2		BIT_SET	(gcfg[4], 	0x00040000)
#define		CHIP3_SET_CB4_1		BIT_SET	(gcfg[4], 	0x00080000)
#define		CHIP3_SET_CB4_0		BIT_SET	(gcfg[4], 	0x00100000)
#define		CHIP3_SET_CB3_2		BIT_SET	(gcfg[4], 	0x00200000)
#define		CHIP3_SET_CB3_1		BIT_SET	(gcfg[4], 	0x00400000)
#define		CHIP3_SET_CB3_0		BIT_SET	(gcfg[4], 	0x00800000)

#define		CHIP3_SET_TX1_9		BIT_SET	(gcfg[4], 	0x01000000)
#define		CHIP3_SET_TX1_8		BIT_SET	(gcfg[4], 	0x02000000)
#define		CHIP3_SET_TX1_7		BIT_SET	(gcfg[4], 	0x04000000)
#define		CHIP3_SET_TX1_6		BIT_SET	(gcfg[4], 	0x08000000)
#define		CHIP3_SET_TX1_5		BIT_SET	(gcfg[4], 	0x10000000)
#define		CHIP3_SET_TX1_4		BIT_SET	(gcfg[4], 	0x20000000)
#define		CHIP3_SET_TX1_3		BIT_SET	(gcfg[4], 	0x40000000)
#define		CHIP3_SET_TX1_2		BIT_SET	(gcfg[4], 	0x80000000)
#define		CHIP3_SET_TX1_1		BIT_SET	(gcfg[5], 	0x00000001)
#define		CHIP3_SET_TX1_0		BIT_SET	(gcfg[5], 	0x00000002)

#define		CHIP3_SET_TY1_9		BIT_SET	(gcfg[5], 	0x00000004)
#define		CHIP3_SET_TY1_8		BIT_SET	(gcfg[5], 	0x00000008)
#define		CHIP3_SET_TY1_7		BIT_SET	(gcfg[5], 	0x00000010)
#define		CHIP3_SET_TY1_6		BIT_SET	(gcfg[5], 	0x00000020)
#define		CHIP3_SET_TY1_5		BIT_SET	(gcfg[5], 	0x00000040)
#define		CHIP3_SET_TY1_4		BIT_SET	(gcfg[5], 	0x00000080)
#define		CHIP3_SET_TY1_3		BIT_SET	(gcfg[5], 	0x00000100)
#define		CHIP3_SET_TY1_2		BIT_SET	(gcfg[5], 	0x00000200)
#define		CHIP3_SET_TY1_1		BIT_SET	(gcfg[5], 	0x00000400)
#define		CHIP3_SET_TY1_0		BIT_SET	(gcfg[5], 	0x00000800)

#define		CHIP3_SET_CB2_2		BIT_SET	(gcfg[5], 	0x00001000)
#define		CHIP3_SET_CB2_1		BIT_SET	(gcfg[5], 	0x00002000)
#define		CHIP3_SET_CB2_0		BIT_SET	(gcfg[5], 	0x00004000)
#define		CHIP3_SET_CB1_2		BIT_SET	(gcfg[5], 	0x00008000)
#define		CHIP3_SET_CB1_1		BIT_SET	(gcfg[5], 	0x00010000)
#define		CHIP3_SET_CB1_0		BIT_SET	(gcfg[5], 	0x00020000)

#define		CHIP3_SET_PHS			BIT_SET	(gcfg[5], 	0x00040000)

#define		CHIP3_SET_CAP3_4		BIT_SET	(gcfg[5], 	0x00080000)
#define		CHIP3_SET_CAP3_3		BIT_SET	(gcfg[5], 	0x00100000)
#define		CHIP3_SET_CAP3_2		BIT_SET	(gcfg[5], 	0x00200000)
#define		CHIP3_SET_CAP3_1		BIT_SET	(gcfg[5], 	0x00400000)
#define		CHIP3_SET_CAP3_0		BIT_SET	(gcfg[5], 	0x00800000)

#define		CHIP3_SET_BS3_3		BIT_SET	(gcfg[5], 	0x01000000)
#define		CHIP3_SET_BS3_2		BIT_SET	(gcfg[5], 	0x02000000)
#define		CHIP3_SET_BS3_1		BIT_SET	(gcfg[5], 	0x04000000)
#define		CHIP3_SET_BS3_0		BIT_SET	(gcfg[5], 	0x08000000)

#define		CHIP3_SET_MDIV3_6		BIT_SET	(gcfg[5], 	0x10000000)
#define		CHIP3_SET_MDIV3_5		BIT_SET	(gcfg[5], 	0x20000000)
#define		CHIP3_SET_MDIV3_4		BIT_SET	(gcfg[5], 	0x40000000)
#define		CHIP3_SET_MDIV3_3		BIT_SET	(gcfg[5], 	0x80000000)
#define		CHIP3_SET_MDIV3_2		BIT_SET	(gcfg[6], 	0x00000001)
#define		CHIP3_SET_MDIV3_1		BIT_SET	(gcfg[6], 	0x00000002)
#define		CHIP3_SET_MDIV3_0		BIT_SET	(gcfg[6], 	0x00000004)


#define		CHIP3_SET_CAP2_4		BIT_SET	(gcfg[6], 	0x00000008)
#define		CHIP3_SET_CAP2_3		BIT_SET	(gcfg[6], 	0x00000010)
#define		CHIP3_SET_CAP2_2		BIT_SET	(gcfg[6], 	0x00000020)
#define		CHIP3_SET_CAP2_1		BIT_SET	(gcfg[6], 	0x00000040)
#define		CHIP3_SET_CAP2_0		BIT_SET	(gcfg[6], 	0x00000080)

#define		CHIP3_SET_BS2_3		BIT_SET	(gcfg[6], 	0x00000100)
#define		CHIP3_SET_BS2_2		BIT_SET	(gcfg[6], 	0x00000200)
#define		CHIP3_SET_BS2_1		BIT_SET	(gcfg[6], 	0x00000400)
#define		CHIP3_SET_BS2_0		BIT_SET	(gcfg[6], 	0x00000800)

#define		CHIP3_SET_MDIV2_6		BIT_SET	(gcfg[6], 	0x00001000)
#define		CHIP3_SET_MDIV2_5		BIT_SET	(gcfg[6], 	0x00002000)
#define		CHIP3_SET_MDIV2_4		BIT_SET	(gcfg[6], 	0x00004000)
#define		CHIP3_SET_MDIV2_3		BIT_SET	(gcfg[6], 	0x00008000)
#define		CHIP3_SET_MDIV2_2		BIT_SET	(gcfg[6], 	0x00010000)
#define		CHIP3_SET_MDIV2_1		BIT_SET	(gcfg[6], 	0x00020000)
#define		CHIP3_SET_MDIV2_0		BIT_SET	(gcfg[6], 	0x00040000)


#define		CHIP3_SET_CAP1_4		BIT_SET	(gcfg[6], 	0x00080000)
#define		CHIP3_SET_CAP1_3		BIT_SET	(gcfg[6], 	0x00100000)
#define		CHIP3_SET_CAP1_2		BIT_SET	(gcfg[6], 	0x00200000)
#define		CHIP3_SET_CAP1_1		BIT_SET	(gcfg[6], 	0x00400000)
#define		CHIP3_SET_CAP1_0		BIT_SET	(gcfg[6], 	0x00800000)

#define		CHIP3_SET_BS1_3		BIT_SET	(gcfg[6], 	0x01000000)
#define		CHIP3_SET_BS1_2		BIT_SET	(gcfg[6], 	0x02000000)
#define		CHIP3_SET_BS1_1		BIT_SET	(gcfg[6], 	0x04000000)
#define		CHIP3_SET_BS1_0		BIT_SET	(gcfg[6], 	0x08000000)

#define		CHIP3_SET_MDIV1_6		BIT_SET	(gcfg[6], 	0x10000000)
#define		CHIP3_SET_MDIV1_5		BIT_SET	(gcfg[6], 	0x20000000)
#define		CHIP3_SET_MDIV1_4		BIT_SET	(gcfg[6], 	0x40000000)
#define		CHIP3_SET_MDIV1_3		BIT_SET	(gcfg[6], 	0x80000000)
#define		CHIP3_SET_MDIV1_2		BIT_SET	(gcfg[7], 	0x00000001)
#define		CHIP3_SET_MDIV1_1		BIT_SET	(gcfg[7], 	0x00000002)
#define		CHIP3_SET_MDIV1_0		BIT_SET	(gcfg[7], 	0x00000004)


#define		CHIP3_SET_CAP0_4		BIT_SET	(gcfg[7], 	0x00000008)
#define		CHIP3_SET_CAP0_3		BIT_SET	(gcfg[7], 	0x00000010)
#define		CHIP3_SET_CAP0_2		BIT_SET	(gcfg[7], 	0x00000020)
#define		CHIP3_SET_CAP0_1		BIT_SET	(gcfg[7], 	0x00000040)
#define		CHIP3_SET_CAP0_0		BIT_SET	(gcfg[7], 	0x00000080)

#define		CHIP3_SET_BS0_3		BIT_SET	(gcfg[7], 	0x00000100)
#define		CHIP3_SET_BS0_2		BIT_SET	(gcfg[7], 	0x00000200)
#define		CHIP3_SET_BS0_1		BIT_SET	(gcfg[7], 	0x00000400)
#define		CHIP3_SET_BS0_0		BIT_SET	(gcfg[7], 	0x00000800)

#define		CHIP3_SET_MDIV0_6		BIT_SET	(gcfg[7], 	0x00001000)
#define		CHIP3_SET_MDIV0_5		BIT_SET	(gcfg[7], 	0x00002000)
#define		CHIP3_SET_MDIV0_4		BIT_SET	(gcfg[7], 	0x00004000)
#define		CHIP3_SET_MDIV0_3		BIT_SET	(gcfg[7], 	0x00008000)
#define		CHIP3_SET_MDIV0_2		BIT_SET	(gcfg[7], 	0x00010000)
#define		CHIP3_SET_MDIV0_1		BIT_SET	(gcfg[7], 	0x00020000)
#define		CHIP3_SET_MDIV0_0		BIT_SET	(gcfg[7], 	0x00040000)

/// macro for Reset bits
#define		CHIP3_RST_FS_1		BIT_RST	(gcfg[0], 	0x00000001)
#define		CHIP3_RST_FS_0		BIT_RST	(gcfg[0], 	0x00000002)

#define		CHIP3_RST_ANA_BITS_7		BIT_RST	(gcfg[0], 	0x00000004)
#define		CHIP3_RST_ANA_BITS_6		BIT_RST	(gcfg[0], 	0x00000008)
#define		CHIP3_RST_ANA_BITS_5		BIT_RST	(gcfg[0], 	0x00000010)
#define		CHIP3_RST_ANA_BITS_4		BIT_RST	(gcfg[0], 	0x00000020)
#define		CHIP3_RST_ANA_BITS_3		BIT_RST	(gcfg[0], 	0x00000040)
#define		CHIP3_RST_ANA_BITS_2		BIT_RST	(gcfg[0], 	0x00000080)
#define		CHIP3_RST_ANA_BITS_1		BIT_RST	(gcfg[0], 	0x00000100)
#define		CHIP3_RST_ANA_BITS_0		BIT_RST	(gcfg[0], 	0x00000200)

#define		CHIP3_RST_DBFACTOR1_9		BIT_RST	(gcfg[0], 	0x00000400)
#define		CHIP3_RST_DBFACTOR1_8		BIT_RST	(gcfg[0], 	0x00000800)
#define		CHIP3_RST_DBFACTOR1_7		BIT_RST	(gcfg[0], 	0x00001000)
#define		CHIP3_RST_DBFACTOR1_6		BIT_RST	(gcfg[0], 	0x00002000)
#define		CHIP3_RST_DBFACTOR1_5		BIT_RST	(gcfg[0], 	0x00004000)
#define		CHIP3_RST_DBFACTOR1_4		BIT_RST	(gcfg[0], 	0x00008000)
#define		CHIP3_RST_DBFACTOR1_3		BIT_RST	(gcfg[0], 	0x00010000)
#define		CHIP3_RST_DBFACTOR1_2		BIT_RST	(gcfg[0], 	0x00020000)
#define		CHIP3_RST_DBFACTOR1_1		BIT_RST	(gcfg[0], 	0x00040000)
#define		CHIP3_RST_DBFACTOR1_0		BIT_RST	(gcfg[0], 	0x00080000)

#define		CHIP3_RST_DBFACTOR0_9		BIT_RST	(gcfg[0], 	0x00100000)
#define		CHIP3_RST_DBFACTOR0_8		BIT_RST	(gcfg[0], 	0x00200000)
#define		CHIP3_RST_DBFACTOR0_7		BIT_RST	(gcfg[0], 	0x00400000)
#define		CHIP3_RST_DBFACTOR0_6		BIT_RST	(gcfg[0], 	0x00800000)
#define		CHIP3_RST_DBFACTOR0_5		BIT_RST	(gcfg[0], 	0x01000000)
#define		CHIP3_RST_DBFACTOR0_4		BIT_RST	(gcfg[0], 	0x02000000)
#define		CHIP3_RST_DBFACTOR0_3		BIT_RST	(gcfg[0], 	0x04000000)
#define		CHIP3_RST_DBFACTOR0_2		BIT_RST	(gcfg[0], 	0x08000000)
#define		CHIP3_RST_DBFACTOR0_1		BIT_RST	(gcfg[0], 	0x10000000)
#define		CHIP3_RST_DBFACTOR0_0		BIT_RST	(gcfg[0], 	0x20000000)

#define		CHIP3_RST_THRESHOLD_7		BIT_RST	(gcfg[0], 	0x40000000)
#define		CHIP3_RST_THRESHOLD_6		BIT_RST	(gcfg[0], 	0x80000000)
#define		CHIP3_RST_THRESHOLD_5		BIT_RST	(gcfg[1], 	0x00000001)
#define		CHIP3_RST_THRESHOLD_4		BIT_RST	(gcfg[1], 	0x00000002)
#define		CHIP3_RST_THRESHOLD_3		BIT_RST	(gcfg[1], 	0x00000004)
#define		CHIP3_RST_THRESHOLD_2		BIT_RST	(gcfg[1], 	0x00000008)
#define		CHIP3_RST_THRESHOLD_1		BIT_RST	(gcfg[1], 	0x00000010)
#define		CHIP3_RST_THRESHOLD_0		BIT_RST	(gcfg[1], 	0x00000020)

#define		CHIP3_RST_FACTORS_7		BIT_RST	(gcfg[1], 	0x00000040)
#define		CHIP3_RST_FACTORS_6		BIT_RST	(gcfg[1], 	0x00000080)
#define		CHIP3_RST_FACTORS_5		BIT_RST	(gcfg[1], 	0x00000100)
#define		CHIP3_RST_FACTORS_4		BIT_RST	(gcfg[1], 	0x00000200)
#define		CHIP3_RST_FACTORS_3		BIT_RST	(gcfg[1], 	0x00000400)
#define		CHIP3_RST_FACTORS_2		BIT_RST	(gcfg[1], 	0x00000800)
#define		CHIP3_RST_FACTORS_1		BIT_RST	(gcfg[1], 	0x00001000)
#define		CHIP3_RST_FACTORS_0		BIT_RST	(gcfg[1], 	0x00002000)

#define		CHIP3_RST_AVRG_NUM			BIT_RST	(gcfg[1], 	0x00004000)
#define		CHIP3_RST_RAND_GROUP			BIT_RST	(gcfg[1], 	0x00008000)
#define		CHIP3_RST_DIV_ACCU			BIT_RST	(gcfg[1], 	0x00010000)
#define		CHIP3_RST_FLAG4D			BIT_RST	(gcfg[1], 	0x00020000)
#define		CHIP3_RST_OPT_MODE			BIT_RST	(gcfg[1], 	0x00040000)

#define		CHIP3_RST_STEP_T_2		BIT_RST	(gcfg[1], 	0x00080000)
#define		CHIP3_RST_STEP_T_1		BIT_RST	(gcfg[1], 	0x00100000)
#define		CHIP3_RST_STEP_T_0		BIT_RST	(gcfg[1], 	0x00200000)

#define		CHIP3_RST_INIT_T_5		BIT_RST	(gcfg[1], 	0x00400000)
#define		CHIP3_RST_INIT_T_4		BIT_RST	(gcfg[1], 	0x00800000)
#define		CHIP3_RST_INIT_T_3		BIT_RST	(gcfg[1], 	0x01000000)
#define		CHIP3_RST_INIT_T_2		BIT_RST	(gcfg[1], 	0x02000000)
#define		CHIP3_RST_INIT_T_1		BIT_RST	(gcfg[1], 	0x04000000)
#define		CHIP3_RST_INIT_T_0		BIT_RST	(gcfg[1], 	0x08000000)

#define		CHIP3_RST_SS_ITER_10		BIT_RST	(gcfg[1], 	0x10000000)
#define		CHIP3_RST_SS_ITER_9		BIT_RST	(gcfg[1], 	0x20000000)
#define		CHIP3_RST_SS_ITER_8		BIT_RST	(gcfg[1], 	0x40000000)
#define		CHIP3_RST_SS_ITER_7		BIT_RST	(gcfg[1], 	0x80000000)
#define		CHIP3_RST_SS_ITER_6		BIT_RST	(gcfg[2], 	0x00000001)
#define		CHIP3_RST_SS_ITER_5		BIT_RST	(gcfg[2], 	0x00000002)
#define		CHIP3_RST_SS_ITER_4		BIT_RST	(gcfg[2], 	0x00000004)
#define		CHIP3_RST_SS_ITER_3		BIT_RST	(gcfg[2], 	0x00000008)
#define		CHIP3_RST_SS_ITER_2		BIT_RST	(gcfg[2], 	0x00000010)
#define		CHIP3_RST_SS_ITER_1		BIT_RST	(gcfg[2], 	0x00000020)
#define		CHIP3_RST_SS_ITER_0		BIT_RST	(gcfg[2], 	0x00000040)

#define		CHIP3_RST_SA_ITER_10		BIT_RST	(gcfg[2], 	0x00000080)
#define		CHIP3_RST_SA_ITER_9		BIT_RST	(gcfg[2], 	0x00000100)
#define		CHIP3_RST_SA_ITER_8		BIT_RST	(gcfg[2], 	0x00000200)
#define		CHIP3_RST_SA_ITER_7		BIT_RST	(gcfg[2], 	0x00000400)
#define		CHIP3_RST_SA_ITER_6		BIT_RST	(gcfg[2], 	0x00000800)
#define		CHIP3_RST_SA_ITER_5		BIT_RST	(gcfg[2], 	0x00001000)
#define		CHIP3_RST_SA_ITER_4		BIT_RST	(gcfg[2], 	0x00002000)
#define		CHIP3_RST_SA_ITER_3		BIT_RST	(gcfg[2], 	0x00004000)
#define		CHIP3_RST_SA_ITER_2		BIT_RST	(gcfg[2], 	0x00008000)
#define		CHIP3_RST_SA_ITER_1		BIT_RST	(gcfg[2], 	0x00010000)
#define		CHIP3_RST_SA_ITER_0		BIT_RST	(gcfg[2], 	0x00020000)

#define		CHIP3_RST_INIT_Y1_4		BIT_RST	(gcfg[2], 	0x00040000)
#define		CHIP3_RST_INIT_Y1_3		BIT_RST	(gcfg[2], 	0x00080000)
#define		CHIP3_RST_INIT_Y1_2		BIT_RST	(gcfg[2], 	0x00100000)
#define		CHIP3_RST_INIT_Y1_1		BIT_RST	(gcfg[2], 	0x00200000)
#define		CHIP3_RST_INIT_Y1_0		BIT_RST	(gcfg[2], 	0x00400000)
#define		CHIP3_RST_INIT_X1_4		BIT_RST	(gcfg[2], 	0x00800000)
#define		CHIP3_RST_INIT_X1_3		BIT_RST	(gcfg[2], 	0x01000000)
#define		CHIP3_RST_INIT_X1_2		BIT_RST	(gcfg[2], 	0x02000000)
#define		CHIP3_RST_INIT_X1_1		BIT_RST	(gcfg[2], 	0x04000000)
#define		CHIP3_RST_INIT_X1_0		BIT_RST	(gcfg[2], 	0x08000000)
#define		CHIP3_RST_INIT_Y2_4		BIT_RST	(gcfg[2], 	0x10000000)
#define		CHIP3_RST_INIT_Y2_3		BIT_RST	(gcfg[2], 	0x20000000)
#define		CHIP3_RST_INIT_Y2_2		BIT_RST	(gcfg[2], 	0x40000000)
#define		CHIP3_RST_INIT_Y2_1		BIT_RST	(gcfg[2], 	0x80000000)
#define		CHIP3_RST_INIT_Y2_0		BIT_RST	(gcfg[3], 	0x00000001)
#define		CHIP3_RST_INIT_X2_4		BIT_RST	(gcfg[3], 	0x00000002)
#define		CHIP3_RST_INIT_X2_3		BIT_RST	(gcfg[3], 	0x00000004)
#define		CHIP3_RST_INIT_X2_2		BIT_RST	(gcfg[3], 	0x00000008)
#define		CHIP3_RST_INIT_X2_1		BIT_RST	(gcfg[3], 	0x00000010)
#define		CHIP3_RST_INIT_X2_0		BIT_RST	(gcfg[3], 	0x00000020)

#define		CHIP3_RST_NXT_TEST			BIT_RST	(gcfg[3], 	0x00000040)
#define		CHIP3_RST_TRDY_TEST			BIT_RST	(gcfg[3], 	0x00000080)

#define		CHIP3_RST_AOFF_9		BIT_RST	(gcfg[3], 	0x00000100)
#define		CHIP3_RST_AOFF_8		BIT_RST	(gcfg[3], 	0x00000200)
#define		CHIP3_RST_AOFF_7		BIT_RST	(gcfg[3], 	0x00000400)
#define		CHIP3_RST_AOFF_6		BIT_RST	(gcfg[3], 	0x00000800)
#define		CHIP3_RST_AOFF_5		BIT_RST	(gcfg[3], 	0x00001000)
#define		CHIP3_RST_AOFF_4		BIT_RST	(gcfg[3], 	0x00002000)
#define		CHIP3_RST_AOFF_3		BIT_RST	(gcfg[3], 	0x00004000)
#define		CHIP3_RST_AOFF_2		BIT_RST	(gcfg[3], 	0x00008000)
#define		CHIP3_RST_AOFF_1		BIT_RST	(gcfg[3], 	0x00010000)
#define		CHIP3_RST_AOFF_0		BIT_RST	(gcfg[3], 	0x00020000)

#define		CHIP3_RST_TOL_5		BIT_RST	(gcfg[3], 	0x00040000)
#define		CHIP3_RST_TOL_4		BIT_RST	(gcfg[3], 	0x00080000)
#define		CHIP3_RST_TOL_3		BIT_RST	(gcfg[3], 	0x00100000)
#define		CHIP3_RST_TOL_2		BIT_RST	(gcfg[3], 	0x00200000)
#define		CHIP3_RST_TOL_1		BIT_RST	(gcfg[3], 	0x00400000)
#define		CHIP3_RST_TOL_0		BIT_RST	(gcfg[3], 	0x00800000)

#define		CHIP3_RST_TRG_TEST			BIT_RST	(gcfg[3], 	0x01000000)
#define		CHIP3_RST_TEST_MUX			BIT_RST	(gcfg[3], 	0x02000000)
#define		CHIP3_RST_CAL			BIT_RST	(gcfg[3], 	0x04000000)

#define		CHIP3_RST_SRC			BIT_RST	(gcfg[3], 	0x08000000)
#define		CHIP3_RST_OSCD			BIT_RST	(gcfg[3], 	0x10000000)

#define		CHIP3_RST_SW			BIT_RST	(gcfg[3], 	0x20000000)

#define		CHIP3_RST_TX2_9		BIT_RST	(gcfg[3], 	0x40000000)
#define		CHIP3_RST_TX2_8		BIT_RST	(gcfg[3], 	0x80000000)
#define		CHIP3_RST_TX2_7		BIT_RST	(gcfg[4], 	0x00000001)
#define		CHIP3_RST_TX2_6		BIT_RST	(gcfg[4], 	0x00000002)
#define		CHIP3_RST_TX2_5		BIT_RST	(gcfg[4], 	0x00000004)
#define		CHIP3_RST_TX2_4		BIT_RST	(gcfg[4], 	0x00000008)
#define		CHIP3_RST_TX2_3		BIT_RST	(gcfg[4], 	0x00000010)
#define		CHIP3_RST_TX2_2		BIT_RST	(gcfg[4], 	0x00000020)
#define		CHIP3_RST_TX2_1		BIT_RST	(gcfg[4], 	0x00000040)
#define		CHIP3_RST_TX2_0		BIT_RST	(gcfg[4], 	0x00000080)

#define		CHIP3_RST_TY2_9		BIT_RST	(gcfg[4], 	0x00000100)
#define		CHIP3_RST_TY2_8		BIT_RST	(gcfg[4], 	0x00000200)
#define		CHIP3_RST_TY2_7		BIT_RST	(gcfg[4], 	0x00000400)
#define		CHIP3_RST_TY2_6		BIT_RST	(gcfg[4], 	0x00000800)
#define		CHIP3_RST_TY2_5		BIT_RST	(gcfg[4], 	0x00001000)
#define		CHIP3_RST_TY2_4		BIT_RST	(gcfg[4], 	0x00002000)
#define		CHIP3_RST_TY2_3		BIT_RST	(gcfg[4], 	0x00004000)
#define		CHIP3_RST_TY2_2		BIT_RST	(gcfg[4], 	0x00008000)
#define		CHIP3_RST_TY2_1		BIT_RST	(gcfg[4], 	0x00010000)
#define		CHIP3_RST_TY2_0		BIT_RST	(gcfg[4], 	0x00020000)

#define		CHIP3_RST_CB4_2		BIT_RST	(gcfg[4], 	0x00040000)
#define		CHIP3_RST_CB4_1		BIT_RST	(gcfg[4], 	0x00080000)
#define		CHIP3_RST_CB4_0		BIT_RST	(gcfg[4], 	0x00100000)
#define		CHIP3_RST_CB3_2		BIT_RST	(gcfg[4], 	0x00200000)
#define		CHIP3_RST_CB3_1		BIT_RST	(gcfg[4], 	0x00400000)
#define		CHIP3_RST_CB3_0		BIT_RST	(gcfg[4], 	0x00800000)

#define		CHIP3_RST_TX1_9		BIT_RST	(gcfg[4], 	0x01000000)
#define		CHIP3_RST_TX1_8		BIT_RST	(gcfg[4], 	0x02000000)
#define		CHIP3_RST_TX1_7		BIT_RST	(gcfg[4], 	0x04000000)
#define		CHIP3_RST_TX1_6		BIT_RST	(gcfg[4], 	0x08000000)
#define		CHIP3_RST_TX1_5		BIT_RST	(gcfg[4], 	0x10000000)
#define		CHIP3_RST_TX1_4		BIT_RST	(gcfg[4], 	0x20000000)
#define		CHIP3_RST_TX1_3		BIT_RST	(gcfg[4], 	0x40000000)
#define		CHIP3_RST_TX1_2		BIT_RST	(gcfg[4], 	0x80000000)
#define		CHIP3_RST_TX1_1		BIT_RST	(gcfg[5], 	0x00000001)
#define		CHIP3_RST_TX1_0		BIT_RST	(gcfg[5], 	0x00000002)

#define		CHIP3_RST_TY1_9		BIT_RST	(gcfg[5], 	0x00000004)
#define		CHIP3_RST_TY1_8		BIT_RST	(gcfg[5], 	0x00000008)
#define		CHIP3_RST_TY1_7		BIT_RST	(gcfg[5], 	0x00000010)
#define		CHIP3_RST_TY1_6		BIT_RST	(gcfg[5], 	0x00000020)
#define		CHIP3_RST_TY1_5		BIT_RST	(gcfg[5], 	0x00000040)
#define		CHIP3_RST_TY1_4		BIT_RST	(gcfg[5], 	0x00000080)
#define		CHIP3_RST_TY1_3		BIT_RST	(gcfg[5], 	0x00000100)
#define		CHIP3_RST_TY1_2		BIT_RST	(gcfg[5], 	0x00000200)
#define		CHIP3_RST_TY1_1		BIT_RST	(gcfg[5], 	0x00000400)
#define		CHIP3_RST_TY1_0		BIT_RST	(gcfg[5], 	0x00000800)

#define		CHIP3_RST_CB2_2		BIT_RST	(gcfg[5], 	0x00001000)
#define		CHIP3_RST_CB2_1		BIT_RST	(gcfg[5], 	0x00002000)
#define		CHIP3_RST_CB2_0		BIT_RST	(gcfg[5], 	0x00004000)
#define		CHIP3_RST_CB1_2		BIT_RST	(gcfg[5], 	0x00008000)
#define		CHIP3_RST_CB1_1		BIT_RST	(gcfg[5], 	0x00010000)
#define		CHIP3_RST_CB1_0		BIT_RST	(gcfg[5], 	0x00020000)

#define		CHIP3_RST_PHS			BIT_RST	(gcfg[5], 	0x00040000)

#define		CHIP3_RST_CAP3_4		BIT_RST	(gcfg[5], 	0x00080000)
#define		CHIP3_RST_CAP3_3		BIT_RST	(gcfg[5], 	0x00100000)
#define		CHIP3_RST_CAP3_2		BIT_RST	(gcfg[5], 	0x00200000)
#define		CHIP3_RST_CAP3_1		BIT_RST	(gcfg[5], 	0x00400000)
#define		CHIP3_RST_CAP3_0		BIT_RST	(gcfg[5], 	0x00800000)

#define		CHIP3_RST_BS3_3		BIT_RST	(gcfg[5], 	0x01000000)
#define		CHIP3_RST_BS3_2		BIT_RST	(gcfg[5], 	0x02000000)
#define		CHIP3_RST_BS3_1		BIT_RST	(gcfg[5], 	0x04000000)
#define		CHIP3_RST_BS3_0		BIT_RST	(gcfg[5], 	0x08000000)

#define		CHIP3_RST_MDIV3_6		BIT_RST	(gcfg[5], 	0x10000000)
#define		CHIP3_RST_MDIV3_5		BIT_RST	(gcfg[5], 	0x20000000)
#define		CHIP3_RST_MDIV3_4		BIT_RST	(gcfg[5], 	0x40000000)
#define		CHIP3_RST_MDIV3_3		BIT_RST	(gcfg[5], 	0x80000000)
#define		CHIP3_RST_MDIV3_2		BIT_RST	(gcfg[6], 	0x00000001)
#define		CHIP3_RST_MDIV3_1		BIT_RST	(gcfg[6], 	0x00000002)
#define		CHIP3_RST_MDIV3_0		BIT_RST	(gcfg[6], 	0x00000004)


#define		CHIP3_RST_CAP2_4		BIT_RST	(gcfg[6], 	0x00000008)
#define		CHIP3_RST_CAP2_3		BIT_RST	(gcfg[6], 	0x00000010)
#define		CHIP3_RST_CAP2_2		BIT_RST	(gcfg[6], 	0x00000020)
#define		CHIP3_RST_CAP2_1		BIT_RST	(gcfg[6], 	0x00000040)
#define		CHIP3_RST_CAP2_0		BIT_RST	(gcfg[6], 	0x00000080)

#define		CHIP3_RST_BS2_3		BIT_RST	(gcfg[6], 	0x00000100)
#define		CHIP3_RST_BS2_2		BIT_RST	(gcfg[6], 	0x00000200)
#define		CHIP3_RST_BS2_1		BIT_RST	(gcfg[6], 	0x00000400)
#define		CHIP3_RST_BS2_0		BIT_RST	(gcfg[6], 	0x00000800)

#define		CHIP3_RST_MDIV2_6		BIT_RST	(gcfg[6], 	0x00001000)
#define		CHIP3_RST_MDIV2_5		BIT_RST	(gcfg[6], 	0x00002000)
#define		CHIP3_RST_MDIV2_4		BIT_RST	(gcfg[6], 	0x00004000)
#define		CHIP3_RST_MDIV2_3		BIT_RST	(gcfg[6], 	0x00008000)
#define		CHIP3_RST_MDIV2_2		BIT_RST	(gcfg[6], 	0x00010000)
#define		CHIP3_RST_MDIV2_1		BIT_RST	(gcfg[6], 	0x00020000)
#define		CHIP3_RST_MDIV2_0		BIT_RST	(gcfg[6], 	0x00040000)


#define		CHIP3_RST_CAP1_4		BIT_RST	(gcfg[6], 	0x00080000)
#define		CHIP3_RST_CAP1_3		BIT_RST	(gcfg[6], 	0x00100000)
#define		CHIP3_RST_CAP1_2		BIT_RST	(gcfg[6], 	0x00200000)
#define		CHIP3_RST_CAP1_1		BIT_RST	(gcfg[6], 	0x00400000)
#define		CHIP3_RST_CAP1_0		BIT_RST	(gcfg[6], 	0x00800000)

#define		CHIP3_RST_BS1_3		BIT_RST	(gcfg[6], 	0x01000000)
#define		CHIP3_RST_BS1_2		BIT_RST	(gcfg[6], 	0x02000000)
#define		CHIP3_RST_BS1_1		BIT_RST	(gcfg[6], 	0x04000000)
#define		CHIP3_RST_BS1_0		BIT_RST	(gcfg[6], 	0x08000000)

#define		CHIP3_RST_MDIV1_6		BIT_RST	(gcfg[6], 	0x10000000)
#define		CHIP3_RST_MDIV1_5		BIT_RST	(gcfg[6], 	0x20000000)
#define		CHIP3_RST_MDIV1_4		BIT_RST	(gcfg[6], 	0x40000000)
#define		CHIP3_RST_MDIV1_3		BIT_RST	(gcfg[6], 	0x80000000)
#define		CHIP3_RST_MDIV1_2		BIT_RST	(gcfg[7], 	0x00000001)
#define		CHIP3_RST_MDIV1_1		BIT_RST	(gcfg[7], 	0x00000002)
#define		CHIP3_RST_MDIV1_0		BIT_RST	(gcfg[7], 	0x00000004)


#define		CHIP3_RST_CAP0_4		BIT_RST	(gcfg[7], 	0x00000008)
#define		CHIP3_RST_CAP0_3		BIT_RST	(gcfg[7], 	0x00000010)
#define		CHIP3_RST_CAP0_2		BIT_RST	(gcfg[7], 	0x00000020)
#define		CHIP3_RST_CAP0_1		BIT_RST	(gcfg[7], 	0x00000040)
#define		CHIP3_RST_CAP0_0		BIT_RST	(gcfg[7], 	0x00000080)

#define		CHIP3_RST_BS0_3		BIT_RST	(gcfg[7], 	0x00000100)
#define		CHIP3_RST_BS0_2		BIT_RST	(gcfg[7], 	0x00000200)
#define		CHIP3_RST_BS0_1		BIT_RST	(gcfg[7], 	0x00000400)
#define		CHIP3_RST_BS0_0		BIT_RST	(gcfg[7], 	0x00000800)

#define		CHIP3_RST_MDIV0_6		BIT_RST	(gcfg[7], 	0x00001000)
#define		CHIP3_RST_MDIV0_5		BIT_RST	(gcfg[7], 	0x00002000)
#define		CHIP3_RST_MDIV0_4		BIT_RST	(gcfg[7], 	0x00004000)
#define		CHIP3_RST_MDIV0_3		BIT_RST	(gcfg[7], 	0x00008000)
#define		CHIP3_RST_MDIV0_2		BIT_RST	(gcfg[7], 	0x00010000)
#define		CHIP3_RST_MDIV0_1		BIT_RST	(gcfg[7], 	0x00020000)
#define		CHIP3_RST_MDIV0_0		BIT_RST	(gcfg[7], 	0x00040000)

/// macro for check
#define		CHIP3_CHK_FS_1		BIT_CHK	(gcfg[0], 	0x00000001)
#define		CHIP3_CHK_FS_0		BIT_CHK	(gcfg[0], 	0x00000002)

#define		CHIP3_CHK_ANA_BITS_7		BIT_CHK	(gcfg[0], 	0x00000004)
#define		CHIP3_CHK_ANA_BITS_6		BIT_CHK	(gcfg[0], 	0x00000008)
#define		CHIP3_CHK_ANA_BITS_5		BIT_CHK	(gcfg[0], 	0x00000010)
#define		CHIP3_CHK_ANA_BITS_4		BIT_CHK	(gcfg[0], 	0x00000020)
#define		CHIP3_CHK_ANA_BITS_3		BIT_CHK	(gcfg[0], 	0x00000040)
#define		CHIP3_CHK_ANA_BITS_2		BIT_CHK	(gcfg[0], 	0x00000080)
#define		CHIP3_CHK_ANA_BITS_1		BIT_CHK	(gcfg[0], 	0x00000100)
#define		CHIP3_CHK_ANA_BITS_0		BIT_CHK	(gcfg[0], 	0x00000200)

#define		CHIP3_CHK_DBFACTOR1_9		BIT_CHK	(gcfg[0], 	0x00000400)
#define		CHIP3_CHK_DBFACTOR1_8		BIT_CHK	(gcfg[0], 	0x00000800)
#define		CHIP3_CHK_DBFACTOR1_7		BIT_CHK	(gcfg[0], 	0x00001000)
#define		CHIP3_CHK_DBFACTOR1_6		BIT_CHK	(gcfg[0], 	0x00002000)
#define		CHIP3_CHK_DBFACTOR1_5		BIT_CHK	(gcfg[0], 	0x00004000)
#define		CHIP3_CHK_DBFACTOR1_4		BIT_CHK	(gcfg[0], 	0x00008000)
#define		CHIP3_CHK_DBFACTOR1_3		BIT_CHK	(gcfg[0], 	0x00010000)
#define		CHIP3_CHK_DBFACTOR1_2		BIT_CHK	(gcfg[0], 	0x00020000)
#define		CHIP3_CHK_DBFACTOR1_1		BIT_CHK	(gcfg[0], 	0x00040000)
#define		CHIP3_CHK_DBFACTOR1_0		BIT_CHK	(gcfg[0], 	0x00080000)

#define		CHIP3_CHK_DBFACTOR0_9		BIT_CHK	(gcfg[0], 	0x00100000)
#define		CHIP3_CHK_DBFACTOR0_8		BIT_CHK	(gcfg[0], 	0x00200000)
#define		CHIP3_CHK_DBFACTOR0_7		BIT_CHK	(gcfg[0], 	0x00400000)
#define		CHIP3_CHK_DBFACTOR0_6		BIT_CHK	(gcfg[0], 	0x00800000)
#define		CHIP3_CHK_DBFACTOR0_5		BIT_CHK	(gcfg[0], 	0x01000000)
#define		CHIP3_CHK_DBFACTOR0_4		BIT_CHK	(gcfg[0], 	0x02000000)
#define		CHIP3_CHK_DBFACTOR0_3		BIT_CHK	(gcfg[0], 	0x04000000)
#define		CHIP3_CHK_DBFACTOR0_2		BIT_CHK	(gcfg[0], 	0x08000000)
#define		CHIP3_CHK_DBFACTOR0_1		BIT_CHK	(gcfg[0], 	0x10000000)
#define		CHIP3_CHK_DBFACTOR0_0		BIT_CHK	(gcfg[0], 	0x20000000)

#define		CHIP3_CHK_THRESHOLD_7		BIT_CHK	(gcfg[0], 	0x40000000)
#define		CHIP3_CHK_THRESHOLD_6		BIT_CHK	(gcfg[0], 	0x80000000)
#define		CHIP3_CHK_THRESHOLD_5		BIT_CHK	(gcfg[1], 	0x00000001)
#define		CHIP3_CHK_THRESHOLD_4		BIT_CHK	(gcfg[1], 	0x00000002)
#define		CHIP3_CHK_THRESHOLD_3		BIT_CHK	(gcfg[1], 	0x00000004)
#define		CHIP3_CHK_THRESHOLD_2		BIT_CHK	(gcfg[1], 	0x00000008)
#define		CHIP3_CHK_THRESHOLD_1		BIT_CHK	(gcfg[1], 	0x00000010)
#define		CHIP3_CHK_THRESHOLD_0		BIT_CHK	(gcfg[1], 	0x00000020)

#define		CHIP3_CHK_FACTORS_7		BIT_CHK	(gcfg[1], 	0x00000040)
#define		CHIP3_CHK_FACTORS_6		BIT_CHK	(gcfg[1], 	0x00000080)
#define		CHIP3_CHK_FACTORS_5		BIT_CHK	(gcfg[1], 	0x00000100)
#define		CHIP3_CHK_FACTORS_4		BIT_CHK	(gcfg[1], 	0x00000200)
#define		CHIP3_CHK_FACTORS_3		BIT_CHK	(gcfg[1], 	0x00000400)
#define		CHIP3_CHK_FACTORS_2		BIT_CHK	(gcfg[1], 	0x00000800)
#define		CHIP3_CHK_FACTORS_1		BIT_CHK	(gcfg[1], 	0x00001000)
#define		CHIP3_CHK_FACTORS_0		BIT_CHK	(gcfg[1], 	0x00002000)

#define		CHIP3_CHK_AVRG_NUM			BIT_CHK	(gcfg[1], 	0x00004000)
#define		CHIP3_CHK_RAND_GROUP			BIT_CHK	(gcfg[1], 	0x00008000)
#define		CHIP3_CHK_DIV_ACCU			BIT_CHK	(gcfg[1], 	0x00010000)
#define		CHIP3_CHK_FLAG4D			BIT_CHK	(gcfg[1], 	0x00020000)
#define		CHIP3_CHK_OPT_MODE			BIT_CHK	(gcfg[1], 	0x00040000)

#define		CHIP3_CHK_STEP_T_2		BIT_CHK	(gcfg[1], 	0x00080000)
#define		CHIP3_CHK_STEP_T_1		BIT_CHK	(gcfg[1], 	0x00100000)
#define		CHIP3_CHK_STEP_T_0		BIT_CHK	(gcfg[1], 	0x00200000)

#define		CHIP3_CHK_INIT_T_5		BIT_CHK	(gcfg[1], 	0x00400000)
#define		CHIP3_CHK_INIT_T_4		BIT_CHK	(gcfg[1], 	0x00800000)
#define		CHIP3_CHK_INIT_T_3		BIT_CHK	(gcfg[1], 	0x01000000)
#define		CHIP3_CHK_INIT_T_2		BIT_CHK	(gcfg[1], 	0x02000000)
#define		CHIP3_CHK_INIT_T_1		BIT_CHK	(gcfg[1], 	0x04000000)
#define		CHIP3_CHK_INIT_T_0		BIT_CHK	(gcfg[1], 	0x08000000)

#define		CHIP3_CHK_SS_ITER_10		BIT_CHK	(gcfg[1], 	0x10000000)
#define		CHIP3_CHK_SS_ITER_9		BIT_CHK	(gcfg[1], 	0x20000000)
#define		CHIP3_CHK_SS_ITER_8		BIT_CHK	(gcfg[1], 	0x40000000)
#define		CHIP3_CHK_SS_ITER_7		BIT_CHK	(gcfg[1], 	0x80000000)
#define		CHIP3_CHK_SS_ITER_6		BIT_CHK	(gcfg[2], 	0x00000001)
#define		CHIP3_CHK_SS_ITER_5		BIT_CHK	(gcfg[2], 	0x00000002)
#define		CHIP3_CHK_SS_ITER_4		BIT_CHK	(gcfg[2], 	0x00000004)
#define		CHIP3_CHK_SS_ITER_3		BIT_CHK	(gcfg[2], 	0x00000008)
#define		CHIP3_CHK_SS_ITER_2		BIT_CHK	(gcfg[2], 	0x00000010)
#define		CHIP3_CHK_SS_ITER_1		BIT_CHK	(gcfg[2], 	0x00000020)
#define		CHIP3_CHK_SS_ITER_0		BIT_CHK	(gcfg[2], 	0x00000040)

#define		CHIP3_CHK_SA_ITER_10		BIT_CHK	(gcfg[2], 	0x00000080)
#define		CHIP3_CHK_SA_ITER_9		BIT_CHK	(gcfg[2], 	0x00000100)
#define		CHIP3_CHK_SA_ITER_8		BIT_CHK	(gcfg[2], 	0x00000200)
#define		CHIP3_CHK_SA_ITER_7		BIT_CHK	(gcfg[2], 	0x00000400)
#define		CHIP3_CHK_SA_ITER_6		BIT_CHK	(gcfg[2], 	0x00000800)
#define		CHIP3_CHK_SA_ITER_5		BIT_CHK	(gcfg[2], 	0x00001000)
#define		CHIP3_CHK_SA_ITER_4		BIT_CHK	(gcfg[2], 	0x00002000)
#define		CHIP3_CHK_SA_ITER_3		BIT_CHK	(gcfg[2], 	0x00004000)
#define		CHIP3_CHK_SA_ITER_2		BIT_CHK	(gcfg[2], 	0x00008000)
#define		CHIP3_CHK_SA_ITER_1		BIT_CHK	(gcfg[2], 	0x00010000)
#define		CHIP3_CHK_SA_ITER_0		BIT_CHK	(gcfg[2], 	0x00020000)

#define		CHIP3_CHK_INIT_Y1_4		BIT_CHK	(gcfg[2], 	0x00040000)
#define		CHIP3_CHK_INIT_Y1_3		BIT_CHK	(gcfg[2], 	0x00080000)
#define		CHIP3_CHK_INIT_Y1_2		BIT_CHK	(gcfg[2], 	0x00100000)
#define		CHIP3_CHK_INIT_Y1_1		BIT_CHK	(gcfg[2], 	0x00200000)
#define		CHIP3_CHK_INIT_Y1_0		BIT_CHK	(gcfg[2], 	0x00400000)
#define		CHIP3_CHK_INIT_X1_4		BIT_CHK	(gcfg[2], 	0x00800000)
#define		CHIP3_CHK_INIT_X1_3		BIT_CHK	(gcfg[2], 	0x01000000)
#define		CHIP3_CHK_INIT_X1_2		BIT_CHK	(gcfg[2], 	0x02000000)
#define		CHIP3_CHK_INIT_X1_1		BIT_CHK	(gcfg[2], 	0x04000000)
#define		CHIP3_CHK_INIT_X1_0		BIT_CHK	(gcfg[2], 	0x08000000)
#define		CHIP3_CHK_INIT_Y2_4		BIT_CHK	(gcfg[2], 	0x10000000)
#define		CHIP3_CHK_INIT_Y2_3		BIT_CHK	(gcfg[2], 	0x20000000)
#define		CHIP3_CHK_INIT_Y2_2		BIT_CHK	(gcfg[2], 	0x40000000)
#define		CHIP3_CHK_INIT_Y2_1		BIT_CHK	(gcfg[2], 	0x80000000)
#define		CHIP3_CHK_INIT_Y2_0		BIT_CHK	(gcfg[3], 	0x00000001)
#define		CHIP3_CHK_INIT_X2_4		BIT_CHK	(gcfg[3], 	0x00000002)
#define		CHIP3_CHK_INIT_X2_3		BIT_CHK	(gcfg[3], 	0x00000004)
#define		CHIP3_CHK_INIT_X2_2		BIT_CHK	(gcfg[3], 	0x00000008)
#define		CHIP3_CHK_INIT_X2_1		BIT_CHK	(gcfg[3], 	0x00000010)
#define		CHIP3_CHK_INIT_X2_0		BIT_CHK	(gcfg[3], 	0x00000020)

#define		CHIP3_CHK_NXT_TEST			BIT_CHK	(gcfg[3], 	0x00000040)
#define		CHIP3_CHK_TRDY_TEST			BIT_CHK	(gcfg[3], 	0x00000080)

#define		CHIP3_CHK_AOFF_9		BIT_CHK	(gcfg[3], 	0x00000100)
#define		CHIP3_CHK_AOFF_8		BIT_CHK	(gcfg[3], 	0x00000200)
#define		CHIP3_CHK_AOFF_7		BIT_CHK	(gcfg[3], 	0x00000400)
#define		CHIP3_CHK_AOFF_6		BIT_CHK	(gcfg[3], 	0x00000800)
#define		CHIP3_CHK_AOFF_5		BIT_CHK	(gcfg[3], 	0x00001000)
#define		CHIP3_CHK_AOFF_4		BIT_CHK	(gcfg[3], 	0x00002000)
#define		CHIP3_CHK_AOFF_3		BIT_CHK	(gcfg[3], 	0x00004000)
#define		CHIP3_CHK_AOFF_2		BIT_CHK	(gcfg[3], 	0x00008000)
#define		CHIP3_CHK_AOFF_1		BIT_CHK	(gcfg[3], 	0x00010000)
#define		CHIP3_CHK_AOFF_0		BIT_CHK	(gcfg[3], 	0x00020000)

#define		CHIP3_CHK_TOL_5		BIT_CHK	(gcfg[3], 	0x00040000)
#define		CHIP3_CHK_TOL_4		BIT_CHK	(gcfg[3], 	0x00080000)
#define		CHIP3_CHK_TOL_3		BIT_CHK	(gcfg[3], 	0x00100000)
#define		CHIP3_CHK_TOL_2		BIT_CHK	(gcfg[3], 	0x00200000)
#define		CHIP3_CHK_TOL_1		BIT_CHK	(gcfg[3], 	0x00400000)
#define		CHIP3_CHK_TOL_0		BIT_CHK	(gcfg[3], 	0x00800000)

#define		CHIP3_CHK_TRG_TEST			BIT_CHK	(gcfg[3], 	0x01000000)
#define		CHIP3_CHK_TEST_MUX			BIT_CHK	(gcfg[3], 	0x02000000)
#define		CHIP3_CHK_CAL			BIT_CHK	(gcfg[3], 	0x04000000)

#define		CHIP3_CHK_SRC			BIT_CHK	(gcfg[3], 	0x08000000)
#define		CHIP3_CHK_OSCD			BIT_CHK	(gcfg[3], 	0x10000000)

#define		CHIP3_CHK_SW			BIT_CHK	(gcfg[3], 	0x20000000)

#define		CHIP3_CHK_TX2_9		BIT_CHK	(gcfg[3], 	0x40000000)
#define		CHIP3_CHK_TX2_8		BIT_CHK	(gcfg[3], 	0x80000000)
#define		CHIP3_CHK_TX2_7		BIT_CHK	(gcfg[4], 	0x00000001)
#define		CHIP3_CHK_TX2_6		BIT_CHK	(gcfg[4], 	0x00000002)
#define		CHIP3_CHK_TX2_5		BIT_CHK	(gcfg[4], 	0x00000004)
#define		CHIP3_CHK_TX2_4		BIT_CHK	(gcfg[4], 	0x00000008)
#define		CHIP3_CHK_TX2_3		BIT_CHK	(gcfg[4], 	0x00000010)
#define		CHIP3_CHK_TX2_2		BIT_CHK	(gcfg[4], 	0x00000020)
#define		CHIP3_CHK_TX2_1		BIT_CHK	(gcfg[4], 	0x00000040)
#define		CHIP3_CHK_TX2_0		BIT_CHK	(gcfg[4], 	0x00000080)

#define		CHIP3_CHK_TY2_9		BIT_CHK	(gcfg[4], 	0x00000100)
#define		CHIP3_CHK_TY2_8		BIT_CHK	(gcfg[4], 	0x00000200)
#define		CHIP3_CHK_TY2_7		BIT_CHK	(gcfg[4], 	0x00000400)
#define		CHIP3_CHK_TY2_6		BIT_CHK	(gcfg[4], 	0x00000800)
#define		CHIP3_CHK_TY2_5		BIT_CHK	(gcfg[4], 	0x00001000)
#define		CHIP3_CHK_TY2_4		BIT_CHK	(gcfg[4], 	0x00002000)
#define		CHIP3_CHK_TY2_3		BIT_CHK	(gcfg[4], 	0x00004000)
#define		CHIP3_CHK_TY2_2		BIT_CHK	(gcfg[4], 	0x00008000)
#define		CHIP3_CHK_TY2_1		BIT_CHK	(gcfg[4], 	0x00010000)
#define		CHIP3_CHK_TY2_0		BIT_CHK	(gcfg[4], 	0x00020000)

#define		CHIP3_CHK_CB4_2		BIT_CHK	(gcfg[4], 	0x00040000)
#define		CHIP3_CHK_CB4_1		BIT_CHK	(gcfg[4], 	0x00080000)
#define		CHIP3_CHK_CB4_0		BIT_CHK	(gcfg[4], 	0x00100000)
#define		CHIP3_CHK_CB3_2		BIT_CHK	(gcfg[4], 	0x00200000)
#define		CHIP3_CHK_CB3_1		BIT_CHK	(gcfg[4], 	0x00400000)
#define		CHIP3_CHK_CB3_0		BIT_CHK	(gcfg[4], 	0x00800000)

#define		CHIP3_CHK_TX1_9		BIT_CHK	(gcfg[4], 	0x01000000)
#define		CHIP3_CHK_TX1_8		BIT_CHK	(gcfg[4], 	0x02000000)
#define		CHIP3_CHK_TX1_7		BIT_CHK	(gcfg[4], 	0x04000000)
#define		CHIP3_CHK_TX1_6		BIT_CHK	(gcfg[4], 	0x08000000)
#define		CHIP3_CHK_TX1_5		BIT_CHK	(gcfg[4], 	0x10000000)
#define		CHIP3_CHK_TX1_4		BIT_CHK	(gcfg[4], 	0x20000000)
#define		CHIP3_CHK_TX1_3		BIT_CHK	(gcfg[4], 	0x40000000)
#define		CHIP3_CHK_TX1_2		BIT_CHK	(gcfg[4], 	0x80000000)
#define		CHIP3_CHK_TX1_1		BIT_CHK	(gcfg[5], 	0x00000001)
#define		CHIP3_CHK_TX1_0		BIT_CHK	(gcfg[5], 	0x00000002)

#define		CHIP3_CHK_TY1_9		BIT_CHK	(gcfg[5], 	0x00000004)
#define		CHIP3_CHK_TY1_8		BIT_CHK	(gcfg[5], 	0x00000008)
#define		CHIP3_CHK_TY1_7		BIT_CHK	(gcfg[5], 	0x00000010)
#define		CHIP3_CHK_TY1_6		BIT_CHK	(gcfg[5], 	0x00000020)
#define		CHIP3_CHK_TY1_5		BIT_CHK	(gcfg[5], 	0x00000040)
#define		CHIP3_CHK_TY1_4		BIT_CHK	(gcfg[5], 	0x00000080)
#define		CHIP3_CHK_TY1_3		BIT_CHK	(gcfg[5], 	0x00000100)
#define		CHIP3_CHK_TY1_2		BIT_CHK	(gcfg[5], 	0x00000200)
#define		CHIP3_CHK_TY1_1		BIT_CHK	(gcfg[5], 	0x00000400)
#define		CHIP3_CHK_TY1_0		BIT_CHK	(gcfg[5], 	0x00000800)

#define		CHIP3_CHK_CB2_2		BIT_CHK	(gcfg[5], 	0x00001000)
#define		CHIP3_CHK_CB2_1		BIT_CHK	(gcfg[5], 	0x00002000)
#define		CHIP3_CHK_CB2_0		BIT_CHK	(gcfg[5], 	0x00004000)
#define		CHIP3_CHK_CB1_2		BIT_CHK	(gcfg[5], 	0x00008000)
#define		CHIP3_CHK_CB1_1		BIT_CHK	(gcfg[5], 	0x00010000)
#define		CHIP3_CHK_CB1_0		BIT_CHK	(gcfg[5], 	0x00020000)

#define		CHIP3_CHK_PHS			BIT_CHK	(gcfg[5], 	0x00040000)

#define		CHIP3_CHK_CAP3_4		BIT_CHK	(gcfg[5], 	0x00080000)
#define		CHIP3_CHK_CAP3_3		BIT_CHK	(gcfg[5], 	0x00100000)
#define		CHIP3_CHK_CAP3_2		BIT_CHK	(gcfg[5], 	0x00200000)
#define		CHIP3_CHK_CAP3_1		BIT_CHK	(gcfg[5], 	0x00400000)
#define		CHIP3_CHK_CAP3_0		BIT_CHK	(gcfg[5], 	0x00800000)

#define		CHIP3_CHK_BS3_3		BIT_CHK	(gcfg[5], 	0x01000000)
#define		CHIP3_CHK_BS3_2		BIT_CHK	(gcfg[5], 	0x02000000)
#define		CHIP3_CHK_BS3_1		BIT_CHK	(gcfg[5], 	0x04000000)
#define		CHIP3_CHK_BS3_0		BIT_CHK	(gcfg[5], 	0x08000000)

#define		CHIP3_CHK_MDIV3_6		BIT_CHK	(gcfg[5], 	0x10000000)
#define		CHIP3_CHK_MDIV3_5		BIT_CHK	(gcfg[5], 	0x20000000)
#define		CHIP3_CHK_MDIV3_4		BIT_CHK	(gcfg[5], 	0x40000000)
#define		CHIP3_CHK_MDIV3_3		BIT_CHK	(gcfg[5], 	0x80000000)
#define		CHIP3_CHK_MDIV3_2		BIT_CHK	(gcfg[6], 	0x00000001)
#define		CHIP3_CHK_MDIV3_1		BIT_CHK	(gcfg[6], 	0x00000002)
#define		CHIP3_CHK_MDIV3_0		BIT_CHK	(gcfg[6], 	0x00000004)


#define		CHIP3_CHK_CAP2_4		BIT_CHK	(gcfg[6], 	0x00000008)
#define		CHIP3_CHK_CAP2_3		BIT_CHK	(gcfg[6], 	0x00000010)
#define		CHIP3_CHK_CAP2_2		BIT_CHK	(gcfg[6], 	0x00000020)
#define		CHIP3_CHK_CAP2_1		BIT_CHK	(gcfg[6], 	0x00000040)
#define		CHIP3_CHK_CAP2_0		BIT_CHK	(gcfg[6], 	0x00000080)

#define		CHIP3_CHK_BS2_3		BIT_CHK	(gcfg[6], 	0x00000100)
#define		CHIP3_CHK_BS2_2		BIT_CHK	(gcfg[6], 	0x00000200)
#define		CHIP3_CHK_BS2_1		BIT_CHK	(gcfg[6], 	0x00000400)
#define		CHIP3_CHK_BS2_0		BIT_CHK	(gcfg[6], 	0x00000800)

#define		CHIP3_CHK_MDIV2_6		BIT_CHK	(gcfg[6], 	0x00001000)
#define		CHIP3_CHK_MDIV2_5		BIT_CHK	(gcfg[6], 	0x00002000)
#define		CHIP3_CHK_MDIV2_4		BIT_CHK	(gcfg[6], 	0x00004000)
#define		CHIP3_CHK_MDIV2_3		BIT_CHK	(gcfg[6], 	0x00008000)
#define		CHIP3_CHK_MDIV2_2		BIT_CHK	(gcfg[6], 	0x00010000)
#define		CHIP3_CHK_MDIV2_1		BIT_CHK	(gcfg[6], 	0x00020000)
#define		CHIP3_CHK_MDIV2_0		BIT_CHK	(gcfg[6], 	0x00040000)


#define		CHIP3_CHK_CAP1_4		BIT_CHK	(gcfg[6], 	0x00080000)
#define		CHIP3_CHK_CAP1_3		BIT_CHK	(gcfg[6], 	0x00100000)
#define		CHIP3_CHK_CAP1_2		BIT_CHK	(gcfg[6], 	0x00200000)
#define		CHIP3_CHK_CAP1_1		BIT_CHK	(gcfg[6], 	0x00400000)
#define		CHIP3_CHK_CAP1_0		BIT_CHK	(gcfg[6], 	0x00800000)

#define		CHIP3_CHK_BS1_3		BIT_CHK	(gcfg[6], 	0x01000000)
#define		CHIP3_CHK_BS1_2		BIT_CHK	(gcfg[6], 	0x02000000)
#define		CHIP3_CHK_BS1_1		BIT_CHK	(gcfg[6], 	0x04000000)
#define		CHIP3_CHK_BS1_0		BIT_CHK	(gcfg[6], 	0x08000000)

#define		CHIP3_CHK_MDIV1_6		BIT_CHK	(gcfg[6], 	0x10000000)
#define		CHIP3_CHK_MDIV1_5		BIT_CHK	(gcfg[6], 	0x20000000)
#define		CHIP3_CHK_MDIV1_4		BIT_CHK	(gcfg[6], 	0x40000000)
#define		CHIP3_CHK_MDIV1_3		BIT_CHK	(gcfg[6], 	0x80000000)
#define		CHIP3_CHK_MDIV1_2		BIT_CHK	(gcfg[7], 	0x00000001)
#define		CHIP3_CHK_MDIV1_1		BIT_CHK	(gcfg[7], 	0x00000002)
#define		CHIP3_CHK_MDIV1_0		BIT_CHK	(gcfg[7], 	0x00000004)


#define		CHIP3_CHK_CAP0_4		BIT_CHK	(gcfg[7], 	0x00000008)
#define		CHIP3_CHK_CAP0_3		BIT_CHK	(gcfg[7], 	0x00000010)
#define		CHIP3_CHK_CAP0_2		BIT_CHK	(gcfg[7], 	0x00000020)
#define		CHIP3_CHK_CAP0_1		BIT_CHK	(gcfg[7], 	0x00000040)
#define		CHIP3_CHK_CAP0_0		BIT_CHK	(gcfg[7], 	0x00000080)

#define		CHIP3_CHK_BS0_3		BIT_CHK	(gcfg[7], 	0x00000100)
#define		CHIP3_CHK_BS0_2		BIT_CHK	(gcfg[7], 	0x00000200)
#define		CHIP3_CHK_BS0_1		BIT_CHK	(gcfg[7], 	0x00000400)
#define		CHIP3_CHK_BS0_0		BIT_CHK	(gcfg[7], 	0x00000800)

#define		CHIP3_CHK_MDIV0_6		BIT_CHK	(gcfg[7], 	0x00001000)
#define		CHIP3_CHK_MDIV0_5		BIT_CHK	(gcfg[7], 	0x00002000)
#define		CHIP3_CHK_MDIV0_4		BIT_CHK	(gcfg[7], 	0x00004000)
#define		CHIP3_CHK_MDIV0_3		BIT_CHK	(gcfg[7], 	0x00008000)
#define		CHIP3_CHK_MDIV0_2		BIT_CHK	(gcfg[7], 	0x00010000)
#define		CHIP3_CHK_MDIV0_1		BIT_CHK	(gcfg[7], 	0x00020000)
#define		CHIP3_CHK_MDIV0_0		BIT_CHK	(gcfg[7], 	0x00040000)

/// the macro for Scanchain B. tapeout 3
#define		CHIP3_SET_ADC_9		BIT_SET	(gscB, 	0x00000001)
#define		CHIP3_SET_ADC_8		BIT_SET	(gscB, 	0x00000002)
#define		CHIP3_SET_ADC_7		BIT_SET	(gscB, 	0x00000004)
#define		CHIP3_SET_ADC_6		BIT_SET	(gscB, 	0x00000008)
#define		CHIP3_SET_ADC_5		BIT_SET	(gscB, 	0x00000010)
#define		CHIP3_SET_ADC_4		BIT_SET	(gscB, 	0x00000020)
#define		CHIP3_SET_ADC_3		BIT_SET	(gscB, 	0x00000040)
#define		CHIP3_SET_ADC_2		BIT_SET	(gscB, 	0x00000080)
#define		CHIP3_SET_ADC_1		BIT_SET	(gscB, 	0x00000100)
#define		CHIP3_SET_ADC_0		BIT_SET	(gscB, 	0x00000200)

#define		CHIP3_SET_ADC_FNL			BIT_SET	(gscB, 	0x00000400)
#define		CHIP3_SET_RSTN_ADC			BIT_SET	(gscB, 	0x00000800)
#define		CHIP3_SET_CLK_ADC			BIT_SET	(gscB, 	0x00001000)
#define		CHIP3_SET_CLRN			BIT_SET	(gscB, 	0x00002000)

/// Reset
#define		CHIP3_RST_ADC_9		BIT_RST	(gscB, 	0x00000001)
#define		CHIP3_RST_ADC_8		BIT_RST	(gscB, 	0x00000002)
#define		CHIP3_RST_ADC_7		BIT_RST	(gscB, 	0x00000004)
#define		CHIP3_RST_ADC_6		BIT_RST	(gscB, 	0x00000008)
#define		CHIP3_RST_ADC_5		BIT_RST	(gscB, 	0x00000010)
#define		CHIP3_RST_ADC_4		BIT_RST	(gscB, 	0x00000020)
#define		CHIP3_RST_ADC_3		BIT_RST	(gscB, 	0x00000040)
#define		CHIP3_RST_ADC_2		BIT_RST	(gscB, 	0x00000080)
#define		CHIP3_RST_ADC_1		BIT_RST	(gscB, 	0x00000100)
#define		CHIP3_RST_ADC_0		BIT_RST	(gscB, 	0x00000200)

#define		CHIP3_RST_ADC_FNL			BIT_RST	(gscB, 	0x00000400)
#define		CHIP3_RST_RSTN_ADC			BIT_RST	(gscB, 	0x00000800)
#define		CHIP3_RST_CLK_ADC			BIT_RST	(gscB, 	0x00001000)
#define		CHIP3_RST_CLRN			BIT_RST	(gscB, 	0x00002000)

/// Check
#define		CHIP3_CHK_ADC_9		BIT_CHK	(gscB, 	0x00000001)
#define		CHIP3_CHK_ADC_8		BIT_CHK	(gscB, 	0x00000002)
#define		CHIP3_CHK_ADC_7		BIT_CHK	(gscB, 	0x00000004)
#define		CHIP3_CHK_ADC_6		BIT_CHK	(gscB, 	0x00000008)
#define		CHIP3_CHK_ADC_5		BIT_CHK	(gscB, 	0x00000010)
#define		CHIP3_CHK_ADC_4		BIT_CHK	(gscB, 	0x00000020)
#define		CHIP3_CHK_ADC_3		BIT_CHK	(gscB, 	0x00000040)
#define		CHIP3_CHK_ADC_2		BIT_CHK	(gscB, 	0x00000080)
#define		CHIP3_CHK_ADC_1		BIT_CHK	(gscB, 	0x00000100)
#define		CHIP3_CHK_ADC_0		BIT_CHK	(gscB, 	0x00000200)

#define		CHIP3_CHK_ADC_FNL			BIT_CHK	(gscB, 	0x00000400)
#define		CHIP3_CHK_RSTN_ADC			BIT_CHK	(gscB, 	0x00000800)
#define		CHIP3_CHK_CLK_ADC			BIT_CHK	(gscB, 	0x00001000)
#define		CHIP3_CHK_CLRN			BIT_CHK	(gscB, 	0x00002000)

#endif
