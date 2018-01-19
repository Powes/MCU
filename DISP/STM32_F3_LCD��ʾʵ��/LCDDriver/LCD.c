/******************************************************************************

*重要说明！
在.h文件中，#define Immediately时是立即显示当前画面
而如果#define Delay，则只有在执行了LCD_WR_REG(0x0007,0x0173);
之后才会显示，执行一次LCD_WR_REG(0x0007,0x0173)后，所有写入数
据都立即显示。
#define Delay一般用在开机画面的显示，防止显示出全屏图像的刷新过程
******************************************************************************/
#include "stm32f10x.h"
#include "LCD.h"
//#include "ff.h"	
#include <stdio.h>
//#include "const_data.h"
//#include "MM1_240.h"
//#include "MM3_240.h"
/*
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */
 void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* config lcd gpio clock base on FSMC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* config tft back_light gpio base on the PT4101 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* config tft rst gpio */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 
    GPIO_Init(GPIOA, &GPIO_InitStructure);  		   
    
    /* config tft data lines base on FSMC
	 * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	 */	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* config tft control lines base on FSMC
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    /* tft control gpio init */	 
    GPIO_SetBits(GPIOA, GPIO_Pin_1);		 // LIGHT	
//     GPIO_SetBits(GPIOD, GPIO_Pin_13);	 	 //	RST = 1   
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		 // RD = 1  
    GPIO_SetBits(GPIOD, GPIO_Pin_5);		 // WR = 1 
    GPIO_SetBits(GPIOD, GPIO_Pin_7);		 //	CS = 1 

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//按键 OK
		GPIO_Init(GPIOA, &GPIO_InitStructure);
															// right          up           left        down
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6;		
		GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*
 * 函数名：LCD_FSMC_Config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */

static void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;

    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // 一般使用模式B来控制LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

static void Delay(__IO u32 nCount)
{	
	int i;
	for(i=0;i<7200;i++)
    for(; nCount != 0; nCount--);
}  
  
u16 ssd1289_GetPoint(u16 x,u8 y)
{
	 u16 a = 0;
	
	*(__IO u16 *) (Bank1_LCD_C) = 0x4f;	
	*(__IO u16 *) (Bank1_LCD_D) = x;

	*(__IO u16 *) (Bank1_LCD_C) = 0x4e;	
	*(__IO u16 *) (Bank1_LCD_D) = y;

	*(__IO u16 *) (Bank1_LCD_C) = 0x22;

	 a = *(__IO u16 *) (Bank1_LCD_D); 
   return(a);	  
}
static void LCD_Rst(void)
{			
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    Delay(100);					   
    GPIO_SetBits(GPIOD, GPIO_Pin_13 );		 	 
    Delay(100);	
}
static void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}
static void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

