
### Hoverboards for Assistive Devices (HAD) 
By: Phil Malone


### Purpose:
The HAD project goal is to enable Hoverboards, or Hoverboard drive components to be re-purposed to provide low-cost mobility for assistive devices.

### Components:
The following components are part of this project:

HUGS (Hoverboard Universal Gateway System).  HUGS is new firmware that is loaded into a hoverboard to replace the tilt-based drive system with a command driven velocity system.  HUGS provides a documented command/response protocol whereby commands can be issued to single or dual drive conteollers.

BluJoy-PCB (Bluetooth Joystick PCB).  BluJoy-PCB is a custom two-part Circuit board with PIC microcontroller and two Bluetooth module.  Once sepparated, the two boards enables the Hoverboard to be controlled by a joystick, over a dedicated Bluetooth interface.  

BluJoy.X (Bluetooth Joystick Firmware). BluJoy.X is a custom MPLAB.X program loaded into the PIC processor on the BluJoy PCB.  It performs automatic Bluetooth pairing, and joystick command transmission.  The firmware currently supports microswitch based joysticks, but will be augmented for Potentiometer based joysticks soon.

