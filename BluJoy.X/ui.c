/*
 * ui.c :  Reads switch inputs and generates axis motions.
 * Implements acceleration limits
 * Assumes constant 50mS cycle time from Timer 1 (20 updates per second)
 * 
 * To enable small variation using integers, speeds will be left shifted SHIFT_BITS places
 * Integers holding shifted values will have FP (Fixed Point) suffix.
 * 
 */

#include "mcc_generated_files/mcc.h"
#include "configure.h"
#include "timers.h"
#include "joystick.h"
#include "ui.h"

__eeprom uint8_t  EEPROM_uiSpeedMode ;
__eeprom uint8_t  EEPROM_uiBrakeMode ;

uint8_t  uiSpeedMode ;
uint8_t  uiBrakeMode ;

uint8_t  timeoutSequence = 0;

uint8_t  R_LED ;
uint8_t  G_LED ;
uint8_t  B_LED ;
uint8_t  dutyCycle ;
uint8_t  uiState ;
uint32_t uiStateTime;

#define UI_IDLE             0
#define UI_USER1_HOLD       1
#define UI_USER1_DEBOUNCE   2
#define UI_USER2_HOLD       3
#define UI_USER2_DEBOUNCE   4
#define UI_USER12_HOLD      5
#define UI_USER12_LONG_HOLD 6

#define UI_SPEED_MODES 3
#define UI_BRAKE_MODES 2
#define UI_DEBOUNCE     200
#define UI_LONG_HOLD   4000

void    initUI(void) {
    R_LED = 0 ;
    G_LED = 0 ;
    B_LED = 0 ;
    dutyCycle = 0;
    uiState = UI_IDLE;
    uiStateTime = getTicks();
    timeoutSequence = 0;
    
    RED_SetHigh();
    GREEN_SetHigh();
    BLUE_SetHigh();
    
    uiSpeedMode = EEPROM_uiSpeedMode;
    uiBrakeMode = EEPROM_uiBrakeMode;
    
    TMR3_SetInterruptHandler(UI_PWM_handler);
    IOCCF0_SetInterruptHandler(turnPowerOn);
    IOCCF2_SetInterruptHandler(turnPowerOn);
}

void    runUI(void) {
    int32_t timeRemaining;
    
    switch (uiState) {
        case UI_IDLE:
            if (USER1_pressed()) {
                uiStateTime = getTicks();
                uiState = UI_USER1_DEBOUNCE; 
            } else if (USER2_pressed()) {
                uiStateTime = getTicks();
                uiState = UI_USER2_DEBOUNCE; 
            }
            break;

        case UI_USER1_DEBOUNCE:
            if (USER1_pressed()){
                if (getTicksSince(uiStateTime) > UI_DEBOUNCE) {
                    uiState = UI_USER1_HOLD;
                    pulseLEDColor(COLOR_RED, 50, 100);
                }
            }
            else {       
                uiState = UI_IDLE;
            }
            break;

        case UI_USER2_DEBOUNCE:
            if (USER2_pressed()){
                if (getTicksSince(uiStateTime) > UI_DEBOUNCE) {
                    uiState = UI_USER2_HOLD;
                    pulseLEDColor(COLOR_GREEN, 50, 100);
                }

            }
            else {       
                uiState = UI_IDLE;
            }
            break;

        case UI_USER1_HOLD:
            if (!USER1_pressed()){
                bumpUISpeedMode();
                uiState = UI_IDLE;
            }
            if (USER2_pressed()){
                pulseLEDColor(COLOR_BLUE, 50, 100);
                uiState = UI_USER12_HOLD;
            }
            
            break;
            
        case UI_USER2_HOLD:
            if (!USER2_pressed()){
                bumpUIBrakeMode();
                uiState = UI_IDLE;
            }
            if (USER1_pressed()){
                pulseLEDColor(COLOR_BLUE, 50, 100);
                uiState = UI_USER12_HOLD;
            }
            break;
            
        case UI_USER12_HOLD:
            if (getTicksSince(uiStateTime) > UI_LONG_HOLD) {
                pulseLEDColor(COLOR_WHITE, 50, 100);
                uiState = UI_USER12_LONG_HOLD;
            }
            
            if (!USER1_pressed() && !USER2_pressed()) {
                disableJoystick();
                pairBluetoothDevices();
                enableJoystick();
                uiState = UI_IDLE;
            }
                
            break;
            
        case UI_USER12_LONG_HOLD:
            if (!USER1_pressed() && !USER2_pressed()) {
                disableJoystick();
                // Do a factory reset
                doFactoryReset();
                enableJoystick();
                uiState = UI_IDLE;
            }
            break;
            
        default:
            break;
    }
    
    // Show operational state with 1 sec blink.
    if (oneSec()) {
        timeRemaining = BTTimeRemaining();
        if (timeRemaining < 5000)
            pulseLEDColor(0x100, 2, 1);
        else if (timeRemaining < 15000)
            pulseLEDColor(0x110, 2, 1);
        else
            pulseLEDColor(0x010, 2, 1);
    }
}

