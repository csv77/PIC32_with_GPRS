#include <p32xxxx.h>
#include <plib.h>
#include "panel.h"
#include "fgv.h"

extern I2C_STATUS state;
extern unsigned int H0_rH_x2;
extern unsigned int H1_rH_x2;
extern unsigned int T0_degC_x8;
extern unsigned int T1_degC_x8;
extern int H0_T0_OUT;
extern int H1_T0_OUT;
extern int T0_OUT;
extern int T1_OUT;
extern float H0_RH;
extern float H1_RH;
extern float T0_degC;
extern float T1_degC;
extern float T;
extern float RH;
extern unsigned char adat[16];

void I2CInit(void) {                                               //I2C konfigurálás
    I2CConfigure(HTS_221, 0);
    I2CSetFrequency(HTS_221, GetPeripheralClock(), I2C_CLOCK_FREQ);
    I2CEnable(HTS_221, TRUE);
}

void I2CWrite(int SlaveAddress, int reg_address, int data) {       //1 byte írása a slave adott regiszterébe
    while(!I2CBusIsIdle(HTS_221));
    I2CStart(HTS_221);
    do {
        state=I2CGetStatus(HTS_221);
    } while(state!=I2C_START);
    while(!I2CTransmitterIsReady(HTS_221));
    I2CSendByte(HTS_221, (SlaveAddress<<1 | 0));
    while(!I2CTransmissionHasCompleted(HTS_221));
    while(!I2CByteWasAcknowledged(HTS_221));
    while(!I2CTransmitterIsReady(HTS_221));
    I2CSendByte(HTS_221, reg_address);
    while(!I2CTransmissionHasCompleted(HTS_221));
    while(!I2CByteWasAcknowledged(HTS_221));
    while(!I2CTransmitterIsReady(HTS_221));
    I2CSendByte(HTS_221, data);
    while(!I2CTransmissionHasCompleted(HTS_221));
    while(!I2CByteWasAcknowledged(HTS_221));
    I2CStop(HTS_221);
    do {
        state=I2CGetStatus(HTS_221);
    } while(state!=I2C_STOP);
}

void I2CRead(int SlaveAddress, unsigned char reg_address, unsigned char *data, unsigned char data_pc) {     //adott számú byte adat kiolvasása a slave adott
    int i;                                                                                                   //regiszterét?l kezdve és beírása a tömbbe
    if (data_pc>1)
        reg_address=(1<<7) | reg_address;                           //több byte olvasásához 1<<7 | reg_address
    while(!I2CBusIsIdle(HTS_221));                                  
    I2CStart(HTS_221);
    do {
        state=I2CGetStatus(HTS_221);
    } while(state!=I2C_START);
    while(!I2CTransmitterIsReady(HTS_221));
    I2CSendByte(HTS_221, (SlaveAddress<<1 | 0));
    while(!I2CTransmissionHasCompleted(HTS_221));
    while(!I2CByteWasAcknowledged(HTS_221));
    while(!I2CTransmitterIsReady(HTS_221));
    I2CSendByte(HTS_221, reg_address);                              
    while(!I2CTransmissionHasCompleted(HTS_221));
    while(!I2CByteWasAcknowledged(HTS_221));
    I2CRepeatStart(HTS_221);
    do {
        state=I2CGetStatus(HTS_221);
    } while(state!=I2C_START);
    while(!I2CTransmitterIsReady(HTS_221));
    I2CSendByte(HTS_221, (HTS_221_ADDRESS<<1 | 1));
    while(!I2CTransmissionHasCompleted(HTS_221));
    while(!I2CByteWasAcknowledged(HTS_221));
    for (i=0; i<data_pc; i++) {
        I2CReceiverEnable(HTS_221,TRUE);
        while(!I2CReceivedDataIsAvailable(HTS_221));
        data[i]=I2CGetByte(HTS_221);
        if(i==data_pc-1)
            I2CAcknowledgeByte(HTS_221,FALSE);
        else
            I2CAcknowledgeByte(HTS_221,TRUE);
        while(!I2CAcknowledgeHasCompleted(HTS_221));
    }
    I2CStop(HTS_221);
    do {
        state=I2CGetStatus(HTS_221);
    } while(state!=I2C_STOP);
}

void HTS221Init (void) {
    I2CWrite(HTS_221_ADDRESS, 0x10, 0x1B);
    I2CWrite(HTS_221_ADDRESS, 0x20, 0x85);
    I2CWrite(HTS_221_ADDRESS, 0x21, 0x00);
    I2CWrite(HTS_221_ADDRESS, 0x22, 0x00);
}

void HTS221Calibration (void) {
    unsigned char c_data[16];
    I2CRead(HTS_221_ADDRESS, 0x30, c_data, 16);
    H0_rH_x2=c_data[0];
    H1_rH_x2=c_data[1];
    T0_degC_x8=((c_data[5] & 0x03)<<8) | c_data[2];
    T1_degC_x8=((c_data[5] & 0x0C)<<6) | c_data[3];
    H0_T0_OUT=(c_data[7]<<8) | c_data[6];
    H1_T0_OUT=(c_data[11]<<8) | c_data[10];
    T0_OUT=(c_data[13]<<8) | c_data[12];
    T1_OUT=(c_data[15]<<8) | c_data[14];
    
    if(H0_T0_OUT & 0x8000)
        H0_T0_OUT=-(0x8000-(0x7FFF & H0_T0_OUT));
    if(H1_T0_OUT & 0x8000)
        H1_T0_OUT=-(0x8000-(0x7FFF & H1_T0_OUT));
    if(T0_OUT & 0x8000)
        T0_OUT=-(0x8000-(0x7FFF & T0_OUT));
    if(T1_OUT & 0x8000)
        T1_OUT=-(0x8000-(0x7FFF & T1_OUT));
    
    H0_RH=(float)H0_rH_x2/2;
    H1_RH=(float)H1_rH_x2/2;;
    T0_degC=(float)T0_degC_x8/8;
    T1_degC=(float)T1_degC_x8/8;
}

float LinIntpol(int x0, float y0, int x1, float y1, int x_m) {
    float y_m;
    y_m=((float)((y1-y0)/(x1-x0)))*((float)(x_m-x0))+y0;
    return y_m;
}

void TRHCalc(void) {
    int H_OUT;
    int T_OUT;
    unsigned char c_data[4];
    I2CRead(HTS_221_ADDRESS, 0x28, c_data, 4);

    H_OUT=(c_data[1]<<8) | c_data[0];
    T_OUT=(c_data[3]<<8) | c_data[2];
    
    if(H_OUT & 0x8000) H_OUT=-(0x8000-(0x7FFF & H_OUT));
    if(T_OUT & 0x8000) T_OUT=-(0x8000-(0x7FFF & T_OUT));
    
    T=LinIntpol(T0_OUT, T0_degC, T1_OUT, T1_degC, T_OUT);
    RH=LinIntpol(H0_T0_OUT, H0_RH, H1_T0_OUT, H1_RH, H_OUT);
    if ((int)RH>99 | (int)RH==-72)
        RH=100;
    if ((int)RH<0)
        RH=0;
    if ((int)T>119)
        T=120;
    if ((int)T<-40)
        T=-40;
}