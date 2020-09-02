/*
 * joystick.c :  Reads switch inputs and generates axis motions.
 * Implements acceleration limits
 * Assumes constant 50mS cycle time from Timer 1 (20 updates per second)
 * 
 * To enable small variation using integers, speeds will be left shifted SHIFT_BITS places
 * Integers holding shifted values will have FP (Fixed Point) suffix.
 * 
 */

#include <math.h>
#include "joystick.h"
#include "serial.h"
#include "timers.h"
#include "ui.h"

#define     MAX_REPLY       2

#define     ESTOP_HOLD      1000

#define     SHIFT_BITS      2
#define     AXIAL_ACC_LIMIT 600      //  mm/s/s
#define     YAW_STOP_LIMIT 1000      //  deg/s/s 
#define     YAW_ACC_LIMIT   100      //  deg/s/s 
#define     SWEEP_ACC_LIMIT  50      //  deg/s/s   
#define     CYCLE_PER_SEC    20       

#define     TOP_AXIAL_SPEED 500
#define     TOP_YAW_SPEED   70
#define     TOP_SWEEP_SPEED 50

bool        joystickEnabled  = false;
bool        estopPending     = false;
bool        estopActive      = false;
uint32_t    estopTimer       = 0;

uint8_t     replyBuffer[4 * MAX_REPLY];
int16_t     targetAxialFP = 0;
int16_t     targetYawFP   = 0;

int16_t     limitedAxialFP = 0;
int16_t     limitedYawFP   = 0;

int16_t     accelAxialFP = 0;
int16_t     accelYawFP   = 0;

int16_t     accelLimitAxialFP   = (AXIAL_ACC_LIMIT << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitYawFP     = (YAW_ACC_LIMIT   << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitYawStopFP = (YAW_STOP_LIMIT   << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitSweepFP   = (SWEEP_ACC_LIMIT << SHIFT_BITS) / CYCLE_PER_SEC ;

int16_t     topAxialSpeedFP   = (TOP_AXIAL_SPEED << SHIFT_BITS) ;
int16_t     topYawSpeedFP     = (TOP_YAW_SPEED   << SHIFT_BITS) ;
int16_t     topSweepSpeedFP   = (TOP_SWEEP_SPEED << SHIFT_BITS) ;

void    initJoystick(void) {
    TMR1_SetInterruptHandler(readJoystick);
    joystickEnabled = false;
    stopMotion();
}

void    enableJoystick(){
    joystickEnabled = true;
    resetBTTimer();
    TMR1_StartTimer();
}

void    disableJoystick(){
    TMR1_StopTimer();
    joystickEnabled = false;
    TMR1_StopTimer();
}

void    stopMotion(void) {
    targetAxialFP   = 0;
    targetYawFP     = 0;
    limitedAxialFP  = 0;
    limitedYawFP    = 0;
    if (joystickEnabled)
        sendBTSpeedCmd(0, 0, false);
}

void    readJoystick(void) {
    
    accelAxialFP = accelLimitAxialFP;
    accelYawFP   = accelLimitYawFP;
    
    if (joystickEnabled) {

        // check for estop condition
        if (JSBU_GetValue() == 0) {
            if (estopPending){
                if (getTicksSince(estopTimer) > ESTOP_HOLD) {
                    estopActive = true;
                }
            } else {
                estopTimer = getTicks();
                estopPending = true;
                
                // Stop any pending motion
                stopMotion();
            }
        } else {
            estopPending = false;
            estopActive  = false;
        }
        
        if (estopActive){
            sendBTEstopCmd();
            
            targetAxialFP   = 0;
            targetYawFP     = 0;
            limitedAxialFP  = 0;
            limitedYawFP    = 0;
        } else {
            // run regular joystick processing
            
            if (JSUP_GetValue() == 0)
                targetAxialFP =  topAxialSpeedFP;
            else if (JSDO_GetValue() == 0)
                targetAxialFP = -topAxialSpeedFP;
            else
                targetAxialFP =   0;

            if (JSRI_GetValue() == 0){
                if (targetAxialFP == 0)
                    targetYawFP =  topYawSpeedFP;
                else {
                    targetYawFP =  topSweepSpeedFP;
                    accelYawFP   = accelLimitSweepFP;
                }
            }
            else if (JSLE_GetValue() == 0){
                if (targetAxialFP == 0)
                    targetYawFP =  -topYawSpeedFP;
                else {
                    targetYawFP =  -topSweepSpeedFP;
                    accelYawFP   = accelLimitSweepFP;
                }
            }
            else {
                targetYawFP =   0;
                accelYawFP   = accelLimitYawStopFP;
            }
            
            // calculate motion profile and send to serial port.
            calculateMotion();
            sendBTSpeedCmd(limitedAxialFP >> SHIFT_BITS, limitedYawFP >> SHIFT_BITS, false);
        }

        // check for recent replies
        while (EUSART1_is_rx_ready()) {
            if (EUSART1_Read() == '/') {
                resetBTTimer();
                setBTTimeout(BT_TIMEOUT);  // engage longer timeout
            }
        }
    }
}
    
void    calculateMotion(void) {
    // use target speeds and acceleration limits to generate limited speeds.
    limitedAxialFP = limitSpeed(targetAxialFP, limitedAxialFP, accelAxialFP);
    limitedYawFP   = limitSpeed(targetYawFP,   limitedYawFP,   accelYawFP);
}

int16_t limitSpeed(int16_t targetFP, int16_t limitedFP, int16_t accelFP) {

    int16_t diffFP  = targetFP - limitedFP;
    int16_t speedFP = targetFP;
    
    // boost deceleration rate if we are stopping
    if (targetFP == 0)
        accelFP *= 2;
    
    // Ensure acceleration limit is not too small.
    if (accelFP == 0)
        accelFP = 1;
    
    // Limit acceleration (speed change) if required.
    if (diffFP > accelFP){
        speedFP = limitedFP + accelFP;
    } else if (diffFP < -accelFP){
        speedFP = limitedFP - accelFP;
    }
    
    return (speedFP);
}
