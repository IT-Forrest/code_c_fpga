#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "../ana_opt_2/psoc_port.h"

int main(int argc, char **argv)
{
    unsigned int i;
    unsigned int v1, v2, v3, v4;
    unsigned int pos;

    if (init_mem()) return (1);

    if (argc <= 1)
    {
        for (i=0; i<6; i++) hex_set_val(i,0x0e);
        return 1;
    } else
    {
        sscanf(argv[1], "%u.%u.%u.%u", &v1, &v2, &v3, &v4);

        pos = 0;
        for (i=0; i<3; i++)
        {
            hex_set_val(pos, v4 % 10);
            v4 /= 10;
            pos ++;
        }
        for (i=0; i<3; i++)
        {
            hex_set_val(pos, v3 % 10);
            v3 /= 10;
            pos ++;
        }

        return 0;
    }
}
