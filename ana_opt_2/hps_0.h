#ifndef _ALTERA_HPS_0_H_
#define _ALTERA_HPS_0_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'soc_system' in
 * file './soc_system.sopcinfo'.
 */

/*
 * This file contains macros for module 'hps_0' and devices
 * connected to the following masters:
 *   h2f_axi_master
 *   h2f_lw_axi_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'onchip_memory2_0', class 'altera_avalon_onchip_memory2'
 * The macros are prefixed with 'ONCHIP_MEMORY2_0_'.
 * The prefix is the slave descriptor.
 */
#define ONCHIP_MEMORY2_0_COMPONENT_TYPE altera_avalon_onchip_memory2
#define ONCHIP_MEMORY2_0_COMPONENT_NAME onchip_memory2_0
#define ONCHIP_MEMORY2_0_BASE 0x0
#define ONCHIP_MEMORY2_0_SPAN 65536
#define ONCHIP_MEMORY2_0_END 0xffff
#define ONCHIP_MEMORY2_0_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY2_0_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY2_0_CONTENTS_INFO ""
#define ONCHIP_MEMORY2_0_DUAL_PORT 0
#define ONCHIP_MEMORY2_0_GUI_RAM_BLOCK_TYPE AUTO
#define ONCHIP_MEMORY2_0_INIT_CONTENTS_FILE soc_system_onchip_memory2_0
#define ONCHIP_MEMORY2_0_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY2_0_INSTANCE_ID NONE
#define ONCHIP_MEMORY2_0_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY2_0_RAM_BLOCK_TYPE AUTO
#define ONCHIP_MEMORY2_0_READ_DURING_WRITE_MODE DONT_CARE
#define ONCHIP_MEMORY2_0_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY2_0_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY2_0_SIZE_VALUE 65536
#define ONCHIP_MEMORY2_0_WRITABLE 1
#define ONCHIP_MEMORY2_0_MEMORY_INFO_DAT_SYM_INSTALL_DIR SIM_DIR
#define ONCHIP_MEMORY2_0_MEMORY_INFO_GENERATE_DAT_SYM 1
#define ONCHIP_MEMORY2_0_MEMORY_INFO_GENERATE_HEX 1
#define ONCHIP_MEMORY2_0_MEMORY_INFO_HAS_BYTE_LANE 0
#define ONCHIP_MEMORY2_0_MEMORY_INFO_HEX_INSTALL_DIR QPF_DIR
#define ONCHIP_MEMORY2_0_MEMORY_INFO_MEM_INIT_DATA_WIDTH 64
#define ONCHIP_MEMORY2_0_MEMORY_INFO_MEM_INIT_FILENAME soc_system_onchip_memory2_0

/*
 * Macros for device 'pio_led', class 'altera_avalon_pio'
 * The macros are prefixed with 'PIO_LED_'.
 * The prefix is the slave descriptor.
 */
#define PIO_LED_COMPONENT_TYPE altera_avalon_pio
#define PIO_LED_COMPONENT_NAME pio_led
#define PIO_LED_BASE 0x0
#define PIO_LED_SPAN 32
#define PIO_LED_END 0x1f
#define PIO_LED_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_LED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_LED_CAPTURE 0
#define PIO_LED_DATA_WIDTH 10
#define PIO_LED_DO_TEST_BENCH_WIRING 0
#define PIO_LED_DRIVEN_SIM_VALUE 0
#define PIO_LED_EDGE_TYPE NONE
#define PIO_LED_FREQ 50000000
#define PIO_LED_HAS_IN 0
#define PIO_LED_HAS_OUT 1
#define PIO_LED_HAS_TRI 0
#define PIO_LED_IRQ_TYPE NONE
#define PIO_LED_RESET_VALUE 1023

/*
 * Macros for device 'pio_sw', class 'altera_avalon_pio'
 * The macros are prefixed with 'PIO_SW_'.
 * The prefix is the slave descriptor.
 */
