// *** EEPROM variables ***
#define EEPROM_PID_P				 2 // int P value for PID control
#define EEPROM_PID_I				 4 // int I value for PID control
#define EEPROM_PID_D				 6 // int D value for PID control
#define EEPROM_CONTROLTYPE     		10 // byte Store temperature controlling method (AUTO or MANUAL)
#define EEPROM_MANUAL_TEMP 			12 // int Preset temperature for manual control.
#define EEPROM_AUTO_PREHEAT_TEMP	14 // int Preheat temperature in Auto mode
#define EEPROM_AUTO_REFLOW_TEMP		16 // int Reflow temperature in Auto mode
#define EEPROM_AUTO_PREHEAT_TIME	18 // byte Seconds for Preheat stage in Auto mode
#define EEPROM_AUTO_REFLOW_TIME		20 // byte Seconds for Reflow stage in Auto mode