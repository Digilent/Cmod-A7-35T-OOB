/******************************************************************************
 * @file userio.h
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

#ifndef USERIO_H_
#define USERIO_H_

#include "xstatus.h"
#include "xgpio.h"
#include "xtmrctr.h"

#define RGB_CHANNEL	 1	/* Channel 2 of the output GPIO Device */
#define SW_INTERRUPT XGPIO_IR_CH1_MASK  /* Channel 1 Interrupt Mask */
#define BTN_INTERRUPT XGPIO_IR_CH2_MASK  /* Channel 2 Interrupt Mask */

//#define BTNU_MASK 	0x0100
//#define BTNR_MASK 	0x0200
//#define BTND_MASK 	0x0400
//#define BTNL_MASK 	0x0800
//#define BTNC_MASK 	0x1000
#define SWS_MASK	0x000F
#define BTNS_MASK 0x001F
#define RESET_MASK 0x0010

#define LEDS_MASK	0x00F
#define RGBS_MASK	0xFFF

#define RETURN_ON_FAILURE(x) if ((x) != XST_SUCCESS) return XST_FAILURE;

#define RGB_WHITE 0b000
#define RGB_RED 0b011
#define RGB_GREEN 0b101
#define RGB_YELLOW 0b101
#define RGB_CYAN 0b101
#define RGB_MAGENTA 0b101
#define RGB_BLUE 0b110
#define RGB_OFF 0b111


XStatus fnInitUserIO(XGpio *psGpio_out, XTmrCtr *psTmr);
void fnUserIOIsr(void *pvInst);
void fnUserIOTmrIsr(void *CallBackRef, u8 TmrCtrNumber);
void fnUpdateRgbsFromButtons(int rgb_col);

#endif /* USERIO_H_ */
