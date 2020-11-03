# MMA8452 motion detection with interrupts

This Arduino sketch sets up the MMA8452 accelerometer in the Motion detection mode, and routes it's Motion detection function to the physical interrupt port INT1.

I have followed the steps of https://www.nxp.com/docs/en/application-note/AN4070.pdf section 6.1 exactly to set up the accelerometer registries. 

The exaample sketch assumes that your MMA8452 INT1 pin is attached to hardware interrupt port of your microcontroller. In my case it was Atmega644P Pin 2(Arduino) or INT2 (Hardware Interrupt).

My Interrupt routine (ISR_isMoving) is triggered on the CHANGE event, that is - at this point we don't know what the state of the accelerometer pin INT1 is - so we are reading it - using digitalRead(2).


