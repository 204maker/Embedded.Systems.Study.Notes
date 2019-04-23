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
 *          --|RST         P5.5  |<--- A0 (Analog Input - Pot/Thermistor
 *            |                  |         with external pulldown resistor)
 *            |                  |
 *            |            P2.4  |--> Output PWM
 *            |                  |
 *            |                  |
 *            |            AVCC  |<--- VREF_POS
 *            |            VCC   |<--- VREF_NEG
 *            |                  |
 *            |                  |
 *
 * ADC LAB #.
 *  1. Interface the MSP432 to a FET driver with the following circuit so that it
 *     can drive a 50ohm resistive load with a 12-15V output that is pulse width
 *     modulated at approximately 100Hz.
 *
 *  2.  Write a C program that will regulate the temperature of the load using a
 *      closed loop control system.  The program will measure the temperature of
 *      the load using th A/D channels (Lab 1) and Turn the load on and off using
 *      the PWM output (LAB 2) The goal is to and maintain the temperature of the
 *      load at 60 degrees C.
 *
 *  Notes:
 *      The PWM frequency must be reduced to about 100Hz.
 *      The gate of the FET is capacitive and you can not drive it at 48MHz.
 *      The table conversion program from lab 2 can be used to convert the temp sensor voltage into a temperature.

 *  3.  Parts:
 *      a.  P Channel FET IRF5305PBF IRLIB9343  Digikey: IRF5305PBF- ND
 *      b.  NTC Thermistor NXRT15XH103FA1B040   Digikey: 490-7167-ND
 *      c.  NPN Transistor 2N3904BU Digikey: 2N3904FS-ND
 *
 *
 *      Goal: to generate a PWM frequency = 100Hz
 *                                  SMCLK = 3 MHz
 *
 *      PWM_PERIOD           = 1/100 Hz
 *                           = 10ms
 *                           = 1/(100/3MHz), or
 *                         N = 3MHz/100 = 30000
 *
 * Author: Jai
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define PWM_PERIOD_100HZ    30000
#define LOAD_TIMER          TIMER_A0_BASE      // The load is wired to TA0.1. for PWM control
#define LOAD_CHANNEL        TIMER_A_CAPTURECOMPARE_REGISTER_1
#define LOAD_DUTY_CYCLE     100                // Starting duty cycle
//#define LOAD_DUTY_CYCLE     15000

Timer_A_PWMConfig pwmConfig_load = {
                                        TIMER_A_CLOCKSOURCE_SMCLK,
                                        TIMER_A_CLOCKSOURCE_DIVIDER_16,
                                        PWM_PERIOD_100HZ,
                                        LOAD_CHANNEL,
                                        TIMER_A_OUTPUTMODE_RESET_SET,
                                        LOAD_DUTY_CYCLE
                                   };

void initPWM()
{
    // For the load to get PWM control, we first configure them as a GPIO and turn them off
    // Then, we configure them to work as output channel of a timer

    GPIO_setAsOutputPin    (GPIO_PORT_P2,    GPIO_PIN4);
    GPIO_setOutputLowOnPin (GPIO_PORT_P2,    GPIO_PIN4);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
                                                    GPIO_PORT_P2,
                                                    GPIO_PIN4,
                                                    GPIO_PRIMARY_MODULE_FUNCTION
                                               );
}


/* Global variables */
static volatile uint16_t curADCResult;
volatile float tempC;
volatile float tempF;
float voltage, resistance;

int cnt;

float NTCtable[] = {
                      29139.31845, 0,
                      18571.4251, 10,
                      12205.57116, 20,
                      10000, 25,
                      8247.022563, 30,
                      5713.606666, 40,
                      4049.375675, 50,
                      2929.827648, 60,
                      2160.168336, 70,
                      1620.424388, 80,
                      1234.94148, 90
                   };

float linear_interpolation (float resistance_rdg)
{
  int counter, lowindex, highindex;
  float x2, x3, y2, y3, x, y;
  float temp;

  for (counter = 0; counter <= 20; counter += 2)    //counter limit is highest index from 0
    {
      if (NTCtable[counter] < resistance_rdg)
    {
      lowindex = counter - 2;                       //skips odd index
      highindex = counter;
      counter = 20;                                 //counter limit is highest index from 0
    }
    }

  x = resistance;                                   //resistance is calculated from ADC reading

  x2 = NTCtable[lowindex];                          //x2, y2 are lower coordinates
  y2 = NTCtable[lowindex + 1];

  x3 = NTCtable[highindex];                         //x3, y3 are higher coordinates
  y3 = NTCtable[highindex + 1];

  y = y2 + ((x - x2) * (y3 - y2) / (x3 - x2));      //result of interpolation

  temp = y;
  return (float) temp;
}

int main(void)
{
    uint16_t pwm_dutycycle;
    /* Halting the Watchdog  */
    WDT_A_holdTimer();
    initPWM();

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
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A2, false);

    /* Configuring Sample Timer */
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Enabling/Toggling Conversion */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
    //![Single Sample Mode Configure]

    /* Enabling interrupts */
    ADC14_enableInterrupt(ADC_INT0);
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();

    /*while(tempC < 60)
            {
                pwmConfig_load.dutyCycle +=3200;
                pwm_dutycycle = pwmConfig_load.dutyCycle/PWM_PERIOD_100HZ;
                Timer_A_generatePWM(LOAD_TIMER, &pwmConfig_load);
                printf("ADC rdg: 0x%.4x, V-rdg: %.2f volts, Temp: %.2f [C], DC = %d%, Res = %.2f\n", ADC14_getResult(ADC_MEM0), voltage, tempC, pwm_dutycycle, resistance);
            }
    */
    while (1)
    {
        if(tempC < 50)
        {
            pwmConfig_load.dutyCycle +=3200;
            pwm_dutycycle = pwmConfig_load.dutyCycle/PWM_PERIOD_100HZ;
            Timer_A_generatePWM(LOAD_TIMER, &pwmConfig_load);
            printf("ADC rdg: 0x%.4x, V-rdg: %.2f volts, Temp: %.2f [C], DC = %d%, Res = %.2f\n", ADC14_getResult(ADC_MEM0), voltage, tempC, pwm_dutycycle, resistance);
        }

        else
        {
            pwmConfig_load.dutyCycle =10;
            pwm_dutycycle = pwmConfig_load.dutyCycle/PWM_PERIOD_100HZ;
            Timer_A_generatePWM(LOAD_TIMER, &pwmConfig_load);
            printf("ADC rdg: 0x%.4x, V-rdg: %.2f volts, Temp: %.2f [C], DC = %d%, Res = %.2f\n", ADC14_getResult(ADC_MEM0), voltage, tempC, pwm_dutycycle, resistance);
        }

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
        curADCResult = ADC14_getResult(ADC_MEM0);
        voltage = (float)(curADCResult*3.3/16383);
        resistance = (3.3*10000/voltage)-10000;

        tempC = linear_interpolation(resistance);
        tempF = tempC * 9.0f / 5.0f + 32.0f;
        //printf("ADC rdg: 0x%.4x, V-rdg: %.2f volts, Temp: %.2f [F] or %.2f [C], Res = %.2f\n", ADC14_getResult(ADC_MEM0), voltage, tempF, tempC, resistance);

        ADC14_toggleConversionTrigger();
    }
}


