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
* Creation Date: 24-01-2021
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

/* Create a new instance of data structure RTCCounterValue */
rtc_counter_value_t st_rtc_data;

/* Declare a buffer to hold the time for displaying on the LCD panel */
int8_t  g_lcd_buffer[10] = "00:00:00";

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
    g_lcd_buffer[0] = (int8_t)(((st_rtc_data.hour >> 4) & 0x03) + 0x30);

    /* Update the time on the LCD panel.
       Casting to ensure use of correct data type.*/
    Display_Panel_String(PANEL_LCD_LINE2, g_lcd_buffer);
    Display_Panel_String(PANEL_LCD_LINE3, (g_lcd_buffer + 6u));

    /* Display the day of the week symbol */
    SECTF_Glyph_Map(g_lcd_buffer[8]);



    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
