#define LOGGER_XPOS 127-68
#define LOGGER_YPOS 31-10	// center


//9X5
static const uint8_t u8g_plus10_bits[] U8X8_PROGMEM = { 0xD0, 0x01, 0x52, 0x01, 0x57, 0x01, 0x52, 0x01, 0xD0, 0x01, };
//9X5
static const uint8_t u8g_minus10_bits[] U8X8_PROGMEM = { 0xD0, 0x01, 0x50, 0x01, 0x57, 0x01, 0x50, 0x01, 0xD0, 0x01, };

unsigned long loggerMillis=millis();
byte logvalues[60];	// 60 elements (-127:+127)
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
	u8g2.drawXBMP(127-9, LOGGER_YPOS-3-5, 9, 5, u8g_plus10_bits);
	u8g2.drawXBMP(127-9, LOGGER_YPOS+3, 9, 5, u8g_minus10_bits);
	for(uint8_t i=0;i<60;i++){
		byte tmpVal=logvalues[i];
		if(tmpVal!=127){
			u8g2.drawPixel(LOGGER_XPOS+i,LOGGER_YPOS-tmpVal);
		}
	}
	u8g2.setDrawColor(2); //restore mode
	#endif
}

void clearLogArray(){
	for(uint8_t i=0;i<60;i++){
		logvalues[i]=127;
	}
}

