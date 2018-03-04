#include <p32xxxx.h>
#include <plib.h>
#include "panel.h"
#include "fgv.h"
#include <stdbool.h>

void GetResponse(char *buffer, int timeout) {
    timeout *= 1000;
    long j = 0;
    unsigned char data;
    while(1) {
        while(!UARTReceivedDataIsAvailable(UART_ID) && j < timeout) {
            j++;
        }
        if(j < timeout) {
            data = UARTGetDataByte(UART_ID);
            *buffer = data;
            buffer++;
            j = 0;
        }
        else {
            return;
        }
    }
}

void SendATCommand(unsigned char *buffer) {
    int size;
    for(size = 0; buffer[size] != '\0'; size++)
        ;
    while(size) {
        while(!UARTTransmitterIsReady(UART_ID))
            ;
        UARTSendDataByte(UART_ID, *buffer);
        buffer++;
        size--;
    }
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\r');
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\n');
    while(!UARTTransmissionHasCompleted(UART_ID))
        ;
}

void SendSMS(unsigned char *buffer) {
    int size;
    for(size = 0; buffer[size] != '\0'; size++)
        ;
    while(size) {
        while(!UARTTransmitterIsReady(UART_ID))
            ;
        UARTSendDataByte(UART_ID, *buffer);
        buffer++;
        size--;
    }
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, 0x1A);
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\r');
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\n');
    while(!UARTTransmissionHasCompleted(UART_ID))
        ;
}

void SendDataFloat(float floatValue) {
    int size = sizeof(float);
    union {
        float a;
        unsigned char bytes[size];
    } byteArray;
    byteArray.a = floatValue;
    while(size) {
        while(!UARTTransmitterIsReady(UART_ID))
            ;
        UARTSendDataByte(UART_ID, byteArray.bytes[size - 1]);
        size--;
    }
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, 0x1A);
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\r');
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\n');
    while(!UARTTransmissionHasCompleted(UART_ID))
        ;
}

void SendDataChar(char character) {
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, character);
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, 0x1A);
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\r');
    while(!UARTTransmitterIsReady(UART_ID))
        ;
    UARTSendDataByte(UART_ID, '\n');
    while(!UARTTransmissionHasCompleted(UART_ID))
        ;
}
