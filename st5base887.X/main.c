/*
 * File:   main.c
 * Author: pablo
 *
 * Created on July 27, 2025, 7:22 AM
 */
#pragma config FOSC=INTRC_NOCLKOUT, WDTE=OFF, LVP=OFF
#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdio.h>
#include "lcd.h"
volatile __bit tickms, setmode;
uint8_t adcres; //Guarda resultado ADC
uint8_t adcset = 50; //Guarda ajuste ADC
void setup(void); //Configuracion del PIC
void task1(void); //Destello led RE2 al 10% 1Hz
void task2(void); //Lectura ADC canal ANS0
void task3(void); //Envio dato ADC y verifica alerta
void task4(void); //Lectura del encoder y ajuste
void __interrupt() isr()
{
    if(INTCONbits.T0IF) //Activa cada 1ms 8MHz
     {
        INTCONbits.T0IF = 0;  //Limpia bandera
        TMR0 += 131; //Reinicia contador 
        tickms = 1; //Activa bandera 1ms
    }
}
void main(void) 
{
    setup();
    LCDSetup();
    while(1)
    {
        if(tickms)
        {
            tickms = 0;
            task1();
            task2();
            if(setmode == 0) task3();
            task4();
        }
    }
}
void setup(void) //Configuracion del PIC
{
    OSCCONbits.IRCF = 0b111; //Ajusta INTRC a 8MHz, Tcy=0.5u
    ANSEL = 0;
    ANSELH = 0;
    nRBPU = 0; //Activa las pull-ups del PORTB
    TRISBbits.TRISB3 = 0; //Salida RB3 Alarma buzzer
    PORTBbits.RB3 = 0;
    TRISEbits.TRISE2 = 0; //Salida RE2 Led de estado
    
    /* CONFIGURACION UART A 9600BPS 8MHz*/
    TXSTAbits.BRGH = 1; //Alta del Generador
    TXSTAbits.TXEN = 1; //Activa el transmisor
    RCSTAbits.SPEN = 1; //Habilita el modulo USART
    SPBRG = 51; //Formula [8MHz/(16 * 9600)] - 1
    /* CONFIGURACION ADC 8MHz CANAL 0*/
    ANSELbits.ANS0 = 1; //Activa canal AN0
    ADCON0bits.ADCS = 0b10; //TAD=4us > 1.6us (8MHz/32)
    ADCON0bits.CHS = 0; //Selecciona Canal 0
    ADCON0bits.ADON = 1; //Activa el modulo ADC
    /* CONFIGURACION TIMER0 0.001s A 8MHz */
    OPTION_REGbits.T0CS = 0;//Modo Temporizador
    OPTION_REGbits.PSA = 0; //Con prescala
    OPTION_REGbits.PS = 0b011; //Prescala 1:16
    TMR0 = 131; //256-[(time*Fosc)/(pre*4)] time=0.001 seg
    INTCONbits.T0IF = 0; //Limpia bandera
    INTCONbits.T0IE = 1; //Activa interrupcion del TMR0
    
    INTCONbits.GIE = 1; //Activa la interrupcion en el CPU
}
void task1(void) //Destellar led al 10%
{
    static uint16_t cnt = 0; //Contador de 16-bit
    cnt++; //Incrementa el contador
    if(cnt == 100) PORTEbits.RE2 = 0; //Si es 200 (200ms)
    if(cnt == 1000)  //Si es 1000 (1seg)
    {
        PORTEbits.RE2 = 1; //Apaga led
        cnt = 0;
    }
}
void task2(void) //Lectura de canal ADC
{
    static uint16_t cnt = 0;
    static uint8_t state = 0;
    uint16_t res;
    switch(state)
    {
        case 0: //Inicia Captura ADC
            ADCON0bits.GO = 1;
            state++; //Siguiente estado
            break;
        case 1: //Lectura de conversion ADC
            res = ADRESL; //Carga 2-bits LSB
            res |= (uint16_t) (ADRESH << 8); //Carga 8-bit MSB
            res >>= 6; //Alinea el resultado ADH:ADL  
            state++;
            break;
        case 2:
            cnt++;
            if(cnt > 499) //Espera medio segundo entre lecturas
            {
                cnt = 0;
                adcres = (uint8_t) (res / 10); //Ajusta a escala 0-100
                state = 0;
            }
            break;
    }
}
void task3(void) //Envio de datos puerto serial y pantalla 
{
    static uint8_t adcres0; //Guarda valor previo adcred
    static uint8_t adcset0; //Guarda valor previo adcset
    if(adcres0 != adcres) //Si adcres ha cambiado
    {
        adcres0 = adcres; //Actualiza adcres0
        printf("ADC:%u\n", adcres0); //Envia valor por puerto 
        LCDGotoXY(0,0); //Posiciona cursor LCD
        LCDWriteMsgROM("ADC:"); //Escribe en LCD
        LCDWriteNum(adcres0,3); //Escribe el valor adcres0
        LCDGotoXY(0,1); //Posiciona cursor LCD segunda linea
        if(adcres0 > adcset0)   //Si adcres es mayor al ajuste
        {
            
            LCDWriteMsgROM("** ALERTA **"); //Mensaje 
            PORTBbits.RB3 = 1; //Activa el buzzer pin RB3
        }
        else 
        {
            LCDWriteMsgROM("            "); //Limpia mensaje
            PORTBbits.RB3 = 0; //Apaga buzzer pin RB3
        }
    }
    if(adcset0 != adcset) //Si adcset ha cambiado
    {
        adcset0 = adcset; //Actualiza adcset0
        LCDGotoXY(8,0); //Posiciona cursor LCD
        LCDWriteMsgROM("SET:"); //Escribe mensaje
        LCDWriteNum(adcset0,3); //Escribe el valor adcset0
    }
}
void task4(void) //Lectura del encoder
{
    static uint8_t state = 0;
    static uint16_t cnt = 0;
    static uint8_t ckpin, val;
    switch(state)
    {
        case 0: //Detecta pulsador presionado RB2 SW
            if(PORTBbits.RB2 == 0) //Si esta presionado
            {
                cnt++;
                if(cnt > 999) //presionado por 1000ms
                {
                    cnt = 0;
                    state++; //Siguiente estado
                }
            }
            else cnt = 0;
            break;
        case 1: //Espera liberacion del pulsador RB2
            if(PORTBbits.RB2 == 1)//Si pulsador esta liberado
            {
                cnt++;
                if(cnt > 99) //espera por 99ms
                {
                    cnt = 0;
                    ckpin = PORTBbits.RB0; //Lectura valor CK inicial
                    val = adcset; //Copia valor de ajuste adcset
                    LCDGotoXY(12,0);//Posiciona el cursor LCD
                    LCDWriteCMD(LCD_DISPLAY & LCD_DBLINKOFF);
                    setmode = 1;//Activa modo configuracion
                    state++; //Seguiente estado
                }
            }
            else cnt = 0;
            break;
        case 2: //Lectura del Encoder
            if(ckpin != PORTBbits.RB0) //Si cambio CKpin
            {
                ckpin = PORTBbits.RB0; //Actualiza valor ckpin
                if(PORTBbits.RB0 ^ PORTBbits.RB1) //sentido horario
                {
                    val++;
                    if(val > 100) val = 100;
                    LCDWriteNum(val,3);
                }
                else //Sentido antihorario
                {
                    if(val > 0) val--;
                    LCDWriteNum(val,3);
                }
                LCDGotoXY(12,0); //Podivionst rl vutdod LCD
            }
            if(PORTBbits.RB2 == 0) //Si esta presionado pulsador RB2
            {
                cnt++;
                if(cnt > 999) //presionado por 1000ms
                {
                    cnt = 0;
                    state++;
                }
            }
            else cnt = 0;
            break;
        case 3: //Espera liberacion del pulsador RB2
            if(PORTBbits.RB2 == 1)
            {
                cnt++;
                if(cnt > 99) //espera por 99ms
                {
                    cnt = 0;
                    adcset = val; //Actualiza valor de ajuste adcset
                    LCDWriteCMD(LCD_DISPLAY & LCD_DCURSOROFF & LCD_DBLINKOFF);
                    setmode = 0; //Cambia a modo normal
                    state = 0; //Reinicia estado 
                }
            }
            else cnt = 0;
            break;     
    }
}

void putch(char byte)
{
    while(PIR1bits.TXIF == 0) {};
    TXREG = byte; 
}
