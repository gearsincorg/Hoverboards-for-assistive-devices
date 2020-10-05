/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC16LF18446
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB 	          :  MPLAB X 5.40	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set JSLE aliases
#define JSLE_TRIS                 TRISAbits.TRISA1
#define JSLE_LAT                  LATAbits.LATA1
#define JSLE_PORT                 PORTAbits.RA1
#define JSLE_WPU                  WPUAbits.WPUA1
#define JSLE_OD                   ODCONAbits.ODCA1
#define JSLE_ANS                  ANSELAbits.ANSA1
#define JSLE_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define JSLE_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define JSLE_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define JSLE_GetValue()           PORTAbits.RA1
#define JSLE_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define JSLE_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define JSLE_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define JSLE_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define JSLE_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define JSLE_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define JSLE_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define JSLE_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set JSDO aliases
#define JSDO_TRIS                 TRISAbits.TRISA2
#define JSDO_LAT                  LATAbits.LATA2
#define JSDO_PORT                 PORTAbits.RA2
#define JSDO_WPU                  WPUAbits.WPUA2
#define JSDO_OD                   ODCONAbits.ODCA2
#define JSDO_ANS                  ANSELAbits.ANSA2
#define JSDO_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define JSDO_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define JSDO_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define JSDO_GetValue()           PORTAbits.RA2
#define JSDO_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define JSDO_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define JSDO_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define JSDO_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define JSDO_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define JSDO_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define JSDO_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define JSDO_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set USER2 aliases
#define USER2_TRIS                 TRISAbits.TRISA4
#define USER2_LAT                  LATAbits.LATA4
#define USER2_PORT                 PORTAbits.RA4
#define USER2_WPU                  WPUAbits.WPUA4
#define USER2_OD                   ODCONAbits.ODCA4
#define USER2_ANS                  ANSELAbits.ANSA4
#define USER2_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define USER2_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define USER2_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define USER2_GetValue()           PORTAbits.RA4
#define USER2_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define USER2_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define USER2_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define USER2_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define USER2_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define USER2_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define USER2_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define USER2_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set POWER_EN aliases
#define POWER_EN_TRIS                 TRISAbits.TRISA5
#define POWER_EN_LAT                  LATAbits.LATA5
#define POWER_EN_PORT                 PORTAbits.RA5
#define POWER_EN_WPU                  WPUAbits.WPUA5
#define POWER_EN_OD                   ODCONAbits.ODCA5
#define POWER_EN_ANS                  ANSELAbits.ANSA5
#define POWER_EN_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define POWER_EN_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define POWER_EN_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define POWER_EN_GetValue()           PORTAbits.RA5
#define POWER_EN_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define POWER_EN_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define POWER_EN_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define POWER_EN_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define POWER_EN_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define POWER_EN_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define POWER_EN_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define POWER_EN_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set BLUE aliases
#define BLUE_TRIS                 TRISBbits.TRISB4
#define BLUE_LAT                  LATBbits.LATB4
#define BLUE_PORT                 PORTBbits.RB4
#define BLUE_WPU                  WPUBbits.WPUB4
#define BLUE_OD                   ODCONBbits.ODCB4
#define BLUE_ANS                  ANSELBbits.ANSB4
#define BLUE_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define BLUE_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define BLUE_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define BLUE_GetValue()           PORTBbits.RB4
#define BLUE_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define BLUE_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define BLUE_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define BLUE_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define BLUE_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define BLUE_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define BLUE_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define BLUE_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set GREEN aliases
#define GREEN_TRIS                 TRISBbits.TRISB5
#define GREEN_LAT                  LATBbits.LATB5
#define GREEN_PORT                 PORTBbits.RB5
#define GREEN_WPU                  WPUBbits.WPUB5
#define GREEN_OD                   ODCONBbits.ODCB5
#define GREEN_ANS                  ANSELBbits.ANSB5
#define GREEN_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define GREEN_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define GREEN_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define GREEN_GetValue()           PORTBbits.RB5
#define GREEN_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define GREEN_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define GREEN_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define GREEN_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define GREEN_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define GREEN_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define GREEN_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define GREEN_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RED aliases
#define RED_TRIS                 TRISBbits.TRISB6
#define RED_LAT                  LATBbits.LATB6
#define RED_PORT                 PORTBbits.RB6
#define RED_WPU                  WPUBbits.WPUB6
#define RED_OD                   ODCONBbits.ODCB6
#define RED_ANS                  ANSELBbits.ANSB6
#define RED_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RED_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RED_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RED_GetValue()           PORTBbits.RB6
#define RED_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RED_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RED_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define RED_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define RED_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define RED_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define RED_SetAnalogMode()      do { ANSELBbits.ANSB6 = 1; } while(0)
#define RED_SetDigitalMode()     do { ANSELBbits.ANSB6 = 0; } while(0)

// get/set USER1 aliases
#define USER1_TRIS                 TRISBbits.TRISB7
#define USER1_LAT                  LATBbits.LATB7
#define USER1_PORT                 PORTBbits.RB7
#define USER1_WPU                  WPUBbits.WPUB7
#define USER1_OD                   ODCONBbits.ODCB7
#define USER1_ANS                  ANSELBbits.ANSB7
#define USER1_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define USER1_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define USER1_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define USER1_GetValue()           PORTBbits.RB7
#define USER1_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define USER1_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define USER1_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define USER1_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define USER1_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define USER1_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define USER1_SetAnalogMode()      do { ANSELBbits.ANSB7 = 1; } while(0)
#define USER1_SetDigitalMode()     do { ANSELBbits.ANSB7 = 0; } while(0)

