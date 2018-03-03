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
unsigned char rsp14[100] = "";
GSM_STATES gsm_state;

void main (void) {
    while(1) {
        if(SW1 == 0) {
            Wait(1);
            if(SW1 == 1) {
                mJTAGPortEnable(0);
                LedInit();
                ButtonInit();

                I2CInit();
                HTS221Init();
                HTS221Calibration();
                UARTInit();
                GSMStart();
                TRHCalc();

                Wait(10000);
                LED1On();

                SendATCommand("AT");
                GetResponse(rsp1, 200);

                SendATCommand("AT+CREG?");
                GetResponse(rsp2, 200);

                SendATCommand("AT+QIMODE=0");
                GetResponse(rsp3, 200);

                SendATCommand("AT+QICSGP=1,\"internet.telekom\"");
                GetResponse(rsp4, 400);

                SendATCommand("AT+QIREGAPP");
                GetResponse(rsp5, 400);

                SendATCommand("AT+QIACT");
                GetResponse(rsp6, 400);

                SendATCommand("AT+QIDNSIP=1");
                GetResponse(rsp7, 400);

                SendATCommand("AT+QIOPEN=\"TCP\",\"vocsabi.asuscomm.com\",\"2048\"");
                GetResponse(rsp8, 800);

                SendATCommand("AT+QISTAT");
                GetResponse(rsp14, 200);

                SendATCommand("AT+QISEND=4");
                GetResponse(rsp9, 200);

                SendDataFloat(T);
                GetResponse(rsp10, 200);
                Wait(2000);

                SendATCommand("AT+QISEND=4");
                GetResponse(rsp9, 200);

                SendDataFloat(RH);
                GetResponse(rsp10, 200);
                Wait(2000);

                SendATCommand("AT+QISACK");
                GetResponse(rsp11, 200),

                SendATCommand("AT+QICLOSE");
                GetResponse(rsp12, 200);

                SendATCommand("AT+QIDEACT");
                GetResponse(rsp13, 200);
                
                LED1Off();
//                while(1);
            }
        }
    }
}

