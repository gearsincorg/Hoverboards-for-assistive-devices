#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "configure.h"
#include "serial.h"
#include "timers.h"
#include "ui.h"
#include "joystick.h"

uint8_t slaveMAC[MAC_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t masterMAC[MAC_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t RX_Buffer[40];
uint8_t charsRead;
bool    powerOn = false;

void    initConfiguration() {
    setSerialBaud(38400);
    SetSlaveTXRX();
}

void    SetSlaveTXRX(void){
    RX1DTPPS = 0x15;   //        RC5->EUSART1:RX1;    
    RC4PPS = 0x0F;     //EUSART1:TX1->RC4 ;    
    RC6PPS = 0x16;     //        RC6->RC6;    
    // sleep(5);
}

void    SetPowerdownTXRX(void){
    RX1DTPPS = 0x15;   //        RC5->EUSART1:RX1;    
    RC4PPS = 0x14;     //        RC4->RC4;    
    RC6PPS = 0x16;     //        RC6->RC6;    
    sleep(5);
}

void    SetDualReceive(void){
    RX1DTPPS = 0x15;   //        RC5->EUSART1:RX1;    
    RC4PPS   = 0x0F;   //EUSART1:TX1->RC4 ;    
    RC6PPS   = 0x0F;   //EUSART1:TX1->RC6 ;    
    sleep(5);
}

void    SetMasterTXRX(void){
    RX1DTPPS = 0x13;   //        RC3->EUSART1:RX1;    
    RC6PPS = 0x0F;     //EUSART1:TX1->RC6;    
    RC4PPS = 0x14;     //        RC4->RC4;    
    sleep(5);
}

void    SetSlaveTXMasterRX(void){
    RX1DTPPS = 0x13;   //RC3->EUSART1:RX1;    
    
    RC4PPS = 0x0F;     //EUSART1:TX1->RC4 ;    
    RC6PPS = 0x16;     //        RC6->RC6;    
    sleep(5);
}

void    turnPowerOn(){
    POT_ENA_SetHigh();
    POWER_EN_SetLow();
    TRISC = 0x2F;  // Pins 4 and 6,7 are outputs
    SetSlaveTXRX();
    powerOn = true;
}

void    turnPowerOff(){
    POWER_EN_SetHigh();
    POT_ENA_SetLow();
    SetPowerdownTXRX();
    powerOn = false;
}

bool    powerIsOn() {
    return powerOn;
}

void    setSerialBaud(uint16_t baud){
    if (baud == 38400) {
        // 38400
        SP1BRGL = 0x67;
        SP1BRGH = 0x00;
    } else {
        // 9600
        SP1BRGL = 0xA0;
        SP1BRGH = 0x01;
    }
    sleep(200);
}


void    pairBluetoothDevices(void){
    setBTBaudRatesTo38400();
    
    // Get two Mac Addresses
    getBTAddress(slaveMAC, false);
    getBTAddress(masterMAC, true);
         
    // now setup the default connections
    setBTConnection(slaveMAC, true);    
    setBTConnection(masterMAC, false);    
    
    // put ports in test mode
    SetSlaveTXMasterRX();

    while (!USER1_pressed() && !USER2_pressed()) {
        sendBTString("HUGS\n");
        if (receiveBTBuffer(RX_Buffer, 5, 1000) == 5){
            if (strstr((char *)RX_Buffer, "HUGS\n"))
                pulseLEDColor( COLOR_CYAN, 4, 1);
            else
                pulseLEDColor( COLOR_RED, 4, 1);
        } else {
            pulseLEDColor( COLOR_YELLOW, 4, 1);
        }
        sleep(50);
    }

    // Return ports to normal configuration
    SetSlaveTXRX();
    sleep(1000);
}

void    setBTBaudRatesTo38400() {
    // switch to 9600 baud and tell modules to run at 38400
    // This will be ignored if they are already there.
    SetDualReceive();
    setSerialBaud(9600);
    
    sendBTString("AT");
    sleep(100);
    sendBTString("AT+BAUD2");
    sleep(250);
    sendBTString("AT");
    sleep(500);
    sendBTString("AT+RESET");  // expect OK+RESET
    sleep(500);
    setSerialBaud(38400);
}

bool    getBTAddress(uint8_t * MAC, bool isMaster) {

    uint8_t * addrPointer ;
    
    if (isMaster) 
        SetMasterTXRX();
    else
        SetSlaveTXRX();
        
    sendBTString("AT");
    sleep(300);  // was pulse
    sleep(10);
    if (isMaster) 
        sendBTString("AT+ROLE1");
    else
        sendBTString("AT+ROLE0");
        
    charsRead = receiveBTBuffer(RX_Buffer, 20, 400);
    pulseLEDColor((strstr((void *)RX_Buffer, "OK") != NULL) ? COLOR_GREEN : COLOR_YELLOW, 100, 200);
    
    // get the MAC address  Expect reply:   OK+ADDR:xxxxxxxxxxxx
    sleep(10);  // need to keep short to stay disconnected, dut long enough to process prior command.
    sendBTString("AT+ADDR?");
    charsRead = receiveBTBuffer(RX_Buffer, 30, 400);
    addrPointer = (void *)strstr((void *)RX_Buffer, "ADDR:");
            
    if (addrPointer != 0) {
        memcpy(MAC, addrPointer + 5, 12);
        pulseLEDColor( COLOR_GREEN, 100, 200);
        return true;
    } else {
        pulseLEDColor( COLOR_RED, 100, 200);
        return false;
    }
}

void    setBTConnection(uint8_t * MAC, bool isMaster){

    if (isMaster) 
        SetMasterTXRX();
    else
        SetSlaveTXRX();
        
    sleep(1000);
    sendBTString("AT");
    charsRead = receiveBTBuffer(RX_Buffer, 9, 100); // could be OK or ATOK+CONN or ATOK+LOST
    pulseLEDColor((strstr((void *)RX_Buffer, "OK") != NULL) ? COLOR_GREEN : COLOR_YELLOW, 100, 200);

    sleep(10); // ????
    if (*MAC != 0) {
        sendBTString("AT+CON");
        sendBTBuffer(MAC, MAC_LENGTH, true);
        charsRead = receiveBTBuffer(RX_Buffer, 28, 1000);
        pulseLEDColor((strstr((void *)RX_Buffer, "OK") != NULL) ? COLOR_GREEN : COLOR_RED, 100, 200);
    }
}    

void    doFactoryReset() {
    // switch to 38400 baud and tell modules to run at 9600
    // This will be ignored if they are already there.
    pulseLEDColor(COLOR_BLUE, 1000, 200);
    
    SetDualReceive();
    setSerialBaud(38400);
    sleep(500);

    sendBTString("AT");
    sleep(100);
    sendBTString("AT+CLEAR");
    sleep(100);
    sendBTString("AT+BAUD0");
    sleep(500);
    sendBTString("AT+RESET");  // expect OK+RESET
    sleep(1000);
    setSerialBaud(9600);
   
    sendBTString("AT+RENEW");
    charsRead = receiveBTBuffer(RX_Buffer, 16, 100); 
    pulseLEDColor((strstr((void *)RX_Buffer, "OK") != NULL) ? COLOR_GREEN : COLOR_YELLOW, 100, 200);
    sleep(1000);
    
    SetSlaveTXRX();
}
