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

// Function declarations
void    enableJoystick(void);
void    disableJoystick(void);

void    initJoystick(void);
void    readJoystick(void);
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

