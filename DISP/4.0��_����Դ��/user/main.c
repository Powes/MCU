//��ӭ�������ǵĹ�˾����ҳ��www.hjrkj.com  �����к�����Ƽ����޹�˾
//��ӭ�������ǵĹٷ��Ա��꣺http://tiky.taobao.com
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
  SystemInit();//��ʼ��STM32
//	GUI_Init();	//��ʼ��GUI
//	GUI_SetFont(&GUI_Font32B_ASCII);//����24��ASCII����
//	GUI_SetColor(GUI_WHITE);//����ǰ��ɫΪ��ɫ
//	GUI_SetBkColor(GUI_RED);//���ñ���ɫΪ��ɫ
//  GUI_Clear();//����
//	GUI_DispStringAt("Wellcome to Tiky LCD world", 200, 15);//��(200, 15)��������д��һ����
	
	Lcd_Initialize();
//	LCD_GPIO_Config();
//	LCD_Rst();
//	LCD_delay(10);
	
	
}



/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
