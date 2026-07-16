#include "my_I2c.h"
#define myI2C_SCL_Pin GPIO_NUM_1
#define myI2C_SDA_Pin GPIO_NUM_2

void myI2C_Write_SCL(uint8_t BitValue)
{
    gpio_set_level(myI2C_SCL_Pin,(uint8_t)BitValue);
 ;
}

void myI2C_Write_SDA(uint8_t BitValue)
{
    gpio_set_level(myI2C_SDA_Pin,(uint8_t)BitValue);
}

uint8_t myI2C_Read_SCL()
{
    int BitVale;
    gpio_set_direction(myI2C_SCL_Pin, GPIO_MODE_INPUT);//临时设为输入模式
    BitVale=gpio_get_level(myI2C_SCL_Pin);//读取电平状态
    gpio_set_direction(myI2C_SCL_Pin, GPIO_MODE_INPUT_OUTPUT_OD);//切换为开漏输出模式
    return BitVale;
}

uint8_t myI2C_Read_SDA()
{
    int BitVale;
    gpio_set_direction(myI2C_SDA_Pin, GPIO_MODE_INPUT);//临时设为输入模式
    BitVale=gpio_get_level(myI2C_SDA_Pin);//读取电平状态
    gpio_set_direction(myI2C_SDA_Pin, GPIO_MODE_INPUT_OUTPUT_OD);//切换为开漏输出模式
    return BitVale;
}

void myI2C_Init(void)
{
    gpio_config_t gpio_config_InitStructure;//初始化gpio
    gpio_config_InitStructure.intr_type=GPIO_INTR_DISABLE;//中断失能
    gpio_config_InitStructure.pull_down_en=GPIO_PULLDOWN_ENABLE;//下拉失能
    gpio_config_InitStructure.pull_up_en=GPIO_PULLUP_DISABLE;//上拉失能
    gpio_config_InitStructure.mode=GPIO_MODE_OUTPUT_OD;//开漏输出
    gpio_config_InitStructure.pin_bit_mask=(1ull<<myI2C_SCL_Pin)|((1ull<<myI2C_SDA_Pin));//引脚掩码
    gpio_config(&gpio_config_InitStructure);
    //拉高电平，为起始信号做准备
    myI2C_Write_SCL(1);
    myI2C_Write_SDA(1);

}

/**
 * @description: I2C起始信号
 * @tip:scl高电平期间，sda由高电平切换为低电平
 * @return {*}无
 */
void myI2C_Start(void)
{
    //首先恢复空闲状态,由于在指定地址读时 会有个str ,此时SDA的电平并不知道，
	//如果先释放SCL，此时若SDA为低电平，然后在释放完SCL后才释放SDA，
    //会导致停止条件的产生，所以先释放sda，再释放scl
    myI2C_Write_SDA(1);
    myI2C_Write_SCL(1);
    //在scl高电平期间，sda由高电平切换为低电平，为保持后续的时序的连续性，将scl也拉低
    myI2C_Write_SDA(0);//拉低sda
    myI2C_Write_SCL(0);//拉低scl
}


void myI2C_SendByte(uint8_t Byte)
{
   uint8_t i;
   for(i=0;i<8;i++)
   {
        myI2C_Write_SDA(Byte&(0x80>>i));//将数据放至数据线上
        myI2C_Write_SCL(1);//释放scl,释放主动权，让从机读取，此时不允许数据再有变化
        myI2C_Write_SCL(0);//拉低scl
   }
}


uint8_t myI2C_ReceiveByte(void)
{
    uint8_t byte=0x00,i=0;  
    myI2C_Write_SDA(1);//先释放sda，防止主机一直占用sda信号，从机无法掌握主动权
    for(i=0;i<8;i++)
    {
        myI2C_Write_SCL(1);//拉高scl
        if(myI2C_Read_SDA()==1)//如果该位为1
        {
            byte|=(0x80>>i);//接收数据，此时不允许数据变化
        }
         myI2C_Write_SCL(0);//拉低scl

    }
    return byte;//返回接收的字节

}


void myI2C_SendAck(uint8_t ack)
{
    myI2C_Write_SDA(ack);//发送应答
    myI2C_Write_SCL(1);//拉高scl，主动权交给从机
    myI2C_Write_SCL(0);//拉高scl
}


uint8_t myI2C_ReceiveAck(void)
{
    uint8_t ack=0;
    myI2C_Write_SDA(1);//拉高释放，主动权交给从机，让从机发送应答
    myI2C_Write_SCL(1);//拉高scl，主动权交给从机，采样
    ack=myI2C_Read_SDA();//接收应答
    myI2C_Write_SCL(0);//拉高scl
    return ack;//返回应答

}

void myI2C_Stop(void)
{
    //SDA若一开始就是低电平，则可直接拉高 ，但如果SDA开始时是高电平 则需要先拉低再拉高
    myI2C_Write_SDA(0);
    myI2C_Write_SCL(1);//在scl高电平期间，SDA从低电平切换到高电平
    myI2C_Write_SDA(1);
    
}
