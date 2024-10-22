/*
 *  serialdevices.h
 *  BeebEm3
 *
 *  Created by Jon Welch on 28/08/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

extern unsigned char TouchScreenEnabled;
extern unsigned char EthernetPortEnabled;
//**CARBON** extern WindowRef mSerialPortWindow;
extern int mSerialHandle;
//**CARBON** extern WindowRef mEthernetPortWindow;
extern int mEthernetHandle;

void EthernetPortOpenDialog();
void EthernetPortCloseDialog();
//**CARBON**OSStatus MyEthernetPortStatusThread(void *parameter);
//**CARBON**OSStatus MyEthernetPortReadThread(void *parameter);
//**CARBON**OSStatus MyListenThread(void *parameter);
void EthernetPortOpen(void);
bool EthernetPortPoll(void);
void EthernetPortClose(void);
void EthernetPortWrite(unsigned char data);
unsigned char EthernetPortRead(void);
void EthernetPortStore(unsigned char data);
unsigned char EthernetPortGet(void);

void TouchScreenOpen(void);
bool TouchScreenPoll(void);
void TouchScreenClose(void);
void TouchScreenWrite(unsigned char data);
unsigned char TouchScreenRead(void);
void TouchScreenStore(unsigned char data);
void TouchScreenReadScreen(bool check);

void SerialPortOpenDialog();
void SerialPortCloseDialog();
int OpenSerialPort(const char *bsdPath);
void CloseSerialPort(int fileDescriptor);
//**CARBON**OSStatus MySerialReadThread(void *parameter);
//**CARBON**OSStatus MySerialStatusThread(void *parameter);
void SetSerialPortFormat(int Data_Bits, int Stop_Bits, int Parity, int RTS);
void SetSerialPortBaud(int Tx_Rate, int Rx_Rate);
void SerialPortWrite(unsigned char TDR);
unsigned char SerialPortGetChar(void);
int SerialPortIsChar(void);
