1. What is causing the bias/baseline in the polling example to be higher than the interrupt example.
	With polling, the CPU is constantly on consuming some energy in the while(1) loop checking the if condition constantly. In the interrupt function, the GIE puts the computer to sleep until the interrupt
	or button pressed is fired off. So unlike the polling function, the interrupt function is not actually checking conditions constantly therefore not using power. It waits for a button press to turn on and off. 	

2. Even with the LED unplugged, why is there still power increases when you press the button?
	In the interrupt example, the button press for P2.3 is programmed (set to input, cleared, and has its edge select toggled) 
	in the function to turn the CPU on upon being pressed. The LED has nothing to do with the interrupt vectors firing or not.