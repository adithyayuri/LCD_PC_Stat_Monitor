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
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/L12 V2.04.03.01 [14 Aug 2020]
* Device(s)    : R5F10RLC
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 02-02-2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
#include "rcg_lcd.h"
#include "lcd_panel.h"
#include "r_cg_userdefine.h"
#include "buffer.h"
#include "rcg_serial_user.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
cbuff_t * cb;
long delay=0;
int a;
uint8_t temp;
uint8_t text[6] = "Hello\n";


struct cpu_elements{
	uint8_t pkg_temp;
	uint8_t pkg_temp_fraction;
	uint8_t max_core_temp;
	uint8_t max_core_temp_fraction;
};

struct gpu_elements{
	uint8_t pkg_temp;
	uint8_t pkg_temp_fraction;
};

struct reserved_elements{
	uint8_t reserved_1;
};

struct packet{
	uint8_t start_b1;
	uint8_t start_b2;
	rtc_counter_value_t date_time;
	struct cpu_elements cpu;
	struct gpu_elements gpu;
	struct reserved_elements reserved;
};


void check_for_packet(void);
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    R_UART0_Start();
    for(delay=0; delay<200; delay++);
    R_RTC_Start();
    //R_UART0_Send(text, 6);
    while (1U)
    {

    	for(delay=0; delay<200; delay++);
    	check_for_packet();

    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* Setup the LCD panel */
    R_LCD_Create();
    /* Initialize the LCD panel */
    Init_Display_Panel();
    /* Initialize ring buffer */
    init_buffer();

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void check_for_packet(void)
{
	uint8_t temp_byte;
	while (cb->count >= 16)
	{
		temp_byte = cbuff_remove(cb);
		if (temp_byte == 0xFE)
		{
			TXD0 = 0x11;
			cbuff_reset(cb);
		}
	}
}

/* End user code. Do not edit comment generated here */
