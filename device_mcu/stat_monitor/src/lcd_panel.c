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
* File Name    : lcd_panel.c
* Version      : Applilet3 for RL78/L12 V1.00.00 [23 Mar 2012]
* Device(s)    : R5F10RLC
* Tool-Chain   : GNURL78 v13.01
* Description  : This file contains the LCD panel driver.
* Creation Date: 21/01/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes 
***********************************************************************************************************************/
/* Defines the I/O register structures */
#include "iodefine.h"
/* Defines the extended I/O register structures */
#include "iodefine_ext.h"

#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"
#include "lcd_panel.h"

/* Compiler instruction to optimise the following
   code from this point on until the end of the file */
#pragma GCC optimize ("O2")

/***********************************************************************************************************************
Global Variables & Defined Constants 
***********************************************************************************************************************/
/* Declare Array Maps for writing to section A, the middle line of the LCD panel */
LCDMAP SECTA_map[20];

/* Declare Array Maps for writing to section B, the top line of the LCD panel */
LCDMAP SECTB_map[20];

/* Declare Array Maps for writing to section C, the bottom line of the LCD panel */
LCDMAP SECTC_map[41];

/* Function prototype for initialising Section A glyph maps */
static void init_secta_maps (void);

/* Function prototype for initialising Section B glyph maps */
static void init_sectb_maps (void);

/* Function prototype for initialising Section C glyph maps */
static void init_sectc_maps (void);

/* Function prototype for initialising Section C's letters glyph maps */
static void init_sectc_letters_maps (void);

