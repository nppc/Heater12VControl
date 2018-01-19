#define LOGGER_XPOS 127-68
#define LOGGER_YPOS 31-10	// center


//9X5
static const uint8_t u8g_plus10_bits[] U8X8_PROGMEM = { 0xD0, 0x01, 0x52, 0x01, 0x57, 0x01, 0x52, 0x01, 0xD0, 0x01, };
//9X5
static const uint8_t u8g_minus10_bits[] U8X8_PROGMEM = { 0xD0, 0x01, 0x50, 0x01, 0x57, 0x01, 0x50, 0x01, 0xD0, 0x01, };

unsigned long loggerMillis=millis();
byte logvalues[59];	// 60 elements (-127:+127)
byte loggerXcoord=0;

	
void inlineLogger_fill(){
	if(loggerMillis+1000<millis()){	// step every 2 seconds
		loggerMillis=millis();
		int curTmp = round(currentTemp);
		int setPnt = setPoint;

		int tempDiff = abs(curTmp-setPnt);
		if (tempDiff>10 || loggerXcoord>59){
			clearLogArray();
			loggerXcoord=0;
		} else {
			logvalues[loggerXcoord]=curTmp-setPnt;
		}
		loggerXcoord++;
	}
}

void inlineLogger_draw(){
	#ifdef OLED
	u8g2.setDrawColor(1);
	// draw dots in middle of 128x32 OLED
	for(uint8_t i=0;i<60;i+=4){
		u8g2.drawPixel(LOGGER_XPOS+i,LOGGER_YPOS);
	}
	u8g2.drawXBMP(127-9, LOGGER_YPOS-2-5, 9, 5, u8g_plus10_bits);
	u8g2.drawXBMP(127-9, LOGGER_YPOS+2, 9, 5, u8g_minus10_bits);
	for(uint8_t i=0;i<60;i++){
		byte tmpVal=logvalues[i];
		if(tmpVal!=127){
			u8g2.drawPixel(LOGGER_XPOS+i,LOGGER_YPOS-tmpVal);
		}
	}
	u8g2.setDrawColor(2); //restore mode
	#endif
}

#ifdef LOGGER
uint8_t loggerYcoordPrev=255; // something that out of bounds of the screen
	
void logger128secOLED(){
	
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
#endif

#endif

void clearLogArray(){
	for(uint8_t i=0;i<60;i++){
		logvalues[i]=127;
	}
}

