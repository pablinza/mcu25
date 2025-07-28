/*
 * File:   main.c
 * Author: pablo
 *
 * Created on July 16, 2025, 12:30 AM
 */
#pragma config FOSC=INTRC_NOCLKOUT, WDTE = OFF
#include <xc.h>
char portval;
void main(void) 
{
    ANSEL = 0x00; //Deshabilita canales ANS7:0
    ANSELH = 0x00;//Deshabilita canales ANS13:8
    TRISB = 0xFC; //RB1:0 salida y RB7:4 entrada 
    OPTION_REG &= 0x7F; //Acitva pull-up bit7 RBPU=0 
    TRISC = 0xFF; //RC0 y RC1 como entrada
    TRISD = 0xF0; //RD3:0 es salida
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
        portval = PORTB >> 4; //Desplaza 4 bits a la derecha
        PORTD = ~portval; //Invierte y carga el valor
    }
}