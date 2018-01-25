void WDT_Init() {
	cli();
	wdt_reset();
	WDTCSR = (1<<WDCE)|(1<<WDE); 
#ifdef SOFTRESET
	WDTCSR = (1<<WDIE)|(1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0); //Start watchdog timer with 2s prescaller
#else
	WDTCSR = (0<<WDIE)|(1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0); //Start watchdog timer with 2s prescaller
#endif
	sei();
}


#ifdef SOFTRESET
ISR(WDT_vect) {
	asm volatile ("jmp 0");
}
#endif