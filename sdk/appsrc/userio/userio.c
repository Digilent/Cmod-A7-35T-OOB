/******************************************************************************
 * @file userio.c
 *
 * @authors Elod Gyorgy
 *
 * @date 2015-Jan-03
 *
 * @copyright
 * (c) 2015 Copyright Digilent Incorporated
 * All Rights Reserved
 *
 * This program is free software; distributed under the terms of BSD 3-clause
 * license ("Revised BSD License", "New BSD License", or "Modified BSD License")
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name(s) of the above-listed copyright holder(s) nor the names
 *    of its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 * @desciption
 *
 * @note
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who          Date        Changes
 * ----- ------------ ----------- --------------------------------------------
 * 1.00  Elod Gyorgy  2015-Jan-03 First release
 *
 * </pre>
 *
 *****************************************************************************/

#include <stdio.h>
#include "xparameters.h"
#include "userio.h"

/*
 * The timer is used to generate a PWM signal for the RGB LEDs.
 * the first timer is used for the PWM Period, the second for setting the PWM Pulse
 */
#define TMR_NUM_PERIOD 0
#define TMR_NUM_PULSE 1

/*
 * To achieve a 1 KHz PWM frequency, the Timer should count to 100,000 ticks,
 * assuming the clock is 100 MHz (100,000,000/1,000/2).
 */
#define TMR_PERIOD_CNT 2000

/*
 * This is the number of ticks to keep the pulse high. The duty cycle is
 * TMR_PULSE_CNT/TMR_PERIOD_CNT
 */
#define TMR_PULSE_CNT 150

extern unsigned char u8UserIO;

static u32 rgb_color;
const u32 rgb_colorarray[] = {
		RGB_RED,
		RGB_GREEN,
		RGB_BLUE,
		RGB_MAGENTA,
		RGB_CYAN,
		RGB_YELLOW,
		RGB_WHITE,
		RGB_OFF
};

//Yea, these globals are stupid, but I needed access to the input and output XGpios from the ISR,
//and this is simpler than defining a custom struct that contains both. --Sam
static XGpio *RGB_global;

void fnUpdateLedsFromSwitches();
void fnUpdateRgbsFromButtons();

XStatus fnInitUserIO(XGpio *Gpio2, XTmrCtr *psTmr)
{
	/* Initialize the GPIO driver. If an error occurs then exit */

	/*
	 * Perform a self-test on the GPIO.  This is a minimal test and only
	 * verifies that there is not any bus error when reading the data
	 * register
	 */

	/*
	 * Setup direction register so the switches and buttons are inputs and the LED is
	 * an output of the GPIO
	 */


	RGB_global = Gpio2;
//
//	fnUpdateLedsFromSwitches();
	rgb_color = RGB_OFF;
	XGpio_DiscreteWrite(Gpio2, RGB_CHANNEL, 0);

	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */

	/*
	 * Initialize timer for RGB LED PWMing
	 */
	RETURN_ON_FAILURE(XTmrCtr_Initialize(psTmr, XPAR_AXI_TIMER_0_DEVICE_ID));
	RETURN_ON_FAILURE(XTmrCtr_SelfTest(psTmr, TMR_NUM_PERIOD));
	RETURN_ON_FAILURE(XTmrCtr_SelfTest(psTmr, TMR_NUM_PULSE));

	/*
	 * Setup the handler for the timer counter
	 */
	XTmrCtr_SetHandler(psTmr, fnUserIOTmrIsr, psTmr);

	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only. Also count down
	 */
	XTmrCtr_SetOptions(psTmr, TMR_NUM_PERIOD,
				XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION);
	/*
	 * The Pulse timer also uses the interrupt and counts down
	 */
	XTmrCtr_SetOptions(psTmr, TMR_NUM_PULSE,
				XTC_INT_MODE_OPTION | XTC_DOWN_COUNT_OPTION);


	XTmrCtr_SetResetValue(psTmr, TMR_NUM_PERIOD, TMR_PERIOD_CNT);
	XTmrCtr_SetResetValue(psTmr, TMR_NUM_PULSE, TMR_PULSE_CNT);

	/*
	 * Start the timer counter. Interrupts will begin being triggered (at the
	 * timer layer). Don't start the pulse timer until in the ISR
	 */
	XTmrCtr_Start(psTmr, TMR_NUM_PERIOD);

	return XST_SUCCESS;
}

void fnUpdateRgbsFromButtons(int rgb_col)
{
	rgb_color = rgb_col;
}

/*
 * Timer interrupt service routine
 */
void fnUserIOTmrIsr(void *CallBackRef, u8 TmrCtrNumber)
{
	XTmrCtr *psTmr = (XTmrCtr *)CallBackRef;


	if (TmrCtrNumber == TMR_NUM_PERIOD)
	{

		XTmrCtr_Start(psTmr, TMR_NUM_PULSE);

		XGpio_DiscreteWrite(RGB_global, RGB_CHANNEL, rgb_color);
	}
	else
	{
		XGpio_DiscreteWrite(RGB_global, RGB_CHANNEL, 7);
	}
}

