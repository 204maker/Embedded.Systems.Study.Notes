/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//***************************************************************************************
//  RAMP generation
//
//  Description; Toggle P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP432P4xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  E. Chen
//  Texas Instruments, Inc
//  March 2015
//  Built with Code Composer Studio v6
//***************************************************************************************

// #include <c:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX8 0xff

//uint8_t data = 0;
uint8_t cnt = 0;
uint8_t data[]={0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,
                          0x98,0x9c,0x9f,0xa2,0xa5,0xa8,0xab,0xae,
                          0xb0,0xb3,0xb6,0xb9,0xbc,0xbf,0xc1,0xc4,
                          0xc7,0xc9,0xcc,0xce,0xd1,0xd3,0xd5,0xd8,
                          0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,
                          0xea,0xeb,0xed,0xef,0xf0,0xf2,0xf3,0xf4,
                          0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfb,0xfc,
                          0xfd,0xfd,0xfe,0xfe,0xfe,0xff,0xff,0xff,
                          0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfd,
                          0xfc,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,
                          0xf5,0xf4,0xf2,0xf1,0xef,0xee,0xec,0xeb,
                          0xe9,0xe7,0xe5,0xe3,0xe1,0xdf,0xdd,0xdb,
                          0xd9,0xd7,0xd4,0xd2,0xcf,0xcd,0xca,0xc8,
                          0xc5,0xc3,0xc0,0xbd,0xba,0xb8,0xb5,0xb2,
                          0xaf,0xac,0xa9,0xa6,0xa3,0xa0,0x9d,0x9a,
                          0x97,0x94,0x91,0x8e,0x8a,0x87,0x84,0x81,
                          0x7e,0x7b,0x78,0x75,0x71,0x6e,0x6b,0x68,
                          0x65,0x62,0x5f,0x5c,0x59,0x56,0x53,0x50,
                          0x4d,0x4a,0x47,0x45,0x42,0x3f,0x3c,0x3a,
                          0x37,0x35,0x32,0x30,0x2d,0x2b,0x28,0x26,
                          0x24,0x22,0x20,0x1e,0x1c,0x1a,0x18,0x16,
                          0x14,0x13,0x11,0x10,0xe,0xd,0xb,0xa,
                          0x9,0x8,0x7,0x6,0x5,0x4,0x3,0x3,
                          0x2,0x2,0x1,0x1,0x0,0x0,0x0,0x0,
                          0x0,0x0,0x0,0x1,0x1,0x1,0x2,0x2,
                          0x3,0x4,0x4,0x5,0x6,0x7,0x8,0x9,
                          0xb,0xc,0xd,0xf,0x10,0x12,0x14,0x15,
                          0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,0x25,
                          0x27,0x2a,0x2c,0x2e,0x31,0x33,0x36,0x38,
                          0x3b,0x3e,0x40,0x43,0x46,0x49,0x4c,0x4f,
                          0x51,0x54,0x57,0x5a,0x5d,0x60,0x63,0x67,
                          0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c,0x80};

void Initialize_Timer32(void);
void Timer32_IRQHandler(void);

//************************************************************************************//
void Timer32_IRQHandler(void)
{
    //* Put your code for the interrupt here�
    //* It should be something like P9OUT = data[cnt++};
    P9OUT = data[cnt++];

    //* You must clear the interrupt flag before exiting the interrupt
    Timer32_clearInterruptFlag(TIMER32_0_BASE );
}

//****************************************************************************************
// * The Device Driver Lib provides functions to configure the timer32 */
void Initialize_Timer32(void)
{
/* Enable T32 in periodic wrapping mode,  interrupt enabled */
Timer32_initModule (TIMER32_0_BASE, TIMER32_PRESCALER_1,
    TIMER32_16BIT, TIMER32_PERIODIC_MODE);

/* Set the initial count value, this will change the time of the period. */
//Timer32_setCount ( TIMER32_0_BASE, 0xffff); //t = 5.56s
Timer32_setCount ( TIMER32_0_BASE, 0xf000); //t = 5.20s
//Timer32_setCount ( TIMER32_0_BASE, 0x0f00); //t = 328ms
//Timer32_setCount ( TIMER32_0_BASE, 0x000f); //t = 5.430ms
/* Link the timer interrupt to the interrupt handler  */
Timer32_registerInterrupt ( TIMER32_0_INTERRUPT,  &Timer32_IRQHandler );

/* Enable the Timer decrementing */
Timer32_startTimer ( TIMER32_0_BASE, false );

/* Enable the Timer interrupts */
Timer32_enableInterrupt (TIMER32_0_BASE);
}


int main(void)
{
    //uint_fast8_t data=0;

    WDT_A_hold(WDT_A_BASE);                         // Stop watchdog timer
    printf("Configuring Port 9: ");                 // Print to console
    Initialize_Timer32();
    GPIO_setAsOutputPin(GPIO_PORT_P9, 0xff);        // Set P9 to output
    printf("Starting SINE WAVE:\n");                // Print to console

   while(1);
}


