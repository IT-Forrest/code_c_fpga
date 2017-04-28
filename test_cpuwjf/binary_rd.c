#include <stdio.h>
#include <stdlib.h>
int main(void) {
    char *data = "00001000";
    char c = strtol(data, 0, 2);
    printf("%s = %c = %d = 0x%.2X\n", data, c, c, c);

    FILE *fd;
    fd = fopen("binary_data.bin", "r");
    int rd_sz = -1;
    char line[10];
    unsigned int val;
    while (fgets(line, sizeof(line), fd)) {
	// since fgets doesn't strip the \n, thus a longer string is needed
	printf("Retrive str = %s", line);
	//printf("LSB=%d, MSB=%d\n", line[0], line[7]);
	val = strtol(line, 0, 2);
	printf("value = %d\n", val);
    }
    fclose(fd);

    return (0);
}
