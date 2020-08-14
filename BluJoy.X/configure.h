// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIGURE_HEADER_H
#define	CONFIGURE_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MAC_LENGTH  12

void    initConfiguration(void);

void    pairBluetoothDevices(void);
void    SetMasterTXRX(void);
void    SetSlaveTXRX(void);
void    SetSlaveTXMasterRX(void);
void    SetDualReceive(void);

void    setSerialBaud(uint16_t baud);
bool    powerIsOn(void);

void    setBTBaudRatesTo38400(void);
bool    getBTAddress(uint8_t * MAC, bool isMaster);
void    setBTConnection(uint8_t * MAC, bool isMaster);

void    doFactoryReset(void);
void    turnPowerOn(void);
void    turnPowerOff(void);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CONFIGURE_HEADER_H */

