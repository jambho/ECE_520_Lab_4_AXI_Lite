/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xil_io.h"

#define MULT_BASE_ADDR (0x43C00000)
#define BRAM_BASE_ADDR (0x40000000)
#define A_REG_ADDR (MULT_BASE_ADDR + 0x00)
#define B_REG_ADDR (MULT_BASE_ADDR + 0x04)
#define P_REG_ADDR (MULT_BASE_ADDR + 0x08)

int main()
{
    init_platform();
    xil_printf("ECE 520/L - Custom AXI IP Lab - Jamal Bhola\n");
    xil_printf("Start of Multiplier + BRAM Test\n");
    for(int i = 0; i < 32; i++){
        Xil_Out32(A_REG_ADDR, i+1);
        Xil_Out32(B_REG_ADDR, i+1);
        Xil_Out32(BRAM_BASE_ADDR + (i*4), Xil_In32(P_REG_ADDR));
        xil_printf("Stored: %d x %d = %d at BRAM[%d]\n", i+1, i+1, Xil_In32(BRAM_BASE_ADDR + (i*4)), i);
    }


    xil_printf("BRAM Contents:\n");
    for(int i = 0; i < 32; i++){
        xil_printf("BRAM[%d]: %d\n",i, Xil_In32(BRAM_BASE_ADDR + (i*4)));
        }


    xil_printf("End of Multiplier + BRAM Test\n");

    cleanup_platform();
    return 0;
}
