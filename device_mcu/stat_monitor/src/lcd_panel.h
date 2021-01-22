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
* File Name    : lcd_panel.h
* Version      : 1.00
* Device(s)    : R5F10RLC
* Tool-Chain   : CA78K0R
* Description  : This file contains functions and definitions used in the lcd_panel.c file.
* Creation Date: 21/01/2013
***********************************************************************************************************************/

#ifndef LCD_PANEL_H
#define LCD_PANEL_H

/***********************************************************************************************************************
Includes <System Includes> , Project Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"

/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/
/* Set to ensure base delay */
#define PANEL_DELAY_TIMING     0x05

/* Unsupported Segments can be Controlled by the RSK when the unconnected 
   pins are connected to the RSK. Set the correct pin addresses to the 
   #defines below. */
#define Aux_pin5               0xFFFFu
#define Aux_pin6               0xFFFFu
#define Aux_pin47              0xFFFFu
#define Aux_pin48              0xFFFFu
    
/* Use the ROM area to indicate that a data pointer is not to be used */
#define PDL_NO_PTR            (uint8_t *)0xFFFFu
    
/* Define Section Names */
#define EMPTY                  0
#define SECT1                 '1'
#define SECT2                 '2'
#define SECT3                 '3'
#define SECT4                 '4'
    
#define PANEL_LCD_LINE1       '3'
#define PANEL_LCD_LINE2       '1'
#define PANEL_LCD_LINE3       '2'
#define PANEL_LCD_SYMBOL      '4'
    
/* Define Symbol Names */
#define LCD_HEAT_ON            0x3131u
#define LCD_HEAT_OFF           0x3130u
#define LCD_FAN_ON             0x3231u
#define LCD_FAN_OFF            0x3230u
#define LCD_ZONE_ON            0x3331u
#define LCD_ZONE_OFF           0x3330u
#define LCD_MGML_ON            0x3431u
#define LCD_MGML_OFF           0x3430u
#define LCD_MMHG_ON            0x3531u
#define LCD_MMHG_OFF           0x3530u
#define LCD_VOLTS_ON           0x3631u
#define LCD_VOLTS_OFF          0x3630u
#define LCD_HEART_ON           0x3731u
#define LCD_HEART_OFF          0x3730u
#define LCD_DEGREESC_ON        0x3831u
#define LCD_DEGREESC_OFF       0x3830u
#define LCD_DEGREESF_ON        0x3931u
#define LCD_DEGREESF_OFF       0x3930u
#define LCD_COOL_ON            0x6531u
#define LCD_COOL_OFF           0x6630u
#define LCD_ALARM_ON           0x4131u
#define LCD_ALARM_OFF          0x4130u
#define LCD_ALARM_ON_1         0x6131u
#define LCD_ALARM_OFF_1        0x6130u
#define LCD_AM_ON              0x4231u
#define LCD_AM_OFF             0x4230u
#define LCD_AM_ON_1            0x6231u
#define LCD_AM_OFF_1           0x6230u
#define LCD_PM_ON              0x4331u
#define LCD_PM_OFF             0x4330u
#define LCD_PM_ON_1            0x6331u
#define LCD_PM_OFF_1           0x6330u

/***********************************************************************************************************************
Other definitions
***********************************************************************************************************************/
/* Case definitions */
#define CASE_0                 0u
#define CASE_1                 1u
#define CASE_2                 2u
#define CASE_3                 3u
#define CASE_4                 4u
#define CASE_5                 5u
#define CASE_6                 6u
#define CASE_7                 7u
#define CASE_8                 8u

/* LCD panel's mid-section 7-segments */
#define DIGIT_1                0u
#define DIGIT_2                1u
#define DECIMAL_P1             2u
#define DIGIT_3                3u

/* Glyph check value */
#define GLYPH_CHECK            0x29u
/* Battery levels */
#define BATT_LEVEL_0           0u
#define BATT_LEVEL_1           1u
#define BATT_LEVEL_2           2u
#define BATT_LEVEL_3           3u
#define BATT_LEVEL_4           4u
    
#define BATTERY_OUTLINE_ON     1u
#define BATTERY_OUTLINE_OFF    0u
    
