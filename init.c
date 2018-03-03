#include <p32xxxx.h>
#include <plib.h>
#include "panel.h"
#include "fgv.h"
#include <stdbool.h>

extern GSM_STATES gsm_state;

void LedInit(void) {
    PORTSetPinsDigitalOut(IOPORT_E, BIT_2 | BIT_3);
    PORTSetPinsDigitalOut(IOPORT_A, BIT_10);
    PORTSetPinsDigitalOut(IOPORT_D, BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_5 | BIT_6 | BIT_7 | BIT_8);
    PORTSetPinsDigitalIn(IOPORT_B, BIT_0);
    LED1Off();
    LED2Off();
}

void ButtonInit(void) {
    PORTSetPinsDigitalIn(IOPORT_E, BIT_4);
    PORTSetPinsDigitalIn(IOPORT_G, BIT_9);
}

void Wait(unsigned int hatar) {
    int i;
    for (i = 0; i < hatar * 1000; i++)
        ;
}

void UARTInit(void) {
    UARTConfigure(UART_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetLineControl(UART_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_ID, GetPeripheralClock(), UART_DATA_RATE);
    UARTEnable(UART_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}

void GSMStart(void) {
    PORTClearBits(IOPORT_E, BIT_2);
    Wait(10);
    PORTSetBits(IOPORT_E, BIT_2);
    while(PORTReadBits(IOPORT_B, BIT_0) != 1)
        ;
    PORTClearBits(IOPORT_E, BIT_2);
    gsm_state = GSM_POWER_ON;
}

void T1Init(int oszto, int hatar) {
    OpenTimer1(T1_ON | T1_GATE_OFF | T1_SOURCE_INT, hatar);
    T1CONbits.TCKPS = oszto;
    WriteTimer1(0);
    mT1ClearIntFlag();
}

void T2Init(int oszto, int hatar) {
    OpenTimer2(T2_ON | T2_GATE_OFF | T2_SOURCE_INT, hatar);
    T2CONbits.TCKPS = oszto;
    WriteTimer2(0);
    mT2ClearIntFlag();
}

void T2Init32bit(int oszto, int hatar) {
    OpenTimer23(T23_32BIT_MODE_ON | T23_ON | T23_GATE_OFF | T23_SOURCE_INT, hatar);
    T2CONbits.TCKPS = oszto;
    WriteTimer23(0);
    mT23ClearIntFlag();
}

void T3Init(int oszto, int hatar) {
    OpenTimer3(T3_ON | T3_GATE_OFF | T3_SOURCE_INT, hatar);
    T3CONbits.TCKPS = oszto;
    WriteTimer3(0);
    mT3ClearIntFlag();
}

void InitInterrupt() {
    INTEnableSystemMultiVectoredInt();
    mT1SetIntPriority(4);
    mT1IntEnable(1);
    mT2SetIntPriority(3);
    mT2IntEnable(1);
}

#pragma interrupt T1Interrupt ipl4 vector 4
void T1Interrupt (void) {
    mT1ClearIntFlag();
}

#pragma interrupt T2Interrupt ipl3 vector 8
void T2Interrupt (void) {
    mT2ClearIntFlag();
}

