/**
 *@file soft_iic.c
 *@brief IIC communication protocol code for different MCU
 *@author dsf
 *@data 2022-06-14
 */

/**
  * @brief  延时函数
  * @param  us  	循环次数
  * @param       
  * @retval none
  */
static void i2c_delay(uint32_t us)
{
	uint8_t i;
	for (i = 0; i < us; i++);

}
/**
  * @brief  iic io 初始化  配置io引脚模式
  * @param    	    
  * @retval none
  */
static void i2c_init(void)
{

}
/**
  * @brief  iic_sck_set 自行添加具体内容  
  * @param  state  	： 1：高电平 0：低电平    
  * @retval none
  */
static void i2c_sck_set(uint8_t state)
{
	
}
/**
  * @brief  iic_sda_set 自行添加具体内容  
  * @param  state  	： 1：高电平 0：低电平    
  * @retval none
  */
static void i2c_sda_set(uint8_t state)
{
	
}
/**
  * @brief  iic_sck_set 自行添加具体内容  
  * @param  none    
  * @retval 一字节数据
  */
static uint8_t i2c_sda_read()
{
	return 0;
}

/***********************用户结构体赋值********************************************/ 
//example：
#ifdef SoftI2C_SENSOR
 soft_i2c_device_t sensor = 
{
		i2c_init,//io 初始化
		i2c_sck_set ,//sck 拉高拉低
		i2c_sda_set,//sda 拉高拉低
		i2c_sda_read ,//sda 读取
		i2c_delay, //延时函数
		80,//延时时间
		slaveaddr//从机地址
		SLAVE_ADDR_7BIT,//从机类型
		"SoftI2C_Sensor"	//设备名
};
#endif


/**
  * @brief  i2c_start CPU发起I2C总线启动信号
  * @param  ops    
  * @retval none
  */
void i2c_start(soft_i2c_device_t *ops)
{
    /* 当SCL高电平时，SDA出现一个下降沿表示I2C总线启动信号 */
		ops->iic_sda_set(1);
		ops->iic_sck_set(1);
		ops->delay(ops->delay_time);
		ops->iic_sda_set(0);
		ops->delay(ops->delay_time);
		ops->iic_sck_set(0);
		ops->delay(ops->delay_time);
}

/**
  * @brief  i2c_stop CPU发起I2C总线停止信号
  * @param  ops    
  * @retval none
  */
void i2c_stop(soft_i2c_device_t *ops)
{
    /* 当SCL高电平时，SDA出现一个上升沿表示I2C总线停止信号 */
    ops->iic_sda_set(0);
    ops->iic_sck_set(1);
    ops->delay(ops->delay_time);
    ops->iic_sda_set(1);
    ops->delay(ops->delay_time);
}

/**
  * @brief  i2c_sendbyte CPU向I2C总线设备发送8bit数据
  * @param  ucByte ： 等待发送的字节    高电平数据有效
  * @retval none
  */
void i2c_sendbyte(soft_i2c_device_t *ops,uint8_t ucByte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (ucByte & 0x80)
        {
            ops->iic_sda_set(1);
        }
        else
        {
            ops->iic_sda_set(0);
        }
		ops->delay(ops->delay_time);
        ops->iic_sck_set(1);
        ops->delay(ops->delay_time);
        ops->iic_sck_set(0);
        if (i == 7)
        {
             ops->iic_sda_set(1);// 释放总线
        }
        ucByte <<= 1; /* 左移一个bit */

    }
}

/**
  * @brief  i2c_ReceiveByte CPU从I2C总线设备读取8bit数据
  * @param  ops    
  * @retval 读到的数据
  */
uint8_t i2c_receivebyte(soft_i2c_device_t *ops)
{
    uint8_t i;
    uint8_t value = 0;
	ops->iic_sda_set(1);//测试是否需要
    /* 读到第1个bit为数据的bit7 */
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        ops->iic_sck_set(1);
        ops->delay(ops->delay_time);
        if (ops->iic_sda_read())
        {
            value++;
        }
        ops->iic_sck_set(0);
        ops->delay(ops->delay_time);
    }
    return value;
}

/**
  * @brief  i2c_waitack CPU产生一个时钟，并读取器件的ACK应答信号
  * @param  ops    
  * @retval 返回0表示正确应答，1表示无器件响应
  */
