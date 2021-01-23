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
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/L12 V2.04.03.01 [14 Aug 2020]
* Device(s)    : R5F10RLC
* Tool-Chain   : CCRL
* Description  : This file includes user definition.
* Creation Date: 24-01-2021
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */


/* Logical values  */
#define TRUE                     (1)
#define FALSE                    (0)

/* LEDs */
#define LED1                     P13_bit.no0
#define LED2                     P2_bit.no1
#define LED3                     P12_bit.no5

/* LEDs Port Direction */
#define LED1_DIR                 PM2_bit.no1
#define LED2_DIR                 PM12_bit.no5

#define LED_ON                   (0)
#define LED_OFF                  (1)

#define SET					     (1U)
#define CLEAR                    (0U)

/* Switches */
#define SW1                      P13_bit.no7
#define SW2                      P3_bit.no1
#define SW3                      P3_bit.no2

#define ASCII_0					 ('0')

#define DIV_10                   (10)
#define DIV_100                  (100)
#define DIV_1000                 (1000)

#define MULT_10 				 (10)
#define MULT_100                 (100)
#define MULT_1000                (1000)

#define MODULUS_10 				 (10)
#define MODULUS_100              (100)
#define MODULUS_1000             (1000)

#define CHECK_AM_PM				 (0x20u)

#define SENSOR_REF_TEMP_SCALED   (250)
#define INT_REF_V_145            (145)
#define INT_REF_V_SCALED         (145000)
#define INT_REF_TEMP_SCALED      (103000)
#define TEMP_SENSOR_GAIN_SCALED  (36)

#define CPU_IS_AWAKE			 (0x54)

#define ADC_RESULT_ALIGN	     (6U)

#define TAU0_CH3_CLCK_FREQ       (732U)

#define BYTE_SET_ALL             (0xFFU)
#define BYTE_CLEAR_ALL           (0x00U)

#define WORD_SET_ALL             (0xFFFFU)
#define WORD_CLEAR_ALL           (0x0000U)

#define BYTE_SHIFT               (8)

#define BITMASK_8B_UP_NIBBLE     (0xF0)
#define BITMASK_8B_LO_NIBBLE     (0x0F)

#define BITMASK_16B_UP_NIBBLE    (0xF000)
#define BITMASK_16B_UM_NIBBLE    (0x0F00)
#define BITMASK_16B_LM_NIBBLE    (0x00F0)
#define BITMASK_16B_LO_NIBBLE    (0x000F)

#define BITMASK_16B_UP_BYTE      (0xFF00)
#define BITMASK_16B_LO_BYTE      (0x00FF)

#define ASCII_NEW_LINE           ('\n')
#define ASCII_CARRIAGE_RETURN    ('\r')

#define LCD_DELAY                (0x5FFFF)


/* End user code. Do not edit comment generated here */
#endif
