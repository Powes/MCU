#include  "touch.h"

void GUI_TOUCH_X_ActivateX(void) {}
void GUI_TOUCH_X_ActivateY(void) {}

//*************************************************
//函数名称 : void Touch_GPIO_Config(void)  
//功能描述 : 设置触屏的SPI引脚,用软件模拟的方法实现SPI功能
//输入参数 : 
//输出参数 : 
//返回值   : 
//*************************************************

void Touch_GPIO_Config(void) 
{

    GPIO_InitTypeDef GPIO_InitStructure ;
	//////下面是SPI相关GPIO初始化//////
    GPIO_InitStructure.GPIO_Pin=SPI_CLK|SPI_MOSI|SPI_CS;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_SetBits(GPIOC,GPIO_Pin_3);	
	
    GPIO_InitStructure.GPIO_Pin = SPI_MISO; 
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //GPIO_Mode_IN_FLOATING上拉输入
    GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin = TP_INT_PIN; 
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //GPIO_Mode_IN_FLOATING上拉输入
    GPIO_Init(GPIOC,&GPIO_InitStructure);	

    GPIO_ResetBits(GPIOB,SPI_CS);
}

//====================================================================================
static void WR_CMD (unsigned char cmd) 
{
    unsigned char buf;
    unsigned char i;
//     TP_CS(1);
    TP_DIN(0);
    TP_DCLK(0);
//     TP_CS(0);
    for(i=0;i<8;i++) 
    {
        buf=(cmd>>(7-i))&0x1;
        TP_DIN(buf);
        //Delayus(5);
        TP_DCLK(1);
        //Delayus(5);
        TP_DCLK(0);
    }
}
//====================================================================================
static unsigned short RD_AD(void) 
{
    unsigned short buf=0,temp;
    unsigned char i;
    TP_DIN(0);
    TP_DCLK(1);
    for(i=0;i<12;i++) 
    {
        TP_DCLK(0);          
        temp= (TP_DOUT) ? 1:0;
        buf|=(temp<<(11-i));
        //Delayus(5);
        TP_DCLK(1);
    }
//     TP_CS(1);
    buf&=0x0fff;
    return(buf);
}
//====================================================================================
 int GUI_TOUCH_X_MeasureX(void) 
{ 
	int i[5],temp;
	u8 cou=0,k=0;
	WR_CMD(CHX);
	RD_AD();
	while(cou<5)	//循环读数5次
	{	
		WR_CMD(CHX); 
		cou++;	
		i[cou]=RD_AD();		  
	}
        //将数据升序排列
	for(k=0;k<4;k++)
	{	  
		for(cou=1;cou<5-k;cou++)
		{
			if(i[cou]>i[cou+1])
			{
				temp=i[cou+1];
				i[cou+1]=i[cou];
				i[cou]=temp;
			}  
		}
	}
	return (i[1]+i[2]+i[3])/3;   
}
//====================================================================================
 int GUI_TOUCH_X_MeasureY(void) 
{ 
int i[5],temp;
	u8 cou=0,k=0;
	WR_CMD(CHY);
	RD_AD();
	while(cou<5)	//循环读数5次
	{	
		WR_CMD(CHY); 
		cou++;	
		i[cou]=RD_AD();		  
	}
        //将数据升序排列
	for(k=0;k<4;k++)
	{	  
		for(cou=1;cou<5-k;cou++)
		{
			if(i[cou]>i[cou+1])
			{
				temp=i[cou+1];
				i[cou+1]=i[cou];
				i[cou]=temp;
			}  
		}
	}
	return (i[1]+i[2]+i[3])/3;
}
unsigned int TOUCH_X(void)
{
	unsigned int i;
	i=GUI_TOUCH_X_MeasureX();
	if(i<250||i>3800)return 800;
		else return (3800-i)*40/(380-25);//2011年6月3日之后的屏用3850-350或3600-1000，之前的屏用3850-150
} 
unsigned int TOUCH_Y(void)
{
	unsigned int i;
	i=GUI_TOUCH_X_MeasureY();
	if(i<300||i>3800)return 800;
		else return (3800-i)*24/(380-30);
}

void TP_GetAdXY(unsigned int *x,unsigned int *y) 
{
    unsigned int adx,ady;
    adx=GUI_TOUCH_X_MeasureX();
    ady=GUI_TOUCH_X_MeasureY();
    *x=adx;
    *y=ady;
}

////触屏的中断输入引脚设置
//void Touch_Interrupt_Config(void)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//  NVIC_InitTypeDef NVIC_InitStructure;
//  EXTI_InitTypeDef EXTI_InitStructure;
//
//
//  //++++++++++触屏的中断输入+++++++++++
//  // Configure GPIO Pin as input floating 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//
//  // Connect EXTI Line to GPIO Pin
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);
//  // Enable the EXTI8 Interrupt //
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//
//  //触摸屏的中断输入为PD8
//  // Enable the EXTI Line8 Interrupt //
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//
//} 

