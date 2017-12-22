void restore_settingsEEPROM(){
	pid_P = constrain(readEEPROMint(EEPROM_PID_P),1,999);
	pid_I = constrain(readEEPROMint(EEPROM_PID_I),1,999);
	pid_D = constrain(readEEPROMint(EEPROM_PID_D),1,999);
	auto_preheatTemp = constrain(readEEPROMint(EEPROM_AUTO_PREHEAT_TEMP),20,300);
	auto_preheatTime = constrain(readEEPROMint(EEPROM_AUTO_PREHEAT_TIME),1,999);
	auto_reflowTemp = constrain(readEEPROMint(EEPROM_AUTO_REFLOW_TEMP),20,300);
	auto_reflowTime = constrain(readEEPROMint(EEPROM_AUTO_REFLOW_TIME),1,999);
}

void store_settingsEEPROM(){
	writeEEPROMint(EEPROM_PID_P,pid_P);
	writeEEPROMint(EEPROM_PID_I,pid_I);
	writeEEPROMint(EEPROM_PID_D,pid_D);
	writeEEPROMint(EEPROM_AUTO_PREHEAT_TEMP,auto_preheatTemp);
	writeEEPROMint(EEPROM_AUTO_PREHEAT_TIME,auto_preheatTime);
	writeEEPROMint(EEPROM_AUTO_REFLOW_TEMP,auto_reflowTemp);
	writeEEPROMint(EEPROM_AUTO_REFLOW_TIME,auto_reflowTime);
}

uint16_t readEEPROMint(uint8_t addr){
	return EEPROM.read(addr)+EEPROM.read(addr+1)*256;
}

void writeEEPROMint(uint8_t addr, uint16_t value){
	EEPROM.update(addr, lowByte(value));
	EEPROM.update(addr+1, highByte(value));
}

