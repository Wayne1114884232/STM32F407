#ifndef __IOI2C_H
#define __IOI2C_H

#include "sys.h"

// STM32F4
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  

#define GPIOA_ODR_Addr    (GPIOA_BASE+0x14) // 0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14) // 0x40020414
#define GPIOC_ODR_Addr    (GPIOC_BASE+0x14) // 0x40020814
#define GPIOD_ODR_Addr    (GPIOD_BASE+0x14) // 0x40020C14
#define GPIOE_ODR_Addr    (GPIOE_BASE+0x14) // 0x40021014
#define GPIOF_ODR_Addr    (GPIOF_BASE+0x14) // 0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+0x14) // 0x40021814    
#define GPIOH_ODR_Addr    (GPIOH_BASE+0x14) // 0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+0x14) // 0x40022014    

#define GPIOA_IDR_Addr    (GPIOA_BASE+0x10) // 0x40020010
#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10) // 0x40020410
#define GPIOC_IDR_Addr    (GPIOC_BASE+0x10) // 0x40020810
#define GPIOD_IDR_Addr    (GPIOD_BASE+0x10) // 0x40020C10
#define GPIOE_IDR_Addr    (GPIOE_BASE+0x10) // 0x40021010
#define GPIOF_IDR_Addr    (GPIOF_BASE+0x10) // 0x40021410
#define GPIOG_IDR_Addr    (GPIOG_BASE+0x10) // 0x40021810
#define GPIOH_IDR_Addr    (GPIOH_BASE+0x10) // 0x40021C10
#define GPIOI_IDR_Addr    (GPIOI_BASE+0x10) // 0x40022010

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)   


#define I2C_GPIO_PORT     GPIOB
#define I2C_SCL_PIN       GPIO_Pin_10
#define I2C_SDA_PIN       GPIO_Pin_11
#define I2C_GPIO_CLK      RCC_AHB1Periph_GPIOB

// SDA
#define SDA_IN()  {I2C_GPIO_PORT->MODER &= ~(3UL << (11*2));} 
#define SDA_OUT() {I2C_GPIO_PORT->MODER &= ~(3UL << (11*2)); I2C_GPIO_PORT->MODER |= (1UL << (11*2));} 

// I2C
#define IIC_SCL    PBout(10)  // SCL
#define IIC_SDA    PBout(11)  // SDA
#define READ_SDA   PBin(11)   // SDA

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

int32_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length);
int32_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t* data, uint32_t length);

#endif

