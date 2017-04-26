#include "psoc_port.h"

uint8_t      hex_code[16] =     // Hex code talbe
             {  0x40,   0x79,   0x24,   0x30,
                0x19,   0x12,   0x02,   0x78,
                0x00,   0x18,   0x08,   0x03,
                0x46,   0x21,   0x06,   0x0e    };

//// Private ////
int          fd;                 // File handle
void         *virtual_base;

uint32_t     *spi_ctrl_addr;     // SPI master control address
uint32_t     *spi_ctrl_txdata;
uint32_t     *spi_ctrl_status;
uint32_t     *spi_ctrl_control;
uint32_t     *spi_ctrl_ss;

uint32_t     *hex_mux_addr;     // HEX display controller

//// Public ////

uint32_t     *ctrl_addr;         // Control register
uint32_t     *stat_addr;         // Status register
uint32_t     *reg_cntclk_addr;   // SCLK generation delay counter
uint32_t     local_ctrl_addr;    // the local variable for Control register

uint32_t     *reg_cntclrn_addr;  // Counter for CLRN delay
uint32_t     *reg_cntsmpl_addr;  // Counter for pre-ADC-Sampling delay
uint32_t     *reg_cntckad_addr;  // Counter for CLK_ADC delay
uint32_t     *reg_cntradc_addr;  // Counter for RSTN_ADC delay
uint32_t     *reg_cntconv_addr;  // Counter for ADC conversion delay

// the AMM I/O of sa_test_logic module
uint32_t     local_sactrl_addr;
uint32_t     *sactrl_addr;       // SA control register
uint32_t     *sastat_addr;       // SA status register (read only)
uint32_t     *initx1_addr;       // Initia1 X1 register
uint32_t     *initx2_addr;       // Initia1 X2 register
uint32_t     *initx3_addr;       // Initia1 X3 register
uint32_t     *initx4_addr;       // Initia1 X4 register

uint32_t     *iternum_addr;      // Initial iteration number
uint32_t     *anain_addr;        // Ana bits value
uint32_t     *tunex1_addr;        // Tune X1 register
uint32_t     *tunex2_addr;        // Tune X2 register
uint32_t     *tunex3_addr;        // Tune X3 register
uint32_t     *tunex4_addr;        // Tune X4 register

// the AMM I/O of sa_4D_logic module
uint32_t     local_sa4Dctrl_addr;
uint32_t     *sa4Dctrl_addr;       // SA control register
uint32_t     *sa4Dstat_addr;       // SA status register (read only)
uint32_t     *init4Dx1_addr;       // Initia1 X1 register
uint32_t     *init4Dx2_addr;       // Initia1 X2 register
uint32_t     *init4Dx3_addr;       // Initia1 X3 register
uint32_t     *init4Dx4_addr;       // Initia1 X4 register

uint32_t     *iter4Dsa_addr;      // Initial iteration number for SA
uint32_t     *iter4Dss_addr;      // Initial iteration number for SS
uint32_t     *initT4D_addr;       // Initial temparature
uint32_t     *anain4D_addr;       // Ana bits value
uint32_t     *tune4Dx1_addr;      // Tune X1 register
uint32_t     *tune4Dx2_addr;      // Tune X2 register
uint32_t     *tune4Dx3_addr;      // Tune X3 register
uint32_t     *tune4Dx4_addr;      // Tune X4 register

// the AMM I/O of CFSA_4D_logic module
uint32_t     local_cfsa4Dctrl_addr;
uint32_t     *cfsa4Dctrl_addr;     // CFSA control registers
uint32_t     *cfsa4Dstat_addr;
uint32_t     *cfsainit4Dx1_addr;       // Initia1 X1 register
uint32_t     *cfsainit4Dx2_addr;       // Initia1 X2 register
uint32_t     *cfsainit4Dx3_addr;       // Initia1 X3 register
uint32_t     *cfsainit4Dx4_addr;       // Initia1 X4 register

uint32_t     *cfsaiter4Dsa_addr;      // Initial iteration number for SA
uint32_t     *cfsaiter4Dss_addr;      // Initial iteration number for SS
uint32_t     *cfsainitT4D_addr;       // Initial temparature
uint32_t     *cfsatolin4D_addr;       // TOL bits value
uint32_t     *cfsaoffin4D_addr;       // Offset bits value
uint32_t     *cfsa3dbfactor0_addr;    // 3DB factor0
uint32_t     *cfsa3dbfactor1_addr;    // 3DB factor1
uint32_t     *cfsaadcin4D_addr;       // ADC bits value
uint32_t     *cfsathreshold_addr;     // threshold for multi-start SS