#define PIO_SW_COMPONENT_TYPE altera_avalon_pio
#define PIO_SW_COMPONENT_NAME pio_sw
#define PIO_SW_BASE 0x10
#define PIO_SW_SPAN 32
#define PIO_SW_END 0x2f
#define PIO_SW_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_SW_CAPTURE 0
#define PIO_SW_DATA_WIDTH 10
#define PIO_SW_DO_TEST_BENCH_WIRING 0
#define PIO_SW_DRIVEN_SIM_VALUE 0
#define PIO_SW_EDGE_TYPE NONE
#define PIO_SW_FREQ 50000000
#define PIO_SW_HAS_IN 1
#define PIO_SW_HAS_OUT 0
#define PIO_SW_HAS_TRI 0
#define PIO_SW_IRQ_TYPE NONE
#define PIO_SW_RESET_VALUE 0

/*
 * Macros for device 'hex_mux', class 'hex_mux'
 * The macros are prefixed with 'HEX_MUX_'.
 * The prefix is the slave descriptor.
 */
#define HEX_MUX_COMPONENT_TYPE hex_mux
#define HEX_MUX_COMPONENT_NAME hex_mux
#define HEX_MUX_BASE 0x20
#define HEX_MUX_SPAN 4
#define HEX_MUX_END 0x23

/*
 * Macros for device 'ana_opt_logic_ctrl', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_CTRL_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_CTRL_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_CTRL_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_CTRL_BASE 0x30
#define ANA_OPT_LOGIC_CTRL_SPAN 4
#define ANA_OPT_LOGIC_CTRL_END 0x33

/*
 * Macros for device 'ana_opt_logic_stat', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_STAT_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_STAT_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_STAT_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_STAT_BASE 0x34
#define ANA_OPT_LOGIC_STAT_SPAN 4
#define ANA_OPT_LOGIC_STAT_END 0x37

/*
 * Macros for device 'ana_opt_logic_cntsclk', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_CNTSCLK_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_CNTSCLK_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_CNTSCLK_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_CNTSCLK_BASE 0x38
#define ANA_OPT_LOGIC_CNTSCLK_SPAN 4
#define ANA_OPT_LOGIC_CNTSCLK_END 0x3b

/*
 * Macros for device 'ana_opt_logic_cntclrn', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_CNTCLRN_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_CNTCLRN_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_CNTCLRN_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_CNTCLRN_BASE 0x3c
#define ANA_OPT_LOGIC_CNTCLRN_SPAN 4
#define ANA_OPT_LOGIC_CNTCLRN_END 0x3f

/*
 * Macros for device 'ana_opt_logic_cntsmpl', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_CNTSMPL_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_CNTSMPL_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_CNTSMPL_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_CNTSMPL_BASE 0x40
#define ANA_OPT_LOGIC_CNTSMPL_SPAN 4
#define ANA_OPT_LOGIC_CNTSMPL_END 0x43

/*
 * Macros for device 'ana_opt_logic_cntckad', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_CNTCKAD_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_CNTCKAD_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_CNTCKAD_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_CNTCKAD_BASE 0x44
#define ANA_OPT_LOGIC_CNTCKAD_SPAN 4
#define ANA_OPT_LOGIC_CNTCKAD_END 0x47

/*
 * Macros for device 'ana_opt_logic_cntradc', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_CNTRADC_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_CNTRADC_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_CNTRADC_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_CNTRADC_BASE 0x48
#define ANA_OPT_LOGIC_CNTRADC_SPAN 4
#define ANA_OPT_LOGIC_CNTRADC_END 0x4b

/*
 * Macros for device 'ana_opt_logic_cntconv', class 'ana_opt_logic'
 * The macros are prefixed with 'ANA_OPT_LOGIC_CNTCONV_'.
 * The prefix is the slave descriptor.
 */
#define ANA_OPT_LOGIC_CNTCONV_COMPONENT_TYPE ana_opt_logic
#define ANA_OPT_LOGIC_CNTCONV_COMPONENT_NAME ana_opt_logic
#define ANA_OPT_LOGIC_CNTCONV_BASE 0x4c
#define ANA_OPT_LOGIC_CNTCONV_SPAN 4
#define ANA_OPT_LOGIC_CNTCONV_END 0x4f

