//#include "oled.h"
//#include "stdlib.h"
//#include "oledfont.h"  	 
//#include "board.h"

//// OLED显存数组：144列(宽)×8页(高，每页8像素)，存储待显示的像素数据
//u8 OLED_GRAM[144][8];

///**********************************************************
// * 函数名称：OLED_ColorTurn
// * 函数功能：设置OLED显示颜色模式（正常/反色）
// * 参数说明：i - 0：正常显示；1：反色显示
// * 实现说明：通过写入OLED指令0xA6（正常）或0xA7（反色）控制显示模式
//**********************************************************/
//void OLED_ColorTurn(u8 i)
//{
//	if(i==0)
//	{
//		OLED_WR_Byte(0xA6,OLED_CMD);// 正常显示：像素值1点亮，0熄灭
//	}
//	if(i==1)
//	{
//		OLED_WR_Byte(0xA7,OLED_CMD);// 反色显示：像素值0点亮，1熄灭
//	}
//}

///**********************************************************
// * 函数名称：OLED_DisplayTurn
// * 函数功能：设置OLED屏幕旋转方向（0°/180°）
// * 参数说明：i - 0：正常方向；1：旋转180°
// * 实现说明：通过设置COM扫描方向和SEG映射方向实现旋转
//**********************************************************/
//void OLED_DisplayTurn(u8 i)
//{
//	if(i==0)
//	{
//		OLED_WR_Byte(0xC8,OLED_CMD);// COM扫描方向：正常（从下到上）
//		OLED_WR_Byte(0xA1,OLED_CMD);// SEG列映射：正常（从左到右）
//	}
//	if(i==1)
//	{
//		OLED_WR_Byte(0xC0,OLED_CMD);// COM扫描方向：反向（从上到下）
//		OLED_WR_Byte(0xA0,OLED_CMD);// SEG列映射：反向（从右到左）
//	}
//}

///**********************************************************
// * 函数名称：IIC_delay
// * 函数功能：I2C通信时序延迟
// * 功能说明：保证I2C信号电平稳定的最小延迟（约13us）
//**********************************************************/
//void IIC_delay(void)
//{
//	delay_us(13);
//}

///**********************************************************
// * 函数名称：I2C_Start
// * 函数功能：产生I2C起始信号
// * 时序说明：SCL高电平时，SDA从高到低跳变
//**********************************************************/
//void I2C_Start(void)
//{
//	OLED_SDA_Set();// SDA置高
//	OLED_SCL_Set();// SCL置高
//	IIC_delay();
//	OLED_SDA_Clr();// SDA拉低（起始信号）
//	IIC_delay();
//	OLED_SCL_Clr();// SCL拉低，准备发送数据
//	IIC_delay();
//}

///**********************************************************
// * 函数名称：I2C_Stop
// * 函数功能：产生I2C停止信号
// * 时序说明：SCL高电平时，SDA从低到高跳变
//**********************************************************/
//void I2C_Stop(void)
//{
//	OLED_SDA_Clr();// SDA置低
//	OLED_SCL_Set();// SCL置高
//	IIC_delay();
//	OLED_SDA_Set();// SDA拉高（停止信号）
//}

///**********************************************************
// * 函数名称：I2C_WaitAck
// * 函数功能：等待I2C从机应答
// * 功能说明：等待OLED模块对数据的应答信号，确保通信正常
//**********************************************************/
//void I2C_WaitAck(void) 
//{
//	OLED_SDA_Set();// 释放SDA线，等待从机应答
//	IIC_delay();
//	OLED_SCL_Set();// SCL置高，读取应答
//	IIC_delay();
//	OLED_SCL_Clr();// SCL拉低，结束应答检测
//	IIC_delay();
//}

///**********************************************************
// * 函数名称：Send_Byte
// * 函数功能：通过I2C发送一个字节数据
// * 参数说明：dat - 待发送的字节
// * 实现说明：高位在前，逐位发送，每发送一位后SCL电平翻转
//**********************************************************/
//void Send_Byte(u8 dat)
//{
//	u8 i;
//	for(i=0;i<8;i++)// 循环发送8位
//	{
//		if(dat&0x80)// 发送高位
//		{
//			OLED_SDA_Set();
//		}
//		else
//		{
//			OLED_SDA_Clr();
//		}
//		IIC_delay();
//		OLED_SCL_Set();// SCL高电平，从机采样
//		IIC_delay();
//		OLED_SCL_Clr();// SCL低电平，准备下一位
//		dat<<=1;// 移位到下一位
//	}
//}

