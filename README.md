

First, I check whether the ID matches or not.

    If it matches, I prompt the user to press SW1 to open the gate. This invokes EXTINT0 and activates the motor.

    At the end of the above function, I prompt the user to press SW2 to close the gate. This also invokes EXTINT1 and runs the motor.

    Finally, I prompt the user to press SW3 to invoke EXTINT1 and display the total collection.

I used UART communication for the RFID reader and tag.
I used a PWM pin (not part of any communication protocol) to control the motor as an output device.
For the switches, I implemented the interrupt concept to handle inputs.
