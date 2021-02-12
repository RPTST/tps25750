#include <stdio.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>


#define I2C_ADAPTER "/dev/i2c-0"
#define I2C_SLAVE_ADDR 0x40

int main (void) {

        int file;

        printf("Your I2C utility\n");
        file = open(I2C_ADAPTER, O_RDWR);

        if(file < 0) {
                printf("Couldn't open the I2C Adapter\n");
                return 0;
        }

        printf("Opened the I2C Adapter\n");

        if (ioctl(file, I2C_SLAVE, I2C_SLAVE_ADDR) < 0) {
                printf("Couldn't set I2C slave address\n");
                return 0;
        }


        return 0;
}