/* Define Port1's pins 1 and 2 output mode macro */
#define P1_NORM_PINS           0u
#define P1_UART_PINS           1u
    
    
/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
/* Define Map Structure Used */
typedef union LCD_MAP
{    
    /* Declare a 16-bit variable which shares the same address space as  */
    uint16_t WORD;
    
    /* Define a 16 bit structure */
    struct
    {
        uint8_t ONE   : 4;
        uint8_t TWO   : 4;
        uint8_t THREE : 4;
        uint8_t FOUR  : 4;
    } NIBBLE;
} LCDMAP;

/* Week Days */
typedef enum 
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    ALL_DAYS
}Week_Days;

/* Battery Levels*/
typedef enum 
{
    LEVEL_0,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4
}BATTERY_LEVELs;

/***********************************************************************************************************************   
Function Prototypes
***********************************************************************************************************************/
/* Clears the screen by the next LCD frame duration */
void Clear_Display(void);
/* Turns on all available segments */
void LCD_On(void);
/* Turns off all available segments */
void LCD_Off(void);
/* Prepares the LCD Panel for use */
void Init_Display_Panel (void);

/* Maps the character to a segment pattern, 
   to be displayed on Section A of the LCD panel */
void SECTA_Glyph_Map (uint8_t const glyph, uint16_t const digit);
/* Maps the character to a segment pattern, 
   to be displayed on Section B of the LCD panel */
void SECTB_Glyph_Map (uint8_t const glyph, uint16_t const digit);
/* Maps the character to a segment pattern, 
   to be displayed on Section C of the LCD panel */
void SECTC_Glyph_Map (uint8_t const glyph, uint16_t const digit);
/* Maps the character to a segment pattern, 
   to be displayed on Section D of the LCD panel */
void SECTD_Glyph_Map (uint8_t const level, uint8_t const bat_outline);
/* Maps the character to a segment pattern, 
   to be displayed on Section F of the LCD panel */
void SECTF_Glyph_Map (uint8_t const day);
/* Turns on/off LCD Panel Symbols */
uint8_t Symbol_Map (uint16_t const input);
/* Initialises the glyph-segment maps used to display letters,
   symbols and numbers */
void Init_Maps (void);
/* Validates the input for the Update_Display */
uint16_t Validate_Input (uint8_t const input, uint8_t * const output);
/* Display string on LCD panel */
void Display_Panel_String (const uint8_t position, char * const string);
/* Delay routine for LCD */
void Display_Panel_Delay (uint32_t units);

/***********************************************************************************************************************
Segment Re-definitions
***********************************************************************************************************************/
#define     SEG1_DEF    SEG0
#define     SEG2_DEF    SEG0
#define     SEG3_DEF    SEG0
#define     SEG4_DEF    SEG1
#define     SEG5_DEF    SEG2
#define     SEG6_DEF    SEG3
#define     SEG7_DEF    SEG4
#define     SEG8_DEF    SEG5
#define     SEG9_DEF    SEG6
#define     SEG10_DEF   SEG7
#define     SEG11_DEF   SEG8
#define     SEG12_DEF   SEG9
#define     SEG13_DEF   SEG10
#define     SEG14_DEF   SEG11
#define     SEG15_DEF   SEG12
#define     SEG16_DEF   SEG13
#define     SEG17_DEF   SEG14
#define     SEG18_DEF   SEG15
#define     SEG19_DEF   SEG16
#define     SEG20_DEF   SEG19
#define     SEG21_DEF   SEG20
#define     SEG22_DEF   SEG21
#define     SEG23_DEF   SEG22
#define     SEG24_DEF   SEG23
#define     SEG25_DEF   SEG24
#define     SEG26_DEF   SEG25
#define     SEG27_DEF   SEG26
#define     SEG28_DEF   SEG27
#define     SEG29_DEF   SEG27
#define     SEG30_DEF   SEG28
#define     SEG31_DEF   SEG31
#define     SEG32_DEF   SEG32
#define     SEG33_DEF   SEG33
#define     SEG34_DEF   SEG34
#define     SEG35_DEF   SEG35
#define     SEG36_DEF   SEG36
#define     SEG37_DEF   SEG37
#define     SEG38_DEF   SEG38
#define     SEG39_DEF   SEG38


#endif
