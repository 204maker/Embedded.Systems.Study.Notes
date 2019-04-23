
/************************************************************************************
 * MSP432 ADC14 - Single Channel Repeat Temperature Sensor
 *
 * Description: This example shows the use of the internal temperature sensor.
 * A simple continuous ADC sample/conversion is set up with a software trigger.
 * The sample time is set to TBD as speced by the User's Guide. All calculations
 * take place in the ISR which take advantage of the Stacking Mode of the FPU.
 * The temperature is calculated in both Celsius and Fahrenheit.
 *  *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P5.5  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 * LAB 2B. Write a C program called lab2b.c, which will enable the internal temp sensor
 *         and output the temperature to the console.
 *
 * Calibration values can be found on p.316 MSP432_DriverLib_Users_Guide-MSP432P4xx-3_21_00_05.pdf
 *  cal30 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_30_DEGREES_C); //lo temp cal value
 *  cal85 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_85_DEGREES_C); //hi temp cal value
 * Author: Jai
************************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

volatile uint32_t cal30;
volatile uint32_t cal85;
volatile float calRange;
volatile float tempC;
volatile float tempF;

int main(void)
{
    /* Halting WDT  */
    WDT_A_holdTimer();
    Interrupt_enableSleepOnIsrExit();

    /* Enabling the FPU with stacking enabled (for use within ISR) */
    FPU_enableModule();
    FPU_enableLazyStacking();

    /* Initializing ADC (MCLK/1/1) with temperature sensor routed */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_TEMPSENSEMAP);

    /* Configuring ADC Memory (ADC_MEM0 A22 (Temperature Sensor) in repeat mode). */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A22, false);

    /* Configuring the sample/hold time for 192 */
    ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192,ADC_PULSE_WIDTH_192);

    /* Enabling sample timer in auto iteration mode and interrupts*/
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableInterrupt(ADC_INT0);

    /* Setting reference voltage to 2.5 and enabling temperature sensor */
    REF_A_enableTempSensor();
    REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    //REF_A_setReferenceVoltage(REF_A_VREF1_2V);
    //REF_A_setReferenceVoltage(REF_A_VREF1_45V);
    REF_A_enableReferenceVoltage();

    cal30 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_30_DEGREES_C); //lo temp cal value
    cal85 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_85_DEGREES_C); //hi temp cal value
    calRange = cal85 - cal30;                                                        //difference between hi & lo temp

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();

    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();

    /* Going to sleep */
    while (1)
    {
        PCM_gotoLPM0();
    }
}

/* This interrupt happens every time a conversion has completed. Since the FPU
 * is enabled in stacking mode, we are able to use the FPU safely to perform
 * efficient floating point arithmetic.*/
void ADC14_IRQHandler(void)
{
    uint64_t status;
    int16_t convResult;
    int16_t ADCrdg;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT0)
    {
        ADCrdg = ADC14_getResult(ADC_MEM0);
        convResult = (ADCrdg - cal30) * 55;                        //converted result via interpolation
        tempC = (convResult / calRange) + 30.0f;
        tempF = tempC * 9.0f / 5.0f + 32.0f;
        printf("%.2f [F] and %.2f [C] cal30 = 0x%04x, cal85 = 0x%04x, ADC reading: 0x%04x\n", tempF, tempC, cal30, cal85, ADCrdg);
    }

}
