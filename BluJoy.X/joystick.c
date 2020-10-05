/*
 * joystick.c :  Reads switch inputs and generates axis motions.
 * Implements acceleration limits
 * Assumes constant 50mS cycle time from Timer 1 (20 updates per second)
 * 
 * To enable small variation using integers, speeds will be left shifted SHIFT_BITS places
 * Integers holding shifted values will have FP (Fixed Point) suffix.
 * 
 */

#include "mcc_generated_files/mcc.h"
#include <math.h>
#include "joystick.h"
#include "serial.h"
#include "timers.h"
#include "ui.h"

#define     MAX_REPLY       2
#define     ESTOP_HOLD      1000

#define     SHIFT_BITS      2
#define     DEAD_BAND       512

#define     AXIAL_ACC_LIMIT   600      //  mm/s/s
#define     AXIAL_STOP_LIMIT 1024      //  mm/s/s
#define     YAW_STOP_LIMIT   2048      //  deg/s/s 
#define     YAW_ACC_LIMIT     800      //  deg/s/s 
#define     SWEEP_ACC_LIMIT   200      //  deg/s/s   
#define     CYCLE_PER_SEC      20       

#define     TOP_AXIAL_SPEED 300      //  mm/s  
#define     TOP_YAW_SPEED    30      //  deg/s  
#define     TOP_SWEEP_SPEED  15      //  deg/s      

uint8_t     joystickType    = JOYSTICK_BUTTONS;
bool        joystickEnabled = false;
bool        estopPending    = false;
bool        estopActive     = false;
uint32_t    estopTimer      = 0;

int16_t     axialCenter     = 2048;
int16_t     yawCenter       = 2048;
int32_t     axialCenterSum  = 0;
int32_t     yawCenterSum    = 0;
#define     CALIBRATE_COUNT 128
#define     CALIBRATE_SHIFT   7

uint8_t     replyBuffer[4 * MAX_REPLY];
int16_t     targetAxialFP   = 0;
int16_t     targetYawFP     = 0;

int16_t     limitedAxialFP  = 0;
int16_t     limitedYawFP    = 0;

int16_t     accelAxialFP    = 0;
int16_t     accelYawFP      = 0;

