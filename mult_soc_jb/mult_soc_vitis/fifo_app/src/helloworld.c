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
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"

#define FIFO_BASE 0x43C10000

#define FPGA_REV_REG (FIFO_BASE + 0x00)
#define FIFO_RST_REG  (FIFO_BASE + 0x04)
#define WR_DATA_REG (FIFO_BASE + 0x08)
#define RD_REQ_REG (FIFO_BASE + 0x0C)
#define RD_DATA_REG (FIFO_BASE + 0x10)
#define STATUS_REG (FIFO_BASE + 0x14)
#define DATA_COUNT_REG (FIFO_BASE + 0x18)
#define CONFIG_REG (FIFO_BASE + 0x1C)
#define THRESHOLD_REG (FIFO_BASE + 0x20)
#define ST_EMPTY (1u << 0)
#define ST_FULL (1u << 1)
#define ST_RD_VALID (1u << 4)

static void print_status(void) {
    u32 s = Xil_In32(STATUS_REG);
    xil_printf("STATUS=0x%08x (rd_valid=%d a_full=%d a_empty=%d full=%d empty=%d) COUNT=%d\r\n", s, (s >> 4) & 1, (s >> 3) & 1, (s >> 2) & 1, (s >> 1) & 1, s & 1,
    Xil_In32(DATA_COUNT_REG));
}

static int fifo_pop(u32 *data) {
    Xil_Out32(RD_REQ_REG, 1);
    for (int i = 0; i < 1000; i++) {
        if (Xil_In32(STATUS_REG) & ST_RD_VALID) {
            *data = Xil_In32(RD_DATA_REG);
            return 1;
        }
    }
    return 0;
}


int main()
{
    init_platform();
    u32 cfg, thr, data;
    int errors = 0;

    xil_printf("\r\nECE 520/L - Custom AXI-Lite FIFO IP - Jamal Bhola\r\n");
    xil_printf("FPGA_REV_REG  = 0x%08x\r\n", Xil_In32(FPGA_REV_REG));
    cfg = Xil_In32(CONFIG_REG);
    thr = Xil_In32(THRESHOLD_REG);
    xil_printf("CONFIG_REG    = 0x%08x (READ_LATENCY=%d DEPTH=%d WIDTH=%d)\r\n",
               cfg, (cfg >> 24) & 0xF, (cfg >> 8) & 0xFFFF, cfg & 0xFF);
    xil_printf("THRESHOLD_REG = 0x%08x (A_FULL=%d A_EMPTY=%d)\r\n",
               thr, thr >> 16, thr & 0xFFFF);

    Xil_Out32(FIFO_RST_REG, 1);
    xil_printf("After FIFO reset:\r\n");
    print_status();

    xil_printf("Pushing 32 words \r\n");
    for (u32 i = 1; i <= 32; i++) {
        Xil_Out32(WR_DATA_REG, i * 100);
        xil_printf("  Push %2d: %d\r\n", i, i * 100);
    }
    print_status();

    xil_printf("Push to full FIFO \r\n");
    Xil_Out32(WR_DATA_REG, 0xFFFF);
    print_status();

    xil_printf("Popping 32 words \r\n");
    for (u32 i = 1; i <= 32; i++) {
        if (!fifo_pop(&data)) {
            xil_printf("  Pop %2d: RD_VALID timeout\r\n", i);
            errors++;
        } else {
            xil_printf("  Pop %2d: %d %s\r\n", i, data, (data == i * 100) ? "" : "<-- MISMATCH");
            if (data != i * 100) errors++;
        }
    }
    print_status();

    xil_printf("Pop from empty FIFO (RD_VALID should stay 0)\r\n");
    if (fifo_pop(&data)) { xil_printf("  ERROR: got data from empty FIFO\r\n"); errors++; }
    print_status();

    xil_printf("%s (%d errors)\r\nEnd of FIFO Test\r\n", errors ? "FAIL" : "PASS", errors);
    cleanup_platform();
    return 0;
}
