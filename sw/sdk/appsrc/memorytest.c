/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#include <stdio.h>
#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xil_testmem.h"
#include "xgpio.h"
#include "microblaze_sleep.h"

#include "platform.h"
#include "memory_config.h"


#include "intc/intc.h"
#include "userio/userio.h"

static XIntc sIntc;

/*
 * memory_test.c: Test memory ranges present in the Hardware Design.
 *
 * This application runs with D-Caches disabled. As a result cacheline requests
 * will not be generated.
 *
 * For MicroBlaze/PowerPC, the BSP doesn't enable caches and this application
 * enables only I-Caches. For ARM, the BSP enables caches by default, so this
 * application disables D-Caches before running memory tests.
 */

/************************** Variable Definitions *****************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

XGpio Gpio1; /* The Instance of the GPIO Driver for Leds and buttons */
#define LED_CHANNEL 1
#define BTN_CHANNEL 2
#define LED_MASK  0x03
#define BTN_MASK  0x01

XGpio Gpio2; /* The Instance of the GPIO Driver for the RGB Leds */
#define RGB_MASK 0x07

void putnum(unsigned int num);
//void xil_printf(char *ptr);



static XTmrCtr sTmr;

// interrupt vector table
const ivt_t ivt[] = {
	// User I/O
	{XPAR_AXI_INTC_0_AXI_TIMER_0_INTERRUPT_INTR, (XInterruptHandler)XTmrCtr_InterruptHandler, &sTmr}
};


void test_memory_range(struct memory_range_s *range) {
    XStatus status;

    /* This application uses xil_printf statements instead of xil_xil_printff/xil_printff
     * to reduce the text size.
     *
     * The default linker script generated for this application does not have
     * heap memory allocated. This implies that this program cannot use any
     * routines that allocate memory on heap (xil_printff is one such function).
     * If you'd like to add such functions, then please generate a linker script
     * that does allocate sufficient heap memory.
     */

    xil_printf("Testing memory region: "); xil_printf(range->name);  xil_printf("\n\r");
    xil_printf("    Memory Controller: "); xil_printf(range->ip);  xil_printf("\n\r");
    xil_printf("         Base Address: 0x"); putnum(range->base); xil_printf("\n\r");
    xil_printf("                 Size: 0x"); putnum(range->size); xil_printf (" bytes \n\r");

    status = Xil_TestMem32((u32*)range->base, ((range->size)/32), 0xAAAA5555, XIL_TESTMEM_ALLMEMTESTS);
    xil_printf("          32-bit test: "); xil_printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); xil_printf("\n\r");

    status = Xil_TestMem16((u16*)range->base, ((range->size)/16), 0xAA55, XIL_TESTMEM_ALLMEMTESTS);
    xil_printf("          16-bit test: "); xil_printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); xil_printf("\n\r");

    status = Xil_TestMem8((u8*)range->base, ((range->size)/8), 0xA5, XIL_TESTMEM_ALLMEMTESTS);
    xil_printf("           8-bit test: "); xil_printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); xil_printf("\n\r");

}

int main()
{
    int i;
    int Status = 0;
    int leds = 0;
    int rgb_col = 0;
    int button = 0;
    int last_button = 0;

    init_platform();

    xil_printf("--Starting Memory Test Application--\n\r");
    xil_printf("NOTE: This application runs with D-Cache disabled.");
    xil_printf("As a result, cacheline requests will not be generated\n\r");

    for (i = 0; i < n_memory_ranges; i++) {
        test_memory_range(&memory_ranges[i]);
    }

    xil_printf("--Memory Test Application Complete--\n\r");


    //once past memory testing enter GPIO loop
    Status = XGpio_Initialize(&Gpio1, XPAR_AXI_GPIO_0_DEVICE_ID);
    	if (Status != XST_SUCCESS) {
    		return XST_FAILURE;
    	}

    	Status = XGpio_Initialize(&Gpio2, XPAR_AXI_GPIO_1_DEVICE_ID);
    	    	if (Status != XST_SUCCESS) {
    	    		return XST_FAILURE;
    	    	}

	//set leds to outputs and the button to an input
	XGpio_SetDataDirection(&Gpio1, LED_CHANNEL, ~LED_MASK);
	XGpio_SetDataDirection(&Gpio1, BTN_CHANNEL, BTN_MASK);
	XGpio_SetDataDirection(&Gpio2, RGB_CHANNEL, ~RGB_MASK);

	// Initialize the interrupt controller
		Status = fnInitInterruptController(&sIntc);
		if(Status != XST_SUCCESS) {
			xil_printf("\r\nError initializing interrupts");
			return XST_FAILURE;
		}

	// Initializing User I/O GPIO driver
		Status = fnInitUserIO(&Gpio2, &sTmr);
		if(Status != XST_SUCCESS) {
			xil_printf("\r\nError initializing User I/O");
			return XST_FAILURE;
		}

		// Enable all interrupts in our interrupt vector table
		// Make sure all driver instances using this IVT are initialized first
		fnEnableInterrupts(&sIntc, &ivt[0], sizeof(ivt)/sizeof(ivt[0]));


    while(1){
    	//read button
    	button = XGpio_DiscreteRead(&Gpio1, BTN_CHANNEL);
    	//if it is 1 and the last press was zero then assign new leds
    	if(button == BTN_MASK && button != last_button){

    		last_button = button;
    		rgb_col ++;
    		rgb_col = rgb_col % 8;
    		leds ++;
    		leds = leds % 4;
    		switch(rgb_col){									  //colors	  RGB
    		    		case 1: fnUpdateRgbsFromButtons(3);break; //red 	0b011
    		    		case 2: fnUpdateRgbsFromButtons(5);break; //green 	0b101
    		    		case 3: fnUpdateRgbsFromButtons(6);break; //blue	0b110
    		    		case 4: fnUpdateRgbsFromButtons(4);break; //cyan    0b100
    		    		case 5: fnUpdateRgbsFromButtons(2);break; //magenta	0b010
    		    		case 6: fnUpdateRgbsFromButtons(1);break; //yellow  0b001
    		    		case 7: fnUpdateRgbsFromButtons(0);break; //white   0b000
    		    		case 0: fnUpdateRgbsFromButtons(7);break; //off		0b111
    		    		}
    		XGpio_DiscreteWrite(&Gpio1, LED_CHANNEL, leds & LED_MASK);
    		xil_printf("\r\nButton Pressed");
    	}
    	else
    		last_button = button;


    }

    cleanup_platform();
    return 0;
}