uint8_t i2c_waitack(soft_i2c_device_t *ops)
{
    uint8_t ret;

    ops->iic_sda_set(1); /* CPU释放SDA总线 */
    ops->delay(ops->delay_time);
    ops->iic_sck_set(1); /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    ops->delay(ops->delay_time);
    if (ops->iic_sda_read()) /* CPU读取SDA口线状态 */
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    ops->iic_sck_set(0);
    return ret;
}

/**
  * @brief  i2c_sendack CPU产生一个ACK信号
  * @param  ops    
  * @retval none
  */
void i2c_sendack(soft_i2c_device_t *ops)
{
    ops->iic_sda_set(0);  /* CPU驱动SDA = 0 */
    ops->delay(ops->delay_time);
    ops->iic_sck_set(1); /* CPU产生1个时钟 */
    ops->delay(ops->delay_time);
    ops->iic_sck_set(0);
    ops->delay(ops->delay_time);
    ops->iic_sda_set(1);  /* CPU释放SDA总线 */
}

/**
  * @brief  i2c_writebuff CPU产生1个NACK信号
  * @param  ops    
  * @retval none
  */
void i2c_nack(soft_i2c_device_t *ops)
{
    ops->iic_sda_set(1); /* CPU驱动SDA = 1 */
    ops->delay(ops->delay_time);
    ops->iic_sck_set(1); /* CPU产生1个时钟 */
    ops->delay(ops->delay_time);
    ops->iic_sck_set(0);
    ops->delay(ops->delay_time);
}

/**
  * @brief  i2c_writebuff 软件I2C写一个数据
  * @param  ops    	 _pWriteBuf： 数据内容
  * @retval 1：success 0：failed
  */
uint8_t i2c_writebuff(soft_i2c_device_t *ops, uint8_t _pWriteBuf)
{
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1);	/* 此处是写指令 */		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		

	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr);	/* 此处是写指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}
	/* 第4步：开始写入数据 */
	i2c_sendbyte(ops,(uint8_t)_pWriteBuf);
	/* 第5步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;

}

/**
  * @brief  i2c_writebuffs 软件I2C写多个数据
  * @param  ops     	 _pWriteBuf： 数据内容 uSize： 数据长度
  * @retval 1：success 0：failed
  */
uint8_t i2c_writebuffs(soft_i2c_device_t *ops,uint8_t* _pWriteBuf,uint16_t uSize)
{
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1);	/* 此处是写指令 */		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr);	/* 此处是写指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}
	while(uSize--)
	{
		/* 第4步：开始写入数据 */
		i2c_sendbyte(ops,*_pWriteBuf);

		/* 第5步：检查ACK */
		if (i2c_waitack(ops) != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}

	   _pWriteBuf++;	/* 地址增1 */		
	}
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;
}

/**
  * @brief  i2c_writereg8bit 软件I2C向8bit寄存器地址写数据
  * @param  ops   	 regAddr：寄存器地址  _pWriteBuf：  数据内容 
  * @retval 1：success 0：failed
  */
uint8_t i2c_writereg8bit(soft_i2c_device_t *ops,uint8_t regAddr, uint8_t _pWriteBuf)
{
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1);	/* 此处是写指令 */		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr);	/* 此处是写指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}

	i2c_sendbyte(ops,(uint8_t)regAddr);
	/* 第5步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	i2c_sendbyte(ops,(uint8_t)_pWriteBuf);
	/* 第5步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;

}

/**
  * @brief  i2c_writeregs8bit 软件I2C 向寄存器地址写多个数据
  * @param  ops   	 regAddr：寄存器地址  _pWriteBuf：  数据内容  uSize：数据长度
  * @retval 1：success 0：failed
  */
uint8_t i2c_writeregs8bit(soft_i2c_device_t *ops,uint8_t regAddr, uint8_t* _pWriteBuf,uint16_t uSize)
{	
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1);	/* 此处是写指令 */		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr);	/* 此处是写指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}
	while(uSize--)
	{
		/* 第4步：开始写入数据 */
		i2c_sendbyte(ops,*_pWriteBuf);

		/* 第5步：检查ACK */
		if (i2c_waitack(ops) != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}

	    _pWriteBuf++;	/* 地址增1 */		
	}
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;

}

/**
  * @brief  i2c_writeregs16bits  软件I2C向16bit寄存器地址写多个数据
  * @param  ops     	 regAddr：16位寄存器地址  _pWriteBuf：  数据内容  uSize：数据长度
  * @retval 1：success 0：failed
  */
