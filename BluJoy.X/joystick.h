/* 
 * File:   
 * Author: Phil Malone
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef JOYSTICK_HEADER_H
#define	JOYSTICK_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/pin_manager.h"

// Constants
#define JOYSTICK_BUTTONS    0
#define JOYSTICK_POTS       1
#define JOYSTICK_TWISTGRIP  2


// Function declarations
void    enableJoystick(void);
void    disableJoystick(void);
void    stopMotion(void);

void    initJoystick(void);
uint8_t getJoystickType(void);
void    setJoystickType(uint8_t jsType);
void    setJoystickSpeed(int8_t jsSpeedMode);

int16_t deadband(int16_t jsValue, int16_t center);
void    readJoystick(void);
void    readButtonJoystick(void);
void    readPotJoystick(void);
void    togglePower(void) ;
void    calculateMotion(void);
int16_t limitSpeed(int16_t targetFP, int16_t limitedFP, int16_t accelFP);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