///**********************************************************
// * 函数名称：OLED_WR_Byte
// * 函数功能：向OLED写入一个字节（命令或数据）
// * 参数说明：dat - 待写入的字节；mode - 0：命令；1：数据
// * 通信说明：OLED I2C地址为0x78，命令模式发送0x00，数据模式发送0x40
//**********************************************************/
//void OLED_WR_Byte(u8 dat,u8 mode)
//{
//	I2C_Start();// 起始信号
//	Send_Byte(0x78);// 发送OLED设备地址（写）
//	I2C_WaitAck();// 等待应答
//	if(mode){Send_Byte(0x40);}// 数据模式：0x40
//	else{Send_Byte(0x00);}// 命令模式：0x00
//	I2C_WaitAck();// 等待应答
//	Send_Byte(dat);// 发送数据/命令
//	I2C_WaitAck();// 等待应答
//	I2C_Stop();// 停止信号
//}

///**********************************************************
// * 函数名称：OLED_DisPlay_On
// * 函数功能：开启OLED显示
// * 实现说明：开启电荷泵并点亮屏幕
//**********************************************************/
//void OLED_DisPlay_On(void)
//{
//	OLED_WR_Byte(0x8D,OLED_CMD);// 电荷泵控制
//	OLED_WR_Byte(0x14,OLED_CMD);// 开启电荷泵
//	OLED_WR_Byte(0xAF,OLED_CMD);// 点亮屏幕
//}

///**********************************************************
// * 函数名称：OLED_DisPlay_Off
// * 函数功能：关闭OLED显示
// * 实现说明：关闭电荷泵并关闭屏幕
//**********************************************************/
//void OLED_DisPlay_Off(void)
//{
//	OLED_WR_Byte(0x8D,OLED_CMD);// 电荷泵控制
//	OLED_WR_Byte(0x10,OLED_CMD);// 关闭电荷泵
//	OLED_WR_Byte(0xAE,OLED_CMD);// 关闭屏幕
//}

///**********************************************************
// * 函数名称：OLED_Refresh
// * 函数功能：刷新显存数据到OLED屏幕
// * 实现说明：按页发送显存中的数据，从第0页到第7页
//**********************************************************/
//void OLED_Refresh(void)
//{
//	u8 i,n;
//	for(i=0;i<8;i++)// 共8页（0-7）
//	{
//		OLED_WR_Byte(0xb0+i,OLED_CMD); // 设置页地址（0xb0-0xb7）
//		OLED_WR_Byte(0x00,OLED_CMD);   // 设置列地址低4位
//		OLED_WR_Byte(0x10,OLED_CMD);   // 设置列地址高4位
//		I2C_Start();
//		Send_Byte(0x78);
//		I2C_WaitAck();
//		Send_Byte(0x40);
//		I2C_WaitAck();
//		for(n=0;n<128;n++)// 发送当前页的128列数据
//		{
//			Send_Byte(OLED_GRAM[n][i]);
//			I2C_WaitAck();
//		}
//		I2C_Stop();
//	}
//}

///**********************************************************
// * 函数名称：OLED_Clear
// * 函数功能：清空OLED显存并刷新显示
// * 实现说明：将显存数组全部置0，然后刷新屏幕
//**********************************************************/
//void OLED_Clear(void)
//{
//	u8 i,n;
//	for(i=0;i<8;i++)
//	{
//		for(n=0;n<128;n++)
//		{
//			OLED_GRAM[n][i]=0;// 清空显存
//		}
//	}
//	OLED_Refresh();// 刷新显示
//}

///**********************************************************
// * 函数名称：OLED_DrawPoint
// * 函数功能：在指定位置画点
// * 参数说明：x - 横坐标（0-127）；y - 纵坐标（0-63）；t - 1：点亮；0：熄灭
// * 实现说明：根据坐标计算所在页和位，更新显存对应位
//**********************************************************/
//void OLED_DrawPoint(u8 x,u8 y,u8 t)
//{
//	u8 i,m,n;
//	i=y/8;// 计算所在页（y=0-63对应页0-7）
//	m=y%8;// 计算页内位偏移（0-7）
//	n=1<<m;// 生成位掩码
//	if(t){OLED_GRAM[x][i]|=n;}// 点亮：置位
//	else
//	{
//		OLED_GRAM[x][i]&=~n;// 熄灭：清位
//	}
//}

