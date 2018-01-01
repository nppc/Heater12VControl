void restore_settingsEEPROM(){
	pid_P = constrain(readEEPROMint(EEPROM_PID_P),0,990);
	pid_I = constrain(readEEPROMint(EEPROM_PID_I),0,100);
	pid_D = constrain(readEEPROMint(EEPROM_PID_D),0,990);
	manual_temp = constrain(readEEPROMint(EEPROM_MANUAL_TEMP),1,300);
	auto_preheatTemp = constrain(readEEPROMint(EEPROM_AUTO_PREHEAT_TEMP),20,300);
	auto_preheatTime = constrain(readEEPROMint(EEPROM_AUTO_PREHEAT_TIME),1,999);
	auto_reflowTemp = constrain(readEEPROMint(EEPROM_AUTO_REFLOW_TEMP),20,300);
	auto_reflowTime = constrain(readEEPROMint(EEPROM_AUTO_REFLOW_TIME),1,999);
	ControlType = constrain(EEPROM.read(EEPROM_CONTROLTYPE), 1, 2);

	#ifdef DEBUG
		Serial.println("Restore EEPROM Settings");
		Serial.print("pid_P: ");Serial.println(pid_P);
		Serial.print("pid_I: ");Serial.println(pid_I);
		Serial.print("pid_D: ");Serial.println(pid_D);
		Serial.print("manual_temp: ");Serial.println(manual_temp);
		Serial.print("auto_preheatTemp: ");Serial.println(auto_preheatTemp);
		Serial.print("auto_preheatTime: ");Serial.println(auto_preheatTime);
		Serial.print("auto_reflowTemp: ");Serial.println(auto_reflowTemp);
		Serial.print("auto_reflowTime: ");Serial.println(auto_reflowTime);
		Serial.print("ControlType: ");Serial.println(ControlType);
	#endif
	
}

void store_settingsEEPROM(){
	#ifdef DEBUG
		Serial.println("Write EEPROM Settings");
		Serial.print("pid_P: ");Serial.println(pid_P);
		Serial.print("pid_I: ");Serial.println(pid_I);
		Serial.print("pid_D: ");Serial.println(pid_D);
		//Serial.print("manual_temp: ");Serial.println(manual_temp);
		Serial.print("auto_preheatTemp: ");Serial.println(auto_preheatTemp);
		Serial.print("auto_preheatTime: ");Serial.println(auto_preheatTime);
		Serial.print("auto_reflowTemp: ");Serial.println(auto_reflowTemp);
		Serial.print("auto_reflowTime: ");Serial.println(auto_reflowTime);
		//Serial.print("ControlType: ");Serial.println(ControlType);
	#endif

	writeEEPROMint(EEPROM_PID_P,pid_P);
	writeEEPROMint(EEPROM_PID_I,pid_I);
	writeEEPROMint(EEPROM_PID_D,pid_D);
	//writeEEPROMint(EEPROM_MANUAL_TEMP,manual_temp); // don't need to write it while it written after manual mode exit
	writeEEPROMint(EEPROM_AUTO_PREHEAT_TEMP,auto_preheatTemp);
	writeEEPROMint(EEPROM_AUTO_PREHEAT_TIME,auto_preheatTime);
	writeEEPROMint(EEPROM_AUTO_REFLOW_TEMP,auto_reflowTemp);
	writeEEPROMint(EEPROM_AUTO_REFLOW_TIME,auto_reflowTime);
	//EEPROM.update(EEPROM_CONTROLTYPE, ControlType); // don't need to write it while it written already
}

uint16_t readEEPROMint(uint8_t addr){
	return EEPROM.read(addr)+EEPROM.read(addr+1)*256;
}

void writeEEPROMint(uint8_t addr, uint16_t value){
	EEPROM.update(addr, lowByte(value));
	EEPROM.update(addr+1, highByte(value));
}

