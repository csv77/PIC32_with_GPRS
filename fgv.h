#ifndef FGV_H
#define	FGV_H

typedef enum {
	GSM_POWER_OFF,
    GSM_POWER_ON,
	GSM_PIN_OK,
	GSM_CREG_OK,
	GSM_CGREG_OK,
    GSM_QIREGAPP_OK,
	GSM_TCP_CONNECTION_OPENED,
	GSM_TCP_DATA_SENT,
	GSM_TCP_CONNECTION_CLOSED
} GSM_STATES;

/*Init functions*/
void LedInit(void);
void ButtonInit(void);
void Wait(unsigned int hatar);
void UARTInit(void);
void T1Init(int oszto, int hatar);
void T2Init(int oszto, int hatar);
void T2Init32bit(int oszto, int hatar);
void T3Init(int oszto, int hatar);

/*I2C and sensor handling functions*/
void I2CInit(void);
void I2CWrite(int SlaveAddress, int reg_address, int data);
void I2CRead(int SlaveAddress, unsigned char reg_address,
             unsigned char *data, unsigned char data_pc);
void HTS221Init (void);
void HTS221Calibration (void);
float LinIntpol(int x0, float y0, int x1, float y1, int x_m);
void TRHCalc(void);

/*GSM control functions*/
void GetResponse(char *buffer, int timeout);
void SendATCommand(unsigned char *buffer);
void SendSMS(unsigned char *buffer);
void SendDataFloat(float floatValue);
void SendDataChar(char character);

#endif	/* FGV_H */