/*
 * Macros for device 'chip_test_logic_ctrl', class 'chip_test_logic'
 * The macros are prefixed with 'CHIP_TEST_LOGIC_CTRL_'.
 * The prefix is the slave descriptor.
 */
#define CHIP_TEST_LOGIC_CTRL_COMPONENT_TYPE chip_test_logic
#define CHIP_TEST_LOGIC_CTRL_COMPONENT_NAME chip_test_logic
#define CHIP_TEST_LOGIC_CTRL_BASE 0x50
#define CHIP_TEST_LOGIC_CTRL_SPAN 4
#define CHIP_TEST_LOGIC_CTRL_END 0x53

/*
 * Macros for device 'chip_test_logic_stat', class 'chip_test_logic'
 * The macros are prefixed with 'CHIP_TEST_LOGIC_STAT_'.
 * The prefix is the slave descriptor.
 */
#define CHIP_TEST_LOGIC_STAT_COMPONENT_TYPE chip_test_logic
#define CHIP_TEST_LOGIC_STAT_COMPONENT_NAME chip_test_logic
#define CHIP_TEST_LOGIC_STAT_BASE 0x54
#define CHIP_TEST_LOGIC_STAT_SPAN 4
#define CHIP_TEST_LOGIC_STAT_END 0x57

/*
 * Macros for device 'chip_test_logic_cntsclk_sc', class 'chip_test_logic'
 * The macros are prefixed with 'CHIP_TEST_LOGIC_CNTSCLK_SC_'.
 * The prefix is the slave descriptor.
 */
#define CHIP_TEST_LOGIC_CNTSCLK_SC_COMPONENT_TYPE chip_test_logic
#define CHIP_TEST_LOGIC_CNTSCLK_SC_COMPONENT_NAME chip_test_logic
#define CHIP_TEST_LOGIC_CNTSCLK_SC_BASE 0x58
#define CHIP_TEST_LOGIC_CNTSCLK_SC_SPAN 4
#define CHIP_TEST_LOGIC_CNTSCLK_SC_END 0x5b

/*
 * Macros for device 'cfsa_4D_logic_sactrl', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_SACTRL_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_SACTRL_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_SACTRL_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_SACTRL_BASE 0xec
#define CFSA_4D_LOGIC_SACTRL_SPAN 4
#define CFSA_4D_LOGIC_SACTRL_END 0xef

/*
 * Macros for device 'spi_int_0', class 'altera_avalon_spi'
 * The macros are prefixed with 'SPI_INT_0_'.
 * The prefix is the slave descriptor.
 */
#define SPI_INT_0_COMPONENT_TYPE altera_avalon_spi
#define SPI_INT_0_COMPONENT_NAME spi_int_0
#define SPI_INT_0_BASE 0x100
#define SPI_INT_0_SPAN 64
#define SPI_INT_0_END 0x13f
#define SPI_INT_0_IRQ 3
#define SPI_INT_0_CLOCKMULT 1
#define SPI_INT_0_CLOCKPHASE 0
#define SPI_INT_0_CLOCKPOLARITY 0
#define SPI_INT_0_CLOCKUNITS "Hz"
#define SPI_INT_0_DATABITS 16
#define SPI_INT_0_DATAWIDTH 16
#define SPI_INT_0_DELAYMULT "1.0E-9"
#define SPI_INT_0_DELAYUNITS "ns"
#define SPI_INT_0_EXTRADELAY 0
#define SPI_INT_0_INSERT_SYNC 0
#define SPI_INT_0_ISMASTER 1
#define SPI_INT_0_LSBFIRST 0
#define SPI_INT_0_NUMSLAVES 4
#define SPI_INT_0_PREFIX "spi_"
#define SPI_INT_0_SYNC_REG_DEPTH 2
#define SPI_INT_0_TARGETCLOCK 500000
#define SPI_INT_0_TARGETSSDELAY "0.0"

