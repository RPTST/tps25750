#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "tps_i2c.h"
#include <unistd.h>
#include "tps_defs.h"
#include <string.h>

void tps_print_help () {
        printf("\n tps [-r|read] [regName]");
        printf("\n tps [-w|write] [regName]");
	printf("\n tps [-u|update-firmware] [FileName]");
	printf("\n tps [-h|help]\n");
}


int tps_write(int file, uint8_t addr) {
        int ret;

        ret = i2c_smbus_write_byte(file, addr);
        return ret;
}


char* tps_read(int file, uint8_t addr,  uint8_t count) {

	char *buf;
	int i = 0;
	int ret = 0;

	ret = tps_write(file, addr);
	if (ret < 0 ) {
		printf("Could not read addr 0x%x", addr);
		return NULL;
	}
	buf = (char *) malloc (sizeof(char) * count);

	while (count != 0 ) {
		*(buf + i) = i2c_smbus_read_byte(file);
		count--;
		i++;
	}
	return buf;
}

char* tps_read_type_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_TYPE_REG,  TPS_TYPE_REG_SIZE);
	if (buf == NULL) {
		printf("Could not read TYPE reg\n");
	} else {
        	for (i = 0; i < TPS_TYPE_REG_SIZE; i++) {
                	printf("%c" , buf[i]);
        	}
        	printf("\n");   
	}

        return buf;
}

char* tps_read_mode_reg(int file) {
	char *buf;
	int i;

	buf = tps_read(file, TPS_MODE_REG, TPS_MODE_REG_SIZE);
	if(buf == NULL) {
		printf("Could not read mode reg\n");
	} else {
		for (i = 0; i < TPS_MODE_REG_SIZE; i++) {
                	printf("%c" , buf[i]);
        	}
		printf("\n");	
	}

	return buf;
}


char* tps_read_power_status_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_POWER_STATUS_REG, TPS_POWER_STATUS_REG_SIZE);
	if (buf == NULL) {
		printf("Could not read power status\n");
 	} else {
        	for (i = 0; i < TPS_POWER_STATUS_REG_SIZE; i++) {
                	printf("0x%x " , buf[i]);
        	}
        	printf("\n");
	} 

	return buf;
}

char* tps_read_status_reg(int file) {
	char *buf;
        int i;

        buf = tps_read(file, TPS_STATUS_REG, TPS_POWER_STATUS_REG_SIZE);
	if (buf == NULL) {
		printf("Could not read STATUS reg\n");
	} else {
        	for (i = 0; i < TPS_STATUS_REG_SIZE; i++) {
                	printf("0x%x " , buf[i]);
        	}
        	printf("\n");
	}        

	return buf;
}

char* tps_read_cap_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_DEVICE_CAP_REG, TPS_DEVICE_CAP_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read STATUS reg\n");
        } else {
                for (i = 0; i < TPS_DEVICE_CAP_REG_SIZE; i++) {
                        printf("0x%x " , buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char * tps_read_cmd_reg(int file) {
	char *buf;
	int i;

	buf = tps_read(file, TPS_CMD1_REG, TPS_CMD1_REG_SIZE); 
        if (buf == NULL) {
                printf("Could not read CMD reg\n");
        } else {
                for (i = 0; i < TPS_CMD1_REG_SIZE; i++) {
                        printf("0x%x " , buf[i]);
                }
                printf("\n");
        }

        return buf;
}

int  tps_write_cmd_reg(int file, char* buf) {
	int i; 

	for (i = 0; i < TPS_CMD1_REG_SIZE; i++) {
		tps_write(file, buf[i]); 
	}

	return 0;
}

void tps_update_firmware() {
	printf("\n Inside TPS update firmware\n");
}

void tps_write_reg() {
	printf("\n Inside write reg\n");
}

void tps_read_reg(char* reg) {

	int file;
	char* buf;

	if (reg == NULL) {
		tps_print_help();
		return;
	}

	file = open(I2C_ADAPTER, O_RDWR);
	if(file < 0) {
                printf("Couldn't open the I2C Adapter\n");
                return;
        }

	if (ioctl(file, I2C_SLAVE, I2C_SLAVE_ADDR) < 0) {
		printf("Couldn't set I2C slave address\n");
		return; 
	}

	if(!strncmp(reg, "cap", 3)) {
		buf = tps_read_cap_reg(file);
	} else if(!strncmp(reg, "mode", 4)) {
		buf = tps_read_mode_reg(file); 
	} else if(!strncmp(reg, "type", 4)) {
		buf = tps_read_type_reg(file);
	} else {
		printf("%s reg not supported",reg);
		tps_print_help();
		return;
	}

	if (buf) {
		printf("\nFreeing buf");
		free(buf);
	}
	close(file);
}


int main (int argc, char *argv[]) {

	if (argc > 3 || argc < 2) {
		tps_print_help(); 
		return 0;	
	}

	if (argv[1][0] != '-') {
		printf("\nInvalid input\n");
		tps_print_help();
	} else {
		switch (argv[1][1]) {
			case 'r': tps_read_reg(argv[2]);
				break;
			case 'w': tps_write_reg();
				break;
			case 'u': tps_update_firmware();
				break;
			case 'h': tps_print_help();
				break;
			default:
				printf("\nInvalid arg\n");
				tps_print_help();
		      		break;
		}
	}		

	return 0;
}



