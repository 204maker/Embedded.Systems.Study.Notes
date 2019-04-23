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

//unsigned char data[]  =
uint8_t data[]  =
                   { 0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95, 0x98,0x9b,0x9e,0xa2,0xa5,0xa7,0xaa,0xad,
                     0xb0,0xb3,0xb6,0xb9,0xbc,0xbe,0xc1,0xc4, 0xc6,0xc9,0xcb,0xce,0xd0,0xd3,0xd5,0xd7,
                     0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8, 0xea,0xeb,0xed,0xee,0xf0,0xf1,0xf3,0xf4,
                     0xf5,0xf6,0xf8,0xf9,0xfa,0xfa,0xfb,0xfc, 0xfd,0xfd,0xfe,0xfe,0xfe,0xff,0xff,0xff,
                     0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xfd, 0xfd,0xfc,0xfb,0xfa,0xfa,0xf9,0xf8,0xf6,
                     0xf5,0xf4,0xf3,0xf1,0xf0,0xee,0xed,0xeb, 0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
                     0xda,0xd7,0xd5,0xd3,0xd0,0xce,0xcb,0xc9, 0xc6,0xc4,0xc1,0xbe,0xbc,0xb9,0xb6,0xb3,
                     0xb0,0xad,0xaa,0xa7,0xa5,0xa2,0x9e,0x9b, 0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
                     0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a, 0x67,0x64,0x61,0x5d,0x5a,0x58,0x55,0x52,
                     0x4f,0x4c,0x49,0x46,0x43,0x41,0x3e,0x3b, 0x39,0x36,0x34,0x31,0x2f,0x2c,0x2a,0x28, /*,0x2c,0x2a,0x28,*/
                     0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17, 0x15,0x14,0x12,0x11,0x0f,0x0e,0x0c,0x0b,
                     0x0a,0x09,0x07,0x06,0x05,0x05,0x04,0x03, 0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x00,
                     0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02, 0x02,0x03,0x04,0x05,0x05,0x06,0x07,0x09,
                     0x0a,0x0b,0x0c,0x0e,0x0f,0x11,0x12,0x14, 0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
                     0x25,0x28,0x2a,0x2c,0x2f,0x31,0x34,0x36, 0x39,0x3b,0x3e,0x41,0x43,0x46,0x49,0x4c,
                     0x4f,0x52,0x55,0x58,0x5a,0x5d,0x61,0x64, 0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c };

void Timer_A_IRQHandler(void);
void Initialize_TimerA(void);


//**************************************************************************************
// The IRQHandler is what happens when the Timer interrupt occurs,
// In this lab, the PWM period is modified by loading TAOCCR1 with the “nxt” PWM
// output value.  Then it loads nxt with the next value out of the sine table.
// The table is 256 bytes so that it starts over when the 8 bit counter rolls over.
void Timer_A_IRQHandler(void)
{
   //static unsigned char cnt=0;
   static unsigned char nxt=0;

    TA0CCR1 = nxt;
    nxt = data[cnt++];
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
}

//****************************************************************************************
//* The Device Driver Lib provides a function to configure a timer as a PWM output.
//* This function receives the config parameters in a structure called pwmConfig
//* Timer_A PWM Configuration Parameters

Timer_A_PWMConfig pwmConfig = {
                                TIMER_A_CLOCKSOURCE_SMCLK,         /* Use SMCLK    */
                                TIMER_A_CLOCKSOURCE_DIVIDER_1,     //* Full speed   */
                                //TIMER_A_CLOCKSOURCE_DIVIDER_2,   //* Full speed   */
                                //TIMER_A_CLOCKSOURCE_DIVIDER_8,   /* Full speed   */
                                256,                               //* count 0->255 */
                                TIMER_A_CAPTURECOMPARE_REGISTER_1, //* Use TA0CCR1  */
                                TIMER_A_OUTPUTMODE_RESET_SET,      //* Reset on match */
                                128};                              //* Start at 128 */

//****************************************************************************************
//* Initializing the timer takes a minimum of 5 Device Driver calls

void Initialize_TimerA(void)
{
    Timer_A_clearTimer(TIMER_A0_BASE);

    /* Configuring GPIO2.4 as peripheral output for PWM */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    //* Configuring Timer_A to have a period of approximately 500ms and
    //*  an initial duty cycle of 10% of that (3200 ticks)
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    /* Link the TimerA0 CCRX interrupt to the correct function (Timer_A_IRQHandler)*/
    Timer_A_registerInterrupt ( TIMER_A0_BASE, TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, &Timer_A_IRQHandler );

    /* Enable the Timer interrupts */
    Timer_A_enableCaptureCompareInterrupt (TIMER_A0_BASE, TIMER_A_CCR0_INTERRUPT );
    Timer_A_enableInterrupt (TIMER_A0_BASE);
}


int main(void)
{
    WDT_A_hold(WDT_A_BASE);                         // Stop watchdog timer
    printf("Configuring Port 9: ");                 // Print to console
    GPIO_setAsOutputPin(GPIO_PORT_P9, 0xff);        // Set P9 to output
    printf("Starting SINE WAVE:\n");                // Print to console

    Initialize_TimerA();
    while(1);
}