///**********************************************************
// * 函数名称：OLED_DrawLine
// * 函数功能：绘制直线（Bresenham算法）
// * 参数说明：x1,y1 - 起点坐标；x2,y2 - 终点坐标；mode - 1：实线；0：虚线（未实现）
// * 算法说明：通过误差累积实现整数运算的直线绘制，避免浮点计算
//**********************************************************/
//void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
//{
//	u16 t; 
//	int xerr=0,yerr=0,delta_x,delta_y,distance;
//	int incx,incy,uRow,uCol;
//	delta_x=x2-x1; // 计算x方向增量
//	delta_y=y2-y1; // 计算y方向增量
//	uRow=x1;// 起点x坐标
//	uCol=y1;// 起点y坐标
//	// 确定x方向步进方向
//	if(delta_x>0)incx=1; 
//	else if (delta_x==0)incx=0;// 垂直直线
//	else {incx=-1;delta_x=-delta_x;}
//	// 确定y方向步进方向
//	if(delta_y>0)incy=1;
//	else if (delta_y==0)incy=0;// 水平直线
//	else {incy=-1;delta_y=-delta_y;}
//	// 选择长轴作为迭代次数
//	if(delta_x>delta_y)distance=delta_x; 
//	else distance=delta_y;
//	// 迭代绘制直线
//	for(t=0;t<distance+1;t++)
//	{
//		OLED_DrawPoint(uRow,uCol,mode);// 画点
//		xerr+=delta_x;
//		yerr+=delta_y;
//		// x方向误差超限，步进x
//		if(xerr>distance)
//		{
//			xerr-=distance;
//			uRow+=incx;
//		}
//		// y方向误差超限，步进y
//		if(yerr>distance)
//		{
//			yerr-=distance;
//			uCol+=incy;
//		}
//	}
//}

///**********************************************************
// * 函数名称：OLED_DrawCircle
// * 函数功能：绘制圆形（中点圆算法）
// * 参数说明：x,y - 圆心坐标；r - 半径
// * 算法说明：通过对称原理，计算圆上点并绘制，减少计算量
//**********************************************************/
//void OLED_DrawCircle(u8 x,u8 y,u8 r)
//{
//	int a, b,num;
//	a = 0;
//	b = r;
//	// 利用圆的8对称性绘制
//	while(2 * b * b >= r * r)      
//	{
//		OLED_DrawPoint(x + a, y - b,1);
//		OLED_DrawPoint(x - a, y - b,1);
//		OLED_DrawPoint(x - a, y + b,1);
//		OLED_DrawPoint(x + a, y + b,1);
//		OLED_DrawPoint(x + b, y + a,1);
//		OLED_DrawPoint(x + b, y - a,1);
//		OLED_DrawPoint(x - b, y - a,1);
//		OLED_DrawPoint(x - b, y + a,1);
//		a++;
//		// 计算当前点与圆的误差，调整b
//		num = (a * a + b * b) - r*r;
//		if(num > 0)
//		{
//			b--;
//			a--;
//		}
//	}
//}

///**********************************************************
// * 函数名称：OLED_ShowChar
// * 函数功能：显示单个字符
// * 参数说明：x,y - 起点坐标；chr - 字符ASCII码；size1 - 字体大小；mode - 显示模式
// * 字体说明：支持6x8/12x6/16x8/24x12，从字库数组取模数据绘制
//**********************************************************/
//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
//{
//	u8 i,m,temp,size2,chr1;
//	u8 x0=x,y0=y;
//	if(size1==8)size2=6;// 6x8字体：宽6像素
//	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  // 计算其他字体的字节数
//	chr1=chr-' ';  // 计算字符在字库中的偏移（从空格开始）
//	for(i=0;i<size2;i++)
//	{
//		// 根据字体大小取字库数据
//		if(size1==8)temp=asc2_0806[chr1][i];
//		else if(size1==12)temp=asc2_1206[chr1][i];
//		else if(size1==16)temp=asc2_1608[chr1][i];
//		else if(size1==24)temp=asc2_2412[chr1][i];
//		else return;
//		// 逐位绘制
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp>>=1;
//			y++;
//		}
//		x++;
//		// 换行处理（非8x6字体）
//		if((size1!=8)&&((x-x0)==size1/2))
//		{
//			x=x0;
//			y0=y0+8;
//		}
//		y=y0;
//	}
//}

