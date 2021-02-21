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
	uint8_t ActingAsLegacy; 
	uint8_t	UsbHostPresent;
	uint8_t VBusStatus;
	uint8_t ConnState;


        buf = tps_read(file, TPS_STATUS_REG, TPS_POWER_STATUS_REG_SIZE);
	if (buf == NULL) {
		printf("Could not read STATUS reg\n");
	} else {
        	for (i = 0; i < TPS_STATUS_REG_SIZE; i++) {
                	printf("0x%x " , buf[i]);
        	}
        	printf("\n");
	}        

	printf("\nPlug Status: ");
	if (buf[0] & TPS_STATUS_PLUG_PRESENT_M) { 
		printf("Plug is connected\n");
	} else { 
		printf("No plug connected\n");
	}

	ConnState = buf[0] & TPS_STATUS_CONN_STATE_M; 
	ConnState = ConnState >> 1;
	printf("\nConn State: ");
	switch (ConnState) {
		case 0: printf("No Connection.\n");
			break;
		case 1: printf("Port is disabled.\n");
			break;
		case 2: printf("Audio Connection.\n");
			break;
		case 3: printf("Debug Connection (Rd/Rd).\n");
			break;
		case 4: printf("No Connection, Ra detected (but no Rd).\n");
			break;
		case 5: printf("Reserved, shoud not have this state\n");
			break;
		case 6: printf("Connection present no Ra detected\n");
			break;
		case 7: printf("Connection presnet, Ra detected\n");
			break;
		default:
			break;
	}

	printf("\nOrientation: ");
	if (buf[0] & TPS_STATUS_PLUG_OR_M) {
		printf("Upside-down orientation (plug CC on CC2)..\n");
	} else {
		printf("Upside-up orientation (plug CC on CC1).\n");
	}

	printf("\nPort Role: ");
	if (buf[0] & TPS_STATUS_PORT_ROLE_M) {
		printf("PD Controller is Source\n");
	} else {
		printf("PD Controller is in the Sink role\n");
	}

	printf("\nData Role: ");
	if (buf[0] & TPS_STATUS_DATA_ROLE_M) {
		printf("Downward-facing port (DFP)\n");
	} else {
		printf("Upward-facing port (UFP)\n");
	}


	VBusStatus = buf[2] & TPS_STATUS_VBUS_STATUS_M;
	VBusStatus = VBusStatus >> 4;
	printf("\nVBusStatus: "); 
	switch (VBusStatus) {
		case 0: printf("At vSafe0V (less than 0.8V)\n");
			break; 
		case 1: printf("At vSafe5V (4.75 V to 5.5 V).\n");
			break;
		case 2: printf("Within expected limits.\n");
			break;
		case 3: printf("Not within any specified ranges.\n");
			break;
		default: 
			break;
	}

	UsbHostPresent = buf[2] & TPS_STATUS_USB_HOST_PRSNT_M;
	UsbHostPresent = UsbHostPresent >> 6;
	printf("\nUSB Host Presence: ");
	switch (UsbHostPresent) {
		case 0: printf("No host present.\n");
			printf("Controller power role is Source.\n");
			break;
		case 1: printf("VBUS is being provided by a Port Partner\n");
			printf("PD device not capable of USB comm\n");
			break;
		case 2: printf("VBUS is being provided by a Port Partner\n");
			printf("that is not a PD device.\n");
			break;
		case 3: printf("Host present. This means VBUS\n");
			printf("is being provided by a Port Partner\n");
			printf("that is USB PD capable and also\n");
			printf("capable of USB communications.\n");
			break;
		default: 
			break;
	}

	ActingAsLegacy = buf[3] & TPS_STATUS_ACT_LEGACY_M;
	printf("\nActing as Legacy: ");
	switch (ActingAsLegacy) {
		case 0: printf("PD Controller is not in a legacy\n");
			printf("(non PD) mode\n");
			break;
		case 1: printf("PD Controller is acting like a legacy sink.\n");
			printf("It will not respond to USB PD message traffic.\n");
			break;
		case 2: printf("PD Controller is acting like a legacy source\n");
			printf("It will not respond to USB PD message traffic.\n");
			break;
		case 3: printf("PD controller is acting as a legacy sink\n");
			printf("(non-PD) port until the dead battery flag is cleared.\n");
			break;
		default: 
			break;
	}

	printf("\nBIST: ");
	if (buf[3] & TPS_STATUS_BIST_M) {
		printf("BIST in progress.\n");
	} else {
		printf("No BIST in progress\n");
	}

	return buf;
}

char* tps_read_cap_reg(int file) {
        char *buf;
        int i;
	uint8_t PowerRole;
	uint8_t UsbPdCap;
	uint8_t I2CmLevel;

        buf = tps_read(file, TPS_DEVICE_CAP_REG, TPS_DEVICE_CAP_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read DEVICE CAPABILITIES reg\n");
        } else {
                for (i = 0; i < TPS_DEVICE_CAP_REG_SIZE; i++) {
                        printf("0x%x " , buf[i]);
                }
                printf("\n");
        }

	PowerRole = buf[0] & TPS_CAP_POWER_ROLE_M;
	switch (PowerRole) {
		case 0: printf("PowerRole: Both source and sink supported\n");
			break;
		case 1: printf("PowerRole: Source Only\n");
			break;
		case 2: printf("PowerRole: Sink Only\n");
			break;
		default: 
			break;
	}

	UsbPdCap = buf[0] & TPS_CAP_USBPD_CAP_M; 
	switch (UsbPdCap) {
		case 0: printf("UsbPdCapability: Supported\n");
			break;
		case 1: printf("UsbPdCapability: Not Supported\n");
			break;
		default:
			break;
	}

	I2CmLevel = buf[0] & TPS_CAP_I2CMLEVEL_VOLT_M;
	switch(I2CmLevel) {
		case 0: printf("I2CmLevel: 1.8V or 3.3V\n");
			break;
		case 1: printf("I2CmLevel: 3.3V\n");
			break;
		default:
			break;
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
	} else if(!strncmp(reg, "status", 6)) {
		buf = tps_read_status_reg(file);
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



