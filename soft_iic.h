#pragma once

#ifdef __cplusplus
extern "C" {
#endif
  
#define I2C_WR 0 /* 写控制bit */
#define I2C_RD 1 /* 读控制bit */

#define I2C_SUCCESS 0
#define I2C_FAIL    1

#define I2C_MEMSIZE_8BIT            (1)
#define I2C_MEMSIZE_16BIT           (2)


typedef void (*IIC_IO_INIT)(void);
typedef void (*IIC_SCK_SET)(uint8_t state);
typedef void (*IIC_SDA_SET)(uint8_t state);
typedef uint8_t (*IIC_SDA_READ)(void);
typedef void (*DELAY)(uint32_t us);

typedef enum {
	SLAVE_ADDR_7BIT = 0 ,
	SLAVE_ADDR_10BIT ,
}iic_addr_type;
typedef struct
{
	IIC_IO_INIT iic_io_init;
	IIC_SCK_SET iic_sck_set;
	IIC_SDA_SET iic_sda_set;
	IIC_SDA_READ iic_sda_read;
	DELAY   delay;
	uint32_t delay_time;
	uint16_t slaveaddr;
	iic_addr_type slavetype;
	const char* device_name;	
	
} soft_i2c_device_t;
  
 #ifdef __cplusplus
}
#endif
  
