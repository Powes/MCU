//欢迎光临我们的公司的主页：www.hjrkj.com  深圳市好钜润科技有限公司
//欢迎光临我们的官方淘宝店：http://tiky.taobao.com
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "ILI9325.h" 	  

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{	
  SystemInit();//初始化STM32
//	GUI_Init();	//初始化GUI
//	GUI_SetFont(&GUI_Font32B_ASCII);//设置24号ASCII字体
//	GUI_SetColor(GUI_WHITE);//设置前景色为白色
//	GUI_SetBkColor(GUI_RED);//设置背景色为红色
//  GUI_Clear();//清屏
//	GUI_DispStringAt("Wellcome to Tiky LCD world", 200, 15);//在(200, 15)的座标上写上一行字
	
	Lcd_Initialize();
//	LCD_GPIO_Config();
//	LCD_Rst();
//	LCD_delay(10);
	
	
}



/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