/*
 * Macros for device 'cfsa_4D_logic_sastat', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_SASTAT_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_SASTAT_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_SASTAT_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_SASTAT_BASE 0x120
#define CFSA_4D_LOGIC_SASTAT_SPAN 4
#define CFSA_4D_LOGIC_SASTAT_END 0x123

/*
 * Macros for device 'cfsa_4D_logic_initx1', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_INITX1_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_INITX1_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX1_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX1_BASE 0x124
#define CFSA_4D_LOGIC_INITX1_SPAN 4
#define CFSA_4D_LOGIC_INITX1_END 0x127

/*
 * Macros for device 'cfsa_4D_logic_initx2', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_INITX2_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_INITX2_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX2_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX2_BASE 0x128
#define CFSA_4D_LOGIC_INITX2_SPAN 4
#define CFSA_4D_LOGIC_INITX2_END 0x12b

/*
 * Macros for device 'cfsa_4D_logic_initx3', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_INITX3_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_INITX3_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX3_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX3_BASE 0x12c
#define CFSA_4D_LOGIC_INITX3_SPAN 4
#define CFSA_4D_LOGIC_INITX3_END 0x12f

/*
 * Macros for device 'cfsa_4D_logic_initx4', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_INITX4_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_INITX4_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX4_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_INITX4_BASE 0x130
#define CFSA_4D_LOGIC_INITX4_SPAN 4
#define CFSA_4D_LOGIC_INITX4_END 0x133

/*
 * Macros for device 'cfsa_4D_logic_iternum', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_ITERNUM_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_ITERNUM_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_ITERNUM_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_ITERNUM_BASE 0x134
#define CFSA_4D_LOGIC_ITERNUM_SPAN 4
#define CFSA_4D_LOGIC_ITERNUM_END 0x137

/*
 * Macros for device 'cfsa_4D_logic_iterss', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_ITERSS_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_ITERSS_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_ITERSS_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_ITERSS_BASE 0x138
#define CFSA_4D_LOGIC_ITERSS_SPAN 4
#define CFSA_4D_LOGIC_ITERSS_END 0x13b

/*
 * Macros for device 'cfsa_4D_logic_initt', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_INITT_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_INITT_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_INITT_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_INITT_BASE 0x13c
#define CFSA_4D_LOGIC_INITT_SPAN 4
#define CFSA_4D_LOGIC_INITT_END 0x13f

/*
 * Macros for device 'cfsa_4D_logic_tol', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_TOL_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_TOL_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_TOL_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_TOL_BASE 0x140
#define CFSA_4D_LOGIC_TOL_SPAN 4
#define CFSA_4D_LOGIC_TOL_END 0x143

/*
 * Macros for device 'cfsa_4D_logic_aoff', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_AOFF_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_AOFF_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_AOFF_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_AOFF_BASE 0x144
#define CFSA_4D_LOGIC_AOFF_SPAN 4
#define CFSA_4D_LOGIC_AOFF_END 0x147

/*
 * Macros for device 'cfsa_4D_logic_adc', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_ADC_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_ADC_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_ADC_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_ADC_BASE 0x148
#define CFSA_4D_LOGIC_ADC_SPAN 4
#define CFSA_4D_LOGIC_ADC_END 0x14b

/*
 * Macros for device 'cfsa_4D_logic_tunex1', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_TUNEX1_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_TUNEX1_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX1_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX1_BASE 0x14c
#define CFSA_4D_LOGIC_TUNEX1_SPAN 4
#define CFSA_4D_LOGIC_TUNEX1_END 0x14f

/*
 * Macros for device 'cfsa_4D_logic_tunex2', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_TUNEX2_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_TUNEX2_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX2_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX2_BASE 0x150
#define CFSA_4D_LOGIC_TUNEX2_SPAN 4
#define CFSA_4D_LOGIC_TUNEX2_END 0x153

/*
 * Macros for device 'cfsa_4D_logic_tunex3', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_TUNEX3_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_TUNEX3_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX3_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX3_BASE 0x154
#define CFSA_4D_LOGIC_TUNEX3_SPAN 4
#define CFSA_4D_LOGIC_TUNEX3_END 0x157

/*
 * Macros for device 'cfsa_4D_logic_tunex4', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_TUNEX4_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_TUNEX4_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX4_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_TUNEX4_BASE 0x158
#define CFSA_4D_LOGIC_TUNEX4_SPAN 4
#define CFSA_4D_LOGIC_TUNEX4_END 0x15b

/*
 * Macros for device 'cfsa_4D_logic_ss_threshold', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_SS_THRESHOLD_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_SS_THRESHOLD_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_SS_THRESHOLD_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_SS_THRESHOLD_BASE 0x160
#define CFSA_4D_LOGIC_SS_THRESHOLD_SPAN 4
#define CFSA_4D_LOGIC_SS_THRESHOLD_END 0x163

/*
 * Macros for device 'cfsa_4D_logic_ss_3dbfactor0', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_SS_3DBFACTOR0_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_SS_3DBFACTOR0_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_SS_3DBFACTOR0_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_SS_3DBFACTOR0_BASE 0x164
#define CFSA_4D_LOGIC_SS_3DBFACTOR0_SPAN 4
#define CFSA_4D_LOGIC_SS_3DBFACTOR0_END 0x167

/*
 * Macros for device 'cfsa_4D_logic_ss_3dbfactor1', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_SS_3DBFACTOR1_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_SS_3DBFACTOR1_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_SS_3DBFACTOR1_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_SS_3DBFACTOR1_BASE 0x168
#define CFSA_4D_LOGIC_SS_3DBFACTOR1_SPAN 4
#define CFSA_4D_LOGIC_SS_3DBFACTOR1_END 0x16b

/*
 * Macros for device 'sweep_logic_ctrl', class 'sweep_logic'
 * The macros are prefixed with 'SWEEP_LOGIC_CTRL_'.
 * The prefix is the slave descriptor.
 */
