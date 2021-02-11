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
* Creation Date: 05-02-2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
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

struct packet{
	uint8_t start_b1;
	uint8_t start_b2;
	rtc_counter_value_t date_time;
	struct cpu_elements cpu;
	struct gpu_elements gpu;
	uint8_t checksum;
};

const uint8_t clear[5] = "    ";
volatile uint8_t line1buf[5] = "    ";
char line3_buffer[5] = "00.0";
uint8_t time_delta;
uint8_t disp_mode; // 0 - ClockMode, 1 - CPUMODE, 2 - GPUMODE
uint8_t result;
const uint8_t _LCD_DELAY = 1;

// Global packet structure
struct packet pckt;

// Code to check for valid packet
int check_for_packet(struct packet *data);
// Dummy delay routine
void Delay (uint32_t cycles);
// Difference between RTC time and data provided
int time_difference (struct packet *data);
// Application
void application(void);

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

    // Start serial comms
    R_UART0_Start();
    Delay(200);

    // Start RTC peripheral
    R_RTC_Start();
    Delay(200);

    // Enable interrupt for SW1
    R_INTC0_Start();
    // Enable interrupt for SW2
    R_INTC3_Start();
    // Enable interrupt for SW3
    R_INTC4_Start();

    // Set default MODE
    disp_mode = 0;

    while (1U)
    {
    	application();
    	STOP;
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

int check_for_packet(struct packet *data)
{
	uint16_t total;
	uint8_t temp_byte;
	while (cb->count >= 16)
	{
		temp_byte = cbuff_remove(cb);
		if (temp_byte == 0xFE)
		{
			temp_byte = cbuff_remove(cb);
			if (temp_byte == 0xFD)
			{
				data->date_time.sec = cbuff_remove(cb); 				// 01
				data->date_time.min = cbuff_remove(cb);					// 02
				data->date_time.hour = cbuff_remove(cb);				// 03
				data->date_time.day = cbuff_remove(cb);					// 04
				data->date_time.week = cbuff_remove(cb);				// 05
				data->date_time.month = cbuff_remove(cb);				// 06
				data->date_time.year = cbuff_remove(cb);				// 07
				data->cpu.pkg_temp = cbuff_remove(cb);					// 08
				data->cpu.pkg_temp_fraction = cbuff_remove(cb);			// 09
				data->cpu.max_core_temp = cbuff_remove(cb);				// 10
				data->cpu.max_core_temp_fraction = cbuff_remove(cb);	// 11
				data->gpu.pkg_temp = cbuff_remove(cb);					// 12
				data->gpu.pkg_temp_fraction = cbuff_remove(cb);			// 13

				total = data->date_time.sec +
						data->date_time.min +
						data->date_time.hour +
						data->date_time.day +
						data->date_time.week +
						data->date_time.month +
						data->date_time.year +
						data->cpu.pkg_temp +
						data->cpu.pkg_temp_fraction +
						data->cpu.max_core_temp +
						data->cpu.max_core_temp_fraction +
						data->gpu.pkg_temp +
						data->gpu.pkg_temp_fraction;

				if ((total % 127) == cbuff_remove(cb))
				{
					cbuff_reset(cb);
					return 0;
				}
				else
				{
					cbuff_reset(cb);
					break;
				}
			}
		}
	}
	return 1;

}

void Delay (uint32_t cycles)
{
    /* Declare a delay count */
    uint32_t counter = cycles;

    /* Decrement the counter until it reaches 0 */
    while (counter--)
    {
        /* Do nothing */
    }
}

int time_difference (struct packet *data)
{
	if (abs(YEAR - data->date_time.year) > 0		||
		abs(MONTH - data->date_time.month) > 0		||
		abs(WEEK - data->date_time.week) > 0		||
		abs(DAY - data->date_time.day) > 0			||
		abs(HOUR - data->date_time.hour) > 0		||
		abs(MIN - data->date_time.min) > 0		)
	{
		return 0xFF;
	}

	return abs(SEC - data->date_time.sec);
}

void application(void)
{
	uint8_t temp_mode;
	temp_mode = disp_mode;

	result = check_for_packet(&pckt);
	if (result == 0){
		// Get time delta value
		time_delta = time_difference(&pckt);

		// Update time if outdated
		if (time_delta > 5){
			 R_RTC_Set_CounterValue(pckt.date_time);
		}
		TXD0 = temp_mode;
		if (temp_mode == 1){
			// CPU Temp mode
			line3_buffer[0] = (int8_t)((pckt.cpu.pkg_temp >> 4) + 0x30);
			line3_buffer[1] = (int8_t)((pckt.cpu.pkg_temp & 0x0F) + 0x30);
			line3_buffer[2] = '.';
			line3_buffer[3] = (int8_t)((pckt.cpu.pkg_temp_fraction) + 0x30);
	        Display_Panel_String(PANEL_LCD_LINE3, line3_buffer);
	        Symbol_Map(LCD_DEGREESC_ON);
	        Display_Panel_Delay(_LCD_DELAY);
	        Display_Panel_String(PANEL_LCD_LINE1, "    ");
	    	Display_Panel_Delay(_LCD_DELAY);
	        Display_Panel_String(PANEL_LCD_LINE1, "CPU");
		}
		if (temp_mode == 2){
			// CPU Temp mode
			line3_buffer[0] = (int8_t)((pckt.gpu.pkg_temp >> 4) + 0x30);
			line3_buffer[1] = (int8_t)((pckt.gpu.pkg_temp & 0x0F) + 0x30);
			line3_buffer[2] = '.';
			line3_buffer[3] = (int8_t)((pckt.gpu.pkg_temp_fraction) + 0x30);
	        Display_Panel_String(PANEL_LCD_LINE3, line3_buffer);
	        Symbol_Map(LCD_DEGREESC_ON);
	        Display_Panel_Delay(_LCD_DELAY);
	        Display_Panel_String(PANEL_LCD_LINE1, "    ");
	    	Display_Panel_Delay(_LCD_DELAY);
	        Display_Panel_String(PANEL_LCD_LINE1, "GPU");
		}
	}

	// Get time delta value
	time_delta = time_difference(&pckt);

	// Update time if outdated
	if (time_delta > 10){
		disp_mode = 0;
	}
}

/* End user code. Do not edit comment generated here */
