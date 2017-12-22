void WDT_Init() {
	cli();
	wdt_reset();
	WDTCSR = (1<<WDCE)|(1<<WDE); 
	//WDTCSR = (1<<WDIE)|(1<<WDE)|(1<<WDP3); //Start watchdog timer with 4s prescaller and interrupt
	WDTCSR = (0<<WDIE)|(1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0); //Start watchdog timer with 2s prescaller
	sei();
}

/*
ISR(WDT_vect) {
	// we assume that interrupts already off
	// turn off heater
    digitalWrite(SSD1_PIN, LOW);
    digitalWrite(SSD2_PIN, LOW);
	wdt_disable();	// disable WDT
	// show alert message
	Tft.Bcolor=DARK_RED;Tft.fillRectangle(0,17,319,239);
	Tft.bold = FONT_BOLD;Tft.setXY(50,100);Tft.Fcolor=WHITE;Tft.drawString_P(PSTR("Watchdog: System halted!"));
	#ifdef BUZZER
		for(byte i=0;i<10;i++){
			// beeps
			Beep(800);
			delay(500);
		}	// infinite loop
	#endif
	delay(5*60000);	// wait 5 minutes
	// restart
	cli();
	wdt_reset();
	WDTCSR = (1<<WDCE)|(1<<WDE); //set up WDT interrupt
	WDTCSR = (0<<WDIE)|(1<<WDE)|(1<<WDP3); //Start watchdog timer with 4s prescaller
	sei();
	while(1){}	// reset after 4 seconds
}
*/