///**********************************************************
// * 函数名称：OLED_ShowString
// * 函数功能：显示字符串
// * 参数说明：
// *   x - 字符串起点横坐标（0-127）
// *   y - 字符串起点纵坐标（0-63）
// *   *chr - 待显示字符串的指针（ASCII字符）
// *   size1 - 字体大小（6x8/12x6/16x8/24x12）
// *   mode - 显示模式（0：反色；1：正常）
// * 实现说明：循环调用OLED_ShowChar显示每个字符，自动计算字符间距
//**********************************************************/
//void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
//{
//	// 循环显示每个字符，直到遇到非可打印字符（ASCII范围：空格' '~'~'）
//	while((*chr>=' ')&&(*chr<='~'))
//	{
//		OLED_ShowChar(x,y,*chr,size1,mode); // 显示单个字符
//		// 根据字体大小计算下一个字符的X坐标偏移
//		if(size1==8)x+=6; // 6x8字体：字符宽6像素
//		else x+=size1/2;  // 其他字体：宽度为size1/2（如16x8字体宽8像素）
//		chr++; // 指针移至下一个字符
//	}
//}

///**********************************************************
// * 函数名称：OLED_Pow
// * 函数功能：计算m的n次方（辅助函数）
// * 参数说明：
// *   m - 底数
// *   n - 指数（非负整数）
// * 返回值：m^n的计算结果（u32类型）
// * 实现说明：通过循环乘法实现整数幂运算，用于数字显示时的位提取
//**********************************************************/
//u32 OLED_Pow(u8 m,u8 n)
//{
//	u32 result=1; // 结果初始化为1（任何数的0次方为1）
//	while(n--) // 循环n次，每次乘以m
//	{
//		result*=m;
//	}
//	return result;
//}

///**********************************************************
// * 函数名称：OLED_ShowNum
// * 函数功能：显示整数（支持正负号、指定长度补零）
// * 参数说明：
// *   x - 数字起点横坐标（0-127）
// *   y - 数字起点纵坐标（0-63）
// *   num - 待显示的整数（支持负数）
// *   len - 数字显示位数（不足补零，超出截断）
// *   size1 - 字体大小（6x8/12x6/16x8/24x12）
// *   mode - 显示模式（0：反色；1：正常）
// * 实现说明：处理负数符号，计算数字位数，补零后逐位显示
//**********************************************************/
//void OLED_ShowNum(u8 x, u8 y, int num, u8 len, u8 size1, u8 mode)
//{
//	u8 t, temp, m = 0; 
//	if (size1 == 8)m = 2; // 6x8字体额外偏移量（调整字符间距）
//	
//	// 处理负数：显示负号并转为正数
//	if (num < 0) 
//	{
//		num = -num;        
//		OLED_ShowChar(x, y, '-', size1, mode); // 显示负号
//		len--; // 位数减1（已用1位显示负号）
//		x += size1 / 2 + m; // 移动到数字起始位置
//	}
//	
//	// 计算数字实际位数
//	u8 digits = 0;
//	int tempNum = num;  
//	if (tempNum == 0)digits = 1; // 0的位数为1
//	else
//	{
//		while (tempNum > 0)
//		{
//			digits++;
//			tempNum /= 10;
//		}
//	}
//	
//	// 计算需要补的前导零数量
//	u8 leadingZeros = len - digits;
//	// 显示前导零
//	for (t = 0; t < leadingZeros; t++)
//	{
//		OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
//	}
//	
//	// 逐位显示数字
//	for (t = 0; t < digits; t++)
//	{
//		// 提取当前位数字（从高位到低位）
//		temp = (num / OLED_Pow(10, digits - t - 1)) % 10;
//		// 显示当前位（数字转字符：+ '0'）
//		OLED_ShowChar(x + (size1 / 2 + m) * (t + leadingZeros), y, temp + '0', size1, mode);
//	}
//}

