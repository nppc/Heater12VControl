#ifdef LOGGER
	int loggerXcoord=255;
	uint8_t loggerYcoordPrev=255; // something that out of bounds of the screen
	unsigned long loggerMillis=millis();


void logger128secOLED(){
	int curTmp = round(currentTemp);
	int setPnt = setPoint;
	if(loggerMillis+2000<millis()){	// step every 2 seconds
		loggerMillis=millis();
		// calculate coordinates (currently it is ok, if we are out of the screen bounds)
		int loggerYCoord = round(16.0-(float)(curTmp-setPnt)*(16.0/(float)PID_ABSTEMPDIFFERENCE));
		if(loggerXcoord>127 || loggerYCoord>31){
			#ifdef OLED
			u8g2.clearBuffer();
			#endif
			loggerXcoord=0;
		} else {
			if(loggerYCoord>=0 && loggerYCoord<=31){
				// we are in printable area...
				#ifdef OLED
				// draw dots in middle of 128x32 OLED
				for(uint8_t i=0;i<128;i+=4){
					u8g2.drawPixel(i,16);
				}
				loggerPrintNumbers(curTmp, setPnt);
				//u8g2.drawPixel(loggerXcoord,constrain(loggerYCoord,0,31));
				u8g2.drawLine(constrain(loggerXcoord-1,0,127),loggerYcoordPrev,loggerXcoord,loggerYCoord);
				u8g2.setDrawColor(2);	// restore XOR mode
				u8g2.setFontMode(1);	// restore font mode
				#endif
				#ifdef DEBUG
				Serial.print("DRAW ");
				#endif
			}
			loggerXcoord++;
		}
		#ifdef DEBUG
		Serial.print(F("loggerXcoordPrev: "));Serial.print(loggerXcoord-1);
		Serial.print(F("; loggerYCoordPrev: "));Serial.print(loggerYcoordPrev);
		Serial.print(F("; loggerXcoord: "));Serial.print(loggerXcoord);
		Serial.print(F("; loggerYCoord: "));Serial.println(loggerYCoord);
		#endif
		loggerYcoordPrev = constrain(loggerYCoord,0,31);
	}
	#ifdef OLED
	u8g2.setFontMode(0);
	u8g2.setDrawColor(1);
	u8g2.setFont(u8g2_font_profont12_tn);
	loggerPrintNumbers(curTmp, setPnt);
	printHeaterState(); //print icon of the heater ON/OFF state
	u8g2.sendBuffer();
	#endif
	
}

#ifdef OLED
void loggerPrintNumbers(uint16_t curTmp, uint16_t setPnt) {
	// print current temperature
	u8g2.setCursor(0, 10);
	if(curTmp<100){u8g2.print("0");}
	u8g2.print(curTmp);
	// print preset temperature
	u8g2.setCursor(100, 10);
	if(setPnt<100){u8g2.print("0");}
	u8g2.print(setPnt);

}
#endif

#endif