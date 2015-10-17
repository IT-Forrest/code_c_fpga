#ifndef _ALTERA_PSOC_PORT_H_
#define _ALTERA_PSOC_PORT_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

extern uint32_t     *ctrl_addr;         // Control register
extern uint32_t     *stat_addr;         // Status register
extern uint32_t     *reg_cntclk_addr;   // SCLK generation delay counter
extern uint32_t     local_ctrl_addr;    // the local variable for Control register

extern uint32_t     *reg_cntclrn_addr;
extern uint32_t     *reg_cntsmpl_addr;
extern uint32_t     *reg_cntckad_addr;
extern uint32_t     *reg_cntradc_addr;
extern uint32_t     *reg_cntconv_addr;

extern uint32_t     local_sactrl_addr;
extern uint32_t     *sactrl_addr;      // SA control register
extern uint32_t     *sastat_addr;      // SA status register (read)
extern uint32_t     *cntwsa_addr;       // SA waiting cycles for 1 iteration
extern uint32_t     *initx1_addr;       // Initia1 X1 register
extern uint32_t     *initx2_addr;       // Initia1 X2 register
extern uint32_t     *initx3_addr;       // Initia1 X3 register
extern uint32_t     *initx4_addr;       // Initia1 X4 register

extern uint32_t     *iternum_addr;      // Initial iteration number
extern uint32_t     *anain_addr;        // Ana bits value
extern uint32_t     *tunex1_addr;       // Tune X1 register
extern uint32_t     *tunex2_addr;       // Tune X2 register
extern uint32_t     *tunex3_addr;       // Tune X3 register
extern uint32_t     *tunex4_addr;       // Tune X4 register

extern uint32_t     local_sa4Dctrl_addr;
extern uint32_t     *sa4Dctrl_addr;       // SA control register
extern uint32_t     *sa4Dstat_addr;       // SA status register (read only)
extern uint32_t     *init4Dx1_addr;       // Initia1 X1 register
extern uint32_t     *init4Dx2_addr;       // Initia1 X2 register
extern uint32_t     *init4Dx3_addr;       // Initia1 X3 register
extern uint32_t     *init4Dx4_addr;       // Initia1 X4 register

extern uint32_t     *iter4Dsa_addr;      // Initial iteration number for SA
extern uint32_t     *iter4Dss_addr;      // Initial iteration number for SS
extern uint32_t     *initT4D_addr;       // Initial temparature
extern uint32_t     *anain4D_addr;       // Ana bits value
extern uint32_t     *tune4Dx1_addr;      // Tune X1 register
extern uint32_t     *tune4Dx2_addr;      // Tune X2 register
extern uint32_t     *tune4Dx3_addr;      // Tune X3 register
extern uint32_t     *tune4Dx4_addr;      // Tune X4 register

extern uint32_t     local_cfsa4Dctrl_addr;
extern uint32_t     *cfsa4Dctrl_addr;
extern uint32_t     *cfsainit4Dx1_addr;
extern uint32_t     *cfsainit4Dx2_addr;
extern uint32_t     *cfsainit4Dx3_addr;
extern uint32_t     *cfsainit4Dx4_addr;
extern uint32_t     *cfsaiter4Dsa_addr;
extern uint32_t     *cfsaiter4Dss_addr;
extern uint32_t     *cfsainitT4D_addr;
extern uint32_t     *cfsatolin4D_addr;
extern uint32_t     *cfsaoffin4D_addr;
extern uint32_t     *cfsa3dbfactor0_addr;
extern uint32_t     *cfsa3dbfactor1_addr;
extern uint32_t     *cfsaadcin4D_addr;
extern uint32_t     *cfsathreshold_addr;

extern uint32_t     *cfsatune4Dx1_addr;
extern uint32_t     *cfsatune4Dx2_addr;
extern uint32_t     *cfsatune4Dx3_addr;
extern uint32_t     *cfsatune4Dx4_addr;

extern uint32_t     local_chip3_ctrl_addr;
extern uint32_t     *chip3_set_ctrl_addr;
extern uint32_t     *chip3_chk_stat_addr;
extern uint32_t     *chip3_set_clkcnt_addr;


void avs_wait();

int init_mem();
int clean_mem();
int syn_ctrl();

int spi_set_res(uint8_t val);
int spi_set_res_ss(uint8_t val, uint8_t dev);
int hex_set_val(uint8_t pos, uint8_t val);

#endif //_ALTERA_PSOC_PORT_H_