uint32_t     *cfsatune4Dx1_addr;      // Tune X1 register
uint32_t     *cfsatune4Dx2_addr;      // Tune X2 register
uint32_t     *cfsatune4Dx3_addr;      // Tune X3 register
uint32_t     *cfsatune4Dx4_addr;      // Tune X4 register

// sweep bits module
uint32_t     local_sweepctrl_addr;
uint32_t     *sweepctrl_addr;
uint32_t     *sweepcntsclk_addr;
uint32_t     *sweepstat_addr;
uint32_t     *sweep_lowdata_addr;
uint32_t     *sweep_highdata_addr;

// scpu logic module
uint32_t     *chip4_cpuctrl_wrt_addr;
uint32_t     *chip4_cpustat_rd_addr;
uint32_t     *chip4_cntsclk_wrt_addr;
uint32_t     *chip4_sram_addr_wrt_addr;
uint32_t     *chip4_sram_data_wrt_addr;
uint32_t     *chip4_adc_wrt_addr;
uint32_t     *chip4_sram_addr_rd_addr;
uint32_t     *chip4_sram_data_rd_addr;
uint32_t     *chip4_scan_chain_rd_addr;

// Initialize memory mapping
int init_mem()
{
    /* Initialize the memory addresses */
    if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
        printf( "ERROR: could not open \"/dev/mem\"...\n" );
        return( 1 );
    }

    virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

    if( virtual_base == MAP_FAILED ) {
        printf( "ERROR: mmap() failed...\n" );
        close( fd );
        return( 1 );
    }

    hex_mux_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + HEX_MUX_BASE ) & ( unsigned long)( HW_REGS_MASK ) );

    // ctrl_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_CTRL_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // stat_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_STAT_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // reg_cntclk_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_CNTSCLK_BASE ) & ( unsigned long)( HW_REGS_MASK ) );

    // *reg_cntclk_addr = 4;

    // reg_cntclrn_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_CNTCLRN_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // reg_cntsmpl_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_CNTSMPL_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // reg_cntckad_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_CNTCKAD_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // reg_cntradc_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_CNTRADC_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // reg_cntconv_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ANA_OPT_LOGIC_CNTCONV_BASE ) & ( unsigned long)( HW_REGS_MASK ) );

    ///// spi interface ///////////
    // spi_ctrl_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + SPI_INT_0_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // spi_ctrl_txdata = spi_ctrl_addr + 1;
    // spi_ctrl_status = spi_ctrl_addr + 2;
    // spi_ctrl_control = spi_ctrl_addr + 3;
    // spi_ctrl_ss = spi_ctrl_addr + 5;

    ///// pll calibration //////////
    // pll_tune_ctrl_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PLL_TUNE_0_CTRL_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // pll_tune_cntr_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PLL_TUNE_0_CNTR_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
    // pll_tune_cntf_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PLL_TUNE_0_CNTF_BASE ) & ( unsigned long)( HW_REGS_MASK ) );

    //// the AMM I/O of sa_test_logic module
    // cfsa4Dctrl_addr   = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_SACTRL_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsa4Dstat_addr   = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_SASTAT_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsainit4Dx1_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_INITX1_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsainit4Dx2_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_INITX2_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsainit4Dx3_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_INITX3_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsainit4Dx4_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_INITX4_BASE) & (unsigned long) (HW_REGS_MASK));

    // cfsaiter4Dsa_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_ITERNUM_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsaiter4Dss_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_ITERSS_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsainitT4D_addr  = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_INITT_BASE)  & (unsigned long) (HW_REGS_MASK));
    // cfsatolin4D_addr  = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_TOL_BASE)    & (unsigned long) (HW_REGS_MASK));
    // cfsaoffin4D_addr  = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_AOFF_BASE)   & (unsigned long) (HW_REGS_MASK));
    // cfsa3dbfactor0_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_SS_3DBFACTOR0_BASE)   & (unsigned long) (HW_REGS_MASK));
    // cfsa3dbfactor1_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_SS_3DBFACTOR1_BASE)   & (unsigned long) (HW_REGS_MASK));
    // cfsaadcin4D_addr  = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_ADC_BASE)    & (unsigned long) (HW_REGS_MASK));
    // cfsathreshold_addr= virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_SS_THRESHOLD_BASE)    & (unsigned long) (HW_REGS_MASK));

    // cfsatune4Dx1_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_TUNEX1_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsatune4Dx2_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_TUNEX2_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsatune4Dx3_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_TUNEX3_BASE) & (unsigned long) (HW_REGS_MASK));
    // cfsatune4Dx4_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + CFSA_4D_LOGIC_TUNEX4_BASE) & (unsigned long) (HW_REGS_MASK));

    /////// tapeout chip 3 //////
    sweepctrl_addr    = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + SWEEP_LOGIC_CTRL_BASE)    & (unsigned long) (HW_REGS_MASK));
    sweepcntsclk_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + SWEEP_LOGIC_CNTSCLK_BASE) & (unsigned long) (HW_REGS_MASK));
    sweepstat_addr    = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + SWEEP_LOGIC_STAT_BASE)    & (unsigned long) (HW_REGS_MASK));
    sweep_lowdata_addr= virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + SWEEP_LOGIC_LOW_BASE)     & (unsigned long) (HW_REGS_MASK));
    sweep_highdata_addr=virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + SWEEP_LOGIC_HIGH_BASE)    & (unsigned long) (HW_REGS_MASK));

    /////// tapeout chip 4 //////
    chip4_cpuctrl_wrt_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_CPUCTRL_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_cpustat_rd_addr  = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_CPUSTAT_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_cntsclk_wrt_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_CNTSCLK_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_sram_addr_wrt_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_SRAM_ADDR_WRT_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_sram_data_wrt_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_SRAM_DATA_WRT_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_adc_wrt_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_ADC_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_sram_addr_rd_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_SRAM_ADDR_RD_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_sram_data_rd_addr = virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_SRAM_DATA_RD_BASE) & (unsigned long) (HW_REGS_MASK));
    chip4_scan_chain_rd_addr= virtual_base + ((unsigned long) (ALT_LWFPGASLVS_OFST + FPGA_CHIP_LOGIC_SCAN_CHAIN_BASE) & (unsigned long) (HW_REGS_MASK));

    return(0);
}

