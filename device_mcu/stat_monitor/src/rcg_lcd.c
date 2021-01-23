/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lcd.c
* Version      : Applilet3 for RL78/L12 V1.00.00 [23 Mar 2012]
* Device(s)    : R5F10RLC
* Tool-Chain   : GNURL78 v13.01
* Description  : This file implements device driver for LCD module.
* Creation Date: 21/01/2013
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Defines the I/O register structures */
#include "iodefine.h"
/* Defines the extended I/O register structures */

/* Header files inclusion */
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "rcg_lcd.h"
#include "r_cg_userdefine.h"


/***********************************************************************************************************************
* Function Name: R_LCD_Create
* Description  : This function initialises the LCD module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Create (void)
{
    /* Loop counter */
    uint16_t i = 0u;
    
    /* supply LCD clock */
    RTCEN = 1u;
    
    /* disable LCD clock operation */
    LCDON = 0u;
    
    /* disable INTLCD0 interrupt */
    LCMK0 = 1u;
    
    /* clear INTLCD0 interrupt flag */
    LCIF0 = 0u;
    
    /* Select waveform A, display mode 1 and internal voltage */
    LCDM0 = _00_LCD_DISPLAY_WAVEFORM_A | _0D_LCD_DISPLAY_MODE1 | _40_LCD_VOLTAGE_MODE_INTERNAL;
    
    /* Set segment pins */
    PFSEG0 = 0xF0u;
    PFSEG1 = 0xFFu;
    PFSEG2 = 0xF9u;
    PFSEG3 = 0x9Fu;
    PFSEG4 = 0x7Fu;

    /* Set the following pins as I/O, and not as analog */
    PMC1 &= 0xE7;
    PMC4 &= 0xFD;
    PMC12 &= 0xFE;
    PMC14 &= 0xC3;
    
    /* Set the segment pins as Output */
    PM1  &= 0x06;
    PM3  &= 0xFE;
    PM4  &= 0xF1;
    PM5  &= 0xE0;
    PM6  &= 0xFC;
    PM7  &= 0x00;
    PM12 &= 0xFE;
    PM14 &= 0x00;
    
    COMEXP = 0U;
    LCDM1 |= _00_LCD_DISPLAY_PATTERN_A;
    LCDC0 = _00_LCD_SOURCE_CLOCK_FSUB | _07_LCD_CLOCK_FLCD_256; 
    VLCD = 0x0EU;
    
    /* Wait for the reference voltage setup time */
    for (i = 0u; i <= 300u; i++)
    {
        /* Do nothing */
    }
}
/***********************************************************************************************************************
* End of function R_LCD_Create
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_LCD_Start
* Description  : This function enables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Start (void)
{
    LCDON = 1U;
}
/***********************************************************************************************************************
* End of function R_LCD_Start
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_LCD_Stop
* Description  : This function disables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Stop (void)
{
    LCDON = 0U;
}
/***********************************************************************************************************************
* End of function R_LCD_Stop
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_LCD_Set_Voltage_On
* Description  : This function enables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_Voltage_On (void)
{
     uint16_t i;
    
    VLCON = 1U;
    
    /* Wait for the voltage boost wait time (500 ms or 5s (VDD > VLC0)) */
    for (i = 0U; i <= 30000; i++)
    {
        /* Do nothing */
    }
    SCOC = 1U;
}
/***********************************************************************************************************************
* End of function R_LCD_Set_Voltage_On
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_LCD_Set_Voltage_Off
* Description  : This function disables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_Voltage_Off (void)
{
    /* Disable the voltage */
    SCOC = 0U;
    VLCON = 0U;
}
/***********************************************************************************************************************
* End of function R_LCD_Set_Voltage_Off
***********************************************************************************************************************/