/***********************************************************************************************************************
User Program Code
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Validate_Input
* Description   : Takes any char and validates the input for the Update_Display 
*                 function. Capitalizes all letters, and re-maps symbols. Outputs
*                 to a variable specified by the pointer parameter passed.
* Arguments     : uint8_t input
*                         to be validated
*                 uint8_t * output
*                         pointer to output buffer
* Return Value  : uint16_t
*                 0: Unsupported Character
*                 1: Number (or space)
*                 2: Letter A-F
*                 3: Letter G-Z
*                 4: Supported Symbol
***********************************************************************************************************************/
uint16_t Validate_Input (const uint8_t input, uint8_t * const output)
{
    /* Is space or Carriage Return? */
    if ((' ' == input) || (0x0D == input))
    {
        *output = 0x29u;
        
        return 1u;
    }
    /* Is Number? */
    else if ((input < 0x3Au) && (input > 0x2Fu))
    {
        /* Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x30);
        
        return 1u;
    }
    
    /* Is Upper-case Alpha A-F ? */
    else if ((input < 0x47u) && (input > 0x40u))
    {
        /* Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x33);
        
        return 2u;
    }
    
    /* Is Lower-case Alpha A-F */
    else if ((input < 0x67u) && (input > 0x60u))
    {
        /* Shift Case & Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x53u);
        
        return 2u;
    }
    
    /* Is Upper-case Alpha G-Z ? */
    else if ((input > 0x46u) && (input < 0x5Bu))
    {
        /* Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x33u);
        
        return 3u;
    }
    
    /* Is Lower-case Alpha G-Z ? */
    else if ((input > 0x66u) && (input < 0x7Bu))
    {
        /* Shift Case & Convert from ASCII
           Casting to ensure use of correct data type. */
        *output = (uint8_t)(input - 0x53u);
        
        return 3u;
    }
    
    /* Is Symbol? */
    else
    {
        /* Check input against supported symbols */
        switch (input)
        {
            case 0x2Bu:
                {
                    /* Is Plus */
                    *output = 0x0Au;
                }
            break;
            
            case 0x2Du:
                {
                    /* Is Minus */
                    *output = 0x0Bu;
                }
            break;
            
            case 0x2Fu:
                {
                    /* Is Forward Slash */
                    *output = 0x0Cu;
                }
            break;
            
            case 0x5Cu:
                {
                    /* Is Back Slash */
                    *output = 0x0Du;
                }
             break;
            
            case 0x2Eu:
                {
                    /* Is full stop */
                    *output = 0x2Eu;
                }
            break;
            
            case 0x3Au:
                {
                    /* Is colon */
                    *output = 0x3Au;
                }
            break;
            
            case 0x2Au:
                {
                    /* Is asterisk */
                    *output = 0x28u;
                }
            break;
            
            default:
                {
                    /* Unsupported Character, do nothing */
                    return 0u;
                }
            break;
        }
        
        /* The character received is supported */
        return 4u;
    }
}
/***********************************************************************************************************************
* End of function Validate_Input
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: SECTA_Glyph_Map
* Description  : Takes a validated char input and maps the character to a 
*                segment pattern, to be displayed on Section A (mid-section) of the LCD panel.
* Arguments    : const uint8_t glyph  - character to be displayed, 
*                uint16_t digit - digit position of character
* Return Value : none
***********************************************************************************************************************/
void SECTA_Glyph_Map (uint8_t const glyph, uint16_t const digit)
{
    /* Declare the first pointer for mapping the character to the segments */
    volatile uint8_t * PinPtr1 = PDL_NO_PTR;
    
    /* Declare the second pointer for mapping the character to the segments */
    volatile uint8_t * PinPtr2 = PDL_NO_PTR;
    
    /* Set Pointers to Correct Segments */
    switch(digit)
    {
        /* First Digit */
        case CASE_0:
            {
                /* Map segments */
                PinPtr1 = &SEG12_DEF;
                PinPtr2 = &SEG11_DEF;
            }
        break;
        
        /* Second Digit */
        case CASE_1:
            {
                /* Map segments */
                PinPtr1 = &SEG14_DEF;
                PinPtr2 = &SEG13_DEF;
            }
        break;
        
        /* Decimal Point */
        case CASE_2:
            {
                /* Map segments */
                PinPtr1 = &SEG15_DEF;
                PinPtr2 = PDL_NO_PTR;
            }
        break;
        
        /* Third Digit */
        case CASE_3:
            {
                /* Map segments */
                PinPtr1 = &SEG16_DEF;
                PinPtr2 = &SEG15_DEF;
            }
        break;
        
        default:
            {
                /* Do nothing */
            }
        break;
    }
    
    /* Bit Mask Segments */
    *PinPtr1 &= 0xF0u;
    *PinPtr2 &= 0xF8u;
    
    /* Decimal point? */
    if (2u == digit)
    {
        if ('.' == glyph)
        {
            /* Turn on Decimal Point */
            *PinPtr1 |= 0x08u;
        }
        else
        {
            /* Turn off Decimal Point */
            *PinPtr1 &= 0xF7u;
        }
    }
    else if (GLYPH_CHECK != glyph)
    {        
        /* Digit-Segment Mapping */
        *PinPtr1 |= SECTA_map[glyph].NIBBLE.TWO;
        *PinPtr2 |= SECTA_map[glyph].NIBBLE.ONE;
    }
    else
    {
        /* Do nothing */
    }
}
/***********************************************************************************************************************
End of function SECTA_Glyph_Map
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : SECTB_Glyph_Map
* Description   : Takes a validated char input and maps the character to a 
*                 segment pattern, to be displayed on Section B (top-section) of the LCD panel.
* Arguments     : const uint8_t glyph - character to be displayed, 
*                 uint16_t digit - digit position of character
* Return Value  : none
***********************************************************************************************************************/
void SECTB_Glyph_Map (uint8_t const glyph, uint16_t const digit)
{
    /* Declare the first pointer to the segments */
    volatile uint8_t * PinPtr1 = PDL_NO_PTR;
    
    /* Declare the second pointer to the segments */
    volatile  uint8_t * PinPtr2 = PDL_NO_PTR;
    
    /* Set Pointers to Current Digit Pins */
    switch (digit)
    {
        /* Second Digit */
        case CASE_1:
            {
                /* Map segments */
                PinPtr1 = &SEG27_DEF;
                PinPtr2 = &SEG28_DEF;
            }
        break;
        
        /* Third Digit */
        case CASE_2:
            {
                /* Map segments */
                PinPtr1 = &SEG25_DEF;
                PinPtr2 = &SEG26_DEF;
            }
        break;
        
        /* Colon/Decimal Point */
        case CASE_3:
            {
                /* Map segments */
                PinPtr1 = &SEG24_DEF;
                PinPtr2 = PDL_NO_PTR;
            }
        break;
        
        /* Fourth Digit */
        case CASE_4:
            {
                /* Map segments */
                PinPtr1 = &SEG22_DEF;
                PinPtr2 = &SEG23_DEF;
            }
        break;
        
        /* Fifth Digit */
        case CASE_5:
            {
                /* Map segments */
                PinPtr1 = &SEG20_DEF;
                PinPtr2 = &SEG21_DEF;
            }
        break;
        
        /* AM/PM Indicator */
        case CASE_6:
            {
                /* Nullify Pointers */
                PinPtr1 = PDL_NO_PTR;
                PinPtr2 = PDL_NO_PTR;
                
                /* If last digit is A */
                if (0x0Eu == glyph)
                {
                    /* AM Indicator On */
                    SEG19_DEF |= 0x01u;
                    
                    /* PM Indicator Off */
                    SEG17_DEF &= 0xFDu;
                }
                /* If last digit is P */
                else if (0x1Du == glyph)
                {
                    /* AM Indicator Off */
                    SEG19_DEF &= 0xFEu;
                    
                    /* PM Indicator On */
                    SEG17_DEF |= 0x02u;
                }
                /* If last digit is other */
                else
                {
                    /* AM Indicator Off */
                    SEG19_DEF &= 0xFEu;
                    
                    /* PM Indicator Off */
                    SEG17_DEF &= 0xFDu;
                }
            }
        break;
        
        /* Handles undefined cases */
        default:
            {
                /* Do nothing */
            }
        break;
    }
    
    /* Bit Mask Segments */
    *PinPtr1 &= 0xF0u;
    *PinPtr2 &= 0xF1u;
        
    /* Is colon/decimal point ? */
    if (3u == digit)
    {    
        /* Is Colon? */
        if (':' == glyph)
        {
            /* Turn on Colon */
            *PinPtr1 |= 0x02u;
        }
        /* Is Decimal Point? */
        else if ('.' == glyph)
        {
            /* Turn on decimal */			
            *PinPtr1 |= 0x01u;			
        }
        /* Is neither? */
        else
        {
            /* Turn off Decimal point & colon */
            *PinPtr1 &= 0xFCu;
        }
    }
    else if (GLYPH_CHECK != glyph)
    {
        /* Map Glyph to Segments */
        *PinPtr1 |= SECTB_map[glyph].NIBBLE.TWO;
        *PinPtr2 |= SECTB_map[glyph].NIBBLE.ONE;
    }
    else
    {
        /* Do nothing */
    }
}
/***********************************************************************************************************************
End of function SECTB_Glyph_Map
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : SECTC_Glyph_Map
* Description   : Takes a validated char input and maps the character to a 
*                 segment pattern, to be displayed on Section C of the LCD panel.
* Arguments     : uint8_t const glyph  - character to be displayed, 
                  uint16_t const digit - position of character
* Return Value  : none
***********************************************************************************************************************/
void SECTC_Glyph_Map (uint8_t const glyph, uint16_t const digit)
{
    /* Declare Pin Address Pointers */
    volatile uint8_t * PinPtr1 = PDL_NO_PTR;
    volatile uint8_t * PinPtr2 = PDL_NO_PTR;
    volatile uint8_t * PinPtr3 = PDL_NO_PTR;
    volatile uint8_t * PinPtr4 = PDL_NO_PTR;
    
    /* Map segments */
    switch (digit)
    {    
        /* Third Digit */
        case CASE_2:
            {
                PinPtr1 = &SEG3_DEF;
                PinPtr2 = &SEG4_DEF;
                PinPtr3 = &SEG37_DEF;
                PinPtr4 = &SEG38_DEF;
            }
        break;
        
        /* Four Digit */
        case CASE_3:
            {
                PinPtr1 = &SEG5_DEF;
                PinPtr2 = &SEG6_DEF;
                PinPtr3 = &SEG35_DEF;
                PinPtr4 = &SEG36_DEF;
            }
        break;
        
        /* Fifth Digit */
        case CASE_4:
            {
                PinPtr1 = &SEG7_DEF;
                PinPtr2 = &SEG8_DEF;
                PinPtr3 = &SEG33_DEF;
                PinPtr4 = &SEG34_DEF;
            }
        break;
        
        /* Sixth Digit */
        case CASE_5:
            {
                PinPtr1 = &SEG9_DEF;
                PinPtr2 = &SEG10_DEF;
                PinPtr3 = &SEG31_DEF;
                PinPtr4 = &SEG32_DEF;
            }
        break;
        
        /* LCD display only has 4 available star-burst segments */
        default:
            {
                /* Bad Selection */
            }
        break;
    }
    
    /* Bit Mask All Segments */
    *PinPtr1 &= 0xF0u;
    *PinPtr2 &= 0xF0u;
    *PinPtr3 &= 0xF1u;
    *PinPtr4 &= 0xF0u;
    
    /* Check if glyph is not 0x29 */
    if (GLYPH_CHECK != glyph)
    {
        /* Map glyph To Segments */
        *PinPtr4 |= SECTC_map[glyph].NIBBLE.ONE;
        *PinPtr3 |= SECTC_map[glyph].NIBBLE.TWO;
        *PinPtr2 |= SECTC_map[glyph].NIBBLE.THREE;
        *PinPtr1 |= SECTC_map[glyph].NIBBLE.FOUR;
    }
}
/***********************************************************************************************************************
End of function SECTC_Glyph_Map
***********************************************************************************************************************/

/***********************************************************************************************************************
Function Name : SECTD_Glyph_Map
Description   : Takes a number input, and sets the level of the battery indicator (section D)
Argument      : uint8_t level       - Indicator Level (char)
*               uint8_t bat_outline - Turns on or off the battery outline
Return Values : None
***********************************************************************************************************************/
void SECTD_Glyph_Map (uint8_t const level, uint8_t const bat_outline)
{
    /* Turn off all levels */
    SEG18_DEF &= 0xF0u;
    
    /* Select the case based on the input level */
    switch (level)
    { 
        case LEVEL_0:
            {
                /* Battery Outline & 0 Level */
                SEG18_DEF |= 0x00u;
            }
        break;
        
        case LEVEL_1:
            {
                /* Battery Outline & 1 Level */
                SEG18_DEF |= 0x08u;
            }
        break;
        
        case LEVEL_2:
            {
                /* Battery Outline & 2 Levels */
                SEG18_DEF |= 0x0Cu;
            }
        break;
        
        case LEVEL_3:
            {
                /* Battery Outline & 3 Levels */
                SEG18_DEF |= 0x0Eu;
            }
        break;
        
        case LEVEL_4:
            {
                /* All Levels On */
                SEG18_DEF |= 0x0Fu;
            }
        break;
        
        default:
            {
                /* Bad Selection */
            }
        break;
    }
    
    /* Check if the battery outline is required */
    if (BATTERY_OUTLINE_OFF == bat_outline)
    {
        /* Turn off the battery outline */
        SEG17_DEF &= ~(0x01);
    }
    else
    {
        /* Turn on the battery outline */
        SEG17_DEF |= 0x01;
    }
}
/***********************************************************************************************************************
End of function SECTD_Glyph_Map
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : SECTF_Glyph_Map
* Description   : Sets the week calendar (Section F) based on a number input
*                 0: OFF            4: Thursday
*                 1: Monday         5: Friday    
*                 2: Tuesday        6: Saturday
*                 3: Wednesday      7: Sunday
* Arguments     : uint8_t day - character to be displayed, digit position of character
* Return Value  : None
***********************************************************************************************************************/
void SECTF_Glyph_Map (uint8_t const day)
{
    /* Select the case based on the input day */
    switch(day)
    {
        case ALL_DAYS:
            {
                /* All Days Off */
                SEG26_DEF &= 0x0Eu;
                SEG24_DEF &= 0x03u;
                SEG23_DEF &= 0x0Eu;
                SEG21_DEF &= 0x0Eu;
                SEG19_DEF &= 0x03u;
            }
        break;
       
        case MONDAY:
            {
                /* Monday Lit */
                SEG26_DEF &= 0x0Eu;
                SEG24_DEF &= 0x03u;
                SEG24_DEF |= 0x08u;
                SEG23_DEF &= 0x0Eu;
                SEG21_DEF &= 0x0Eu;
                SEG19_DEF &= 0x03u;
            }
        break;
        
        case TUESDAY:
            {
                /* Tuesday Lit */
                SEG26_DEF &= 0x0Eu;
                SEG26_DEF &= 0x03u;
                SEG26_DEF |= 0x04u;
                SEG23_DEF &= 0x0Eu;
                SEG21_DEF &= 0x0Eu;
                SEG19_DEF &= 0x03u;
            }
        break;
        
        case WEDNESDAY:
            {
                /* Wednesday Lit */
                SEG26_DEF &= 0x0Eu;
                SEG32_DEF &= 0x03u;
                SEG23_DEF |= 0x01u;
                SEG21_DEF &= 0x0Eu;
                SEG19_DEF &= 0x03u;
            }
        break;
        
        case THURSDAY:
            {
                /* Thursday Lit */
                SEG26_DEF &= 0x0Eu;
                SEG24_DEF &= 0x03u;
                SEG23_DEF &= 0x0Eu;
                SEG21_DEF |= 0x01u;
                SEG19_DEF &= 0x03u;
            }
        break;
        
        case FRIDAY:
            {
                /* Friday Lit */
                SEG26_DEF &= 0x0Eu;
                SEG24_DEF &= 0x03u;
                SEG23_DEF &= 0x0Eu;
                SEG21_DEF &= 0x0Eu;
                SEG19_DEF &= 0x03u;
                SEG19_DEF |= 0x08u;
            }
        break;
        
        case SATURDAY:
            {
                /* Saturday Lit */
                SEG26_DEF &= 0x0Eu;
                SEG24_DEF &= 0x03u;
                SEG23_DEF &= 0x0Eu;
                SEG21_DEF &= 0x0Eu;
                SEG19_DEF &= 0x03u;
                SEG19_DEF |= 0x04u;
            }
        break;
        
        case SUNDAY:
            {
                /* Sunday Lit */
                SEG26_DEF |= 0x01u;
                SEG24_DEF &= 0x03u;
                SEG23_DEF &= 0x0Eu;
                SEG21_DEF &= 0x0Eu;
                SEG19_DEF &= 0x03u;
            }
        break;
        
        default:
            {
                /* Bad Selection */
            }
        break;
    }
}
/***********************************************************************************************************************
End of function SECTF_Glyph_Map
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Symbol_Map
* Description   : Takes a symbol code and turns on/off LCD Panel Symbols. The 
*                 first digit should be which symbol to control (see symbols 
*                 listed below) 
*                 The second should be either a 1 or a 0. 
*                 (1 - Turn on, 0 - Turn Off)
*                 1 - Heat         5 - mmHg             9 - Degrees F
*                 2 - Fan          6 - Volts            A - Alarm
*                 3 - Zone         7 - Heart            B - AM
*                 4 - mg/ml        8 - Degrees C        C - PM
*                 For example '61' would turn on the Volts symbol and 
*                 C0 would turn off the PM symbol.
* Arguments     : int16_t input - symbol to be controlled
* Return Value  : uint8_t
*                           0 - valid input
                            1 - invalid input
***********************************************************************************************************************/
uint8_t Symbol_Map (uint16_t const input)
{
    /* Declare a status variable */
    uint8_t status = 0;
    
    /* Select the case based on the input selection. */
    switch(input)
    {
        case LCD_HEAT_ON:
            {
                SEG28_DEF |= 0x01u;
            }
        break;
        
        case LCD_HEAT_OFF:
            {
                SEG28_DEF &= 0xFEu;
            }
        break;
        
        case LCD_FAN_ON:
            {
                SEG34_DEF |= 0x01u;
            }
        break;
        
        case LCD_FAN_OFF:
            {
                SEG34_DEF &= 0xFEu;
            }
        break;
        
        case LCD_ZONE_ON:
            {
                SEG33_DEF |= 0x01u;
            }
        break;
        
        case LCD_ZONE_OFF:
            {
                SEG33_DEF &= 0xFEu;
            }
        break;
        
        case LCD_VOLTS_ON:
            {
                SEG32_DEF |= 0x01u;
            }
        break;
        
        case LCD_VOLTS_OFF:
            {
                SEG32_DEF &= 0xFEu;
            }
        break;
        
        case LCD_HEART_ON:
            {
                SEG31_DEF |= 0x01u;
            }
        break;
        
        case LCD_HEART_OFF:
            {
                SEG31_DEF &= 0xFEu;
            }
        break;
        
        case LCD_DEGREESC_ON:
            {
                SEG17_DEF |= 0x04u;
            }
        break;
        
        case LCD_DEGREESC_OFF:
            {
                SEG17_DEF &= 0xFBu;
            }
        break;
        
        case LCD_DEGREESF_ON:
            {
                SEG17_DEF |= 0x08u;
            }
        break;
        
        case LCD_DEGREESF_OFF:
            {
                SEG17_DEF &= 0xF7u;
            }
        break;
        
        case LCD_ALARM_ON:
            {
                SEG19_DEF |= 0x02u;
            }
        break;
        
        case LCD_ALARM_OFF:
            {
                SEG19_DEF &= 0xFDu;
            }
        break;
        
        case LCD_ALARM_ON_1:
            {
                SEG19_DEF |= 0x02u;
            }
        break;
        
        case LCD_ALARM_OFF_1:
            {
                SEG19_DEF &= 0xFDu;
            }
        break;
        
        case LCD_AM_ON:
            {
                SEG19_DEF |= 0x01u;
            }
        break;
        
        case LCD_AM_OFF:
            {
                SEG19_DEF &= 0xFEu;
            }
        break;
        
        case LCD_AM_ON_1:
            {
                SEG19_DEF |= 0x01u;
            }
        break;
        
        case LCD_AM_OFF_1:
            {
                SEG28_DEF &= 0xFEu;
            }
        break;
        
        case LCD_PM_ON:
            {
                SEG17_DEF |= 0x02u;
            }
        break;
        
        case LCD_PM_OFF:
            {
                SEG17_DEF &= 0xFDu;
            }
        break;
        
        case LCD_PM_ON_1:
            {
                SEG17_DEF |= 0x02u;
            }
        break;
        
        case LCD_PM_OFF_1:
            {
                SEG17_DEF &= 0xFDu;
            }
        break;
        
        case LCD_COOL_ON:
            {
                SEG30_DEF |= 0x01u;
            }
        break;
        
        case LCD_COOL_OFF:
            {
                SEG30_DEF &= 0xF0u;
            }
        break;
        
        default:
            {
                /* Bad Selection - Do Nothing */
                status = 1u;
            }
        break;
    }
    
    /* Return the status */
    return status;
}
/***********************************************************************************************************************
End of function Symbol_Map
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Init_Display_Panel
* Description   : Calls functions in order to prepare the LCD Panel for use
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void Init_Display_Panel (void)
{
    /* Load up Segment Maps */
    Init_Maps();
    
    /* Clear LCD Data Registers */
    Clear_Display();
    
    /* Enable the LCD */
    R_LCD_Start();
    
    /* Enable the voltage boost circuit */
    R_LCD_Set_Voltage_On();
}
/***********************************************************************************************************************
End of function Init_Display_Panel
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Clear_Display
* Description   : Clears all the segment data registers, thereby clearing the 
*                 screen by the end of the next LCD frame duration.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void Clear_Display (void)
{
    /* Declare a loop count variable */
    uint8_t i;
    
     /* Initialise pointer to the start of LCD data registers */
    volatile uint8_t * RegPtr = &SEG1_DEF;
    
    /* Execute the instructions in the loop 40 times */
    for (i = 0; i < 40;i++)
    {
        /* Write 0 to the register being pointed to.*/
        *RegPtr = 0u;
        
        /* Increment the pointer */
        RegPtr++;
    }
}
/***********************************************************************************************************************
End of function Clear_Display
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Init_Maps
* Description   : initialises the glyph-segment maps used to display letters, symbols and numbers.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void Init_Maps (void)
 {
     /* initialise section A maps */
     init_secta_maps();
     
     /* initialise section B maps */
     init_sectb_maps();
     
     /* initialise section C maps */
     init_sectc_maps();
 }
/***********************************************************************************************************************
End of function Init_Maps
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : init_secta_maps
* Description   : initialises Sect A maps.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void init_secta_maps (void)
{
     /* Section A maps START */
     
     /* 0 */
     SECTA_map[0x0].WORD = 0x00F5u;
     
     /* 1 */
     SECTA_map[0x1].WORD = 0x0060u;
     
     /* 2 */
     SECTA_map[0x2].WORD = 0x00B6u;
     
     /* 3 */
     SECTA_map[0x3].WORD = 0x00F2u;
     
     /* 4 */
     SECTA_map[0x4].WORD = 0x0063u;
     
     /* 5 */
     SECTA_map[0x5].WORD = 0x00D3u;
     
     /* 6 */
     SECTA_map[0x6].WORD = 0x00D7u;
     
     /* 7 */
     SECTA_map[0x7].WORD = 0x0070u;
     
     /* 8 */
     SECTA_map[0x8].WORD = 0x00F7u;
     
     /* 9 */
     SECTA_map[0x9].WORD = 0x00F3u;
     
     /* dummy */
     SECTB_map[0xA].WORD = 0x0000u;
     
     /* dummy */
     SECTB_map[0xB].WORD = 0x0000u;
     
     /* dummy */
     SECTB_map[0xC].WORD = 0x0000u;
     
     /* dummy */
     SECTB_map[0xD].WORD = 0x0000u;
     
     /* A */
     SECTA_map[0xE].WORD = 0x0077u;
     
     /* B */
     SECTA_map[0xF].WORD = 0x00C7u;
     
     /* C */
     SECTA_map[0x10].WORD = 0x0095u;
     
     /* D */
     SECTA_map[0x11].WORD = 0x00E6u;
     
     /* E */
     SECTA_map[0x12].WORD = 0x0097u;
     
     /* F */
     SECTA_map[0x13].WORD = 0x0017u;
}
/***********************************************************************************************************************
End of function init_secta_maps
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : init_sectb_maps
* Description   : initialises Sect B maps.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void init_sectb_maps (void)
{
     /* 0 */
     SECTB_map[0x0].WORD = 0x00FAu;
     
     /* 1 */
     SECTB_map[0x1].WORD = 0x0060u;
     
     /* 2 */
     SECTB_map[0x2].WORD = 0x00D6u;
     
     /* 3 */
     SECTB_map[0x3].WORD = 0x00F4u;
     
     /* 4 */
     SECTB_map[0x4].WORD = 0x006Cu;
     
     /* 5 */
     SECTB_map[0x5].WORD = 0x00BCu;
     
     /* 6 */
     SECTB_map[0x6].WORD = 0x00BEu;
     
     /* 7 */
     SECTB_map[0x7].WORD = 0x00E0u;
     
     /* 8 */
     SECTB_map[0x8].WORD = 0x00FEu;
     
     /* 9 */
     SECTB_map[0x9].WORD = 0x00FCu;
     
     /* dummy */
     SECTB_map[0xA].WORD = 0x0000u;
     
     /* dummy */
     SECTB_map[0xB].WORD = 0x0000u;
     
     /* dummy */
     SECTB_map[0xC].WORD = 0x0000u;
     
     /* dummy */
     SECTB_map[0xD].WORD = 0x0000u;
     
     /* A */
     SECTB_map[0xE].WORD = 0x00EEu;
     
     /* B */
     SECTB_map[0xF].WORD = 0x003Eu;
     
     /* C */
     SECTB_map[0x10].WORD = 0x009Au;
     
     /* D */
     SECTB_map[0x11].WORD = 0x0076u;
     
     /* E */
     SECTB_map[0x12].WORD = 0x009Eu;
     
     /* F */
     SECTB_map[0x13].WORD = 0x008Eu;
}
/***********************************************************************************************************************
End of function init_sectb_maps
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : init_sectc_maps
* Description   : initialises Sect C maps.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void init_sectc_maps (void)
{
     /* Section C maps START */
     
     /* Numbers */
     
     /* 0 */
     SECTC_map[0].WORD = 0xC3AAu;
     
     /* 1 */
     SECTC_map[1].WORD = 0x00A0u;
     
     /* 2 */
     SECTC_map[2].WORD = 0x816Cu;
     
     /* 3 */
     SECTC_map[3].WORD = 0x81E4u;
     
     /* 4 */
     SECTC_map[4].WORD = 0x00E6u;
     
     /* 5 */
     SECTC_map[5].WORD = 0x81C6u;
     
     /* 6 */
     SECTC_map[6].WORD = 0x81CEu;
     
     /* 7 */
     SECTC_map[7].WORD = 0x01A0u;
     
     /* 8 */
     SECTC_map[8].WORD = 0x81EEu;
     
     /* 9 */
     SECTC_map[9].WORD = 0x81E6u; 
     
     /* Symbols */
     
     /* + */
     SECTC_map[10].WORD = 0x2444u;
     
     /* - */
     SECTC_map[11].WORD = 0x0044u;
     
     /* / */
     SECTC_map[12].WORD = 0x4200u;
     
     /* \ */
     SECTC_map[13].WORD = 0x1800u;
     
     /* * */
     SECTC_map[40].WORD = 0x7e44u;
     
     /* initialise letters */
     init_sectc_letters_maps();
}
/***********************************************************************************************************************
End of function init_sectc_maps
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : init_sectc_letters_maps
* Description   : initialises Sect C maps.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void init_sectc_letters_maps (void)
{
     /* Letters */
     
     /* A */
     SECTC_map[14].WORD = 0x01EEu;
     
     /* B */
     SECTC_map[15].WORD = 0xA5E0u;
     
     /* C */
     SECTC_map[16].WORD = 0x810Au;
     
     /* D */
     SECTC_map[17].WORD = 0xA5A0u;
     
     /* E */
     SECTC_map[18].WORD = 0x814Eu;
     
     /* F */
     SECTC_map[19].WORD = 0x014Eu;
     
     /* G */
     SECTC_map[20].WORD = 0x81CAu;
     
     /* H */
     SECTC_map[21].WORD = 0x00EEu;
     
     /* I */
     SECTC_map[22].WORD = 0xA500u;
     
     /* J */
     SECTC_map[23].WORD = 0x80A0u;
     
     /* K */
     SECTC_map[24].WORD = 0x0A0Eu;
     
     /* L */
     SECTC_map[25].WORD = 0X800Au;
     
     /* M */
     SECTC_map[26].WORD = 0x12AAu;
     
     /* N */
     SECTC_map[27].WORD = 0x18AAu;
     
     /* O */
     SECTC_map[28].WORD = 0x81AAu;
     
     /* P */
     SECTC_map[29].WORD = 0x016Eu;
     
     /* Q */
     SECTC_map[30].WORD = 0x89AAu;
     
     /* R */
     SECTC_map[31].WORD = 0x096Eu;
     
     /* S */
     SECTC_map[32].WORD = 0x81C6u;
     
     /* T */
     SECTC_map[33].WORD = 0x2500u;
     
     /* U */
     SECTC_map[34].WORD = 0x80AAu;
     
     /* V */
     SECTC_map[35].WORD = 0x420Au;
     
     /* W */
     SECTC_map[36].WORD = 0x48AAu;
     
     /* X */
     SECTC_map[37].WORD = 0x5A00u;
     
     /* Y */
     SECTC_map[38].WORD = 0x1600u;
     
     /* Z */
     SECTC_map[39].WORD = 0xC300u;
}
/***********************************************************************************************************************
End of function init_sectc_letters_maps
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Display_Panel_String
* Description     This function writes to line 1, line 2 or 3 of the LCD. You need to use the defines LCD_LINE1, 
                  LCD_LINE2 and LCD_LINE3 in order to specify the starting position. 
* Arguments     : (uint8_t) position    - LCD row (symbol) number.
*                 (const char *) string - pointer to data to be written to display (up to 8 chars).
* Return Value  : None
***********************************************************************************************************************/
void Display_Panel_String (uint8_t const position, char * const string)
{
    /* Declare variable to hold the output data */
    uint8_t output_buf = 0u;
    
    /* Declare a variable flag to hold the input validation result */
    uint16_t flag       = 0u;
    
    /* Declare a loop count variable */
    uint16_t i         = 0u;
    
    /* Declare a symbol select variable */
    uint16_t symbsel   = 0u;

    /* Display input onto LCD.
       Castings used to ensure correct data types in assignments
       and passing on of arguments */
    switch(position)
    {
        /* Start Section 1, Numerical */
        case PANEL_LCD_LINE2:
            {
                /* Cycle Through Each Digit */
                for (i = 0u; i < 7u; i++)
                {
                    /* Validate Input */
                    flag = Validate_Input((uint8_t)string[i], &output_buf);
                    
                    /* Is Hex Number or recognized symbol? */
                    if ((1u == flag) || (2u == flag) ||(4u == flag))
                    {
                        /* Map Glyph to Segment Pattern */
                        SECTB_Glyph_Map(output_buf, (i+1u));
                    }
                }
            }
        break;
        
        /* Start Section C, Alphanumeric */
        case PANEL_LCD_LINE1:
            {
                /* Cycle through Each Section */
                for (i = 0u; i < 5u; i++)
                {
                    /* Validate Input */
                    flag = Validate_Input((uint8_t)string[i], &output_buf);
                    
                    /* Is Alphanumeric or Supported Symbol? */
                    if (flag)
                    {
                        /* Map Glyph to Segment Pattern */
                        SECTC_Glyph_Map(output_buf, (i+2u));
                    }
                }
            }
        break;
        
        /* Start Section 2, Signed Numerical */
        case PANEL_LCD_LINE3:
            {
                /* Cycle through each digit */
                for (i = 0u; i < 5u; i++)
                {
                    /* Validate Input.
                       Casting used to ensure correct data type is used. */
                    flag = Validate_Input((uint8_t)string[i], &output_buf);
                
                    /* Is Number? */
                    if ((1u == flag) || (2u == flag) || (4u == flag))
                    {
                        /* Map Glyph to Segment Pattern */
                        SECTA_Glyph_Map(output_buf, i);
                    }
                }
            }
        break;
        
        /* Start Symbol Section */
        case PANEL_LCD_SYMBOL:
            {
                /* Check if the symbol is lower or upper case f */
                if (('F' == string[0]) || ('f' == string[0]))
                {
                    /* Validate Input */
                    flag = Validate_Input ((uint8_t)string[1], &output_buf);
                
                    /* Is Number ? */
                    if (1u == flag)
                    {        
                        /* Map Glyph to Segment Pattern */
                        SECTF_Glyph_Map(output_buf);
                    }
                }
                else if ('-' == string[0])
                {
                    if (string[1])
                    {
                        /* Turn on minus symbol */
                        SEG11_DEF |= 0x08u;
                    }
                    else
                    {
                        /* Turn off minus symbol */
                        SEG11_DEF &= 0xF7u;
                    }
                    
                }
                else
                {
                    /* Convert To Symbol Code.
                       Casting used to ensure correct data type is used. */
                    symbsel = (uint16_t)string[0];
                    symbsel = symbsel << 8;
                    symbsel |= (uint16_t)string[1];
                    
                    /* Map Symbol to Segment */
                    Symbol_Map(symbsel);
                }
            }
        break;
        
        /* Do nothing for undefined selections */
        default:
            {
                /* Bad Selection */
            }
        break;
    }
}
/***********************************************************************************************************************
End of function Display_Panel_String
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Display_Panel_Delay
* Description   : Delay routine for LCD or any other devices.
* Arguments     :(uint32_t) units - time in, approximately, microseconds
* Return Value  : None
***********************************************************************************************************************/
void Display_Panel_Delay (uint32_t units)
{
    /* Declare a delay count */
    uint32_t counter = units * PANEL_DELAY_TIMING;
    
    /* Decrement the counter until it reaches 0 */
    while (counter--)
    {
        /* Do nothing */
    }
}
/***********************************************************************************************************************
End of function Display_Panel_Delay
************************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: LCD_Off
* Description  : This function turns off all LCD segments.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void LCD_Off (void)
{
    SEG0 = 0x00;
    SEG1 = 0x00;
    SEG2 = 0x00;
    SEG3 = 0x00;
    SEG4 = 0x00;
    SEG5 = 0x00;
    SEG6 = 0x00;
    SEG7 = 0x00;
    SEG8 = 0x00;
    SEG9 = 0x00;
    SEG10 = 0x00;
    SEG11 = 0x00;
    SEG12 = 0x00;
    SEG13 = 0x00;
    SEG14 = 0x00;
    SEG15 = 0x00;
    SEG16 = 0x00;
    SEG19 = 0x00;
    SEG20 = 0x00;
    SEG21 = 0x00;
    SEG22 = 0x00;
    SEG23 = 0x00;
    SEG24 = 0x00;
    SEG25 = 0x00;
    SEG26 = 0x00;
    SEG27 = 0x00;
    SEG28 = 0x00;
    SEG31 = 0x00;
    SEG32 = 0x00;
    SEG33 = 0x00;
    SEG34 = 0x00;
    SEG35 = 0x00;
    SEG36 = 0x00;
    SEG37 = 0x00;
    SEG38 = 0x00;
}
/***********************************************************************************************************************
End of function LCD_Off
************************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: LCD_On
* Description  : This function turns on all LCD segments.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void LCD_On (void)
{
    SEG0 = 0x0f;
    SEG1 = 0x0f;
    SEG2 = 0x0f;
    SEG3 = 0x0f;
    SEG4 = 0x0f;
    SEG5 = 0x0f;
    SEG6 = 0x0f;
    SEG7 = 0x0f;
    SEG8 = 0x0f;
    SEG9 = 0x0f;
    
    /* Renesas symbol is always off */
    SEG10 = 0x07;
    SEG11 = 0x0f;
    SEG12 = 0x0f;
    SEG13 = 0x0f;
    SEG14 = 0x0f;
    SEG15 = 0x0f;
    SEG16 = 0x0f;
    SEG19 = 0x0f;
    SEG20 = 0x0f;
    SEG21 = 0x0f;
    SEG22 = 0x0f;
    SEG23 = 0x0f;
    SEG24 = 0x0f;
    SEG25 = 0x0f;
    SEG26 = 0x0f;
    SEG27 = 0x0f;
    
    /* The first 7-segment of the digital clock is turned off */
    SEG28 = 0x01;
    SEG31 = 0x0f;
    SEG32 = 0x0f;
    SEG33 = 0x0f;
    SEG34 = 0x0f;
    SEG35 = 0x0f;
    SEG36 = 0x0f;
    
    /* Turn off T12 */
    SEG37 = 0x0e;
    
    /* Turn off T11 */
    SEG38 = 0x0e;
}
/***********************************************************************************************************************
End of function LCD_On
************************************************************************************************************************/
#pragma GCC reset_options
