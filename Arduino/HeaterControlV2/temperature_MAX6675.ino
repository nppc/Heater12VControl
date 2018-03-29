#ifdef MAX6675
	#define MAXSCK_PIN  7
	#define MAXCS_PIN   8
	#define MAXDO_PIN   9

	unsigned long max6675_ms = millis();
	float lastTemperature = 0;

	void Temperature_init(void) {
		digitalWrite(MAXCS_PIN,LOW);
		digitalWrite(MAXSCK_PIN,LOW);
		pinMode(MAXCS_PIN, INPUT); // pin to high
		pinMode(MAXSCK_PIN, OUTPUT); // pin to low 
		pinMode(MAXDO_PIN, INPUT);
		for(uint8_t i=0;i<filterSamples*2;i++){getTemperature();}
	}

	// only positive temperatures
	// timer is messed up (x8 faster), but using delay(1) here is no problem, because 1/8ms is still too much (actually delays can be as low as 100ns)
	int readMAX6675(void) { 
	  uint16_t d = 0;

		pinMode(MAXCS_PIN, OUTPUT); // pin to low
		delay(1);
		pinMode(MAXSCK_PIN, OUTPUT); // pin to low 
		delay(1);
		// we need only 11 bytes of data (Actually it is 10, because sign bit reads always 0). It is no sense with thermocouple to have .00 precision.
		for (byte i=0; i<11; i++) {
			pinMode(MAXSCK_PIN, OUTPUT); // pin to low 
			delay(1);
			d <<= 1;
			if (digitalRead(MAXDO_PIN)){d |= 1;}
			pinMode(MAXSCK_PIN, INPUT); // pin to high 
			delay(1);
		}
		pinMode(MAXCS_PIN, INPUT); // pin to high
		return digitalSmooth(d, BSmoothArray); 
	}

	// did not found conversion time in datasheet so, lets assume it is the same as MAX31855
	float getTemperature(void){
		if(max6675_ms+80 < millis()) {
			max6675_ms = millis();
			lastTemperature = readMAX6675();
		}
		return lastTemperature;
	}
#endif