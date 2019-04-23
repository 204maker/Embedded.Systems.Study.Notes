#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

char message[] = "Red is ON.\n";                  // Global variable
char timestamp[40];
char datestamp[40];
char str[50];
char RED[] = "RED"; char red[] = "red";
char GREEN[] = "GREEN"; char green[] = "green";
char BLUE[] = "BLUE"; char blue[] = "blue";

// http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html

//UART A0, 9600, 8N1
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                      // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

void UART_sendStr(char *msg);                   // Function Prototype for UART TX string
uint8_t UART_getStr(char *ptr_string);          // Function Prototype for UART RX string
void get_time(void);                            // Function prototype to get the TIME

////////////////////////////////////////////////////////////////////////////////////////
// Function for UART TX string
////////////////////////////////////////////////////////////////////////////////////////
void UART_sendStr(char *msg)
{
    //while(0 != *msg)
    while(*msg != '\n')
        UART_transmitData(EUSCI_A0_BASE,*msg++);
 }

///////////////////////////////////////////////////////////////////////////////
//  Reading a string through UART
///////////////////////////////////////////////////////////////////////////////
uint8_t UART_getStr(char *ptr_string)
{
    char ch;
    uint8_t len = 0;
    while(UCA0IFG & UCRXIFG)                //Check UART Rx flags if set
    {
        //ch=UART_getChar();                  //Receive a char
        //UART_TxChar(ch);                //Echo back the received char
        ch = UART_receiveData(EUSCI_A0_BASE);

        if((ch=='\r') || (ch=='\n'))        //read till enter key is pressed
        {                                   //once enter key is pressed null terminate the string
            ptr_string[len]=0;              //and break the loop
            break;
        }

        else if((ch=='\b') && (len!=0))
        {
            len--;                          //If backspace is pressed then decrement the index to remove the old char
        }

        else
        {
            ptr_string[len]=ch;             //copy the char into string and increment the index
            len++;
        }
    }

    return len;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function to get the TIME
////////////////////////////////////////////////////////////////////////////////////////
void get_time(void)
{
    int hours, minutes, seconds, day, month, year;

        // time_t is arithmetic time type
        time_t now;

        // Obtain current time
        // time() returns the current time of the system as a time_t value
        time(&now);

        // Convert to local time format and print to stdout
        //printf("Today is : %s", ctime(&now));

        // localtime converts a time_t value to calendar time and returns a pointer
        // to a tm structure with its members filled with the corresponding values
        struct tm *local = localtime(&now);

        hours = local   ->  tm_hour+1;          // get hours since midnight (0-23)
        minutes = local ->  tm_min;             // get minutes passed after the hour (0-59)
        seconds = local ->  tm_sec;             // get seconds passed after the minute (0-59)

        day = local     ->  tm_mday;            // get day of month (1 to 31)
        month = local   ->  tm_mon + 1;         // get month of year (0 to 11)
        year = local    ->  tm_year + 1900;     // get year since 1900

        // print local time
        if (hours < 12) // before midday
            sprintf(timestamp, "@UART: Date: %02d/%02d/%d, Time: %02d:%02d:%02d am\n", day, month, year, hours, minutes, seconds);
            //sprintf(timestamp, "Time: %02d:%02d:%02d am\n", hours, minutes, seconds);

        else    // after midday
            sprintf(timestamp, "@UART: Date: %02d/%02d/%d, Time: %02d:%02d:%02d am\n", day, month, year, hours - 12, minutes, seconds);
            //sprintf(timestamp, "Time: %02d:%02d:%02d pm\n", hours - 12, minutes, seconds);

        // print current date
        //sprintf(datestamp,"Date is : %02d/%02d/%d", day, month, year);
}


void main(void){


    WDT_A_holdTimer();

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    UART_enableModule(EUSCI_A0_BASE);
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    Interrupt_enableInterrupt(INT_EUSCIA0);
    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableMaster();

    PCM_setPowerState(PCM_LPM0_LDO_VCORE0);
}

void EUSCIA0_IRQHandler(void){
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        int length = UART_getStr(str);

        if(str == RED || str == red)
        {
            P2OUT = 0x01;
            get_time();                           //Get timestamp
            UART_sendStr(timestamp);              //send timestamp to UART
        }

        else if(str == GREEN || str == green)
        {
            P2OUT = 0x02;
            get_time();                           //Get timestamp
            UART_sendStr(timestamp);              //send timestamp to UART
        }

        else if(str == BLUE || str == blue)
        {
            P2OUT = 0x04;
            get_time();                           //Get timestamp
            UART_sendStr(timestamp);              //send timestamp to UART
        }

        else if(UART_receiveData(EUSCI_A0_BASE) == 'R'|| UART_receiveData(EUSCI_A0_BASE) == 'r')
        {
            //GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
            //UART_sendStr(message);              //message is a global variable
            P2OUT = 0x01;
            get_time();                           //Get timestamp
            UART_sendStr(timestamp);              //send timestamp to UART
        }

        else if(UART_receiveData(EUSCI_A0_BASE) == 'G' || UART_receiveData(EUSCI_A0_BASE) == 'g')
        {
            //GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
            //UART_transmitData(EUSCI_A0_BASE,'G');
            //UART_sendStr("Green is ON.\n");
            P2OUT = 0x02;
            get_time();                           //Get timestamp
            UART_sendStr(timestamp);              //send timestamp to UART
        }

        else if(UART_receiveData(EUSCI_A0_BASE) == 'B' || UART_receiveData(EUSCI_A0_BASE) == 'b')
        {
            //GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
            //UART_transmitData(EUSCI_A0_BASE,'B');
            //UART_sendStr("Blue is ON.\n");
            P2OUT = 0x04;
            get_time();                           //Get timestamp
            UART_sendStr(timestamp);              //send timestamp to UART
        }

        else if(UART_receiveData(EUSCI_A0_BASE) == 'X' || UART_receiveData(EUSCI_A0_BASE) == 'x')
        {
            //GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            //UART_transmitData(EUSCI_A0_BASE,'X');
            //UART_sendStr("RGB LED is now OFF!\n");
            P2OUT = 0x00;
            get_time();                           //Get timestamp
            UART_sendStr(timestamp);              //send timestamp to UART
        }

    }
}























   //https://www.w3resource.com/c-programming-exercises/datetime/c-datetime-exercise-1.php
   //Built-in function to get time in C
   /*
   char *cur_t_string;
   time_t cur_time;
   cur_time = time(NULL);
   cur_t_string = ctime(&cur_time);        //convert to local time format
   */
