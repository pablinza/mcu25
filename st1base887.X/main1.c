/*
 * File:   main.c
 * Author: pablo
 *
 * Created on July 16, 2025, 12:30 AM
 */
#pragma config FOSC=INTRC_NOCLKOUT, WDTE = OFF
#include <xc.h>
void main(void) 
{
    ANSEL = 0x00; //Deshabilita canales ANS7:0
    ANSELH = 0x00;//Deshabilita canales ANS13:8
    TRISB = 0b11111100; //RB0 y RB1 como salida
    TRISC = 0b11111111; //RC0 y RC1 como entrada
    while(1)
    {
        if(PORTC & 0x01) //Si RC0==1 (pulsador libre)
            PORTB &= 0xFE; //RB0=0 (apaga led)
        else 
            PORTB |= 0x01;//RB0=1 (enciende led)
       
        if(PORTC & 0x02) //Si RC0==1 (pulsador libre)
            PORTB &= 0xFD;//RB1=0 (apaga led)
        else 
            PORTB |= 0x02;//RB1=1 (enciende led)
    }
}