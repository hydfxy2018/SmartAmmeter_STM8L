#include "lcd_sc.h"
#include "includes.h"
#include "string.h"

lcd_disp_info_t lcd_disp_info;
lcd_event_t lcd_event;
lcd_ram_t   lcd_ram;
uint8_t disp_temp[6] = {0xFF};
uint16_t disp_icon = 0;

static void LCD_SC_DispZero(uint8_t bit_num);
static void LCD_SC_DispOne(uint8_t bit_num);
static void LCD_SC_DispTwo(uint8_t bit_num);
static void LCD_SC_DispThree(uint8_t bit_num);
static void LCD_SC_DispFour(uint8_t bit_num);
static void LCD_SC_DispFive(uint8_t bit_num);
static void LCD_SC_DispSix(uint8_t bit_num);
static void LCD_SC_DispSeven(uint8_t bit_num);
static void LCD_SC_DispEight(uint8_t bit_num);
static void LCD_SC_DispNine(uint8_t bit_num);
static void LCD_SC_DispDot(uint8_t bit_num);
void (*dispBuffer[10])(uint8_t bit_num);

/***********************************************************************
  * @brief  Initialises the close valve icon.
  * @param  None
  * @retval None
************************************************************************/
void LCD_SC_Init(void)
{
	/* Enable LCD clock */
	CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
	CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

	/* Initialize the LCD */
	LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_8,
	   LCD_Bias_1_4, LCD_VoltageSource_Internal);

	/* Mask register*/
	LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xff);
	LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0x3f);

	LCD_ContrastConfig(LCD_Contrast_Level_7);
	LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);

	/*  Enable LCD peripheral */
	LCD_Cmd(ENABLE);

	/* Init  functions */
	dispBuffer[0] = LCD_SC_DispZero;
	dispBuffer[1] = LCD_SC_DispOne;
	dispBuffer[2] = LCD_SC_DispTwo;
	dispBuffer[3] = LCD_SC_DispThree;
	dispBuffer[4] = LCD_SC_DispFour;
	dispBuffer[5] = LCD_SC_DispFive;
	dispBuffer[6] = LCD_SC_DispSix;
	dispBuffer[7] = LCD_SC_DispSeven;
	dispBuffer[8] = LCD_SC_DispEight;
	dispBuffer[9] = LCD_SC_DispNine;
}

/***********************************************************************
  * @brief  Clears the ram for display.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_ClearDisRam(void)
{   
	memset(&lcd_ram,0,sizeof(lcd_ram_t));
}

/***********************************************************************
  * @brief  Just do dispaly the data in ram.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DoDisp(void)
{   
    LCD_WriteRAM(LCD_RAMRegister_0, lcd_ram.com0);
    LCD_WriteRAM(LCD_RAMRegister_1, (uint8_t)(lcd_ram.com0 >> 8));
	
	LCD_WriteRAM(LCD_RAMRegister_3, lcd_ram.com1 << 4);
    LCD_WriteRAM(LCD_RAMRegister_4, (uint8_t)(lcd_ram.com1 >> 4));
	LCD_WriteRAM(LCD_RAMRegister_5, (uint8_t)(lcd_ram.com1 >> 12));
	
	LCD_WriteRAM(LCD_RAMRegister_7, lcd_ram.com2);
    LCD_WriteRAM(LCD_RAMRegister_8, (uint8_t)(lcd_ram.com2 >> 8));
	
	LCD_WriteRAM(LCD_RAMRegister_10, lcd_ram.com3 << 4);
    LCD_WriteRAM(LCD_RAMRegister_11, (uint8_t)(lcd_ram.com3 >> 4));
	LCD_WriteRAM(LCD_RAMRegister_12, (uint8_t)(lcd_ram.com3 >> 12));
}

/***********************************************************************
  * @brief  Display the close valve icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispValveCloseIcon(void)
{   
	lcd_ram.com0 |= 0x01;
}

/***********************************************************************
  * @brief  Display the Arrearage icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispArrearageIcon(void)
{   
	lcd_ram.com1 |= 0x01;
}

/***********************************************************************
  * @brief  Display the gas icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispGasIcon(void)
{   
    lcd_ram.com2 |= 0x01;
}

/***********************************************************************
  * @brief  Display the gas icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispAmountIcon(void)
{   
    lcd_ram.com3 |= 0x01;
} 

/***********************************************************************
  * @brief  Display the Retry icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispRetryIcon(void)
{   
    lcd_ram.com0 |= (0x01 << 13);
}

/***********************************************************************
  * @brief  Display the low voltage icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispLowVoltIcon(void)
{   
    lcd_ram.com1 |= (0x01 << 13);
} 

/***********************************************************************
  * @brief  Display the fault icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispFaultIcon(void)
{   
    lcd_ram.com2 |= (0x01 << 13);
} 

/***********************************************************************
  * @brief  Display the YUAN icon of CNY.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispCNYIcon(void)
{   
	lcd_ram.com3 |= (0x01 << 12);
} 

/***********************************************************************
  * @brief  Display the stere icon of gas.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispStereIcon(void)
{   
    lcd_ram.com3 |= (0x01 << 13);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispZero(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x06 << (bit_num * 2);
	lcd_ram.com1 |= 0x02 << (bit_num * 2);
	lcd_ram.com2 |= 0x06 << (bit_num * 2);
	lcd_ram.com3 |= 0x02 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispOne(uint8_t bit_num)
{   
    lcd_ram.com0 |= (0x01 << 2) << (bit_num * 2);
	lcd_ram.com2 |= (0x01 << 2) << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispTwo(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x06 << (bit_num * 2);
	lcd_ram.com1 |= 0x04 << (bit_num * 2);
	lcd_ram.com2 |= 0x02 << (bit_num * 2);
	lcd_ram.com3 |= 0x02 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispThree(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x06 << (bit_num * 2);
	lcd_ram.com1 |= 0x04 << (bit_num * 2);
	lcd_ram.com2 |= 0x04 << (bit_num * 2);
	lcd_ram.com3 |= 0x02 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispFour(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x04 << (bit_num * 2);
	lcd_ram.com1 |= 0x06 << (bit_num * 2);
	lcd_ram.com2 |= 0x04 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispFive(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x02 << (bit_num * 2);
	lcd_ram.com1 |= 0x06 << (bit_num * 2);
	lcd_ram.com2 |= 0x04 << (bit_num * 2);
	lcd_ram.com3 |= 0x02 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispSix(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x02 << (bit_num * 2);
	lcd_ram.com1 |= 0x06 << (bit_num * 2);
	lcd_ram.com2 |= 0x06 << (bit_num * 2);
	lcd_ram.com3 |= 0x02 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispSeven(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x06 << (bit_num * 2);
	lcd_ram.com2 |= 0x04 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispEight(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x06 << (bit_num * 2);
	lcd_ram.com1 |= 0x06 << (bit_num * 2);
	lcd_ram.com2 |= 0x06 << (bit_num * 2);
	lcd_ram.com3 |= 0x02 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispNine(uint8_t bit_num)
{   
    lcd_ram.com0 |= 0x06 << (bit_num * 2);
	lcd_ram.com1 |= 0x06 << (bit_num * 2);
	lcd_ram.com2 |= 0x04 << (bit_num * 2);
	lcd_ram.com3 |= 0x02 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispDot(uint8_t bit_num)
{   
	lcd_ram.com3 |= 0x04 << (bit_num * 2);
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_DispDigits(uint8_t * pSrc, uint8_t num)
{   
	uint8_t i = 0;

    /* Check parameters */
	usr_assert(pSrc != NULL);
	usr_assert(num);

	for(i = 0;i < num;i ++)
	{
		if(*(pSrc + i) != 0xFF)
		{
			dispBuffer[*(pSrc + i)](i);
		}
	}
}