#define SWEEP_LOGIC_CTRL_COMPONENT_TYPE sweep_logic
#define SWEEP_LOGIC_CTRL_COMPONENT_NAME sweep_logic
#define SWEEP_LOGIC_CTRL_BASE 0x16c
#define SWEEP_LOGIC_CTRL_SPAN 4
#define SWEEP_LOGIC_CTRL_END 0x16f

/*
 * Macros for device 'sweep_logic_stat', class 'sweep_logic'
 * The macros are prefixed with 'SWEEP_LOGIC_STAT_'.
 * The prefix is the slave descriptor.
 */
#define SWEEP_LOGIC_STAT_COMPONENT_TYPE sweep_logic
#define SWEEP_LOGIC_STAT_COMPONENT_NAME sweep_logic
#define SWEEP_LOGIC_STAT_BASE 0x170
#define SWEEP_LOGIC_STAT_SPAN 4
#define SWEEP_LOGIC_STAT_END 0x173

/*
 * Macros for device 'sweep_logic_cntsclk', class 'sweep_logic'
 * The macros are prefixed with 'SWEEP_LOGIC_CNTSCLK_'.
 * The prefix is the slave descriptor.
 */
#define SWEEP_LOGIC_CNTSCLK_COMPONENT_TYPE sweep_logic
#define SWEEP_LOGIC_CNTSCLK_COMPONENT_NAME sweep_logic
#define SWEEP_LOGIC_CNTSCLK_BASE 0x174
#define SWEEP_LOGIC_CNTSCLK_SPAN 4
#define SWEEP_LOGIC_CNTSCLK_END 0x177

/*
 * Macros for device 'sweep_logic_low', class 'sweep_logic'
 * The macros are prefixed with 'SWEEP_LOGIC_LOW_'.
 * The prefix is the slave descriptor.
 */
#define SWEEP_LOGIC_LOW_COMPONENT_TYPE sweep_logic
#define SWEEP_LOGIC_LOW_COMPONENT_NAME sweep_logic
#define SWEEP_LOGIC_LOW_BASE 0x178
#define SWEEP_LOGIC_LOW_SPAN 4
#define SWEEP_LOGIC_LOW_END 0x17b

/*
 * Macros for device 'sweep_logic_high', class 'sweep_logic'
 * The macros are prefixed with 'SWEEP_LOGIC_HIGH_'.
 * The prefix is the slave descriptor.
 */
#define SWEEP_LOGIC_HIGH_COMPONENT_TYPE sweep_logic
#define SWEEP_LOGIC_HIGH_COMPONENT_NAME sweep_logic
#define SWEEP_LOGIC_HIGH_BASE 0x17c
#define SWEEP_LOGIC_HIGH_SPAN 4
#define SWEEP_LOGIC_HIGH_END 0x17f

