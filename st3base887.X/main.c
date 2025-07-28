/*
 * File:   main.c
 * Author: pablo
 *
 * Created on July 23, 2025, 7:47 PM
 */
#pragma config FOSC = INTRC_NOCLKOUT, WDTE = OFF
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdio.h>
void setup(void); //prototipo de procedimiento
void task1(void); 
void task2(void); 
void task3(void); 
void task4(void); 
uint8_t reg1 = 0x01; //variable global
volatile __bit tickms;
void __interrupt() isr(void)
{
    if(INTCONbits.T0IF)
    {
        INTCONbits.T0IF = 0;
        TMR0 += 131;
        tickms = 1;
    }
}
void main(void) 
{
    setup(); //Configuracion
    while(1)
    {
        if(tickms)
        {
            tickms = 0;
            task1(); //Destellar el led 
            task2(); //Lectura del pulsador RB4 y desplazamiento izquierda
            task3(); //Lectura del pulsador RB5 y desplazamiento derecha 
            task4();//Lectura del canal ANS0, y envio de datos
        }
    }
}
void setup(void) //Procedimiento de configuracion
{
    ANSEL = 0b00000000; //Canales ANS0-7 desactivados
    ANSELH = 0; //Canales ANS8-13 desactivados
    TRISEbits.TRISE2 = 0; //Pin E2 es salida
    TRISBbits.TRISB4 = 1; //Pin B4 es entrada
    TRISBbits.TRISB5 = 1; //Pin B5 es entrada
    TRISD = 0x00; //Port D es salida, todos los pines RD0 al RD7
    OPTION_REGbits.nRBPU = 0; //Activa las resistencias pullup del PORTB
    /*Configura USART a 9600bps con 4MHz */
    SPBRG = 25; //[4.000.000/(16*9600)]-1)
    TXSTAbits.BRGH = 1; //Activa el generador Alta velocidad
    TXSTAbits.TXEN = 1; //Habilita el pin de Transmision
    RCSTAbits.SPEN = 1; //Habilita el puerto USART
    /* CONFIGURACION ADC A 4MHz */
    ANSELbits.ANS0 = 1; //Habilita canal ANS0 RA0
    ADCON0bits.ADCS = 0b01; //Tad=4u (Fosc/8) (< 1.6u)
    ADCON0bits.ADON = 1; //Activa el ADC
     /* CONFIGURACION TIMER0 1MS a Fosc=4MHz*/
    OPTION_REGbits.T0CS = 0;//Modo Termporizador
    OPTION_REGbits.PSA = 0; //Con prescala
    OPTION_REGbits.PS = 0b010; //Prescala 1:8
    TMR0 = 131; //256-(time/((pre)*(4/Fosc))) time=0.001 seg
    INTCONbits.T0IF = 0; //Limpia bandera
    INTCONbits.T0IE = 1; //Activa interrupcion del TMR0
    INTCONbits.GIE = 1;
}
void task1(void) //Destellar led al 20%
{
    static uint16_t cnt = 0; //Contador de 16-bit
    cnt++; //Incrementa el contador
    if(cnt == 200) PORTEbits.RE2 = 0; //Si es 200 (200ms)
    if(cnt == 1000)  //Si es 1000 (1seg)
    {
        PORTEbits.RE2 = 1; //Apaga led
        cnt = 0;
    }
}
void task2(void) //Lectura del pulsador RB4 y desplazamiento izquierda
{
    static uint8_t cnt = 0; //Contador de 8-bit
    if(PORTBbits.RB4 == 0) //Pulsador B4 presionado
    {
        cnt++; //Incrementa contador
        if(cnt > 99) //100ms 
        {
            cnt = 0;
            reg1 = (uint8_t) (reg1 << 1); //rotacion izquierda
            if(reg1 == 0) reg1 = 0x01; //restablecer bit LSB
            PORTD = ~reg1; //Complemento 
        }
    }
}
void task3(void) //Lectura del pulsador RB5 y desplazamiento derecha
{
    static uint8_t cnt = 0; //Contador de 8-bit
    if(PORTBbits.RB5 == 0)
    { 
        cnt++;  //Incrementa contador
        if(cnt > 99) //100ms 
        {
            cnt = 0;
            reg1 = reg1 >> 1; //rotacion derecha
            if(reg1 == 0) reg1 = 0x80; //restablecer bit MSB
            PORTD = ~reg1; //Complemento
        }
    }
}
void task4(void) //Lectura del canal ANS0, y envio de datos 
{
    static uint16_t cnt = 0;
    static uint8_t state = 0;
    uint8_t res;
    switch(state)
    {
        case 0: //Inicia la captura del canal
            ADCON0bits.GO = 1;
            state++;
        break;
        case 1: //Lectura del canal y envio de dato 
            res = ADRESH; //Carga los 8-bit de mayor peso
            printf("ADC:%u\n",res); //Envia dato
            state++;
        break;
        case 2: //Espera un tiempo para nuevo ciclo
            cnt++;
            if(cnt > 999) 
            {
                cnt = 0;
                state = 0;
            }
    }
}
void putch(char byte)
{
    while(PIR1bits.TXIF == 0);
    TXREG = byte;
}