/***********************************************************************
  * @brief  Display the one icon.
  * @param  None
  * @retval None
************************************************************************/
static void LCD_SC_Display(lcd_disp_info_t * p_lcd_disp_info)
{
	LCD_SC_ClearDisRam();
	
	if((p_lcd_disp_info->is_disp_digits) && 
		(p_lcd_disp_info->p_disp_contex != NULL) && 
		(p_lcd_disp_info->disp_contex_len))
	{
		LCD_SC_DispDigits(p_lcd_disp_info->p_disp_contex, p_lcd_disp_info->disp_contex_len);
	}
	
	if(p_lcd_disp_info->is_disp_icon)
	{
		if((*(p_lcd_disp_info->p_disp_icon) & DISP_AMOUNT_ICON) == DISP_AMOUNT_ICON)
		{
			LCD_SC_DispAmountIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_ARREARAGE_ICON) == DISP_ARREARAGE_ICON)
		{
			LCD_SC_DispArrearageIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_CNY_ICON) == DISP_CNY_ICON)
		{
			LCD_SC_DispCNYIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_FAULT_ICON) == DISP_FAULT_ICON)
		{
			LCD_SC_DispFaultIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_GAS_ICON) == DISP_GAS_ICON)
		{
			LCD_SC_DispGasIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_LOW_VOLTAGE_ICON) == DISP_LOW_VOLTAGE_ICON)
		{
			LCD_SC_DispLowVoltIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_RETRY_ICON) == DISP_RETRY_ICON)
		{
			LCD_SC_DispRetryIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_STERE_ICON) == DISP_STERE_ICON)
		{
			LCD_SC_DispStereIcon();
		}

		if((*(p_lcd_disp_info->p_disp_icon) & DISP_VALVE_CLOSE_ICON) == DISP_VALVE_CLOSE_ICON)
		{
			LCD_SC_DispValveCloseIcon();
		}
	}

	if(p_lcd_disp_info->is_disp_dot)
	{
		LCD_SC_DispDot(p_lcd_disp_info->disp_dot_num);
	}
}
/***********************************************************************
  * @brief  Handles the event for LCD displaying.
  * @param  None
  * @retval None
************************************************************************/
void lcd_event_handler(void * p_event_data, uint16_t event_size)
{
	lcd_event_t * lcd_event_tmp = p_event_data;
	uint8_t temp[6] = {1,2,3,4,5,6};
	
	switch(lcd_event_tmp->eLcd_event)
	{
        case LCD_INIT:
            //LCD_SC_Init();
			//LCD_DeInit();
            break;
            
        case lCD_HANDLE:
			disp_icon |= (DISP_VALVE_CLOSE_ICON | DISP_STERE_ICON);
			lcd_disp_info.is_disp_digits = 1;
			lcd_disp_info.is_disp_dot = 1;
			lcd_disp_info.is_disp_icon = 1;
			lcd_disp_info.p_disp_contex = temp;
			lcd_disp_info.disp_contex_len = sizeof(temp);
			lcd_disp_info.disp_dot_num = 2;
			lcd_disp_info.p_disp_icon = &disp_icon;
			
			LCD_SC_Init();
			LCD_SC_Display(&lcd_disp_info);
            LCD_SC_DoDisp();

			delay1s(2);

			disp_icon &= 0;
			memset(&lcd_disp_info, 0, sizeof(lcd_disp_info_t));
			LCD_SC_Display(&lcd_disp_info);
			
			/* Initialize the LCD */
			LCD_Cmd(DISABLE);
			LCD_DeInit();
			CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
            break;
        
		default:
			break;
	}
}