// get/set JSUP aliases
#define JSUP_TRIS                 TRISCbits.TRISC0
#define JSUP_LAT                  LATCbits.LATC0
#define JSUP_PORT                 PORTCbits.RC0
#define JSUP_WPU                  WPUCbits.WPUC0
#define JSUP_OD                   ODCONCbits.ODCC0
#define JSUP_ANS                  ANSELCbits.ANSC0
#define JSUP_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define JSUP_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define JSUP_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define JSUP_GetValue()           PORTCbits.RC0
#define JSUP_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define JSUP_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define JSUP_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define JSUP_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define JSUP_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define JSUP_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define JSUP_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define JSUP_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set JSRI aliases
#define JSRI_TRIS                 TRISCbits.TRISC1
#define JSRI_LAT                  LATCbits.LATC1
#define JSRI_PORT                 PORTCbits.RC1
#define JSRI_WPU                  WPUCbits.WPUC1
#define JSRI_OD                   ODCONCbits.ODCC1
#define JSRI_ANS                  ANSELCbits.ANSC1
#define JSRI_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define JSRI_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define JSRI_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define JSRI_GetValue()           PORTCbits.RC1
#define JSRI_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define JSRI_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define JSRI_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define JSRI_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define JSRI_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define JSRI_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define JSRI_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define JSRI_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set JSBU aliases
#define JSBU_TRIS                 TRISCbits.TRISC2
#define JSBU_LAT                  LATCbits.LATC2
#define JSBU_PORT                 PORTCbits.RC2
#define JSBU_WPU                  WPUCbits.WPUC2
#define JSBU_OD                   ODCONCbits.ODCC2
#define JSBU_ANS                  ANSELCbits.ANSC2
#define JSBU_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define JSBU_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define JSBU_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define JSBU_GetValue()           PORTCbits.RC2
#define JSBU_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define JSBU_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define JSBU_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define JSBU_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define JSBU_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define JSBU_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define JSBU_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define JSBU_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set M_RX aliases
#define M_RX_TRIS                 TRISCbits.TRISC3
#define M_RX_LAT                  LATCbits.LATC3
#define M_RX_PORT                 PORTCbits.RC3
#define M_RX_WPU                  WPUCbits.WPUC3
#define M_RX_OD                   ODCONCbits.ODCC3
#define M_RX_ANS                  ANSELCbits.ANSC3
#define M_RX_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define M_RX_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define M_RX_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define M_RX_GetValue()           PORTCbits.RC3
#define M_RX_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define M_RX_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define M_RX_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define M_RX_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define M_RX_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define M_RX_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define M_RX_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define M_RX_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()              PORTCbits.RC5
#define RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()             do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()           do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode()         do { ANSELCbits.ANSC5 = 1; } while(0)
#define RC5_SetDigitalMode()        do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set M_TX aliases
#define M_TX_TRIS                 TRISCbits.TRISC6
#define M_TX_LAT                  LATCbits.LATC6
#define M_TX_PORT                 PORTCbits.RC6
#define M_TX_WPU                  WPUCbits.WPUC6
#define M_TX_OD                   ODCONCbits.ODCC6
#define M_TX_ANS                  ANSELCbits.ANSC6
#define M_TX_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define M_TX_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define M_TX_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define M_TX_GetValue()           PORTCbits.RC6
#define M_TX_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define M_TX_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define M_TX_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define M_TX_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define M_TX_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define M_TX_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define M_TX_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define M_TX_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set POT_ENA aliases
#define POT_ENA_TRIS                 TRISCbits.TRISC7
#define POT_ENA_LAT                  LATCbits.LATC7
#define POT_ENA_PORT                 PORTCbits.RC7
#define POT_ENA_WPU                  WPUCbits.WPUC7
#define POT_ENA_OD                   ODCONCbits.ODCC7
#define POT_ENA_ANS                  ANSELCbits.ANSC7
#define POT_ENA_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define POT_ENA_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define POT_ENA_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define POT_ENA_GetValue()           PORTCbits.RC7
#define POT_ENA_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define POT_ENA_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define POT_ENA_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define POT_ENA_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define POT_ENA_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define POT_ENA_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define POT_ENA_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define POT_ENA_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF0 pin functionality
 * @Example
    IOCCF0_ISR();
 */
void IOCCF0_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF0 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF0 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF0_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF0_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF0 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF0_SetInterruptHandler() method.
    This handler is called every time the IOCCF0 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF0_SetInterruptHandler(IOCCF0_InterruptHandler);

*/
extern void (*IOCCF0_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF0 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF0_SetInterruptHandler() method.
    This handler is called every time the IOCCF0 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF0_SetInterruptHandler(IOCCF0_DefaultInterruptHandler);

*/
void IOCCF0_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF2 pin functionality
 * @Example
    IOCCF2_ISR();
 */
void IOCCF2_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF2 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF2 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF2_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF2_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF2 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF2_SetInterruptHandler() method.
    This handler is called every time the IOCCF2 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF2_SetInterruptHandler(IOCCF2_InterruptHandler);

*/
extern void (*IOCCF2_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF2 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF2_SetInterruptHandler() method.
    This handler is called every time the IOCCF2 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF2_SetInterruptHandler(IOCCF2_DefaultInterruptHandler);

*/
void IOCCF2_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/