#ifdef LOGGER
	uint8_t loggerXcoord;
	unsigned long loggerMillis=millis();
#endif

void logger128secOLED(){
	if(loggerMillis+1000<millis()){
		loggerMillis=millis();
		#ifdef OLED
		u8g2.setFont(u8g2_font_profont12_tn);
		#endif
		// Y=15 is setPoint
		uint16_t curTmp = round(currentTemp);
		uint16_t setPnt = setPoint;
		// wait until temperature reaches drawing ares
		if(curTmp < setPnt-16 || curTmp > setPnt+15 || loggerXcoord>127){
			// currently only print current temperature in upper left corner
			loggerXcoord=0;
			#ifdef OLED
			u8g2.clearBuffer();
			loggerPrintNumbers(curTmp, setPnt);
			#endif
		} else {
			#ifdef OLED
			// draw dots in middle of 128x32 OLED
			for(uint8_t i=0;i<128;i+=2){
				u8g2.drawPixel(i,15);
			}
			loggerPrintNumbers(curTmp, setPnt);
			u8g2.drawPixel(loggerXcoord,31-(curTmp-setPnt-16) );
			#endif
			loggerXcoord++;
			
		}
		#ifdef OLED
		u8g2.sendBuffer();
		#endif
		#ifdef DEBUG
		Serial.print("loggerXcoord: ");Serial.println(loggerXcoord);
		#endif
	}
	
}

#ifdef OLED
void loggerPrintNumbers(uint16_t curTmp, uint16_t setPnt) {
	// print current temperature
	u8g2.setCursor(0, 10);
	u8g2.print(curTmp);
	// print preset temperature
	u8g2.setCursor(100, 10);
	u8g2.print(setPnt);
}
#endif
