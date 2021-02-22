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

/* Help Menu 
 * TBD: Needs to provide reg names options. 
 */ 
void tps_print_help () {
	printf("\nUsage:");
	printf("\n tps [-r|read] [reg]");
        printf("\n tps [-w|write] [reg]");
	printf("\n tps [-u|update-firmware] [file]");
	printf("\n tps [-h|help]\n");
}

/* The low level write used to setup the 
 * reg add to access the I2C registers.
 * Input: I2C dev file and reg addr.
 * Returns: Sucess/Failed.
 */
int tps_write(int file, uint8_t addr) {
        int ret;

        ret = i2c_smbus_write_byte(file, addr);
        return ret;
}

/* The low -level read API used by reg 
 * specific API's to read. 
 *
 * Input: I2C dev node, reg addr, read size
 * Returns: Bytes read from slave addr
 * Uses: tps_write() to change the reg addr
 */

char* tps_read(int file, uint8_t addr,  uint8_t count) {

	char *buf;
	int i = 0;
	int ret = 0;

	ret = tps_write(file, addr);
	if (ret < 0 ) {
		printf("Could not read addr 0x%x", addr);
		return NULL;
	}
	++count;

	buf = (char *) malloc (sizeof(char) * (count));

	//i2c_smbus_read_block_data(file, (__u8)addr, buf);

	while (count != 0 ) {
		*(buf + i) = i2c_smbus_read_byte(file);
		count--;
		i++;
	}

	/* All reg except MODE and APP start with size. */
	if ((addr == TPS_MODE_REG) || (addr == TPS_TYPE_REG)) {
		return buf;
	} else {
		return (buf + 1);
	}
}

/* Begin register specific reads 
 */

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


