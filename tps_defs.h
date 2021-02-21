
#define I2C_ADAPTER "/dev/i2c-0"
#define I2C_SLAVE_ADDR 0x21

#define TPS_MODE_REG 0x03
#define TPS_MODE_REG_SIZE 4

#define TPS_TYPE_REG 0x04
#define TPS_TYPE_REG_SIZE 4

#define TPS_CUSTUSE_REG 0x06
#define TPS_CUSTUSE_REG_SIZE 8

#define TPS_CMD1_REG 0x08
#define TPS_CMD1_REG_SIZE 4

#define TPS_DATA_REG 0x09
#define TPS_DATA_REG_SIZE 64

#define TPS_DEVICE_CAP_REG 0x0D
#define TPS_DEVICE_CAP_REG_SIZE 1 

#define TPS_VERSION_REG 0x0F
#define TPS_VERSION_REG_SIZE 4

#define TPS_INT_EVENT1_REG 0x14
#define TPS_INT_EVENT1_REG_SIZE 11

#define TPS_INT_MASK1_REG 0x16
#define TPS_INT_MASK1_REG_SIZE 11

#define TPS_INT_CLEAR1_REG 0x18
#define TPS_INT_CLEAR1_REG_SIZE 11

#define TPS_STATUS_REG 0x1A
#define TPS_STATUS_REG_SIZE 5

#define TPS_POWER_PATH_STATUS_REG 0x26
#define TPS_POWER_PATH_STATUS_REG_SIZE 5

#define TPS_PORT_CONTROL_REG 0x29
#define TPS_PORT_CONTROL_REG_SIZE 4

#define TPS_BOOT_STATUS_REG 0x2D
#define TPS_BOOT_STATUS_REG_SIZE 5

#define TPS_BUILD_DESCRIPTION 0x2E
#define TPS_BUILD_DESCRIPTION_SIZE

#define TPS_DEVICE_INFO_REG 0x2F
#define TPS_DEVICE_INFO_REG_SIZE 40

#define TPS_RX_SOURCE_CAPS_REG 0X30
#define TPS_RX_SOURCE_CAPS_REG_SIZE 29

#define TPS_RX_SINK_CAPS_REG 0X31
#define TPS_RX_SINK_CAPS_REG_SIZE 29

#define TPS_TX_SOURCE_CAPS_REG 0x32
#define TPS_TX_SOURCE_CAPS_REG_SIZE 31

#define TPS_TX_SINK_CAPS_REG 0x33
#define TPS_TX_SINK_CAPS_REG_SIZE 29

#define TPS_ACTIVE_CONTRACT_PDO_REG 0x34
#define TPS_ACTIVE_CONTRACT_PDO_REG_SIZE 6

#define TPS_ACTIVE_CONTRACT_RDO_REG 0x35
#define TPS_ACTIVE_CONTRACT_RDO_REG_SIZE 4

#define TPS_POWER_STATUS_REG 0x3F
#define TPS_POWER_STATUS_REG_SIZE 2

#define TPS_PD_STATUS_REG 0x40
#define TPS_PD_STATUS_REG_SIZE 4

#define TPS_TYPEC_STATE_REG 0x69
#define TPS_TYPEC_STATE_REG_SIZE 4

#define TPS_GPIO_STATUS_REG 0x72
#define TPS_GPIO_STATUS_REG_SIZE 8