/**********************************************
Lcd初始化函数
***********************************************/
void Lcd_Initialize(void)
{	
	int a,i,j,R,G,B;
LCD_FSMC_Config();
LCD_Rst();

	WriteComm(0x00);
	a = *(__IO u16 *) (Bank1_LCD_D); 
	printf("ID=%04x\r\n",a);
	a = *(__IO u16 *) (Bank1_LCD_D); 
	printf("ID=%04x\r\n",a);
a = *(__IO u16 *) (Bank1_LCD_D); 
	printf("ID=%04x\r\n",a);
	a = *(__IO u16 *) (Bank1_LCD_D); 
	printf("ID=%04x\r\n",a);
a = *(__IO u16 *) (Bank1_LCD_D); 
	printf("ID=%04x\r\n",a);
	a = *(__IO u16 *) (Bank1_LCD_D); 
	printf("ID=%04x\r\n",a);	
	

WriteComm(0x0000);
WriteComm(0x0000);
Delay(10);
WriteComm(0x0000);
WriteComm(0x0000);
WriteComm(0x0000);
WriteComm(0x0000);
WriteComm(0x0000);
WriteComm(0x0000);
WriteComm(0x0000);
WriteComm(0x0000);
Delay(10);
WriteComm(0x00A4); WriteData(0x0001);
Delay(10);
WriteComm(0x0060); WriteData(0xa700);//Driver Output Control NL
WriteComm(0x0008); WriteData(0x0808);//Display Control 2 (R08h) FP BP
//Gamma setting R30~39h: Gamma Control
WriteComm(0x0030); WriteData(0x0203);
WriteComm(0x0031); WriteData(0x080F);
WriteComm(0x0032); WriteData(0x0401);
WriteComm(0x0033); WriteData(0x0508);
WriteComm(0x0034); WriteData(0x3330);
WriteComm(0x0035); WriteData(0x0B05);
WriteComm(0x0036); WriteData(0x0005);
WriteComm(0x0037); WriteData(0x0F08);
WriteComm(0x0038); WriteData(0x0302);
WriteComm(0x0039); WriteData(0x3033);

WriteComm(0x0090); WriteData(0x0018);//85HZ
WriteComm(0x0010); WriteData(0x0530);//BT, AP
WriteComm(0x0011); WriteData(0x0237);//DC,VC
WriteComm(0x0012); WriteData(0x01BF);//Power Charge Pump Need 500ms
WriteComm(0x0013); WriteData(0x1000);//vcom
Delay(100);
WriteComm(0x0001); WriteData(0x0100); //Driver Output Control (R01h)
WriteComm(0x0002); WriteData(0x0200); //Line Inversion
WriteComm(0x0003); WriteData(0x1030); //TRIREG=1
WriteComm(0x0009); WriteData(0x0001);
WriteComm(0x000A); WriteData(0x0008);
WriteComm(0x000C); WriteData(0x0000); //
WriteComm(0x000D); WriteData(0x0000);
WriteComm(0x000E); WriteData(0x0030);
WriteComm(0x000F); WriteData(0x0000); //
//RAM access instruction
WriteComm(0x0020); WriteData(0x0000); //RAM Address Set (Horizontal Address) (R20h)
WriteComm(0x0021); WriteData(0x0000); //RAM Address Set (Vertical Address) (R21h)
//Window address control instruction
WriteComm(0x0029); WriteData(0x002E);
WriteComm(0x0050); WriteData(0x0000); //Window Horizontal RAM Address Start/End (R50h/ R51h)
WriteComm(0x0051); WriteData(0x00EF);
WriteComm(0x0052); WriteData(0x0000); //Window Vertical RAM Address Start/End (R52h/R53h)
WriteComm(0x0053); WriteData(0x013F);
//Base image display control instruction
WriteComm(0x0061); WriteData(0x0001); //Base Image Display Control (R61h)0x0000
WriteComm(0x006A); WriteData(0x0000); ///Vertical Scroll Control (R6Ah)
//Partial display control instruction
WriteComm(0x0080); WriteData(0x0000);
WriteComm(0x0081); WriteData(0x0000);
WriteComm(0x0082); WriteData(0x005F);
//Panel interface control instruction

WriteComm(0x0093); WriteData(0x0701);

WriteComm(0x0007); WriteData(0x0100);
Delay(10);

WriteComm(0x0029); WriteData(0x002E);

LCD_WR_REG( 0x03, 0x1038 );		//水平方向扫描,BGR模式，16位数据线模式，1次传送

WriteComm(0x0020); WriteData(0x00EF); //RAM Address Set (Horizontal Address) (R20h)
WriteComm(0x0021); WriteData(0x0000); //RAM Address Set (Vertical Address) (R21h)

WriteComm(0x0050); WriteData(0x0000); //Window Horizontal RAM Address Start/End (R50h/ R51h)
WriteComm(0x0051); WriteData(0x00EF);
WriteComm(0x0052); WriteData(0x0000); //Window Vertical RAM Address Start/End (R52h/R53h)
WriteComm(0x0053); WriteData(0x013F);
WriteComm(0x022);

Lcd_Light_ON;

Lcd_ColorBox(0,0,100,270,Yellow);Delay(2000000);
}

void dispRefresh(){
	
 Lcd_ColorBox(0,0,240,320,Red);Delay(2000000);
Lcd_ColorBox(0,0,240,320,Green);Delay(2000000);
Lcd_ColorBox(0,0,240,320,Blue);Delay(2000000);
Lcd_ColorBox(0,0,240,320,White);Delay(2000000);
Lcd_ColorBox(0,0,240,320,Black);Delay(2000000);
}

void pic_8bit(u16 x,u16 y,u16 x_long,u16 y_long,u8 *gImage)
{
	u16 i,R,G,B;
	BlockWrite(x,x+x_long-1,y,y+y_long-1);
	for(i=0;i<x_long*y_long;i++)
	{
		R=(4*(((gImage[i])>>5)+1)-1)<<11;
		G=((((gImage[i]&0X1C)>>2)+1)*8-1)<<5;
		B=((gImage[i]&0X03)+1)*8-1;
		WriteData(R|G|B);
	}
}
void Grayscale_16(u16 x,u16 y,u16 x_long,u16 y_long,u8 *gImage)
{
	u16 i,temp_0,temp_1;
	BlockWrite(x,x+x_long-1,y,y+y_long-1);
	x_long=(x_long*y_long)>>1;
	for(i=0;i<x_long;i++)
	{
		temp_0=gImage[i]>>4;
		temp_1=(temp_0<<1)|(temp_0<<7)|(temp_0<<12);
		WriteData(temp_1 | 0x0861);
		
		temp_0=gImage[i]&0X0F;
		temp_1=(temp_0<<1)|(temp_0<<7)|(temp_0<<12);
		WriteData(temp_1 | 0x0861);
	}
}


