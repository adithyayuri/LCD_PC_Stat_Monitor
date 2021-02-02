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
* File Name    : r_cg_port.c
* Version      : CodeGenerator for RL78/L12 V2.04.03.01 [14 Aug 2020]
* Device(s)    : R5F10RLC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PORT module.
* Creation Date: 02-02-2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
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
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    PFSEG2 = _01_PFSEG16_SEG | _00_PFSEG17_PORT | _00_PFSEG18_PORT | _08_PFSEG19_SEG | _10_PFSEG20_SEG |
             _20_PFSEG21_SEG | _40_PFSEG22_SEG | _80_PFSEG23_SEG;
    ISCLCD = _02_ISCVL3_INPUT_EFFECTIVE;
    P2 = _02_Pn1_OUTPUT_1;
    P12 = _20_Pn5_OUTPUT_1;
    P13 = _01_Pn0_OUTPUT_1;
    ADPC = _02_ADPC_DI_ON;
    PM2 = _01_PMn0_NOT_USE | _00_PMn1_MODE_OUTPUT | _FC_PM2_DEFAULT;
    PM3 = _01_PMn0_NOT_USE | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _F8_PM3_DEFAULT;
    PM12 = _01_PMn0_NOT_USE | _00_PMn5_MODE_OUTPUT | _40_PMn6_NOT_USE | _80_PMn7_NOT_USE | _1E_PM12_DEFAULT;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