/*
 * Macros for device 'cfsa_4D_logic_ana_bits_in', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_ANA_BITS_IN_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_ANA_BITS_IN_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_ANA_BITS_IN_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_ANA_BITS_IN_BASE 0x180
#define CFSA_4D_LOGIC_ANA_BITS_IN_SPAN 4
#define CFSA_4D_LOGIC_ANA_BITS_IN_END 0x183

/*
 * Macros for device 'cfsa_4D_logic_ana_bits', class 'cfsa_4D_logic'
 * The macros are prefixed with 'CFSA_4D_LOGIC_ANA_BITS_'.
 * The prefix is the slave descriptor.
 */
#define CFSA_4D_LOGIC_ANA_BITS_COMPONENT_TYPE cfsa_4D_logic
#define CFSA_4D_LOGIC_ANA_BITS_COMPONENT_NAME cfsa_4D_logic
#define CFSA_4D_LOGIC_ANA_BITS_BASE 0x184
#define CFSA_4D_LOGIC_ANA_BITS_SPAN 4
#define CFSA_4D_LOGIC_ANA_BITS_END 0x187

/*
 * Macros for device 'pll_tune_0_cntr', class 'pll_tune'
 * The macros are prefixed with 'PLL_TUNE_0_CNTR_'.
 * The prefix is the slave descriptor.
 */
#define PLL_TUNE_0_CNTR_COMPONENT_TYPE pll_tune
#define PLL_TUNE_0_CNTR_COMPONENT_NAME pll_tune_0
#define PLL_TUNE_0_CNTR_BASE 0x190
#define PLL_TUNE_0_CNTR_SPAN 4
#define PLL_TUNE_0_CNTR_END 0x193

/*
 * Macros for device 'pll_tune_0_cntf', class 'pll_tune'
 * The macros are prefixed with 'PLL_TUNE_0_CNTF_'.
 * The prefix is the slave descriptor.
 */
#define PLL_TUNE_0_CNTF_COMPONENT_TYPE pll_tune
#define PLL_TUNE_0_CNTF_COMPONENT_NAME pll_tune_0
#define PLL_TUNE_0_CNTF_BASE 0x194
#define PLL_TUNE_0_CNTF_SPAN 4
#define PLL_TUNE_0_CNTF_END 0x197

/*
 * Macros for device 'pll_tune_0_ctrl', class 'pll_tune'
 * The macros are prefixed with 'PLL_TUNE_0_CTRL_'.
 * The prefix is the slave descriptor.
 */
#define PLL_TUNE_0_CTRL_COMPONENT_TYPE pll_tune
#define PLL_TUNE_0_CTRL_COMPONENT_NAME pll_tune_0
#define PLL_TUNE_0_CTRL_BASE 0x198
#define PLL_TUNE_0_CTRL_SPAN 4
#define PLL_TUNE_0_CTRL_END 0x19b

/*
 * Macros for device 'sysid_qsys', class 'altera_avalon_sysid_qsys'
 * The macros are prefixed with 'SYSID_QSYS_'.
 * The prefix is the slave descriptor.
 */
#define SYSID_QSYS_COMPONENT_TYPE altera_avalon_sysid_qsys
#define SYSID_QSYS_COMPONENT_NAME sysid_qsys
#define SYSID_QSYS_BASE 0x10000
#define SYSID_QSYS_SPAN 8
#define SYSID_QSYS_END 0x10007
#define SYSID_QSYS_ID 2899645186
#define SYSID_QSYS_TIMESTAMP 1447973899

/*
 * Macros for device 'jtag_uart', class 'altera_avalon_jtag_uart'
 * The macros are prefixed with 'JTAG_UART_'.
 * The prefix is the slave descriptor.
 */
#define JTAG_UART_COMPONENT_TYPE altera_avalon_jtag_uart
#define JTAG_UART_COMPONENT_NAME jtag_uart
#define JTAG_UART_BASE 0x20000
#define JTAG_UART_SPAN 16
#define JTAG_UART_END 0x2000f
#define JTAG_UART_IRQ 2
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


#endif /* _ALTERA_HPS_0_H_ */
