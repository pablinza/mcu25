/*
 * File:   main.c
 * Author: pablo
 *
 * Created on July 16, 2025, 1:29 PM
 */
//#pragma config SUT_CKSEL = SUT_CKSEL_INTRCOSC_8MHZ_6CK_14CK_65MS, CKDIV8 = CLEAR
#pragma config CKDIV8 = CLEAR, RSTDISBL = SET
#include <xc.h>
int main(void) 
{
    DIDR0 = 0b00111111; //Deshabilita canales ADC5:0
    DDRB = 0x03; //RB0 y RB1 como salida
    DDRC = 0x00; //RC0 y RC1 como entrada
    while(1)
    {
        if(PINC & 0x01) //Si RC0==1 (pulsador libre)
            PORTB &= 0xFE;//RB0=0 (apaga led)
        else
            PORTB |= 0x01; //RB0=1 (enciende led)
        if(PINC & 0x02) //Si RC1==1 (pulsador libre)
            PORTB &= 0xFD;//RB1=0 (apaga led)
        else
            PORTB |= 0x02; //RB1=1 (enciende led)
    }
   return 0;
}
