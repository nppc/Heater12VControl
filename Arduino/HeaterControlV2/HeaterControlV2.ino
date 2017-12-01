//*********************
// * HW CONFIGURATION *
//*********************
#define DEBUG	// output the results to Serial
//#define OLED	// Use OLED display
//*********************

#include "temperature.h"
#include "encoder.h"
#include "adc.h"

#ifdef OLED
	#include <U8g2lib.h>
	#include "oled.h"
#endif



#define MOSFET_PIN 		A1
#define TEMPSENSOR_PIN 	A0
#define KNOB_PIN 		A2	// Potentiometer for setting the desired temperature

#define encoderPinA   3	// Interrupt pin (coupled with capacitor to GND)
#define encoderPinB   4 // Interrupt pin (coupled with capacitor to GND)


// ADC=1023 - temp sensor is not connected

// some macros: 
// mosfet (HIGH - OFF, LOW - ON)
#define H_ON 	digitalWrite(MOSFET_PIN,LOW)
#define H_OFF 	digitalWrite(MOSFET_PIN,HIGH)

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

	
}

void loop() {
	
	presetTemp = map(collectADCraw(KNOB_PIN),0,1023*OVERSAMPLENR,20,300);
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
