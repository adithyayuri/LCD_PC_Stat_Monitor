/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_rtc_user.c
* Version      : CodeGenerator for RL78/L12 V2.04.03.01 [14 Aug 2020]
* Device(s)    : R5F10RLC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for RTC module.
* Creation Date: 05-02-2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
#include "rcg_lcd.h"
#include "lcd_panel.h"
#include "r_cg_serial.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_rtc_interrupt(vect=INTRTC)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern uint8_t disp_mode;
/* Create a new instance of data structure RTCCounterValue */
rtc_counter_value_t st_rtc_data;

/* Declare a buffer to hold the time for displaying on the LCD panel */
int8_t  g_lcd_buffer[10] = "00:00:00";
int8_t month_day[4] = "    ";
const uint8_t __LCD_DELAY = 1;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_rtc_interrupt
* Description  : This function is INTRTC interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_rtc_interrupt(void)
{
    if (1U == RIFG)
    {
        RTCC1 &= (uint8_t)~_08_RTC_INTC_GENERATE_FLAG;    /* clear RIFG */
        r_rtc_callback_constperiod();
    }
}

/***********************************************************************************************************************
* Function Name: r_rtc_callback_constperiod
* Description  : This function is real-time clock constant-period interrupt service handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_rtc_callback_constperiod(void)
{
    /* Start user code. Do not edit comment generated here */

	//Display_Panel_String(PANEL_LCD_LINE2, g_lcd_buffer);
	/* Read the RTC time */
	R_RTC_Get_CounterValue(&st_rtc_data);

    /* Read the day of the week value */
    g_lcd_buffer[8] = (int8_t)(st_rtc_data.week & 0x0F);

    /* Read the seconds value */
	g_lcd_buffer[7] = (int8_t)((st_rtc_data.sec & 0x0F) + 0x30);
	g_lcd_buffer[6] = (int8_t)((st_rtc_data.sec >> 4) + 0x30);

    /* Read the minutes value */
    g_lcd_buffer[4] = (int8_t)((st_rtc_data.min & 0x0F) + 0x30);
    g_lcd_buffer[3] = (int8_t)((st_rtc_data.min >> 4) + 0x30);

    /* Read the hours value */
    g_lcd_buffer[1] = (int8_t)((st_rtc_data.hour & 0x0F) + 0x30);
    g_lcd_buffer[0] = (int8_t)(((st_rtc_data.hour >> 4) & 0x01) + 0x30);

    /* Update the time on the LCD panel.
       Casting to ensure use of correct data type.*/

    if (disp_mode == 0)
    {
    	Symbol_Map(LCD_DEGREESC_OFF);
    	Display_Panel_Delay(__LCD_DELAY);
    	Display_Panel_String(PANEL_LCD_LINE3, "    ");
    	Display_Panel_Delay(__LCD_DELAY);
    	Display_Panel_String(PANEL_LCD_LINE3, (g_lcd_buffer + 6u));
    	Display_Panel_Delay(__LCD_DELAY);

    	Display_Panel_String(PANEL_LCD_LINE1, "    ");
    	Display_Panel_Delay(__LCD_DELAY);

        /* Read the month value */
    	month_day[3] = (int8_t)((st_rtc_data.day & 0x0F) + 0x30);
    	month_day[2] = (int8_t)(((st_rtc_data.day >> 4)) + 0x30);

    	Display_Panel_Delay(__LCD_DELAY);
    	switch(st_rtc_data.month){
			case 0x01:
				month_day[0] = (int8_t)'J';
				month_day[1] = (int8_t)'A';
				break;
			case 0x02:
				month_day[0] = (int8_t)'F';
				month_day[1] = (int8_t)'E';
				break;
			case 0x03:
				month_day[0] = (int8_t)'M';
				month_day[1] = (int8_t)'R';
				break;
			case 0x04:
				month_day[0] = (int8_t)'A';
				month_day[1] = (int8_t)'L';
				break;
			case 0x05:
				month_day[0] = (int8_t)'M';
				month_day[1] = (int8_t)'A';
				break;
			case 0x06:
				month_day[0] = (int8_t)'J';
				month_day[1] = (int8_t)'N';
				break;
			case 0x07:
				month_day[0] = (int8_t)'J';
				month_day[1] = (int8_t)'L';
				break;
			case 0x08:
				month_day[0] = (int8_t)'A';
				month_day[1] = (int8_t)'U';
				break;
			case 0x09:
				month_day[0] = (int8_t)'S';
				month_day[1] = (int8_t)'E';
				break;
			case 0x10:
				month_day[0] = (int8_t)'O';
				month_day[1] = (int8_t)'C';
				break;
			case 0x11:
				month_day[0] = (int8_t)'N';
				month_day[1] = (int8_t)'O';
				break;
			case 0x12:
				month_day[0] = (int8_t)'D';
				month_day[1] = (int8_t)'E';
				break;
    		}
        Display_Panel_String(PANEL_LCD_LINE1, month_day);
    	Display_Panel_Delay(__LCD_DELAY);

    }


    /* Display the day of the week symbol */
    SECTF_Glyph_Map(g_lcd_buffer[8]);

    Display_Panel_String(PANEL_LCD_LINE2, g_lcd_buffer);
    Display_Panel_Delay(__LCD_DELAY);

    if (st_rtc_data.hour & 0x20){
    	Symbol_Map(LCD_AM_OFF);
    	Symbol_Map(LCD_PM_ON);
    }
    else{
    	Symbol_Map(LCD_PM_OFF);
    	Symbol_Map(LCD_AM_ON);
    }

    RTCIF = 0u;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
