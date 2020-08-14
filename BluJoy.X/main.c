/**
  Generated Main Source File

  File Name:
    main.c

*/

#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "timers.h"
#include "joystick.h"
#include "serial.h"
#include "configure.h"
#include "ui.h"

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    initTimers();   
    initSerial();   
    initJoystick();   
    initUI();   
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    initConfiguration();

    turnPowerOn();
    showStartup();
    
    sleep(1000);
    enableJoystick();
           
    while (1) {
        runUI();
        
        // check to see if we should shut down the power
        if (BTTimeRemaining() == 0) {
            turnPowerOff();
            showShutdown();

            SLEEP();
            NOP();
            
            while (!powerIsOn()) ;
            
            showStartup();
            resetBTTimer();
        }

        sleep(50);
    }
    
    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();
}

/**
 End of File
*/