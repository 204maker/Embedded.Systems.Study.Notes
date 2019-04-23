/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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
 */

/*
 *    ======== udpEchoCC3X00.c ========
 */

//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "C:\ti\tirtos_msp43x_2_20_00_06\products\msp432_driverlib_3_21_00_05\driverlib\MSP432P4xx\driverlib.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>


/* SimpleLink Wi-Fi Host Driver Header files */
#include <simplelink.h>

/* Example/Board Header file */
#include "Board.h"

/* Local Platform Specific Header file */
#include "sockets.h"

/* Port number for listening for UDP packets */
#define UDPPORT         1000
#define UDPPACKETSIZE   256
#define TASKSTACKSIZE   1024

extern bool smartConfigFlag;
Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];


/* delay milliseconds when system clock is at 3 MHz */
void delayMs(int n) {
    int i, j;
    for (j = 0; j < n; j++)
        for (i = 750; i > 0; i--); /* Delay 1 ms*/
}


/*
 *  ======== gpioButtonFxn ========
 *  Callback function for the GPIO interrupt on Board_BUTTON1.
 */
void gpioButtonFxn(unsigned int index)
{
    /* Begin smart config process */
    smartConfigFlag = true;
}

/*
 *  ======== echoFxn ========
 *  Echoes UDP messages.
 */
void echoFxn(int port)
{
    int         bytesRcvd;
    int         bytesSent;
    int         status;
    int         server;
    fd_set      readSet;
    sockaddr_in localAddr;
    sockaddr_in clientAddr;
    socklen_t   addrlen;
    char        buffer[UDPPACKETSIZE];

    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server == -1) {
        System_printf("Error: socket not created.\n");
        goto shutdown;
    }

    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(port);

    status = bind(server, (const sockaddr *)&localAddr, sizeof(localAddr));
    if (status == -1) {
        System_printf("Error: bind failed.\n");
        goto shutdown;
    }

    do {
        /*
         *  readSet and addrlen are value-result arguments, which must be reset
         *  in between each select() and recvfrom() call
         */
        FD_ZERO(&readSet);
        FD_SET(server, &readSet);
        addrlen = sizeof(clientAddr);

        /* Wait forever for the reply */
        status = select(server + 1, &readSet, NULL, NULL, NULL);
        if (status > 0) {
            //Check for message received byte 0 and byte 1 of buffer
            if (buffer[0] == '0' && buffer[1] == '0')
            {
                //GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_OFF);
                GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_OFF);
                GPIO_write(MSP_EXP432P401R_LED1 , Board_LED_OFF);
            }
            else
            {
                //GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_OFF);
                GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_ON);
                GPIO_write(MSP_EXP432P401R_LED1 , Board_LED_ON);
                delayMs(3000);
                GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_OFF);
                GPIO_write(MSP_EXP432P401R_LED1 , Board_LED_OFF);
            }

            if (FD_ISSET(server, &readSet)) {
                bytesRcvd = recvfrom(server, buffer, UDPPACKETSIZE, 0,
                        (sockaddr *)&clientAddr, &addrlen);

                if (bytesRcvd > 0) {
                    //else if((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)
                    //&& (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == GPIO_INPUT_PIN_LOW))
                    if((P1IN&0x12)== 0x00)
                    {
                        GPIO_write(MSP_EXP432P401R_LED1 , Board_LED_ON);                            //LED1 is ON
                        GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_ON);                          //LED-RGB is ON
                        buffer[0] = '1';
                        buffer[1] = '1';
                        buffer[2] = '1';
                        buffer[3] = '1';
                    }

                    else if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)  //S1 is pressed
                    {
                        GPIO_write(MSP_EXP432P401R_LED1 , Board_LED_ON);                            //LED1 is ON
                        GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_OFF);
                        buffer[0] = '1';
                        buffer[1] = '0';
                        buffer[2] = '1';
                        buffer[3] = '0';
                    }

                        //GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    else if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == GPIO_INPUT_PIN_LOW)   //S2 is pressed
                    {
                        GPIO_write(MSP_EXP432P401R_LED1 , Board_LED_OFF);
                        GPIO_write(MSP_EXP432P401R_LED_RED, Board_LED_ON);                          //LED-RGB is ON
                        buffer[0] = '0';
                        buffer[1] = '1';
                        buffer[2] = '0';
                        buffer[3] = '1';
                    }
                        //GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                    else
                    {
                        //GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);                            //LED is OFF
                        //GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);                            //LED-RGB is OFF
                        buffer[0] = '0';
                        buffer[1] = '0';
                        buffer[2] = '0';
                        buffer[3] = '0';
                    }

                    bytesSent = sendto(server, buffer, bytesRcvd, 0,
                            (sockaddr *)&clientAddr, addrlen);

                    //sendto(server, "10", bytesRcvd, 0,
                    //sendto(server, c, bytesRcvd, 0,
                    //       (sockaddr *)&clientAddr, addrlen);
                    sendto(server, buffer, bytesRcvd, 0,
                           (sockaddr *)&clientAddr, addrlen);


                    if (bytesSent < 0 || bytesSent != bytesRcvd) {
                        System_printf("Error: sendto failed.\n");
                        goto shutdown;
                    }
                }
            }
        }
    } while (status > 0);

shutdown:
    if (server >= 0) {
        close(server);
    }
}

/*
 *  ======== udpEchoTask ========
 */
Void udpEchoTask(UArg arg0, UArg arg1)
{
    void *netIF;

    /* Open WiFi and await a connection */
    netIF = socketsStartUp();

    echoFxn(UDPPORT);

    /* Close the network - don't do this if other tasks are using it */
    socketsShutDown(netIF);
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Construct BIOS objects */
    Task_Params taskParams;


    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initWiFi();

    //Driverlib GPIO init
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                                   //LED1
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);                                   //LED2 - Red (RGB)
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN1);                 //S1
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN4);                 //S2

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    taskParams.priority = 1;
    Task_construct(&task0Struct, (Task_FuncPtr)udpEchoTask, &taskParams, NULL);

    /* Install Button callback */
    GPIO_setCallback(Board_BUTTON1, gpioButtonFxn);

    /* Enable interrupts */
    GPIO_enableInt(Board_BUTTON1);

    /* Turn on user LED */
    //GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the UDP Echo example for the CC3X00 \n"
                  "System provider is set to SysMin. Halt the target to view"
                  " any SysMin contents in ROV.\n");

    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}

//UDP Packet - reply message
//Byte 1 = LED1
//Byte 2 = LED_RGB
//Byte 3 = SW1
//Byte 4 = SW2







//Task_sleep(100);











/*
char c = GPIO_read(Board_BUTTON0);
char a = GPIO_read(MSP_EXP432P401R_LED_RED);
char b = GPIO_read(MSP_EXP432P401R_LED1);
char c = GPIO_read(Board_BUTTON0);
char d = GPIO_read(Board_BUTTON1);
snprintf(buffer, UDPPACKETSIZE, a&b&c&d);

printf("LED_RED= %d, LED1=%d, Btn0= %d, Btn1= %d",a,b,c,d);
*/

/*check for S1 and S2
            if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)       //S1 is pressed
                GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);                           //LED is ON

            else if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == GPIO_INPUT_PIN_LOW)   //S1 is pressed
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);                           //LED is ON

            else
            {
                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);                            //LED is OFF
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);                            //LED is OFF
            }*/

