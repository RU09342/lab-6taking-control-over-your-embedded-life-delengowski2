#include "msp430G2553.h"
// Global variables
unsigned int ADC_temp, usertemp, temp , PWM =0;
// Function prototypes
void ConfigureAdc(void);
void TimerA1setup();
void UARTSetup();

int Error = 0;
int Kp = 10;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;       // Stop WDT
    //BCSCTL1 = CALBC1_1MHZ;          // Set range   DCOCTL = CALDCO_1MHZ;
    //BCSCTL2 &= ~(DIVS_3);           // SMCLK = DCO = 1MHz

    UARTSetup();                    // UART setup function call
    TimerA1setup();
    ConfigureAdc();                 // ADC set-up function call
    __enable_interrupt();           // Enable interrupts.


    while(1)
    {
        __delay_cycles(50000); // Wait for ADC Ref to settle

        // Retrieve ADC temperature input
        ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start
        __bis_SR_register(CPUOFF + GIE);    // Low Power Mode 0 with interrupts enabled

        TA0CCR1 = PWM;
	UCA0TXBUF = ADC10MEM; //lets user read current temp ( in hex)

    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    temp = UCA0RXBUF;                // Set user temp using input from UART
    UCA0TXBUF = UCA0RXBUF;           // TX -> RXed character

    // Select correct initial PWM
    if (temp > 55)
        PWM = (temp - 103.3) / (-1.59) * 10;
    else if (47 < temp < 55 )
        PWM = (temp - 64.2) / (-0.287) * 10;
    else
        PWM = (temp - 52.85) / (-0.0975) * 10;
    if (PWM >= 1000)
        PWM = 1000-1;

    if (PWM < 800)
    {
        TA0CCR1 = 999; // Kickstart the fan
        __delay_cycles(10000); // Wait for fan to start
    }

}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode }
}

// Function containing ADC set-up
void ConfigureAdc(void)
{
    ADC10CTL0 = ADC10SHT_3 + REF2_5V + ADC10ON + ADC10IE + REFON + SREF_1; // ADC10ON, interrupt enabled
    ADC10CTL1 = INCH_0 + ADC10DIV_3;                       // input A0
    ADC10AE0 |= 0x01;                         // PA.0 ADC option select
}
void TimerA1setup()
{
        //pwm pin setup
         P1DIR |= BIT6;            // P1.6 as output
         P1SEL |= BIT6;                  // P1.6 option select


        //pwm timer setup
        TA0CCTL1 = OUTMOD_7;                // CCR1 reset/set
        TA0CCR0 = 1000-1;                   //overflow every 1ms FOR PWM PERIOD
        TA0CCR1 = 500;                      //PWM starts at 50%
        TA0CTL = TASSEL_2 | MC_1;           //SMCLK, up mode
}

void UARTSetup()
{
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);                     // SMCLK = DCO = 1MHz
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
}
