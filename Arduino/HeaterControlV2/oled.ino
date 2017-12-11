#ifdef OLED
	static const uint8_t u8g_clear_bits[] U8X8_PROGMEM = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000
	};

	static const uint8_t u8g_logo_bits[] U8X8_PROGMEM = {
		0b10010000,
		0b01001000,
		0b00100100,
		0b00100100,
		0b01001000,
		0b10010000,
		0b10010000,
		0b01001000,
		0b00100100,
		0b00100100,
		0b00000000,
		0b11111110,
		0b00000000,
		0b11111110
	};


	void screenRedraw() {
		u8g2.clearBuffer();
		if(stateHeater){u8g2.drawXBMP( 86, 1, 8, 14, u8g_logo_bits);}
		u8g2.setCursor(0, 16);
		if (stateAdjustment) {
			// value is adjusted
			u8g2.print("<");
			u8g2.print(presetTemp);
			u8g2.print(">");
		} else {
			u8g2.print(currentTemp,1);
		}
		u8g2.sendBuffer();
	}

	// Draw initial menu with selected item (1 - Auto, 2 - Manual)
	void drawMenu_AutoManual(uint8_t sel) {
		if(sel==1){
			u8g2.drawRBox(0,4,50,24,4);
			u8g2.drawRFrame(44,4,70,24,4);
		}else{
			u8g2.drawRFrame(0,4,50,24,4);
			u8g2.drawRBox(44,4,70,24,4);
		}
		
		u8g2.setFontMode(1);  // activate transparent font mode
		u8g2.setDrawColor(2);	// Xor
		u8g2.drawStr(3,8,"Auto");
		u8g2.drawStr(47,8,"Manual");
	}

	
#endif