
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
 *          --|RST         P5.3  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 * LAB 2B. Write a C program called lab2b.c, which will enable the internal temp sensor
 *         and output the temperature to the console.
 *
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
volatile float calDifference;
volatile float tempC;
volatile float tempF;
unsigned short value;
//unsigned short temperature;
volatile float temperature;
const signed short min_temp = -20;
const signed short max_temp = 100;

signed short thermistor_calculate_celsius(unsigned short adc_value);


//-20 to 100 deg C range = 121 values in the array
int NTC_table[65] = {
                     -630, -528, -426, -360, -310, -269, -234,
                     -203, -175, -150, -126, -103, -82, -62, -43,
                      -24, -6, 12, 29, 45, 62, 78, 94, 110, 125,
                      141, 156, 172, 187, 203, 219, 234, 250, 266,
                      283, 299, 316, 333, 350, 368, 387, 405, 425,
                      445, 466, 488, 511, 535, 560, 587, 615, 645,
                      678, 714, 753, 797, 847, 903, 969, 1050,
                     1151, 1288, 1495, 1898, 2301
};

// Converting the ADC values to Celsius degrees:
float NTC_ADC2Temperature(unsigned int adc_value){

  //int p1,p2;
  float p1,p2;

  p1 = NTC_table[ (adc_value >> 6)  ];
  p2 = NTC_table[ (adc_value >> 6)+1];

  return p1 + ( (p2-p1) * (adc_value & 0x003F) ) / 64;
};

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
    //ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_TEMPSENSEMAP);

    /* Configuring GPIOs (5.5 A0) */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);

    /* Configuring the sample/hold time for 192 */
    ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192,ADC_PULSE_WIDTH_192);

    /* Enabling sample timer in auto iteration mode and interrupts*/
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableInterrupt(ADC_INT0);

    /* Setting reference voltage to 2.5 and enabling temperature sensor */
    //REF_A_enableTempSensor();
    //REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    //REF_A_enableReferenceVoltage();

    cal30 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_30_DEGREES_C); //lo temp cal value
    cal85 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_85_DEGREES_C); //hi temp cal value
    calDifference = cal85 - cal30;  //difference between hi & lo temp

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
    int16_t conRes;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT0)
    {
        conRes = ((ADC14_getResult(ADC_MEM0) - cal30) * 55); //converted result via interpolation
        tempC = (conRes / calDifference) + 30.0f;
        tempF = tempC * 9.0f / 5.0f + 32.0f;

        // Converting the values of ADC into celsius scale
        value = ADC14_getResult(ADC_MEM0);
        temperature = NTC_ADC2Temperature(value);
        printf("%.2f [F] and %.2f [C] cal30 = 0x%04x, cal85 = 0x%04x, value = 0x%.4x, temp = %.2f \n", tempF, tempC, cal30, cal85, value, temperature);
    }

}



