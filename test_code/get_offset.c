#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/device.h"
#include "../ana_opt_2/scan_chain.h"
#include "../ana_opt_2/set_config.h"
#include "../ana_opt_2/serial_port_io.h"
#include "../ana_opt_2/test_adc.h"


int main() {

    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    printf("Stimulus Frequency is set to 36MHz. \n");
    printf("Offset: %d\n", GetADCOffset(32));

    return( clean_mem() );
}