void avs_wait()
{
    /* HPS frequency is 925 MHz, Avalon interconnection clock frequency is 50 MHz.
        CPU should hold data for at least 18.5 clock cycles to guarantee the Avalon slave get the date. */
    int i;
    for (i=0; i<20; i++);
}

// Clean up memory mapping
int clean_mem()
{
    if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
        printf( "ERROR: munmap() failed...\n" );
        close( fd );
        return( 1 );
    }
    close( fd );
    return( 0 );
}

// Initia1 the local ctrl_addr to remember the status in *ctrl_addr
int syn_ctrl()
{
    // local_ctrl_addr = 0;
    // *ctrl_addr = local_ctrl_addr;

    // local_sactrl_addr = 0;
    // *sactrl_addr = local_sactrl_addr;

    // local_sa4Dctrl_addr = 0;
    // *sa4Dctrl_addr = local_sa4Dctrl_addr;

    // local_cfsa4Dctrl_addr = 0;
    // *cfsa4Dctrl_addr = local_cfsa4Dctrl_addr;

    local_sweepctrl_addr = 0;
    *sweepctrl_addr = local_sweepctrl_addr;

    avs_wait();
    return (0);
}

// Set resistance value for Microchip 4132/4152 digital POT
int spi_set_res(uint8_t val)
{
    uint32_t    tx_rdy_flag = 1 << 6;

    if (!((*spi_ctrl_status) & tx_rdy_flag))
        return 1;
    else
    {
        *spi_ctrl_txdata = (uint32_t) val;
        while (!((*spi_ctrl_status) & tx_rdy_flag));   // Wait till ready
    }

    return 0;
}

int spi_set_res_ss(uint8_t val, uint8_t dev)
{
    *spi_ctrl_ss = (0x1<<dev);
    avs_wait();

    spi_set_res(val);

    return 0;
}

// Set hex number
int hex_set_val(uint8_t pos, uint8_t val)
{
    if (pos>5) return 1;

    *hex_mux_addr = (((uint16_t)pos)<<7) + hex_code[val];
    avs_wait();

    return 0;
}