///**********************************************************
// * 函数名称：OLED_ShowChinese
// * 函数功能：显示中文字符
// * 参数说明：
// *   x - 汉字起点横坐标（0-127）
// *   y - 汉字起点纵坐标（0-63）
// *   num - 中文字库索引（对应字库数组中的位置）
// *   size1 - 字体大小（16/24/32/64，单位：像素）
// *   mode - 显示模式（0：反色；1：正常）
// * 实现说明：从汉字字库中读取点阵数据，逐点绘制汉字
//**********************************************************/
//void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
//{
//	u8 m,temp;
//	u8 x0=x,y0=y; // 保存起点坐标，用于换行
//	// 计算汉字点阵数据总字节数（按页计算，每页8像素）
//	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  
//	
//	for(i=0;i<size3;i++) // 遍历汉字点阵数据
//	{
//		// 根据字体大小从对应字库取数据
//		if(size1==16)temp=Hzk1[num][i];// 16x16字库
//		else if(size1==24)temp=Hzk2[num][i];// 24x24字库
//		else if(size1==32)temp=Hzk3[num][i];// 32x32字库
//		else if(size1==64)temp=Hzk4[num][i];// 64x64字库
//		else return; // 不支持的字体大小
//		
//		// 逐位绘制当前字节的8个像素
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode); // 位为1：点亮
//			else OLED_DrawPoint(x,y,!mode); // 位为0：熄灭（或反色）
//			temp>>=1; // 移位到下一位
//			y++; // Y坐标递增（向下移动）
//		}
//		x++; // X坐标递增（向右移动）
//		
//		// 一行绘制完成（达到汉字宽度），换行
//		if((x-x0)==size1)
//		{
//			x=x0; // X坐标复位到起点
//			y0=y0+8; // Y坐标移至下一页（8像素）
//		}
//		y=y0; // Y坐标复位到当前页起点
//	}
//}

///**********************************************************
// * 函数名称：OLED_ScrollDisplay
// * 函数功能：滚动显示中文字符串
// * 参数说明：
// *   num - 待显示的汉字总数
// *   space - 汉字间的滚动暂停帧数（控制滚动速度）
// *   mode - 显示模式（0：反色；1：正常）
// * 实现说明：通过更新显存实现左移滚动效果，支持循环显示
//**********************************************************/
//void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
//{
//	u8 i,n,t=0,m=0,r;
//	while(1) // 无限循环实现持续滚动
//	{
//		// 每16帧添加一个新汉字到右侧
//		if(m==0)
//		{
//			// 在屏幕右侧外（x=128）绘制下一个汉字，准备进入视野
//			OLED_ShowChinese(128,24,t,16,mode); 
//			t++; // 切换到下一个汉字
//		}
//		
//		// 所有汉字显示完成后，暂停指定帧数
//		if(t==num)
//		{
//			// 暂停滚动（重复刷新当前画面space*16次）
//			for(r=0;r<16*space;r++)      
//			{
//				// 显存左移1列（实现滚动）
//				for(i=1;i<144;i++)
//				{
//					for(n=0;n<8;n++)
//					{
//						OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
//					}
//				}
//				OLED_Refresh(); // 刷新显示
//			}
//			t=0; // 重置汉字索引，循环显示
//		}
//		
//		m++; // 帧计数器递增
//		if(m==16)m=0; // 每16帧重置计数器
//		
//		// 显存左移1列（实现滚动效果）
//		for(i=1;i<144;i++)   
//		{
//			for(n=0;n<8;n++)
//			{
//				OLED_GRAM[i-1][n]=OLED_GRAM[i][n]; // 当前列数据覆盖前一列
//			}
//		}
//		OLED_Refresh(); // 刷新显示，呈现滚动效果
//	}
//}

///**********************************************************
// * 函数名称：OLED_ShowPicture
// * 函数功能：显示点阵图片
// * 参数说明：
// *   x - 图片起点横坐标（0-127）
// *   y - 图片起点纵坐标（0-63）
// *   sizex - 图片宽度（像素）
// *   sizey - 图片高度（像素）
// *   BMP[] - 图片点阵数据数组（按字节存储）
// *   mode - 显示模式（0：反色；1：正常）
// * 实现说明：从BMP数组读取点阵数据，逐点绘制图片
//**********************************************************/
//void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
//{
//	u16 j=0; // 点阵数据索引
//	u8 i,n,temp,m;
//	u8 x0=x,y0=y; // 保存起点坐标
//	// 计算图片高度对应的页数（每页8像素，向上取整）
//	sizey=sizey/8+((sizey%8)?1:0);
//	
//	// 按页遍历图片
//	for(n=0;n<sizey;n++)
//	{
//		// 按列遍历当前页
//		for(i=0;i<sizex;i++)
//		{
//			temp=BMP[j]; // 读取当前字节的点阵数据
//			j++; // 索引递增
//			
//			// 逐位绘制当前字节的8个像素
//			for(m=0;m<8;m++)
//			{
//				if(temp&0x01)OLED_DrawPoint(x,y,mode); // 位为1：点亮
//				else OLED_DrawPoint(x,y,!mode); // 位为0：熄灭（或反色）
//				temp>>=1; // 移位到下一位
//				y++; // Y坐标递增
//			}
//			x++; // X坐标递增（向右移动）
//			
//			// 一列绘制完成（达到图片宽度），换行
//			if((x-x0)==sizex)
//			{
//				x=x0; // X坐标复位到起点
//				y0=y0+8; // Y坐标移至下一页（8像素）
//			}
//			y=y0; // Y坐标复位到当前页起点
//		}
//	}
//}

