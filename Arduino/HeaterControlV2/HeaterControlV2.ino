//*********************
// * HW CONFIGURATION *
//*********************
#define DEBUG	// output the results to Serial
#define OLED	// Use OLED display
//*********************


#include "temperature.h"
#include "encoder.h"
#include "adc.h"
#include <AutoPID.h>
#include <EEPROM.h>

#ifdef OLED
	#include <U8g2lib.h>
	#include "oled.h"
#endif



#define MOSFET_PIN 		A1
#define TEMPSENSOR_PIN 	A0
//#define KNOB_PIN 		A2	// Potentiometer for setting the desired temperature

#define encoderPinA   3	// Interrupt pin (coupled with capacitor to GND)
#define encoderPinB   4 // Interrupt pin (coupled with capacitor to GND)
#define BUTTON_PIN   12 // Pin for knob button


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


// RAMP can always be 3 deg per second



// ADC=1023 - temp sensor is not connected

// some macros: 
// mosfet (HIGH - OFF, LOW - ON)
#define H_ON 	digitalWrite(MOSFET_PIN,LOW)
#define H_OFF 	digitalWrite(MOSFET_PIN,HIGH)

uint8_t ControlType;		// 1 - Manual or 2 - Auto

bool stateHeater;
bool stateAdjustment=HIGH;	// Is temperature adjusted?
int presetTemp;
float currentTemp;
int last_presetTemp = 0;
unsigned long adjust_hyst_ms = 0;
unsigned long switch_mosfet_hyst_ms = 0;

#ifdef DEBUG
	unsigned long serial_mosfet_hyst_ms = 0;
#endif

void setup(){
	
	H_OFF;
	pinMode(MOSFET_PIN,OUTPUT);

	#ifdef DEBUG
		Serial.begin(57600);
	#endif
	
	#ifdef OLED
		u8g2.begin();
		u8g2.clearBuffer();
		u8g2.setFont(u8g2_font_inb16_mr);
		//u8g2_font_t0_22_tn - numeric font for current temperature
		//u8g2_font_profont12_tn - numeric font for preset temperature and timer
		
	#endif
	
	#ifdef DEBUG
		Serial.print("Temperature Sensor: ");
		Serial.println(analog2temp(collectADCraw(TEMPSENSOR_PIN)));
	#endif
	
	initEncoder();
	
	// main screen (choose Manual.Auto)
	ControlType = constrain(EEPROM.read(EEPROM_CONTROLTYPE), 1, 2);
	char encVal = 0;  // signed value - nothing is pressed
	while (rotaryEncRead() != 127) {
		encVal = rotaryEncRead();
		if(encVal!=127 && encVal!=0) {
			if(encVal>0){ControlType=1;}else{ControlType=2;}
			drawMenu_AutoManual(ControlType);
		}
	}
	EEPROM.update(EEPROM_CONTROLTYPE,ControlType);	// store selected COntrol Type for the next time
	
}

void loop() {
	// do the controlling of the temperature of the heater.
	// we will hold the temperature 
	
	//presetTemp = map(collectADCraw(KNOB_PIN),0,1023*OVERSAMPLENR,20,300);
	currentTemp = analog2temp(collectADCraw(TEMPSENSOR_PIN));

	// Switch Heater ON/OFF
	if (switch_mosfet_hyst_ms+500 < millis()) {
		// if preset temperature is near 20, consider allways OFF
		if ((float)presetTemp>=currentTemp && presetTemp>21) {
			if(!stateHeater){
				H_ON;
				stateHeater=HIGH;
				#ifdef OLED
					screenRedraw();
				#endif
			}
		} else {
			if(stateHeater){
				H_OFF;
				stateHeater=LOW;
				#ifdef OLED
					screenRedraw();
				#endif
			}
		}
		switch_mosfet_hyst_ms = millis();
	}	

	// Adjust temperature
	if (abs(presetTemp-last_presetTemp)>2) {
		stateAdjustment=HIGH;
		adjust_hyst_ms = millis();
		last_presetTemp = presetTemp;
	} else {
		if(adjust_hyst_ms+2000 < millis()){stateAdjustment=LOW;}
	}
	
	#ifdef OLED
		// Display
		if (disp_hyst_ms+700 < millis()) {
			disp_hyst_ms=millis();
			screenRedraw();
		}	
	#endif
	
	// debug 
	#ifdef DEBUG
	if (serial_mosfet_hyst_ms+500 < millis()) {
		serial_mosfet_hyst_ms=millis();
		/*
		serial_mosfet_hyst_ms = millis();
		Serial.print("Set: ");
		Serial.print(presetTemp);
		Serial.print(", Actual: ");
		Serial.println(currentTemp);
		*/
	}	
	#endif

	if (has_encoderChange) {
		encoderVal+=rotaryEncRead();
		has_encoderChange=LOW;
		#ifdef DEBUG
			Serial.println(encoderVal);
		#endif
	}

}
