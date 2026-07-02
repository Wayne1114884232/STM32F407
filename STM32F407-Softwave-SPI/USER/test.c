//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F407ZGT6,����ԭ��Explorer STM32F4������,��Ƶ168MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/09
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//     LCDģ��                STM32��Ƭ��    
//    SDI(MOSI)      ��          PB5          //Һ����SPI��������д�ź�
//    SDO(MISO)      ��          PB4          //Һ����SPI�������ݶ��źţ��������Ҫ�������Բ�����
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       LED         ��          PB13         //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
//       SCK         ��          PB3          //Һ����SPI����ʱ���ź�
//      DC/RS        ��          PB14         //Һ��������/��������ź�
//       RST         ��          PB12         //Һ������λ�����ź�
//       CS          ��          PB15         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PB1          //�����������ж��ź�
//      T_DO         ��          PB2          //������SPI���߶��ź�
//      T_DIN        ��          PF11         //������SPI����д�ź�
//      T_CS         ��          PC5          //������Ƭѡ�����ź�
//      T_CLK        ��          PB0          //������SPI����ʱ���ź�
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "key.h" 
#include "led.h"
#include "pic.h"

//========================variable==========================//
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//������ɫ����
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09 
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/ 
void DrawTestPage(u8 *str)
{
//���ƹ̶���up
LCD_Clear(WHITE);
LCD_Fill(0,0,lcddev.width,20,BLUE);
//���ƹ̶���down
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
//Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"http://www.lcdwiki.com",16,1);//������ʾ
//���Ʋ�������
//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
	DrawTestPage("ȫ�������ۺϲ��Գ���");	
	Gui_StrCenter(0,30,RED,BLUE,"ȫ������",16,1);//������ʾ
	Gui_StrCenter(0,60,RED,BLUE,"�ۺϲ��Գ���",16,1);//������ʾ	
	Gui_StrCenter(0,90,GREEN,BLUE,"2.4\" ILI9341 240X320",16,1);//������ʾ
	Gui_StrCenter(0,120,BLUE,BLUE,"xiaoFeng@QDtech 2018-08-20",16,1);//������ʾ
	delay_ms(1500);		
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09 
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	//DrawTestPage("����1:��ɫ������");
	LCD_Fill(0,0,lcddev.width,lcddev.height,WHITE);
	Show_Str(20,30,BLUE,YELLOW,"BL Test",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width,lcddev.height,RED);
	Show_Str(20,30,BLUE,YELLOW,"RED ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width,lcddev.height,GREEN);
	Show_Str(20,30,BLUE,YELLOW,"GREEN ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width,lcddev.height,BLUE);
	Show_Str(20,30,RED,YELLOW,"BLUE ",16,1);delay_ms(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09 
 * @function   :Rectangular display and fill test
								Display red,green,blue,yellow,pink rectangular boxes in turn,
								1500 milliseconds later,
								Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("����2:GUI����������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawFillRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09 
 * @function   :circular display and fill test
								Display red,green,blue,yellow,pink circular boxes in turn,
								1500 milliseconds later,
								Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("����3:GUI��Բ������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,0);
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
	DrawTestPage("����5:Ӣ����ʾ����");
	Show_Str(10,30,BLUE,YELLOW,"6X12:abcdefghijklmnopqrstuvwxyz0123456789",12,0);
	Show_Str(10,45,BLUE,YELLOW,"6X12:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",12,1);
	Show_Str(10,60,BLUE,YELLOW,"6X12:~!@#$%^&*()_+{}:<>?/|-+.",12,0);
	Show_Str(10,80,BLUE,YELLOW,"8X16:abcdefghijklmnopqrstuvwxyz0123456789",16,0);
	Show_Str(10,100,BLUE,YELLOW,"8X16:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",16,1);
	Show_Str(10,120,BLUE,YELLOW,"8X16:~!@#$%^&*()_+{}:<>?/|-+.",16,0); 
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09 
 * @function   :triangle display and fill test
								Display red,green,blue,yellow,pink triangle boxes in turn,
								1500 milliseconds later,
								Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	u8 i=0;
	DrawTestPage("����4:GUI Triangle������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Draw_Triangel(lcddev.width/2-80+(i*20),lcddev.height/2-20+(i*15),lcddev.width/2-50-1+(i*20),lcddev.height/2-20-52-1+(i*15),lcddev.width/2-20-1+(i*20),lcddev.height/2-20+(i*15));
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Fill_Triangel(lcddev.width/2-80+(i*20),lcddev.height/2-20+(i*15),lcddev.width/2-50-1+(i*20),lcddev.height/2-20-52-1+(i*15),lcddev.width/2-20-1+(i*20),lcddev.height/2-20+(i*15));
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09 
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{	
	DrawTestPage("����6:������ʾ����");
	Show_Str(10,30,BLUE,YELLOW,"16X16:ȫ�����Ӽ������޹�˾��ӭ��",16,0);
	Show_Str(10,50,BLUE,YELLOW,"16X16:Welcomeȫ������",16,0);
	Show_Str(10,70,BLUE,YELLOW,"24X24:���������Ĳ���",24,1);
	Show_Str(10,100,BLUE,YELLOW,"32X32:�������",32,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09 
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Pic_test(void)
{
	DrawTestPage("����7:ͼƬ��ʾ����");
	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	Gui_Drawbmp16(30,30,gImage_qq);
	Show_Str(30+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(90,30,gImage_qq);
	Show_Str(90+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(150,30,gImage_qq);
	Show_Str(150+12,75,BLUE,YELLOW,"QQ",16,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09 
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rotate_Test(void)
{
	u8 i=0;
	u8 *Direction[4]={"Rotation:0","Rotation:90","Rotation:180","Rotation:270"};
	
	for(i=0;i<4;i++)
	{
	LCD_direction(i);
	DrawTestPage("����8:��Ļ��ת����");
	Show_Str(20,30,BLUE,YELLOW,Direction[i],16,1);
	Gui_Drawbmp16(30,50,gImage_qq);
	delay_ms(1000);delay_ms(1000);
	}
	LCD_direction(USE_HORIZONTAL);
}

/*****************************************************************************
 * @name       :void Touch_Test(void)
 * @date       :2018-08-09 
 * @function   :touch test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Touch_Test(void)
{
	u8 key;
	u16 i=0;
	u16 j=0;
	u16 colorTemp=0;
	TP_Init();
	KEY_Init();
	LED_Init();
	DrawTestPage("����9:Touch(��KEY0У׼)      ");
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
	POINT_COLOR=RED;
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,RED); 
		while(1)
	{
	 	key=KEY_Scan(1);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
		 	if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
			{	
				if(tp_dev.x>(lcddev.width-24)&&tp_dev.y<16)
				{
					DrawTestPage("����9:Touch(��KEY0У׼)      ");//���
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
					POINT_COLOR=colorTemp;
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
				}
				else if((tp_dev.x>(lcddev.width-60)&&tp_dev.x<(lcddev.width-50+20))&&tp_dev.y<20)
				{
				LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,ColorTab[j%5]); 
				POINT_COLOR=ColorTab[(j++)%5];
				colorTemp=POINT_COLOR;
				delay_ms(10);
				}

				else TP_Draw_Big_Point(tp_dev.x,tp_dev.y,POINT_COLOR);		//��ͼ	  			   
			}
		}else delay_ms(10);	//û�а������µ�ʱ�� 	    
		if(key==1)	//KEY_RIGHT����,��ִ��У׼����
		{

			LCD_Clear(WHITE);//����
		    TP_Adjust();  //��ĻУ׼ 
			TP_Save_Adjdata();	 
			DrawTestPage("����9:Touch(��KEY0У׼)      ");
			LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
			POINT_COLOR=colorTemp;
			LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
		}
		i++;
		if(i==30)
		{
			i=0;
			LED0=!LED0;
			//break;
		}
	}   
}

// ȫ�ֱ���������ʪ�ȱ�����أ�
static u16 temp = 255;           // ��ǰ�¶ȣ���10��25.5�棩
static u16 humi = 600;           // ��ǰʪ�ȣ���10��60.0%RH��
static u16 alarm_temp = 250;     // �¶ȱ�����ֵ����10��25.0�棩
static u16 alarm_humi = 600;     // ʪ�ȱ�����ֵ����10��60.0%RH��
static u8 is_sliding = 0;        // �Ƿ��϶����飨0=��
static u8 slide_type = 0;        // �϶����ͣ�1=�¶Ȼ��飬2=ʪ�Ȼ���
static u16 temp_slider_x = 120;  // �¶Ȼ����ʼX����
static u16 humi_slider_x = 120;  // ʪ�Ȼ����ʼX����
static u8 temp_alarm_state = 0;  // �¶ȱ���״̬��0=δ������
static u8 humi_alarm_state = 0;  // ʪ�ȱ���״̬��0=δ������
static u32 temp_alarm_tick = 0;  // �¶ȱ�����ʱ
static u32 humi_alarm_tick = 0;  // ʪ�ȱ�����ʱ
#define ALARM_FLASH_MS 500// ������˸���ڣ����룩

// ������������˫���鲼�֣��������У�
#define TEMP_TRACK_Y 200  // �¶Ȼ�����Y����
#define HUMI_TRACK_Y 220  // ʪ�Ȼ�����Y����
#define SLIDER_TRACK_H 8  // ����߶�
#define SLIDER_W 12       // �������
#define SLIDER_H 20       // ����߶�
#define TRACK_MIN_X 30    // �����߽�
#define TRACK_MAX_X 210   // ����ұ߽�
#define TEMP_MIN 100      // �¶���ֵ��Χ��10.0~40.0�棩
#define TEMP_MAX 400
#define HUMI_MIN 300      // ʪ����ֵ��Χ��30.0~80.0%RH��
#define HUMI_MAX 800

// Temperature-Humidity-Alarm GUI����ʪ��˫�����棩
void Temp_Humi_Alarm_GUI(void)
{
    u32 tick = 0; 
    u8 key;          
    u16 colorTemp = BLUE; 
    u16 colorHumi = 0x07E0; // ��ɫ��ʪ�������ɫ��

    // �����ʼ��
    DrawTestPage("Temp-Humi-Alarm");
    LCD_Fill(0, 20, lcddev.width, lcddev.height-20, WHITE); 
    
    // ���������߿�
    POINT_COLOR = ColorTab[0];
    LCD_DrawRectangle(10, 30, 230, 280);
    POINT_COLOR = WHITE;
    LCD_DrawFillRectangle(10, 30, 230, 280);
    
    // �������б�ǩ������ʪ�ȱ�����ǩ��
    POINT_COLOR = BLACK;
    Show_Str(30, 50, BLACK, WHITE, "Current Temp:", 16, 0);
    Show_Str(30, 100, BLACK, WHITE, "Current Humi:", 16, 0);
    Show_Str(30, 150, BLACK, WHITE, "Alarm Temp:", 16, 0);
    Show_Str(30, 170, BLACK, WHITE, "Alarm Humi:", 16, 0); // ����ʪ�ȱ�����ǩ
    Show_Str(30, 240, BLACK, WHITE, "Drag Slider to Set", 16, 0);

    // ��ʼ������Ԫ�أ�����ʪ�ȱ�����أ�
    POINT_COLOR = BLACK;
    // 1. ��ǰ��ʪ��
    LCD_ShowNum(150, 50, temp/10, 2, 16);
    LCD_ShowChar(175, 50, BLACK, WHITE, '.', 16, 0);
    LCD_ShowNum(180, 50, temp%10, 1, 16);
    Show_Str(185, 50, BLACK, WHITE, "��", 16, 0);
    
    LCD_ShowNum(150, 100, humi/10, 2, 16);
    LCD_ShowChar(175, 100, BLACK, WHITE, '.', 16, 0);
    LCD_ShowNum(180, 100, humi%10, 1, 16);
    Show_Str(185, 100, BLACK, WHITE, "%RH", 16, 0);
    
    // 2. ������ֵ������ʪ����ֵ��ʾ��
    LCD_ShowNum(150, 150, alarm_temp/10, 2, 24);
    LCD_ShowChar(175, 150, BLACK, WHITE, '.', 24, 0);
    LCD_ShowNum(180, 150, alarm_temp%10, 1, 24);
    Show_Str(185, 150, BLACK, WHITE, "��", 24, 0);
    
    LCD_ShowNum(150, 170, alarm_humi/10, 2, 24);
    LCD_ShowChar(175, 170, BLACK, WHITE, '.', 24, 0);
    LCD_ShowNum(180, 170, alarm_humi%10, 1, 24);
    Show_Str(185, 170, BLACK, WHITE, "%RH", 24, 0);
    
    // 3. ��������˫������������У�
    // �¶ȹ������ɫ��
    POINT_COLOR = 0xC618;
    LCD_DrawFillRectangle(TRACK_MIN_X, TEMP_TRACK_Y, TRACK_MAX_X, TEMP_TRACK_Y + SLIDER_TRACK_H);
    // ʪ�ȹ����ǳ��ɫ��
    POINT_COLOR = 0xE6E6;
    LCD_DrawFillRectangle(TRACK_MIN_X, HUMI_TRACK_Y, TRACK_MAX_X, HUMI_TRACK_Y + SLIDER_TRACK_H);
    
    // 4. ���飨�¶���ɫ��ʪ����ɫ��
    POINT_COLOR = colorTemp;
    LCD_DrawFillRectangle(temp_slider_x - SLIDER_W/2, TEMP_TRACK_Y - (SLIDER_H - SLIDER_TRACK_H)/2, 
                          temp_slider_x + SLIDER_W/2, TEMP_TRACK_Y + (SLIDER_H + SLIDER_TRACK_H)/2);
    
    POINT_COLOR = colorHumi;
    LCD_DrawFillRectangle(humi_slider_x - SLIDER_W/2, HUMI_TRACK_Y - (SLIDER_H - SLIDER_TRACK_H)/2, 
                          humi_slider_x + SLIDER_W/2, HUMI_TRACK_Y + (SLIDER_H + SLIDER_TRACK_H)/2);

    while(1)
    {
        // ����У׼���ָ�����Ԫ�أ�����ʪ����أ�
        key = KEY_Scan(1);
        if(key == 1)
        {
            LCD_Clear(WHITE);
            TP_Adjust();  
            TP_Save_Adjdata();
            // �ָ����棨�ظ���ʼ�����裬ʡ��������룩
            DrawTestPage("Temp-Humi-Alarm");
            LCD_Fill(0, 20, lcddev.width, lcddev.height-20, WHITE);
            POINT_COLOR = ColorTab[0];
            LCD_DrawRectangle(10, 30, 230, 280);
            POINT_COLOR = WHITE;
            LCD_DrawFillRectangle(10, 30, 230, 280);
            // �ػ��ǩ����ֵ��˫�����˫���飨ͬ��ʼ����
        }

        // ������⣨˫���������϶���
        tp_dev.scan(0); 
        if(tp_dev.sta & TP_PRES_DOWN) 
        {
            u16 curr_x = tp_dev.x;
            u16 curr_y = tp_dev.y;
            
            // ����¶Ȼ��������Ϲ����
            if(curr_y >= TEMP_TRACK_Y - SLIDER_H && curr_y <= TEMP_TRACK_Y + SLIDER_H)
            {
                is_sliding = 1;
                slide_type = 1;
                // ����X�����ڹ����
                curr_x = (curr_x < TRACK_MIN_X) ? TRACK_MIN_X : (curr_x > TRACK_MAX_X ? TRACK_MAX_X : curr_x);
                temp_slider_x = curr_x;
                // �����¶ȱ�����ֵ
                alarm_temp = TEMP_MIN + (temp_slider_x - TRACK_MIN_X) * (TEMP_MAX - TEMP_MIN) / (TRACK_MAX_X - TRACK_MIN_X);
                // ˢ���¶Ȼ������ֵ��ʾ
                POINT_COLOR = 0xC618;
                LCD_DrawFillRectangle(TRACK_MIN_X, TEMP_TRACK_Y, TRACK_MAX_X, TEMP_TRACK_Y + SLIDER_TRACK_H);
                POINT_COLOR = colorTemp;
                LCD_DrawFillRectangle(temp_slider_x - SLIDER_W/2, TEMP_TRACK_Y - (SLIDER_H - SLIDER_TRACK_H)/2, 
                                      temp_slider_x + SLIDER_W/2, TEMP_TRACK_Y + (SLIDER_H + SLIDER_TRACK_H)/2);
                POINT_COLOR = WHITE;
                LCD_Fill(150, 150, 210, 175, WHITE);
                POINT_COLOR = BLACK;
                LCD_ShowNum(150, 150, alarm_temp/10, 2, 24);
                LCD_ShowChar(175, 150, BLACK, WHITE, '.', 24, 0);
                LCD_ShowNum(180, 150, alarm_temp%10, 1, 24);
                Show_Str(185, 150, BLACK, WHITE, "��", 24, 0);
            }
            // ���ʪ�Ȼ��������¹����
            else if(curr_y >= HUMI_TRACK_Y - SLIDER_H && curr_y <= HUMI_TRACK_Y + SLIDER_H)
            {
                is_sliding = 1;
                slide_type = 2;
                // ����X�����ڹ����
                curr_x = (curr_x < TRACK_MIN_X) ? TRACK_MIN_X : (curr_x > TRACK_MAX_X ? TRACK_MAX_X : curr_x);
                humi_slider_x = curr_x;
                // ����ʪ�ȱ�����ֵ
                alarm_humi = HUMI_MIN + (humi_slider_x - TRACK_MIN_X) * (HUMI_MAX - HUMI_MIN) / (TRACK_MAX_X - TRACK_MIN_X);
                // ˢ��ʪ�Ȼ������ֵ��ʾ
                POINT_COLOR = 0xE6E6;
                LCD_DrawFillRectangle(TRACK_MIN_X, HUMI_TRACK_Y, TRACK_MAX_X, HUMI_TRACK_Y + SLIDER_TRACK_H);
                POINT_COLOR = colorHumi;
                LCD_DrawFillRectangle(humi_slider_x - SLIDER_W/2, HUMI_TRACK_Y - (SLIDER_H - SLIDER_TRACK_H)/2, 
                                      humi_slider_x + SLIDER_W/2, HUMI_TRACK_Y + (SLIDER_H + SLIDER_TRACK_H)/2);
                POINT_COLOR = WHITE;
                LCD_Fill(150, 170, 210, 195, WHITE);
                POINT_COLOR = BLACK;
                LCD_ShowNum(150, 170, alarm_humi/10, 2, 24);
                LCD_ShowChar(175, 170, BLACK, WHITE, '.', 24, 0);
                LCD_ShowNum(180, 170, alarm_humi%10, 1, 24);
                Show_Str(185, 170, BLACK, WHITE, "%RH", 24, 0);
            }
        }
        else 
        {
            is_sliding = 0;
            slide_type = 0;
        }

        // ÿ�������ʪ�ȣ�ģ��仯��
        if(++tick >= 1000)
        {
            tick = 0;
            temp += (rand()%3 - 1);
            temp = (temp < 100) ? 100 : (temp > 400 ? 400 : temp);
            humi += (rand()%3 - 1);
            humi = (humi < 300) ? 300 : (humi > 800 ? 800 : humi);

            // ������ʪ����ʾ
            POINT_COLOR = BLACK;
            LCD_Fill(150, 50, 210, 75, WHITE);
            LCD_ShowNum(150, 50, temp/10, 2, 16);
            LCD_ShowChar(175, 50, BLACK, WHITE, '.', 16, 0);
            LCD_ShowNum(180, 50, temp%10, 1, 16);
            Show_Str(185, 50, BLACK, WHITE, "��", 16, 0);
            LCD_Fill(150, 100, 210, 125, WHITE);
            LCD_ShowNum(150, 100, humi/10, 2, 16);
            LCD_ShowChar(175, 100, BLACK, WHITE, '.', 16, 0);
            LCD_ShowNum(180, 100, humi%10, 1, 16);
            Show_Str(185, 100, BLACK, WHITE, "%RH", 16, 0);
        }

        // �¶ȱ�����ʵʱ���+��������˸
        if(temp >= alarm_temp)
        {
            temp_alarm_state = 1;
        }
        else
        {
            temp_alarm_state = 0;
            POINT_COLOR = WHITE;
            LCD_Fill(30, 260, 110, 280, WHITE); // �¶ȱ���������벿�֣�
        }
        if(temp_alarm_state)
        {
            static u8 temp_flash = 0;
            temp_alarm_tick++;
            if(temp_alarm_tick >= ALARM_FLASH_MS / 10)
            {
                temp_alarm_tick = 0;
                temp_flash = !temp_flash;
                if(temp_flash)
                {
                    POINT_COLOR = 0xF800; // ��ɫ
                    LCD_Fill(30, 260, 110, 280, 0xF800);
                    Show_Str(35, 260, WHITE, 0xF800, "TEMP!", 16, 0);
                }
                else LCD_Fill(30, 260, 110, 280, WHITE);
            }
        }

        // ʪ�ȱ�����ʵʱ���+��������˸����������
        if(humi >= alarm_humi)
        {
            humi_alarm_state = 1;
        }
        else
        {
            humi_alarm_state = 0;
            POINT_COLOR = WHITE;
            LCD_Fill(130, 260, 210, 280, WHITE); // ʪ�ȱ��������Ұ벿�֣�
        }
        if(humi_alarm_state)
        {
            static u8 humi_flash = 0;
            humi_alarm_tick++;
            if(humi_alarm_tick >= ALARM_FLASH_MS / 10)
            {
                humi_alarm_tick = 0;
                humi_flash = !humi_flash;
                if(humi_flash)
                {
                    POINT_COLOR = 0x07E0; // ��ɫ
                    LCD_Fill(130, 260, 210, 280, 0x07E0);
                    Show_Str(135, 260, WHITE, 0x07E0, "HUMI!", 16, 0);
                }
                else LCD_Fill(130, 260, 210, 280, WHITE);
            }
        }

        delay_ms(10);
    }
}

