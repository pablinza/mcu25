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
    char portval;
    DIDR0 = 0x3F; //Deshabilita canales ADC5:0
    DDRB = 0x03; //PB1:0 salida y PB7:4 entrada
    PORTB = 0xF0;//Activa pullups PB7:4
    DDRC = 0x00; //PC0 y PC1 como entrada
    PORTC = 0x03;//Activa pullups PC1:0
    DDRD = 0x0F; //PD3:0 como salida
    while(1)
    {
        if(PINC & 0x01) //Si PC0==1 (pulsador libre)
            PORTB &= 0xFE;//PB0=0 (apaga led)
        else
            PORTB |= 0x01; //PB0=1 (enciende led)
        if(PINC & 0x02) //Si PC1==1 (pulsador libre)
            PORTB &= 0xFD;//PB1=0 (apaga led)
        else
            PORTB |= 0x02; //PB1=1 (enciende led)
        portval = PINB >> 4; //Desplaza 4 bits a la derecha
        PORTD = ~portval; //Invierte y carga el valor
    }
   return 0;
}