char* tps_read_int_event_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_INT_EVENT1_REG, TPS_INT_EVENT1_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read INT_EVENT1 reg\n");
        } else {
                for (i = 0; i < TPS_INT_EVENT1_REG_SIZE; i++) {
                        printf("0x%02x " , buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_int_mask_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_INT_MASK1_REG, TPS_INT_MASK1_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read INT_MASK1 reg\n");
        } else {
                for (i = 0; i < TPS_INT_MASK1_REG_SIZE; i++) {
                        printf("0x%02x " , buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_int_clear_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_INT_CLEAR1_REG, TPS_INT_CLEAR1_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read INT_CLEAR1 reg\n");
        } else {
                for (i = 0; i < TPS_INT_CLEAR1_REG_SIZE; i++) {
                        printf("0x%02x " , buf[i]);
                }
                printf("\n");
        }
        
        return buf;
}

char* tps_read_power_path_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_POWER_PATH_STATUS_REG, 
			TPS_POWER_PATH_STATUS_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read POWER_PATH_STATUS reg\n");
        } else {
                for (i = 0; i < TPS_POWER_PATH_STATUS_REG_SIZE; i++) {
                        printf("0x%02x " , buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_port_control_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_PORT_CONTROL_REG, TPS_PORT_CONTROL_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read PORT_CONTROL reg\n");
        } else {
                for (i = 0; i < TPS_PORT_CONTROL_REG_SIZE; i++) {
                        printf("0x%02x " , buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_rx_source_caps_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_RX_SOURCE_CAPS_REG, TPS_RX_SOURCE_CAPS_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read RX_SOURCE_CAPS reg\n");
        } else {
                for (i = 0; i < TPS_RX_SOURCE_CAPS_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]); 
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_tx_source_caps_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_TX_SOURCE_CAPS_REG, TPS_TX_SOURCE_CAPS_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read TX_SOURCE_CAPS reg\n");
        } else {
                for (i = 0; i < TPS_TX_SOURCE_CAPS_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
                }
                printf("\n");
        }

        return buf;
}


char* tps_read_rx_sink_caps_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_RX_SINK_CAPS_REG, TPS_RX_SINK_CAPS_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read RX_SINK_CAPS reg\n");
        } else {
                for (i = 0; i < TPS_RX_SINK_CAPS_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_tx_sink_caps_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_TX_SINK_CAPS_REG, TPS_TX_SINK_CAPS_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read TX_SINK_CAPS reg\n");
        } else {
                for (i = 0; i < TPS_TX_SINK_CAPS_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_active_contract_pdo_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_ACTIVE_CONTRACT_PDO_REG, TPS_ACTIVE_CONTRACT_PDO_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read ACTIVE_CONTRACT_PDO reg\n");
        } else {
                for (i = 0; i < TPS_ACTIVE_CONTRACT_PDO_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_active_contract_rdo_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_ACTIVE_CONTRACT_RDO_REG, TPS_ACTIVE_CONTRACT_RDO_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read ACTIVE_CONTRACT_RDO reg\n");
        } else {
                for (i = 0; i < TPS_ACTIVE_CONTRACT_RDO_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_pd_status_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_PD_STATUS_REG, TPS_PD_STATUS_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read PD_STATUS reg\n");
        } else {
                for (i = 0; i < TPS_PD_STATUS_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_typec_state_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_TYPEC_STATE_REG, TPS_TYPEC_STATE_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read TYPEC_STATE reg\n");
        } else {
                for (i = 0; i < TPS_TYPEC_STATE_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_gpio_status_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_GPIO_STATUS_REG, TPS_GPIO_STATUS_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read GPIO_STATUS reg\n");
        } else {
                for (i = 0; i < TPS_GPIO_STATUS_REG_SIZE; i++) {
                        printf("B%d:0x%02x  " , i, buf[i]);
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
                	printf("0x%02x " , buf[i]);
        	}
        	printf("\n");
	} 

	return buf;
}

char * tps_read_version_reg(int file) {
	char *buf;
	int i;

	buf = tps_read(file, TPS_VERSION_REG, TPS_VERSION_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read version\n");
        } else {
                for (i = 0; i < TPS_VERSION_REG_SIZE; i++) {
                        printf("%02x." , buf[TPS_VERSION_REG_SIZE-i-1]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_devinfo_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_DEVICE_INFO_REG, TPS_DEVICE_INFO_REG_SIZE);
        if (buf == NULL) {
                printf("Could not read device information\n");
        } else {
                for (i = 0; i < TPS_DEVICE_INFO_REG_SIZE; i++) {
                        printf("%c" , buf[i]);
                }
                printf("\n");
        }

        return buf;
}

char* tps_read_buildinfo_reg(int file) {
        char *buf;
        int i;

        buf = tps_read(file, TPS_BUILD_DESCRIPTION, TPS_BUILD_DESCRIPTION_SIZE);
        if (buf == NULL) {
                printf("Could not read build information\n");
        } else {
                for (i = 0; i < TPS_BUILD_DESCRIPTION_SIZE; i++) {
                        printf("%c" , buf[i]);
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
		case 5: printf("Reserved\n");
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

	UsbPdCap = (buf[0] & TPS_CAP_USBPD_CAP_M) >> 2 ; 
	switch (UsbPdCap) {
		case 0: printf("UsbPdCapability: Supported\n");
			break;
		case 1: printf("UsbPdCapability: Not Supported\n");
			break;
		default:
			break;
	}

	I2CmLevel = (buf[0] & TPS_CAP_I2CMLEVEL_VOLT_M) >> 7;
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
/* End: Register specific reads */


/* Begin Register specific writes */
#if 0
int  tps_write_cmd_reg(int file, char* buf) {
	int i; 

	i2c_smbus_write_block_data(int file, __u8 command,
                                               __u8 length, __u8 *values);
	return 0;
}
#endif

void tps_write_reg(__u8 addr, __u8 length,  char *buf) {
        int file;
     
     	file = open(I2C_ADAPTER, O_RDWR);
        if(file < 0) {
                printf("Couldn't open the I2C Adapter\n");
                return;
        }

        if (ioctl(file, I2C_SLAVE, I2C_SLAVE_ADDR) < 0) {
                printf("Couldn't set I2C slave address\n");
                return;
	}

	i2c_smbus_write_block_data(file, addr, length, (__u8*)buf);
	
}

void tps_list_regs() {
	printf("\nPlease mention one reg from the list below\n");
	printf("\n \t cmd \n \t cap \n \t mode \n \t type \n \t status"
		"\n \t version \n \t devinfo \n \t buildinfo \n \t power"
		"\n \t int \n \t mask \n \t clear \n \t powerp \n \t portcon"
		"\n \t rxsrc \n \t txsrc \n \t rxsink \n \t txsink \n \t pdo"
		"\n \t rdo \n \t pdstat \n \t tsate \n \t gpio\n");
}	

void tps_update_firmware() {
        tps_write_reg(TPS_CMD1_REG, 4, "abcd");
}


/* Main read API
 * Input:  Reg to read
 * Output: Calls register specific read
 * 	   functions to display the contents.
 */ 
void tps_read_reg(char* reg) {

	int file;
	char* buf;

	if (reg == NULL) {
		tps_print_help();
		tps_list_regs();
		return;
	}

	/* Open the TPS I2C adapter for read/write
	 * every read requires a write first to
	 * select the register.
	 */
	file = open(I2C_ADAPTER, O_RDWR);
	if(file < 0) {
                printf("Couldn't open the I2C Adapter\n");
                return;
        }

	if (ioctl(file, I2C_SLAVE, I2C_SLAVE_ADDR) < 0) {
		printf("Couldn't set I2C slave address\n");
		return; 
	}

	/* Add to the list of reg here for reading */
	if(!strncmp(reg, "cap", 3)) {
		buf = tps_read_cap_reg(file);
	} else if(!strncmp(reg, "cmd", 4)) {
		buf = tps_read_cmd_reg(file);
	} else if(!strncmp(reg, "mode", 4)) {
		buf = tps_read_mode_reg(file); 
	} else if(!strncmp(reg, "type", 4)) {
		buf = tps_read_type_reg(file);
	} else if(!strncmp(reg, "status", 6)) {
		buf = tps_read_status_reg(file);
	} else if(!strncmp(reg, "version", 7)) {
		buf = tps_read_version_reg(file);
	} else if(!strncmp(reg, "devinfo", 7)) {
		buf = tps_read_devinfo_reg(file);
	} else if(!strncmp(reg, "buildinfo", 9)) {
		buf = tps_read_buildinfo_reg(file);
	} else if(!strncmp(reg, "power", 5)) {
		buf = tps_read_power_status_reg(file);
	} else if(!strncmp(reg, "int", 3)) {
		buf = tps_read_int_event_reg(file);
	} else if(!strncmp(reg, "mask", 4)) {
		buf = tps_read_int_mask_reg(file);
	} else if(!strncmp(reg, "clear", 5)) {
		buf = tps_read_int_clear_reg(file);
	} else if(!strncmp(reg, "powerp", 6)) {
		buf = tps_read_power_path_reg(file);
	} else if(!strncmp(reg, "portcon", 7)) {
		buf = tps_read_port_control_reg(file);
	} else if(!strncmp(reg, "rxsrc", 5)) {
		buf = tps_read_rx_source_caps_reg(file);
	} else if(!strncmp(reg, "txsrc", 5)) {
		buf = tps_read_tx_source_caps_reg(file);
	} else if(!strncmp(reg, "rxsink", 6)) {
		buf = tps_read_rx_sink_caps_reg(file);
	} else if(!strncmp(reg, "txsink", 6)) {
		buf = tps_read_tx_sink_caps_reg(file);
	} else if(!strncmp(reg, "pdo", 3)) {
		buf = tps_read_active_contract_pdo_reg(file);
	} else if(!strncmp(reg, "rdo", 3)) {
		buf = tps_read_active_contract_rdo_reg(file);
	} else if(!strncmp(reg, "pdstat", 6)) {
		buf = tps_read_pd_status_reg(file);
	} else if(!strncmp(reg, "tstate", 6)) {
		buf = tps_read_typec_state_reg(file);
	} else if(!strncmp(reg, "gpio", 4)) {
		buf = tps_read_gpio_status_reg(file);
	} else {
		printf("%s reg not supported",reg);
		tps_list_regs();
		return;
	}

	if (buf) {
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
			// case 'w': tps_write_reg();
			//		break;
			case 'u': tps_update_firmware(argv[2]);
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



