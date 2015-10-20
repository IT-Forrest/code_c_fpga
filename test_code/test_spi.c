#include <stdlib.h>

#include "../ana_opt_2/device.h"
#include "../ana_opt_2/psoc_port.h"
#include "../ana_opt_2/set_config.h"

int main(int argc, char** argv)
{
    if (init_mem()) return (1);
    if (init_cfg()) return (1);
    if (syn_ctrl()) return (1);

    Chip3_Idx_Ctrl_Sel_A_Write(0);
    init_sc();
    Chip3_Set_Sw(1);

    spi_set_res_ss(atoi(argv[1]),1);
    //spi_set_res_ss(127,0);
    //spi_set_res_ss(55,1);
    return 0;
}
