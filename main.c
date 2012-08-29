#include <htc.h>
// FOSC_INTOSCIO - Internal oscilator
// WDTE_OFF - Watchdog timer disabled
// CP_OFF - Code protection off
// MCLRE_OFF - Used for input
// PWRTE_OFF - Power up timer off
// CPD_OFF - Data code prot off
// BOREN_OFF - Brown out detection off
__CONFIG(BOREN_OFF & CPD_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & WDTE_OFF & FOSC_INTOSCIO);

/* Connections:

Output:
GP0 - To Relay (and relay LED)
GP4 - Gain signal out

Input:
GP3 - CLAP in signal
GP1/AN1 - Trimput for delay of signal

GP2/AN2 - Unused (wired)
GP5 - Unused (not wired)

*/

#define XXFOSC 8000000L
// Delay Function
#define _delay_us(x) { unsigned char us; \
us = (x)/(12000000/XXFOSC)|1; \
while(--us != 0) continue; }

void _delay_ms(unsigned int ms)
{
	unsigned char i;
	if (!ms) return;
	do {
		i = 4;
		do {
			_delay_us(164);
		} while(--i);
	} while(--ms);
}

void _wait_n_blink(unsigned int ms) 
{
// Warning!!! Requires even 20's to work!!!
	int i=1;
	while (ms > 0) {
		GP5=i; // LED4 
		if (i) i=0;
		else i=1;
		_delay_ms(20);
		ms -= 20;

	}
	GP5=0;
}

void main(void)
{
	unsigned int ADvalue;

	GPIO = 0x00; // set GPIO to low outputs 
	OSCCON=0x70; // Select 8 Mhz internal clock
 
	WDTCON = 0x00; // Watchdog not used
	CMCON0 = 7; 
 	TRISIO = 0b00001010; 	// GP4 and AN1 are inputs
	ANSEL=0b0000010;
	ADCON0=0b10000101; // right justify, adc enabled, AN1 selected
	//GPIO port definitions
	GPIO=0;
	

	// ---------------------
	// 
	// 	
	_delay_ms(100);

	ADON=1; // turn on the A2D conversion module
	_delay_ms(100);

	GO_DONE=1;
	GP0=0; // To Relay
	GP4=0; // Gain signal
	
	ADvalue=0;
	GO_DONE=1; // Starta AD
	while (GO_DONE) continue;
	ADIF=0; // clear ADIF bit
	ADvalue=ADRESL + (ADRESH << 8); //get data - right justified

	ADvalue = ADvalue/4;   // Scale delay down - for my application 
                               // this range is enough
#ifdef RUNNING_ON_PCKIT2_LABBOARD
	// Ready! Light LED4
	GP5=1;
#endif

	while(1)
	{
		if(GP3==1) //CLAP in 
		{
#ifdef RUNNING_ON_PCKIT2_LABBOARD
			GP5=0; // Turn off LED4
#endif
			// Wait before driving relay high
			_delay_ms(ADvalue); // 
			GP0 = 1; // Set relay signal high
			
			_delay_ms(100);
			GP0=0; // Close relay
			
#ifdef RUNNING_ON_PCKIT2_LABBOARD
			_wait_n_blink(1000); // Wait 1 second and blink led to show we are not armed
#else
			_delay_ms(1000);  // Wait 1 second to avoid multiple triggs from same event.
#endif
			// Done
			// Read new value from AD for delay value
			ADvalue=0;
			GO_DONE=1; // Start AD
			while (GO_DONE) continue;
			ADIF=0; // clear ADIF bit
			ADvalue=ADRESL + (ADRESH << 8); //get data - right justified

			ADvalue = ADvalue/4;

#ifdef RUNNING_ON_PCKIT2_LABBOARD
			// Ready! Light LED4
			GP5=1;
#endif
						
		} // end if(GPIO3==1)
	}

} // end main

