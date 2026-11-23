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
#define A_REG_ADDR (MULT_BASE_ADDR + 0x00)
#define B_REG_ADDR (MULT_BASE_ADDR + 0x04)
#define P_REG_ADDR (MULT_BASE_ADDR + 0x08)

int main()
{
    init_platform();

    xil_printf("Start of Multiplier Test\n");

    Xil_Out32(A_REG_ADDR, 8);
    Xil_Out32(B_REG_ADDR, 8);

    uint32_t a_val = Xil_In32(A_REG_ADDR);
    uint32_t b_val = Xil_In32(B_REG_ADDR);
    uint32_t p_val = Xil_In32(P_REG_ADDR);

    xil_printf("A: %d\n", a_val);
    xil_printf("B: %d\n", b_val);
    xil_printf("P: %d\n", p_val);

    xil_printf("End of Multiplier Test\n");

    cleanup_platform();
    return 0;
}