void    bumpUISpeedMode(){
    uiSpeedMode = ((uiSpeedMode +1) % UI_SPEED_MODES);
    EEPROM_uiSpeedMode = uiSpeedMode;
    blinkLEDColor(COLOR_YELLOW, uiSpeedMode + 1);
}

uint8_t getUISpeedMode(){
    return (uiSpeedMode);
}

void    bumpUIBrakeMode(){
    uiBrakeMode = ((uiBrakeMode +1) % UI_BRAKE_MODES);
    EEPROM_uiBrakeMode = uiBrakeMode;
    blinkLEDColor(COLOR_CYAN, uiBrakeMode + 1);
}

uint8_t getUIBreakMode(){
    return (uiBrakeMode);
}

void    showStartup(void){
    int8_t ramp = 0;
    
    for (ramp = 0 ; ramp < 7; ramp++) {
        G_LED = ramp;
        sleep(40);
    }
    G_LED = 0;
    setBTTimeout(BT_STARTTIME);  // Start out with a shorter timeout
}

void    showShutdown(void){
    int8_t ramp = 0;
    
    for (ramp = 7 ; ramp >= 0; ramp--) {
        R_LED = ramp;
        sleep(40);
    }
}

/**
 * 
 * @param RGB  16 bit unsigned number where bottom 12 LSB save vales for RG and B as 4 bit values, 
 * packed into number as 0x0RGB
 */
void    setLEDColor(uint16_t RGB) {
    B_LED = RGB & 0x0F;
    G_LED = (RGB >> 4) & 0x0F;
    R_LED = (RGB >> 8) & 0x0F;
}

void    pulseLEDColor(uint16_t RGB, uint16_t onTimeMS, uint16_t offTimeMS){
    setLEDColor(RGB);
    sleep(onTimeMS);
    setLEDColor(COLOR_OFF);
    sleep(offTimeMS);
}

void    blinkLEDColor(uint16_t RGB, uint8_t blinks){
    uint8_t blink;
    
    for(blink=0; blink < blinks; blink++){
        pulseLEDColor(RGB, 100, 400);
    }
}

void    UI_PWM_handler(void){
    // Manage a 16 step PWM Duty cycle for the 3 LEDs that form the RGB LED
    // Set the output low to turn on each cycle at dutyCycle = 0 (if required)
    // Set the output High when the required dutyCycle is reached for each LED
    dutyCycle = (dutyCycle+1) & 0x0F;
    // Turn on each color if has any brightness above 0
    if (dutyCycle == 0){
        if (R_LED > 0) 
            RED_SetLow();
        else
            RED_SetHigh();
            
        if (G_LED > 0) 
            GREEN_SetLow();
        else
            GREEN_SetHigh();
            
        if (B_LED > 0) 
            BLUE_SetLow();
        else
            BLUE_SetHigh();
    } else {
        // Turn off each color once the duty cycle goes over it's brightness
        if (dutyCycle >= R_LED) 
            RED_SetHigh();
        if (dutyCycle >= G_LED) 
            GREEN_SetHigh();
        if (dutyCycle >= B_LED) 
            BLUE_SetHigh();
    }
    
}

bool    USER1_pressed(void) {
    return USER1_GetValue() == 0;
}

bool    USER2_pressed(void) {
    return USER2_GetValue() == 0;
}


