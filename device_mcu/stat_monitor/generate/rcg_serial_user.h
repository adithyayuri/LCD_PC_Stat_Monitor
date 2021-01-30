/*
 * rcg_serial_user.h
 *
 *  Created on: 30-Jan-2021
 *      Author: YURI
 */

#ifndef RCG_SERIAL_USER_H_
#define RCG_SERIAL_USER_H_

#include "rcg_lcd.h"
#include "r_cg_rtc.h"
#include "lcd_panel.h"
#include "buffer.h"

void init_buffer(void);
void cbuff_print_uart(void);
#endif /* RCG_SERIAL_USER_H_ */
