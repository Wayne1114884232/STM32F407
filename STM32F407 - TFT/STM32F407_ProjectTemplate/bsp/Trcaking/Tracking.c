#include "Tracking.h"
#include "control.h"

float turn_speed; 
extern float Target_Speed, Track_Kp, Track_Ki;  

uint8_t PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15;

/**
 * @brief  
 */
void trailing_init(void) {
    // GPIOD(PD8-PD15GPIOD)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | 
                                 GPIO_Pin_10 | GPIO_Pin_11 | 
                                 GPIO_Pin_12 | GPIO_Pin_13 | 
                                 GPIO_Pin_14 | GPIO_Pin_15;  // PD8-PD15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  // GPIOD
}


float Tracking_InnerRing(void) 
{
//PD9 PD8 PD11 PD10 PD13 PD12 PD15 PD14
    PD8 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
    PD9 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
    PD10 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);
    PD11 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11);
    PD12 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12);
    PD13 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13);
    PD14 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14);
    PD15 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15);
		//1
    if (flag_c == 1) 
		{
					 if((PD9==1)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=4;
			else if((PD9==0)&&(PD8==1)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=3;
			else if((PD9==0)&&(PD8==0)&&(PD11==1)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=2;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==1)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=1;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==1)&&(PD13==1)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=0;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==1)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=-1;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==1)&&(PD15==0)&&(PD14==0))turn_speed=-2;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==1)&&(PD14==0))turn_speed=-3;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==1))turn_speed=-4;
    }
		//2
    if (flag_c == 2) 
		{
					 if((PD9==1)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=7;
			else if((PD9==1)&&(PD8==1)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=6;
			else if((PD9==0)&&(PD8==1)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=5;
			else if((PD9==0)&&(PD8==1)&&(PD11==1)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=4;
			else if((PD9==0)&&(PD8==0)&&(PD11==1)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=3;
			else if((PD9==0)&&(PD8==0)&&(PD11==1)&&(PD10==1)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=2;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==1)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=1;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==1)&&(PD13==1)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=0;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==1)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=-1;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==1)&&(PD12==1)&&(PD15==0)&&(PD14==0))turn_speed=-2;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==1)&&(PD15==0)&&(PD14==0))turn_speed=-3;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==1)&&(PD15==1)&&(PD14==0))turn_speed=-4;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==1)&&(PD14==0))turn_speed=-5;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==1)&&(PD14==1))turn_speed=-6;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==1))turn_speed=-7;
    }
		//3
    if (flag_c == 3) 
		{
					 if((PD9==1)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=10;
			else if((PD9==1)&&(PD8==1)&&(PD11==0)&&(PD10==1)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=9;
			else if((PD9==1)&&(PD8==1)&&(PD11==1)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=8;
			else if((PD9==0)&&(PD8==1)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=7;
			else if((PD9==0)&&(PD8==1)&&(PD11==1)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=6;
			else if((PD9==0)&&(PD8==1)&&(PD11==1)&&(PD10==1)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=5;
			else if((PD9==0)&&(PD8==0)&&(PD11==1)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=4;
			else if((PD9==0)&&(PD8==0)&&(PD11==1)&&(PD10==1)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=3;
			else if((PD9==0)&&(PD8==0)&&(PD11==1)&&(PD10==1)&&(PD13==1)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=2;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==1)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=1;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==1)&&(PD13==1)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=0;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==1)&&(PD13==1)&&(PD12==1)&&(PD15==0)&&(PD14==0))turn_speed=-1;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==1)&&(PD12==0)&&(PD15==0)&&(PD14==0))turn_speed=-2;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==1)&&(PD12==1)&&(PD15==0)&&(PD14==0))turn_speed=-3;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==1)&&(PD12==1)&&(PD15==1)&&(PD14==0))turn_speed=-4;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==1)&&(PD15==0)&&(PD14==0))turn_speed=-5;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==1)&&(PD15==1)&&(PD14==0))turn_speed=-6;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==1)&&(PD15==1)&&(PD14==1))turn_speed=-7;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==1)&&(PD14==0))turn_speed=-8;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==1)&&(PD14==1))turn_speed=-9;
			else if((PD9==0)&&(PD8==0)&&(PD11==0)&&(PD10==0)&&(PD13==0)&&(PD12==0)&&(PD15==0)&&(PD14==1))turn_speed=-10;
    }

    return turn_speed;
}


