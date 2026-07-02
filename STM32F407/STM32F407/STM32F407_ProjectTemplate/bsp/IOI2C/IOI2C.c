#include "IOI2C.h"

void Delay(u32 count)//400KHzIIC
{
	unsigned int uiCnt = count*20;
	while (uiCnt --);
}

void Delay_us(uint32_t us) {
    uint32_t ticks = us * (SystemCoreClock / 1000000) / 5; 
    while (ticks--);
}

void IIC_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SDA_OUT();     
	IIC_SDA=1;	  	  
	IIC_SCL=1;
}


void SDA_IN(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;        
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;   
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void SDA_OUT(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;    
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;      
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;    
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_Start(void)
{
	SDA_OUT();    
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	
	Delay_us(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	
	Delay_us(5);
	IIC_SCL=0;
}
	  
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	
	Delay_us(5);
	IIC_SCL=1; 
	IIC_SDA=1;
	
	Delay_us(5);							   	
}

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0; 
	SDA_IN();     
	IIC_SDA=1;
	Delay_us(5);	  
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;
		}
		Delay_us(5);
	}  
	IIC_SCL=1;
	Delay_us(5); 
	IIC_SCL=0;
	return 0;  
} 

void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	Delay_us(5);
	IIC_SCL=1;
	Delay_us(5);
	IIC_SCL=0;
}
	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	
	Delay_us(5);
	IIC_SCL=1;
	Delay_us(5);
	IIC_SCL=0;
}					 				     
		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t; 
		SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
			
		Delay_us(2);   
		IIC_SCL=1;
		Delay_us(5);
		IIC_SCL=0;	
		Delay_us(3);
    }	 
} 	 
  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
    IIC_SCL=0; 
        
		Delay_us(5);
		IIC_SCL=1;
    receive<<=1;
    if(READ_SDA)receive++;   
		
		Delay_us(5); 
    }					 
    if (ack)
        IIC_Ack(); 
    else
        IIC_NAck();
    return receive;
}

int32_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length)
{
    uint32_t count = 0;

    IIC_Start();
    IIC_Send_Byte(dev);	
    if(IIC_Wait_Ack() == 1)return 0;
    IIC_Send_Byte(reg);
    if(IIC_Wait_Ack() == 1)return 0;
    IIC_Start();
    IIC_Send_Byte(dev+1); 
    if(IIC_Wait_Ack() == 1)return 0;

    for(count=0; count<length; count++)
    {
        if(count!=length-1)data[count]=IIC_Read_Byte(1);
        else  data[count]=IIC_Read_Byte(0);	 
    }
    IIC_Stop();
    return 1;
}


int32_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t* data, uint32_t length)
{
    uint32_t count = 0;
    IIC_Start();
    IIC_Send_Byte(dev);	   
    if(IIC_Wait_Ack() == 1)return 0;
    IIC_Send_Byte(reg);   
    if(IIC_Wait_Ack() == 1)return 0;
    for(count=0; count<length; count++)
    {
        IIC_Send_Byte(data[count]);
        if(IIC_Wait_Ack() == 1)return 0;
    }
    IIC_Stop();

    return 1; 
}
