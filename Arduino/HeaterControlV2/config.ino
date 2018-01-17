void configureParams(){
	#ifdef DEBUG
	Serial.println(F("Enter Config"));
	#endif
	uint8_t item = 1;
//	int value = pid_P;	// print first parameter value
	boolean edit = false;
	printConfParam(item, edit); 

	waitUntilButtonReleased();
	
	// wait for encoder
	char encVal = 0;  // signed value - nothing is pressed
	while (1) {
		encVal = rotaryEncRead();
		if(encVal!=0 && encVal!=127) {
			if(!edit){
				item=constrain(item + encVal,1, 8);
			} else {
				// adjust selected value
				switch (item) {
					case 1:
						pid_P = constrain(pid_P+encVal*10,0,990);
						break;
					case 2:
						pid_I = constrain(pid_I+encVal,0,100);
						break;
					case 3:
						pid_D = constrain(pid_D+encVal*10,0,990);
						break;
					case 4:	// Preheat temperature
						auto_preheatTemp = constrain(auto_preheatTemp+encVal,20,300);
						break;
					case 5: // Preheat time in seconds
						auto_preheatTime = constrain(auto_preheatTime+encVal*10,10,990);
						break;
					case 6: // Reflow temperature
						auto_reflowTemp = constrain(auto_reflowTemp+encVal,20,300);
						break;
					case 7: // Reflow time in seconds
						auto_reflowTime = constrain(auto_reflowTime+encVal*10,10,990);
						break;
				} 
			}
		}
		if(encVal==127){ // button is pressed
			if(item==8){
				printSavingSettings();
				store_settingsEEPROM();
				#ifdef DEBUG
				Serial.println(F("Exit Config"));
				#endif
				while(1){}	// reboot board
			}
			// switch edit mode
			edit=!edit;
			waitUntilButtonReleased();
		}
		WDT_Init();
		printConfParam(item, edit);
	}
}

void printSavingSettings(){
	#ifdef OLED
	u8g2.clearBuffer();
	u8g2.setCursor(0, 24);
	u8g2.print(F("Saving..."));
	u8g2.sendBuffer();
	#endif
	
}

// item - number of configurable item
// edit - if TRUE, then highlight the edited value
void printConfParam(uint8_t item, boolean edit){
	#ifdef OLED
	u8g2.clearBuffer();
	u8g2.setDrawColor(2);	//inverse mode
	if(edit && item != 8){
		u8g2.drawBox(95,6,128-95,32-5*2);
	}
	u8g2.setCursor(0, 24);
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
			u8g2.print(F("Warmup t"));
			break;
		case 5: // Preheat time in seconds
			u8g2.print(F("Warmup s"));
			break;
		case 6: // Reflow temperature
			u8g2.print(F("Reflow t"));
			break;
		case 7: // Reflow time in seconds
			u8g2.print(F("Reflow s"));
			break;
		case 8: // Save settings and exit
			u8g2.print(F("Save & Exit"));
			break;
	} 
	if(item!=8){
		u8g2.setCursor(95, 24);
		u8g2.print(getValueForConfigItem(item));
	}
	u8g2.sendBuffer();
	#endif
	#ifdef DEBUG
	Serial.println(F("Change Config"));
	Serial.print(F("item: "));Serial.println(item);
	Serial.print(F("value: "));Serial.println(getValueForConfigItem(item));
	Serial.print(F("edit: "));Serial.println(edit);
	#endif
	
}

uint16_t getValueForConfigItem(uint8_t item) {
	switch (item) {
		case 1:
			return pid_P;
			break;
		case 2:
			return pid_I;
			break;
		case 3:
			return pid_D;
			break;
		case 4:	// Preheat temperature
			return auto_preheatTemp;
			break;
		case 5: // Preheat time in seconds
			return auto_preheatTime;
			break;
		case 6: // Reflow temperature
			return auto_reflowTemp;
			break;
		case 7: // Reflow time in seconds
			return auto_reflowTime;
	} 
}