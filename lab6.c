#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h" //needed for Hardware bit specific addressing
#include "inc/hw_memmap.h"	//needed for Hardware bit specific addressing

#define 	MASK				0x0A //Define MASK as Red and Green LED variables.  In this case it's 0x02 + 0x08 = 0x0A for both Red and Greeb LEDs
#define 	RED_MASK		0x02
#define 	GREEN_MASK	0x08
#define PFE (*((volatile unsigned long *)0x40025038)) //address of all 3 LEDs
#define PF1 (*((volatile unsigned long *)0x40025008))	//address of Red LED
#define PF2 (*((volatile unsigned long *)0x40025010))	//address of Blue LED
#define PF3 (*((volatile unsigned long *)0x40025020))	//address of Green LED

//functions to read current values at PORTF's data and dir
unsigned long PortF_Data_Read (void) 
{
	return (GPIO_PORTF_DATA_R);
}

unsigned long PortF_Dir_Read (void) 
{
	return (GPIO_PORTF_DIR_R);
}

void PortFunctionInit(void)
{
  	volatile uint32_t ui32Loop;   
	  // Enable the GPIO port that is used for the on-board LED.    
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
    
    // Do a dummy read to insert a few cycles after enabling the peripheral.    
    ui32Loop = SYSCTL_RCGC2_R;

    // Unlock GPIO Port F
		GPIO_PORTF_LOCK_R = 0x4C4F434B;   
		GPIO_PORTF_CR_R |= 0x11;           // allow changes to PF0 n PF4
	
	//Important notes: direction configuration must be declared BEFORE digital function
		// Enable the GPIO pin for the Red (PF1) and Green (PF3) LEDs.  Set the direction as output        
    GPIO_PORTF_DIR_R |= MASK;
    // Set the direction of PF0 (SW2) n PF4 (SW1) as input by clearing the bit
    GPIO_PORTF_DIR_R &= ~0x11;    
		// enable the GPIO pins for digital function.
		GPIO_PORTF_DEN_R |= 0x1F; 
	
		//Enable pull-up on PF0 n PF4
		GPIO_PORTF_PUR_R |= 0x11;
}


int main(void)
{	
		//initialize the GPIO ports	
		PortFunctionInit();

		GPIO_PORTF_DATA_R |= RED_MASK;
		//GPIO_PORTF_DATA_R
		PortF_Data_Read();
		PortF_Dir_Read();
	
		/*HARDWARE bit specific addressing code
		while (1)
		{
			if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0x10)	//SW1 is NOT pressed
			{	
				if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0x01)	//SW2 is NOT pressed
				{					
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); //otherwise, turn off LED
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02); //turn on LED (PF1) as output					
					SysCtlDelay(2000000);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //turn on LED (PF1) as output
					SysCtlDelay(2000000);
				}
				else
				{					
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //otherwise, turn off LED
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x082); //turn on LED (PF1) as output					
					SysCtlDelay(2000000);
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08); //turn on LED (PF1) as output
					SysCtlDelay(2000000);
				}
			}
			else
			{
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //otherwise, turn off LED
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); //otherwise, turn off LED
			}
			
		}
		*/
	
		/* SOFTWARE bit specific addressing code
		while(1)
		{
			if((GPIO_PORTF_DATA_R&0x10)==0x10) //SW1 not pressed
				{				        
					if((GPIO_PORTF_DATA_R&0x01)==0x01) //SW2 is NOT pressed
					{
							//turn off Green mask
							GPIO_PORTF_DATA_R &= ~GREEN_MASK;						

							// Toggle the LED.
							GPIO_PORTF_DATA_R ^= RED_MASK;
							// Delay for a bit.
							SysCtlDelay(2000000);
						
					}
					else
					{
							//turn off Red mask
							GPIO_PORTF_DATA_R &= ~RED_MASK;							

							// Toggle the LED.
							GPIO_PORTF_DATA_R ^= GREEN_MASK;
							// Delay for a bit.
							SysCtlDelay(2000000);							
					}
			}		
			else
			{
				//Turn off both Red and Green LEDs
				GPIO_PORTF_DATA_R &= ~GREEN_MASK;
				GPIO_PORTF_DATA_R &= ~RED_MASK;
			}
		}
		*/
		
    
}
