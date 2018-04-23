#include <p32xxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include "panel.h"
#include "system.h"
#include "fgv.h"
#include <stdbool.h>

float T;
float RH;
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
GSM_STATES gsm_state = GSM_POWER_OFF;

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
                LED1On();
                
                while(1) {
                    switch(gsm_state) {
                        case GSM_POWER_OFF :
                            GSMStart();
                            break;
                        case GSM_POWER_ON :
                            SendATCommand("AT");
                            GetResponse(rsp1, 200);

                            SendATCommand("AT+CREG?");
                            GetResponse(rsp2, 200);
                            
                            if(strstr(rsp2, "CREG: 0,1") != NULL) {
                                gsm_state = GSM_CREG_OK;
                            }
                            break;
                        case GSM_CREG_OK :
                            SendATCommand("AT+QIMODE=0");
                            GetResponse(rsp3, 200);

                            SendATCommand("AT+QICSGP=1,\"internet.telekom\"");
                            GetResponse(rsp4, 400);
                            
                            if(strstr(rsp4, "OK")) {
                                gsm_state = GSM_QIREGAPP_OK;
                            }
                            break;
                        case GSM_QIREGAPP_OK :
                            SendATCommand("AT+QIREGAPP");
                            GetResponse(rsp5, 400);

                            SendATCommand("AT+QIACT");
                            GetResponse(rsp6, 400);

                            SendATCommand("AT+QIDNSIP=0");
                            GetResponse(rsp7, 400);

                            SendATCommand("AT+QIOPEN=\"TCP\",\"173.249.44.63\",\"2048\"");
                            GetResponse(rsp8, 800);

                            if(strstr(rsp8, "CONNECT OK") != NULL) {
                                LED2On();
                                gsm_state = GSM_TCP_CONNECTION_OPENED;
                            }
                            break;
                        case GSM_TCP_CONNECTION_OPENED :
                            while(1) {
                                if(SW2 == 0) {
                                    Wait(1);
                                    if(SW2 == 1) {
                                        TRHCalc();
                                        
                                        SendATCommand("AT+QISEND=1");
                                        GetResponse(rsp9, 200);
                                        
                                        SendDataChar('1');
                                        GetResponse(rsp10, 200);
                                        Wait(1000);
                                        
                                        SendATCommand("AT+QISEND=4");
                                        GetResponse(rsp9, 200);

                                        SendDataFloat(T);
                                        GetResponse(rsp10, 200);
                                        Wait(1000);

                                        SendATCommand("AT+QISEND=4");
                                        GetResponse(rsp9, 200);

                                        SendDataFloat(RH);
                                        GetResponse(rsp10, 200);
                                        Wait(1000);
                                    }
                                }
                                if(SW1 == 0) {
                                    Wait(1);
                                    if(SW1 == 1) {
                                        SendATCommand("AT+QISEND=1");
                                        GetResponse(rsp9, 200);
                                        
                                        SendDataChar('0');
                                        GetResponse(rsp10, 200);
                                        Wait(1000);
                                        break;
                                    }
                                }
                            }
                            gsm_state = GSM_TCP_DATA_SENT;
                            break;
                        case GSM_TCP_DATA_SENT :
                            SendATCommand("AT+QICLOSE");
                            GetResponse(rsp12, 200);

                            SendATCommand("AT+QIDEACT");
                            GetResponse(rsp13, 200);
                            gsm_state = GSM_TCP_CONNECTION_CLOSED;
                            break;
                    }
                    if(gsm_state == GSM_TCP_CONNECTION_CLOSED) {
                        gsm_state = GSM_POWER_ON;
                        break;
                    }
                }
                LED1Off();
                LED2Off();
            }
        }
    }
}
