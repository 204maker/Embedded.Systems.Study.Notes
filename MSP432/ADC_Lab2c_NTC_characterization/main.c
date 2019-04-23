/*******************************************************************************
 * MSP432 ADC14 - Single Channel Sample Repeat
 *
 * Description: This code example will demonstrate the basic functionality of
 * of the DriverLib ADC APIs with sampling a single channel repeatedly. Each
 * time the ADC conversion occurs, the result is stored into a local variable.
 * The sample timer is used to continuously grab a new value from the ADC
 * module using a manual iteration that is performed in the ADC ISR. A
 * normalized ADC value with respect to the 3.3v Avcc is also calculated using
 * the FPU.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P5.5  |<--- A0 (Analog Input - Pot/Thermistor)
 *            |                  |
 *            |            AVCC  |<--- VREF_POS
 *            |            VCC   |<--- VREF_NEG
 *            |                  |
 *            |                  |
 *
 * ADC LAB 2A.
 *  1. Write a C program called lab2a.c which will set up the ADC14 to measure
 *     the voltage on an analog input.
 *
 *     Use an external pot to input a voltage from 0 to 3.3 V
 *     Compare the input to an internal reference of 3.3V
 *     Output the raw digital value in Hex form to the console
 *     Output the value as a voltage to the console
 *
 * Author: Jai
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Statics */
static volatile uint16_t curADCResult;
static volatile float normalizedADCRes;

volatile uint32_t cal0;
volatile uint32_t cal100;
volatile float calDifference;
uint64_t status;
int16_t conRes;
volatile float tempC;
volatile float tempF;

table[] = {
           29139.31845,     0
           18571.4251,     10
           12205.57116,    20
           10000,10000,    25
           8247.022563,    30
           5713.606666,    40
           4049.375675,    50
           2929.827648,    60
           2160.168336,    70
           1620.424388,    80
           1234.94148,     90
};
int main(void)
{
    /* Halting the Watchdog  */
    WDT_A_holdTimer();

    /* Initializing Variables */
    curADCResult = 0;

    /* Setting Flash wait state */
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);

    /* Setting DCO to 48MHz  */
    PCM_setPowerState(PCM_AM_LDO_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

    /* Enabling the FPU for floating point operation */
    FPU_enableModule();
    FPU_enableLazyStacking();

    //![Single Sample Mode Configure]
    /* Initializing ADC (MCLK/1/4) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, 0);

    /* Configuring GPIOs (5.5 A0) */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,
    GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);

    /* Configuring Sample Timer */
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Enabling/Toggling Conversion */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
    //![Single Sample Mode Configure]


    cal0 = 0x555;                   //lo temp cal value
    cal100 = 0x3aa9;                //hi temp cal value
    calDifference = cal100 - cal0;  //difference between hi & lo temp

    /* Enabling interrupts */
    ADC14_enableInterrupt(ADC_INT0);
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();

    while (1)
    {
        PCM_gotoLPM0();
    }

}

 //![Single Sample Result]
/* ADC Interrupt Handler. This handler is called whenever there is a conversion
 * that is finished for ADC_MEM0.
 */
void ADC14_IRQHandler(void)
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if (ADC_INT0 & status)
    {
        curADCResult = ((ADC14_getResult(ADC_MEM0) - cal0) * 100);    //converted result via interpolation
        tempC = (curADCResult / calDifference) + 20.0f;
        tempF = tempC * 9.0f / 5.0f + 32.0f;
        printf("ADC reading: 0x%.4x, Temp: %.2f [F] or %.2f [C]\n", ADC14_getResult(ADC_MEM0), tempF, tempC);
        ADC14_toggleConversionTrigger();
    }
}
//![Single Sample Result]

