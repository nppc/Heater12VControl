#ifdef MAX31855
	#define MAXSCK_PIN  7
	#define MAXCS_PIN   8
	#define MAXDO_PIN   9

	unsigned long max31855_ms = millis();
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
	// the conversion time is about 70-100ms
	int readMAX31855(void) { 
	  uint16_t d = 0;

		pinMode(MAXCS_PIN, OUTPUT); // pin to low
		delay(1);
		pinMode(MAXSCK_PIN, OUTPUT); // pin to low 
		delay(1);
		// we need only 12 bytes of data. It is no sense with thermocouple to have .00 precision.
		for (byte i=0; i<12; i++) {
			pinMode(MAXSCK_PIN, OUTPUT); // pin to low 
			delay(1);
			d <<= 1;
			if (digitalRead(MAXDO_PIN)){d |= 1;}
			pinMode(MAXSCK_PIN, INPUT); // pin to high 
			delay(1);
		}
		pinMode(MAXCS_PIN, INPUT); // pin to high
		return digitalSmooth((d>1400 ? 0 : d), BSmoothArray); // higher than 1400 means negative temperature
	}

	// the conversion time is about 70-100ms
	float getTemperature(void){
		if(max31855_ms+80 < millis()) {
			max31855_ms = millis();
			lastTemperature = readMAX31855();
		}
		return lastTemperature;
	}
#endif