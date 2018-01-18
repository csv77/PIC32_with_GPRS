/* 
 * File:   panel.h
 * Author: Csabi
 *
 * Created on 2016. március 22., 20:45
 */

#ifndef PANEL_H
#define	PANEL_H

#define LED1        LATEbits.LATE3
#define LED2        LATAbits.LATA10

#define LED1On()    {LED1=1;}
#define LED1Off()   {LED1=0;}
#define LED2On()    {LED2=1;}
#define LED2Off()   {LED2=0;}

#define LED1Tog()   {LED1=~LED1;}
#define LED2Tog()   {LED2=~LED2;}

#define SW1         PORTEbits.RE4
#define SW2         PORTGbits.RG9

#define SYS_CLOCK                   72000000L
#define GetPeripheralClock()        (SYS_CLOCK)
#define I2C_CLOCK_FREQ              100000

#define HTS_221                     I2C2
#define HTS_221_ADDRESS             0x5F    //1011111, slave 7 bites címe

#define UART_ID                     UART1
#define UART_DATA_RATE              115200

#endif