uint8_t i2c_writeregs16bits(soft_i2c_device_t *ops,uint16_t regAddr, uint8_t* _pWriteBuf,uint16_t uSize)
{
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1);	/* 此处是写指令 */		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr);	/* 此处是写指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}
	/* 第4步：发送寄存器地址高8bit */	
	i2c_sendbyte(ops,regAddr>>8);
	/* 第5步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	/* 第6步：发送寄存器地址低8bit */		
	i2c_sendbyte(ops,regAddr%256);
	/* 第7步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	while(uSize--)
	{
		/* 第8步：开始写入数据 */
		i2c_sendbyte(ops,*_pWriteBuf);

		/* 第9步：检查ACK */
		if (i2c_waitack(ops) != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}

	   _pWriteBuf++;	/* 地址增1 */		
	}
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;

}

/**
  * @brief  i2c_readbuff 软件I2C读取从机数据
  * @param  ops      _pReadBuf：数据缓存 uSize：数据长度
  * @retval 1：success 0：failed
  */
uint8_t i2c_readbuff(soft_i2c_device_t *ops,uint8_t* _pReadBuf,uint16_t uSize)
{
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1| I2C_RD);	/* 此处是读指令 */	
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr| I2C_RD);	/* 此处是读指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}

	while(uSize) 
	{
		*_pReadBuf = i2c_receivebyte(ops);

		/* 读指针自增 */
		_pReadBuf++; 
		  
		/*计数器自减 */
		uSize--;

		if(uSize == 0)
			i2c_nack(ops);	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			i2c_sendack(ops);	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */  
	}
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;

}


/**
  * @brief  i2c_readbuff 软件I2C读取寄存器地址的数据
  * @param  ops    regAddr: 8bit地址  _pReadBuf：数据缓存 uSize：数据长度
  * @retval 1：success 0：failed
  */
uint8_t i2c_readreg8bits(soft_i2c_device_t *ops,uint8_t regAddr,uint8_t *_pReadBuf,uint16_t uSize)
{
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1);	/* 此处是写指令 */	
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr);	/* 此处是写指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}
	i2c_sendbyte(ops,regAddr);	
	 
	/* 第3步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	
	}

	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1| I2C_RD);	/* 此处是读指令 */	
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr| I2C_RD);	/* 此处是读指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}
	
	while(uSize--)
	{
		*_pReadBuf = i2c_receivebyte(ops);
		_pReadBuf++;					//偏移到下一个数据存储地址
		
		if(uSize == 0)
        {
           i2c_nack(ops);			//最后一个数据需要回NOACK
        }
        else
        {
          i2c_sendack(ops);			//回应ACK
		}
	}

	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;

}
/**
  * @brief  i2c_readbuff 软件I2C读取寄存器地址的数据
  * @param  ops    regAddr: 16bit地址  _pReadBuf：数据缓存 uSize：数据长度
  * @retval 1：success 0：failed
  */
static uint8_t i2c_readreg16bits(soft_i2c_device_t *ops,uint16_t regAddr,uint8_t *_pReadBuf,uint16_t uSize)
{
	uint8_t addr1,addr2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_start(ops);	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1);	/* 此处是写指令 */	
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr);	/* 此处是写指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}

	i2c_sendbyte(ops,regAddr>>8);	
	 
	/* 第3步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	i2c_sendbyte(ops,regAddr%256);	
	 
	/* 第3步：等待ACK */
	if (i2c_waitack(ops) != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	//i2c_start(ops);
	
	if(ops->slavetype == SLAVE_ADDR_10BIT)	
	{
        addr1 = 0xf0 | ((ops->slaveaddr >> 7) & 0x06);
        addr2 = ops->slaveaddr & 0xff;
		/* 第2步：发起控制字节，第一个字节的头 7 位 是 11110XX 的组合，
		其中最后两位（XX）是 10 位地址的两个最高位，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,addr1| I2C_RD);	/* 此处是读指令 */	
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		/* 第2步：发起控制字节，10bit地址低八位 */
		i2c_sendbyte(ops,addr2);		
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
		
	}else
	{		
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_sendbyte(ops,ops->slaveaddr| I2C_RD);	/* 此处是读指令 */
					
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_waitack(ops)!= 0)
		{
			goto cmd_fail;
		}
	}
	
	
	while(uSize--)
	{
		*_pReadBuf = i2c_receivebyte(ops);
		_pReadBuf++;					//偏移到下一个数据存储地址
		
		if(uSize == 0)
        {
            i2c_nack(ops);			//最后一个数据需要回NOACK
        }
        else
        {
            i2c_sendack(ops);			//回应ACK
		}
	}

	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_SUCCESS;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop(ops);
	return I2C_FAIL;

}
