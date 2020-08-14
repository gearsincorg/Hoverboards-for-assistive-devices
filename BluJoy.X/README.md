### Hoverboards for Assistive Devices (HAD) 
By: Phil Malone

BluJoy.X is a (Microchip) MPLAB firmware project.
It was created using MPLAB X IDE Version 5.4
It's written in C8 and it makes use of the V3 Code Configurator Module.

The code runs on a PIC16LF18446 processor.  
The '18446 was chosen because it has a serial USART with Configurable Port Pins, and a small quantity of EEPROM memory for storing configuration settings.

I use a PicKit3 to program the processor once it's mounted to the PCB.  

The Firmware is written to run on the BluJoy-PCB which is included as part of this repository.

The code operates in two main modes:

- RUN.	In this mode, the code determines the current joystic configuration from EEPROM, and applies the appropriate speed profiles to the Joystick inputs.  Every 50 mSec, new Axial and Yaw velocities are calculated and sent to the Hoverboard via Bluetooth.

- SETUP.  In this mode, the code reads the device ID's of the two attached Bluetooth modules, and used this information to configure each module to auto-connect with the other module.

To enter the SETUP mode, press and hold both buttons on the PCB for 4 seconds.