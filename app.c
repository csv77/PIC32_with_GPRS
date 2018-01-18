#include <p32xxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <peripheral/ports.h>
#include "panel.h"
#include "system.h"
#include "fgv.h"
#include <stdbool.h>

I2C_STATUS state;
unsigned int H0_rH_x2;
unsigned int H1_rH_x2;
unsigned int T0_degC_x8;
unsigned int T1_degC_x8;
int H0_T0_OUT;
int H1_T0_OUT;
int T0_OUT;
int T1_OUT;
float H0_RH;
float H1_RH;
float T0_degC;
float T1_degC;
float T;
float RH;
unsigned char adat[16];
char menu;
unsigned char rsp1[100] = "";
unsigned char rsp2[100] = "";
unsigned char rsp3[100] = "";
unsigned char rsp4[100] = "";
unsigned char rsp5[100] = "";
unsigned char rsp6[100] = "";
unsigned char rsp7[100] = "";
unsigned char rsp8[100] = "";
unsigned char rsp9[100] = "";
unsigned char rsp10[100] = "";
unsigned char rsp11[100] = "";
unsigned char rsp12[100] = "";
unsigned char rsp13[100] = "";
GSM_STATES gsm_state;

void main (void) {
    mJTAGPortEnable(0);
    LedInit();
    ButtonInit();
    
//    I2CInit();
//    HTS221Init();
//    HTS221Calibration();
    UARTInit();
    GSMStart();
    
    LED1On();
        
    SendATCommand("AT");
    GetResponse(rsp1, 100000);
    
    SendATCommand("AT+CREG?");
    GetResponse(rsp2, 200000);
    
    SendATCommand("AT+QIMODE=0");
    GetResponse(rsp3, 200000);
    
    SendATCommand("AT+QICSGP=1,\"internet.telekom\"");
    GetResponse(rsp4, 800000);
    
    SendATCommand("AT+QIREGAPP");
    GetResponse(rsp5, 200000);
    
    SendATCommand("AT+QIACT");
    GetResponse(rsp6, 100000);
    
    SendATCommand("AT+QIDNSIP=1");
    GetResponse(rsp7, 200000);

    SendATCommand("AT+QIOPEN=\"TCP\",\"vocsabi.asuscomm.com\",\"2048\"");
    GetResponse(rsp8, 800000);

    SendATCommand("AT+QISEND=4");
    GetResponse(rsp9, 200000);
    
    float value = 2.42f;
        
    SendDataFloat(value);
    GetResponse(rsp10, 200000);
    Wait(10000);
    
    SendATCommand("AT+QISACK");
    GetResponse(rsp11, 200000),
    
    SendATCommand("AT+QICLOSE");
    GetResponse(rsp12, 100000);
    
    SendATCommand("AT+QIDEACT");
    GetResponse(rsp13, 100000);
    while(1);
//    char message[] = "Test message\n";
//    SendSMS(message);
//    
//    SendATCommand("AT+CMGF=1");
//    getString(rsp4, 16);
//    
//    SendATCommand("AT+CMGS=\"+36306205338\"");
//    getString(rsp5, 27);
//    
//    SendSMS("Teszt");
//    getString(rsp6, 100);
    
//    while(1){
//        TRHCalc();
//        menu=GetChar();
//        switch(menu){
//            case 't':
////                SendDataBuffer("Homerseklet: ", strlen("Homerseklet: "));
//                SendTorH(T);
//                break;
//            case 'h':
////                SendDataBuffer("Paratartalom: ", strlen("Paratartalom: "));
//                SendTorH(RH);
//                break;
//        }
//    }

}

