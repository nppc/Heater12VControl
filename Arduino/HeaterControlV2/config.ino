void configureParams(){
	#ifdef DEBUG
	Serial.println(F("Enter Config"));
	#endif
	uint8_t item = 1;
	int value;
	boolean edit = false;
	printConfParam(item, value, edit);

	waitUntilButtonReleased();
	
	// wait for encoder
	char encVal = 0;  // signed value - nothing is pressed
	while (1) {
		encVal = rotaryEncRead();
		if(encVal!=127) {
			if(!edit){
				item=constrain(item + encVal,1, 8);
			} else {
				// adjust selected value
				switch (item) {
					case 1:
						pid_P = constrain(pid_P+encVal*10,0,990);
						value=pid_P;
						break;
					case 2:
						pid_I = constrain(pid_I+encVal,0,100);
						value=pid_I;
						break;
					case 3:
						pid_D = constrain(pid_D+encVal*10,0,990);
						value=pid_D;
						break;
					case 4:	// Preheat temperature
						auto_preheatTemp = constrain(auto_preheatTemp+encVal,20,300);
						value = auto_preheatTemp;
						break;
					case 5: // Preheat time in seconds
						auto_preheatTime = constrain(auto_preheatTime+encVal*10,10,990);
						value = auto_preheatTime;
						break;
					case 6: // Reflow temperature
						auto_reflowTemp = constrain(auto_reflowTemp+encVal,20,300);
						value = auto_reflowTemp;
						break;
					case 7: // Reflow time in seconds
						auto_reflowTime = constrain(auto_reflowTime+encVal*10,10,990);
						value = auto_reflowTime;
						break;
				} 
			}
			if(encVal!=0){
				printConfParam(item, value, edit);
			}
		}
		if(encVal==127){ // button is pressed
			if(item==8){
				printSavingSettings();
				store_settingsEEPROM();
				while(1){}	// reboot board
			}
			// switch edit mode
			edit=!edit;
			waitUntilButtonReleased();
		}
		WDT_Init();
	}
	#ifdef DEBUG
	Serial.println(F("Exit Config"));
	#endif

}

void printSavingSettings(){
	#ifdef OLED
	u8g2.clearBuffer();
	u8g2.setCursor(0, 12);
	u8g2.print(F("Saving..."));
	u8g2.sendBuffer();
	#endif
	
}

// item - number of configurable item
// edit - if TRUE, then highlight the edited value
void printConfParam(uint8_t item, uint16_t value, boolean edit){
	#ifdef OLED
	u8g2.clearBuffer();
	u8g2.setDrawColor(2);	//inverse mode
	if(edit && value != 8){
		u8g2.drawBox(90,6,128-90,32-6*2);
	}
	u8g2.setCursor(0, 12);
	switch (item) {
		case 1:
			u8g2.print(F("PID P:"));
			break;
		case 2:
			u8g2.print(F("PID I:"));
			break;
		case 3:
			u8g2.print(F("PID D:"));
			break;
		case 4:	// Preheat temperature
			u8g2.print(F("Preheat t:"));
			break;
		case 5: // Preheat time in seconds
			u8g2.print(F("Preheat s:"));
			break;
		case 6: // Reflow temperature
			u8g2.print(F("Reflow t:"));
			break;
		case 7: // Reflow time in seconds
			u8g2.print(F("Reflow s:"));
			break;
		case 8: // Save settings and exit
			u8g2.print(F("Save and Exit"));
			break;
	} 
	u8g2.setCursor(96, 12);
	u8g2.print(value);
	u8g2.sendBuffer();
	#endif
	#ifdef DEBUG
	Serial.println(F("Change Config"));
	Serial.print(F("item: "));Serial.println(item);
	Serial.print(F("value: "));Serial.println(value);
	Serial.print(F("edit: "));Serial.println(edit);
	#endif
	
}