/******************************************
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	*(__IO u16 *) (Bank1_LCD_C) = Index;	
// 	*(__IO u16 *) (Bank1_LCD_D) = CongfigTemp;
	*(__IO u16 *) (Bank1_LCD_D) = (CongfigTemp&0xf8ff)|((CongfigTemp&0x0300)<<1)|((CongfigTemp&0x0400)>>2);
}
/************************************************
函数名：Lcd写开始函数
功能：控制Lcd控制引脚 执行写操作
************************************************/
void Lcd_WR_Start(void)
{
*(__IO u16 *) (Bank1_LCD_C) = 0x2C;
}
/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	
	LCD_WR_REG(0x0050,Xstart);//水平GRAM起始位置
	LCD_WR_REG(0x0051,Xend);//水平GRAM终止位置 

    //x,y坐标对调 
	LCD_WR_REG(0x0052,Ystart);//垂直 GRAM起始位置
	LCD_WR_REG(0x0053,Yend);//垂直GRAM终止位置
	
	LCD_WR_REG(0x0020,Xstart);//
	LCD_WR_REG(0x0021,Ystart);//
	
  WriteComm(0x022);
}
/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

//	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		*(__IO u16 *) (Bank1_LCD_D) = Color;
	}
}
void LCD_draw_VLine(u16 x,u16 y0,u16 y1,u16 Color)
{
	u16 i;
	BlockWrite(x,x,y0,y1);
	for(i=0;i<y1-y0+1;i++)
		*(__IO u16 *) (Bank1_LCD_D) = Color;
}
void LCD_draw_HLine(u16 x0,u16 x1,u16 y,u16 Color)
{
	u16 i;
	BlockWrite(x0,x1,y,y);
	for(i=0;i<x1-x0+1;i++)
		*(__IO u16 *) (Bank1_LCD_D) = Color;
}
/******************************************
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //横屏，从左下角开始，从左到右，从下到上
//	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xaa);
}

void DrawPixel(u16 x, u16 y, u16 Color)
{
	BlockWrite(x,x+1,y,y+1);
	*(__IO u16 *) (Bank1_LCD_D) = Color;
}


typedef __packed struct
{
	u8  pic_head[2];				//1
	u16 pic_size_l;			    //2
	u16 pic_size_h;			    //3
	u16 pic_nc1;				    //4
	u16 pic_nc2;				    //5
	u16 pic_data_address_l;	    //6
	u16 pic_data_address_h;		//7	
	u16 pic_message_head_len_l;	//8
	u16 pic_message_head_len_h;	//9
	u16 pic_w_l;					//10
	u16 pic_w_h;				    //11
	u16 pic_h_l;				    //12
	u16 pic_h_h;				    //13	
	u16 pic_bit;				    //14
	u16 pic_dip;				    //15
	u16 pic_zip_l;			    //16
	u16 pic_zip_h;			    //17
	u16 pic_data_size_l;		    //18
	u16 pic_data_size_h;		    //19
	u16 pic_dipx_l;			    //20
	u16 pic_dipx_h;			    //21	
	u16 pic_dipy_l;			    //22
	u16 pic_dipy_h;			    //23
	u16 pic_color_index_l;	    //24
	u16 pic_color_index_h;	    //25
	u16 pic_other_l;			    //26
	u16 pic_other_h;			    //27
	u16 pic_color_p01;		    //28
	u16 pic_color_p02;		    //29
	u16 pic_color_p03;		    //30
	u16 pic_color_p04;		    //31
	u16 pic_color_p05;		    //32
	u16 pic_color_p06;		    //33
	u16 pic_color_p07;		    //34
	u16 pic_color_p08;			//35			
}BMP_HEAD;

BMP_HEAD bmp;

 


//任意屏大小范围内显示图片
/*
x，y像素起点坐标
*/
//char display_picture(char *filename)
//{
//	u16 ReadValue;
//	FATFS fs;            // Work area (file system object) for logical drive
//	FIL fsrc;      			// file objects
//	u8 buffer[2048]; 		// file copy buffer
//	FRESULT res;         // FatFs function common result code
//	UINT br;         		// File R/W count
//	u16 r_data,g_data,b_data;	
//	u32	 tx,ty,temp;
//	
//	
//  f_mount(0, &fs);
//  res = f_open(&fsrc, filename, FA_OPEN_EXISTING | FA_READ);	 //打上图片文件名
//  if(res==FR_NO_FILE||res==FR_INVALID_NAME){
//     f_mount(0, NULL);
//	 return 0;
//  }
//	
//  if(res!=FR_OK){
//     f_mount(0, NULL);
//	 SD_Init();//重新初始化SD卡 
//	 return 0;
//  }

