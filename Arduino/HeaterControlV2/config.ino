void configureParams(){
	#ifdef DEBUG
	Serial.println("Enter Config");
	#endif
	uint8_t item = 1;
	uint16_t value;
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
						pid_P = constrain(pid_P+encVal,1,999);
						value=pid_P;
						break;
					case 2:
						pid_I = constrain(pid_I+encVal,1,999);
						value=pid_I;
						break;
					case 3:
						pid_D = constrain(pid_D+encVal,1,999);
						value=pid_D;
						break;
					case 4:	// Preheat temperature
						auto_preheatTemp = constrain(auto_preheatTemp+encVal,1,300);
						value = auto_preheatTemp;
						break;
					case 5: // Preheat time in seconds
						auto_preheatTime = constrain(auto_preheatTime+encVal,1,999);
						value = auto_preheatTime;
						break;
					case 6: // Reflow temperature
						auto_reflowTemp = constrain(auto_reflowTemp+encVal,1,300);
						value = auto_reflowTemp;
						break;
					case 7: // Reflow time in seconds
						auto_reflowTime = constrain(auto_reflowTime+encVal,1,999);
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
				store_settingsEEPROM();
				break;	// exit from procedure
				// switch edit mode
			}
			edit=!edit;
			waitUntilButtonReleased();
		}
	}
	#ifdef DEBUG
	Serial.println("Exit Config");
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
	Serial.println("Change Config");
	Serial.print("item: ");Serial.println(item);
	Serial.print("value: ");Serial.println(value);
	Serial.print("edit: ");Serial.println(edit);
	#endif
	
}