int16_t     accelLimitAxialFP       = (AXIAL_ACC_LIMIT << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitAxialStopFP   = (AXIAL_STOP_LIMIT << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitYawFP         = (YAW_ACC_LIMIT   << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitYawStopFP     = (YAW_STOP_LIMIT   << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitSweepFP       = (SWEEP_ACC_LIMIT << SHIFT_BITS) / CYCLE_PER_SEC ;

int16_t     topAxialSpeedFP ;
int16_t     topYawSpeedFP   ;
int16_t     topSweepSpeedFP ;


void    initJoystick(void) {
    setJoystickType(getUIType());
    setJoystickSpeed(getUISpeedMode());
    TMR1_SetInterruptHandler(readJoystick);
    joystickEnabled = false;
    stopMotion();
}

void    setJoystickSpeed(int8_t jsSpeedMode) {
    topAxialSpeedFP = (TOP_AXIAL_SPEED << SHIFT_BITS) * (3 + jsSpeedMode) / 3;
    topYawSpeedFP   = (TOP_YAW_SPEED   << SHIFT_BITS) * (3 + jsSpeedMode) / 3;
    topSweepSpeedFP = (TOP_SWEEP_SPEED << SHIFT_BITS) * (3 + jsSpeedMode) / 3;
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

uint8_t getJoystickType(void) {
    return joystickType;
}

void    setJoystickType(uint8_t jsType) {
    joystickType = jsType;
    switch (joystickType) {
        case JOYSTICK_BUTTONS:
            ADCON0bits.ADON = 0;  // Turn OFF A-D module
            ANSELA = 0x01;  // Set PA1 and PA2 to Digital
            WPUA   = 0x16;  // Weak Pull Ups on PA1 and PA2
            break;

        case JOYSTICK_POTS:
            ANSELA = 0x07;  // Set PA1 and Pa2 to Analog
            WPUA   = 0x10;  // NO Weak Pull Ups on PA1 and PA2            
            ADCON0bits.ADON = 1;  // Turn ON A-D module
            sleep(20);

            // determine joystick centers
            axialCenterSum     = 0;
            yawCenterSum       = 0;
            for (int i = 0;  i < CALIBRATE_COUNT; i++) {
                axialCenterSum += ADCC_GetSingleConversion(JSDO);
                yawCenterSum   += ADCC_GetSingleConversion(JSLE);
            }
            axialCenter = axialCenterSum >> CALIBRATE_SHIFT;
            yawCenter   = yawCenterSum >> CALIBRATE_SHIFT;
            break;
    }
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

        // Only run JS if no ESTOP
        if (estopActive){
            sendBTEstopCmd();

            targetAxialFP   = 0;
            targetYawFP     = 0;
            limitedAxialFP  = 0;
            limitedYawFP    = 0;
        } else {
            // Read the appropriate JS Type
            switch (joystickType) {
                default:
                case JOYSTICK_BUTTONS:
                    readButtonJoystick();
                    break;

                case JOYSTICK_POTS:
                    readPotJoystick();
                    break;
            }
        }

        // resetBTTimer();  // FOR DEBUG ONLY
        
        // check for recent replies
        while (EUSART1_is_rx_ready()) {
            if (EUSART1_Read() == '/') {
                resetBTTimer();
                setBTTimeout(BT_TIMEOUT);  // engage longer timeout
            }
        }
    }
}

void    readButtonJoystick(void) {
    
    accelAxialFP = accelLimitAxialFP;
    accelYawFP   = accelLimitYawFP;
    
    // run regular joystick processing
    if (JSUP_GetValue() == 0)
        targetAxialFP =  topAxialSpeedFP;
    else if (JSDO_GetValue() == 0)
        targetAxialFP = -topAxialSpeedFP;
    else {
        accelAxialFP  = accelLimitAxialStopFP;
        targetAxialFP =   0;
    }

    if (JSRI_GetValue() == 0){
        if (targetAxialFP == 0) {
            targetYawFP =  topYawSpeedFP;
        }
        else {
            targetYawFP = topSweepSpeedFP;
            accelYawFP  = accelLimitSweepFP;
        }
    }
    else if (JSLE_GetValue() == 0){
        if (targetAxialFP == 0) {
            targetYawFP =  -topYawSpeedFP;
        }
        else {
            targetYawFP = -topSweepSpeedFP;
            accelYawFP  =  accelLimitSweepFP;
        }
    }
    else {
        targetYawFP = 0;
        accelYawFP  = accelLimitYawStopFP;
    }

    // calculate motion profile and send to serial port.
    calculateMotion();
    sendBTSpeedCmd(limitedAxialFP >> SHIFT_BITS, limitedYawFP >> SHIFT_BITS, false);
}

void    readPotJoystick(void) {
    accelAxialFP = 2048;
    accelYawFP   = 2048;
    
    int16_t  axial = - deadband(ADCC_GetSingleConversion(JSDO), axialCenter);
    int16_t  yaw   = - deadband(ADCC_GetSingleConversion(JSLE), yawCenter);
    
    targetAxialFP = (int16_t)((topAxialSpeedFP * (int32_t)axial) >> 11);
    targetYawFP   = (int16_t)((topYawSpeedFP   * (int32_t)yaw)   >> 11);

    // calculate motion profile and send to serial port.
    calculateMotion();
    sendBTSpeedCmd(limitedAxialFP >> SHIFT_BITS, limitedYawFP >> SHIFT_BITS, false);
    // sendBTSpeedCmd(axial, yaw, false);
}

int16_t deadband(int16_t jsValue, int16_t center){
    jsValue -= center ;
    if (jsValue > DEAD_BAND) {
        jsValue = (int16_t)(((int32_t)(jsValue - DEAD_BAND) * 2048) / (2048 - DEAD_BAND));
    } else if (jsValue < -DEAD_BAND) {
        jsValue = (int16_t)(((int32_t)(jsValue + DEAD_BAND) * 2048) / (2048 - DEAD_BAND));
    } else {
        return 0;
    }
    return jsValue;
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
