# HUGS Human Machine Interface (HMI) ESP32 module.  (Superceeded)

This code is used to program an ESP32 DEVKIT board to send command packets to a Hoverboard (dual controller type) that has been reprogrammed to communicate using the HUGS Protocol.

The code is built using the Espressif-IDF environment.

This code was originally intended to be used as the primary way to interface to a "dual controller" type hoverboard because the ESP32 can drive two serial ports, as well as the programming interface.
This concept was used to implement a wired joystick controller. The code is fully operation, but it will probably not be further developed.  See below. 

Unfortunately, this approach did not lend itself to a simple wireless joystick interface.

To provide a wireless control (joystick) interface, the HUGs protocol was extended to permit a single serial command to be sent to the master motor controller, which would then forward the appropriate components to the slave motor controller using the standard serial cable that is part of the split controller system.
This means that both drives to be controlled using a single serial interface.  This approach enables a simpler uController to be used to "drive" the Hoverboard, and also permits a simple Bluetooth-Serial module to be used to create a wireless interface.

This approach was developed into the BluJoy controller that is also contained in this repo. This design will ultimately replace both the wireless and wires control approach.

For additional information, refer to the Hoverboard Utility Gateway System repo.

https://github.com/gearsincorg/Hoverboard_Utility_Gateway_System