///**********************************************************
// * 函数名称：OLED_Init
// * 函数功能：OLED屏幕初始化
// * 实现说明：配置GPIO引脚，发送初始化命令序列，初始化显存
//**********************************************************/
//void OLED_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	// 1. 初始化I2C引脚（SDA和SCL）
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); // 使能GPIOB时钟
//	
//	// 配置PB8（SCL）和PB9（SDA）为推挽输出
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // 通用输出模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速模式
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 上拉电阻（确保空闲时为高电平）
//	GPIO_Init(GPIOB, &GPIO_InitStructure); // 应用配置
//	
//	delay_ms(200); // 上电延时，确保OLED稳定
//	
//	// 2. 发送OLED控制器初始化命令
//	OLED_WR_Byte(0xAE,OLED_CMD); // 关闭显示（休眠模式）
//	OLED_WR_Byte(0x00,OLED_CMD); // 设置列地址低4位
//	OLED_WR_Byte(0x10,OLED_CMD); // 设置列地址高4位
//	OLED_WR_Byte(0x40,OLED_CMD); // 设置显示起始行（0x00~0x3F）
//	OLED_WR_Byte(0x81,OLED_CMD); // 对比度控制使能
//	OLED_WR_Byte(0xCF,OLED_CMD); // 设置对比度（0x00~0xFF，0xCF为中等亮度）
//	OLED_WR_Byte(0xA1,OLED_CMD); // 设置列映射（0xA1：正常；0xA0：反向）
//	OLED_WR_Byte(0xC8,OLED_CMD); // 设置行扫描方向（0xC8：正常；0xC0：反向）
//	OLED_WR_Byte(0xA6,OLED_CMD); // 正常显示（0xA6：正常；0xA7：反色）
//	OLED_WR_Byte(0xA8,OLED_CMD); // 设置多路复用率
//	OLED_WR_Byte(0x3f,OLED_CMD); // 64行复用（0x3F对应64行）
//	OLED_WR_Byte(0xD3,OLED_CMD); // 设置显示偏移
//	OLED_WR_Byte(0x00,OLED_CMD); // 无偏移
//	OLED_WR_Byte(0xD5,OLED_CMD); // 设置时钟分频和振荡器频率
//	OLED_WR_Byte(0x80,OLED_CMD); // 默认分频（100帧/秒）
//	OLED_WR_Byte(0xD9,OLED_CMD); // 设置预充电周期
//	OLED_WR_Byte(0xF1,OLED_CMD); // 预充电15时钟，放电1时钟
//	OLED_WR_Byte(0xDA,OLED_CMD); // 设置COM引脚硬件配置
//	OLED_WR_Byte(0x12,OLED_CMD); // 交替COM引脚配置
//	OLED_WR_Byte(0xDB,OLED_CMD); // 设置VCOMH电压
//	OLED_WR_Byte(0x40,OLED_CMD); // VCOMH = 0.77xVCC
//	OLED_WR_Byte(0x20,OLED_CMD); // 设置地址模式
//	OLED_WR_Byte(0x02,OLED_CMD); // 设置页寻址模式（Page Addressing Mode）
//	OLED_WR_Byte(0x8D,OLED_CMD); // 电荷泵设置命令
//	OLED_WR_Byte(0x14,OLED_CMD); // 启用电荷泵（需要启用才能显示）
//	OLED_WR_Byte(0xA4,OLED_CMD); // 正常显示（0xA4：显示RAM内容；0xA5：全屏点亮）
//	OLED_WR_Byte(0xA6,OLED_CMD); // 正常显示模式（0xA6：正常；0xA7：反色）
//	// 3. 清空显存并开启显示
//	OLED_Clear();                // 清空显存（所有像素置0）
//	OLED_WR_Byte(0xAF,OLED_CMD); // 开启显示
//}