//  res = f_read(&fsrc, &bmp, sizeof(bmp), &br);

//  if(br!=sizeof(bmp))
//	{
//		f_close(&fsrc);
//		f_mount(0, NULL);
//		return 0;
//  }

//  if((bmp.pic_head[0]=='B')&&(bmp.pic_head[1]=='M'))
//  {
//	res = f_lseek(&fsrc, ((bmp.pic_data_address_h<<16)|bmp.pic_data_address_l));
//	if(res!=FR_OK){
//     f_close(&fsrc);
//     f_mount(0, NULL);
//	 return 0;
//    }
//	if(bmp.pic_w_l>bmp.pic_h_l)
//	{
// 	WriteComm(0x03); //Set_address_mode
// 	WriteData(0x1018); //横屏，从左下角开始，从左到右，从下到上
//	if(bmp.pic_w_l<399||bmp.pic_h_l<239)
//		{
//			Lcd_ColorBox(0,0,400,240,0x0000);
//			BlockWrite((400-bmp.pic_w_l)/2,(400-bmp.pic_w_l)/2+bmp.pic_w_l-1,(240-bmp.pic_h_l)/2,(240-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//		}
//		else 
//		{
//			LCD_WR_REG(0x0200, 0);
//			LCD_WR_REG(0x0201, 399);
//			LCD_WR_REG(0x0210, 0);
//			LCD_WR_REG(0x0211, 239);
//			LCD_WR_REG(0x0212, 0);
//			LCD_WR_REG(0x0213, 399);
//			WriteComm(0x0202);
//		}
//	}
//	else
//	{
//		WriteComm(0x03); //Set_address_mode
//		WriteData(0x1010); //竖屏，从左下角开始，从左到右，从下到上
//		if(bmp.pic_w_l<239||bmp.pic_h_l<399)
//			{
//				Lcd_ColorBox(0,0,240,400,0x0000);
//				BlockWrite((240-bmp.pic_w_l)/2,(240-bmp.pic_w_l)/2+bmp.pic_w_l-1,(400-bmp.pic_h_l)/2,(400-bmp.pic_h_l)/2+bmp.pic_h_l-1);
//			}
//			else BlockWrite(0,239,0,399);	
//	}
//	temp=bmp.pic_w_l*3;	
//	
//	for (tx = 0; tx < bmp.pic_h_l; tx++)
//	{
//	f_read(&fsrc, buffer, (bmp.pic_w_l)*3, &br);
//	for(ty=0;ty<temp;ty+=3)
//		{
//			r_data = *(ty +2+buffer);
//			g_data = *(ty +1+buffer);
//			b_data = *(ty +  buffer);			
//			ReadValue=(r_data & 0xF8) << 8 | (g_data & 0xFC) << 3 | b_data >> 3;
//			*(__IO u16 *) (Bank1_LCD_D) = ReadValue; 	
//		}
//	}	
//	
//// // 		temp=bmp.pic_w_l*2;	
//// // 	for (tx = 0; tx < bmp.pic_h_l; tx++)
//// // 	{
//// // 	f_read(&fsrc, buffer, (bmp.pic_w_l)*2, &br);
//// // 	for(ty=0;ty<temp;ty+=2)
//// // 		{

//// // 			 b_data= *(ty +1+buffer);
//// // 			 g_data= *(ty +  buffer);			

//// // 			ReadValue= g_data<<8+b_data;
//// // 			*(__IO u16 *) (Bank1_LCD_D) = ReadValue; 
//// // 			
//// // // 			ReadValue= ((b_data&0xc0)<<8) | ((b_data&0x38)<<5) | ((b_data&0x03)<<2);
//// // // 			*(__IO u16 *) (Bank1_LCD_D) = ReadValue; 	
//// // 		}
//// // 	}	
//		f_close(&fsrc);
//   }
//  f_mount(0, NULL);
//  return 1;
//}