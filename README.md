This is a kick-starter if you want to use the Velleman kit MK139 in photography applications and find that the delay of the circuit is not acceptable.

The Velleman kit comes with a pre-programmed PIC with no source code.
I found that it could be replaced with a PIC12F683 and this repository has the code required to make it work.

As this project developed I added a trim-put to be able to change the delay from the "clap" to when the relay is closed (i.e. when my flash triggers) - if you want to start small and not include this in your design then remove all references to the analog part (AD*) and hardwire the ADvalue to the delay you want.

The code also has some debug code for use with the PCKit2 development board. This will require a jumper between GP5 and the led you want to use.
