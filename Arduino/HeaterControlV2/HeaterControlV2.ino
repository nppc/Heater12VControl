//*********************
// * HW CONFIGURATION *
//*********************
#define DEBUG	// output the results to Serial
//#define OLED	// Use OLED display
#define LOGGER
//*********************


#include "temperature.h"
#include "encoder.h"
#include "adc.h"
#include <PID_v1.h>
#include <EEPROM.h>
#include "eeprom.h"
#include <avr/wdt.h>

#ifdef OLED
	#include <U8g2lib.h>
	#include "oled.h"
#endif



#define MOSFET_PIN 		6
#define LED_PIN 		13
#define TEMPSENSOR_PIN 	A0

#define encoderPinA   3	// Interrupt pin (coupled with capacitor to GND)
#define encoderPinB   4 // Interrupt pin (coupled with capacitor to GND)
#define BUTTON_PIN   12 // Pin for knob button



// RAMP can always be 3 deg per second



// ADC=1023 - temp sensor is not connected

// some macros: 
// mosfet (HIGH - OFF, LOW - ON)
#define H_ON 	digitalWrite(MOSFET_PIN,HIGH);digitalWrite(LED_PIN, LOW);
#define H_OFF 	digitalWrite(MOSFET_PIN,LOW);digitalWrite(LED_PIN, HIGH);

uint8_t ControlType;		// 1 - Manual or 2 - Auto
uint8_t ProcessStage;		// 0 - just hold manual temperature; 1 - Ramp to Preheat; 2 - Preheat; 3 - Ramp to Reflow; 4 - Reflow; 5 - Cool down; 255 - finished

// PID global variables
#define PID_WINDOWSIZE 500	// upper limit of PID output
#define PID_VALUES_FACTOR 10.0	//PID values are stored in EEPROM in int format. So, scale them (div/mult) before use.

double currentTemp=0;
double setPoint=0;
double outputVal;

int pid_P, pid_I, pid_D; // PID values
int auto_preheatTemp, auto_preheatTime, auto_reflowTemp, auto_reflowTime, manual_temp;

unsigned long soft_pwm_millis=0;

//input/output variables passed by reference, so they are updated automatically
PID myPID(&currentTemp, &outputVal, &setPoint, 0, 0, 0, DIRECT); // PID values will be set later

unsigned long timer_millis;
uint16_t timer_seconds;
boolean timer_active=false;


#ifdef DEBUG
	unsigned long serial_mosfet_hyst_ms = 0;
#endif

void setup(){
	
	wdt_disable();
	H_OFF
	pinMode(MOSFET_PIN,OUTPUT);
	pinMode(LED_PIN,OUTPUT);
	digitalWrite(LED_PIN, LOW);

	#ifdef DEBUG
		Serial.begin(115200);
	#endif
	
	#ifdef OLED
		u8g2.begin();
		//u8g2.clearBuffer();
		//u8g2.setFont(u8g2_font_inb16_mr);
		//u8g2_font_t0_22_tn - numeric font for current temperature
		//u8g2_font_profont12_tn - numeric font for preset temperature and timer
		
	#endif
	
	#ifdef DEBUG
		//initialize ADC smoother array for Debug output here...
		for(uint8_t i=0;i<filterSamples*2;i++){digitalSmooth(collectADCraw(TEMPSENSOR_PIN), BSmoothArray);}
		Serial.print("Temperature Sensor: ");
		Serial.println(analog2temp(digitalSmooth(collectADCraw(TEMPSENSOR_PIN), BSmoothArray)));
	#endif
	
		// read PID values from EEPROM
	restore_settingsEEPROM();
	// we use FACTOR for PID values to get rid of comas in interface.
	myPID.SetTunings((float)pid_P / PID_VALUES_FACTOR,(float)pid_I / PID_VALUES_FACTOR,(float)pid_D / PID_VALUES_FACTOR);
	myPID.SetOutputLimits(0, PID_WINDOWSIZE);	//set PID output range

	initEncoder();
		
	waitUntilButtonReleased(); // in case it was

	// main screen (choose Manual/Auto)
	#ifdef OLED
	drawMenu_AutoManual(ControlType);
	#endif
	#ifdef DEBUG
	Serial.print("ControlType is: ");Serial.println(ControlType);
	#endif
	char encVal = 0;  // signed value - nothing is pressed
	while (encVal != 127) {
		encVal = rotaryEncRead();
		if(encVal!=127 && encVal!=0) {
			if(encVal>0){ControlType=1;}else{ControlType=2;}
			#ifdef OLED
			drawMenu_AutoManual(ControlType);
			#endif
			#ifdef DEBUG
			Serial.print("ControlType is: ");Serial.println(ControlType);
			#endif
		}
		digitalSmooth(collectADCraw(TEMPSENSOR_PIN), BSmoothArray); // refresh ADC array to have fresh values there
	}
	EEPROM.update(EEPROM_CONTROLTYPE,ControlType);	// store selected Control Type for the next time
	#ifdef OLED
	u8g2.clearDisplay();
	#endif
	// wait until button released
	encoderLongPressmillis=millis();
	while(rotaryEncRead() == 127){
		if(is_rotaryEncLongPress()){
			// go to config menu
			configureParams();
			encoderLongPressmillis=millis();
		}
	}	

	if(ControlType==1){
		ProcessStage=0; // hold temperature
		setPoint=manual_temp;	// last saved temperature.
		#ifdef DEBUG
		Serial.println("Start Manual mode");
		#endif
	}else{
		ProcessStage=1; // ramp to preheat temperature
		//presetTemp = readEEPROMint(EEPROM_AUTO_PREHEAT_TEMP);
		#ifdef DEBUG
		Serial.println("Start Automatic mode");
		#endif
	}
	
	myPID.SetMode(AUTOMATIC); // turn on PID
	
	timer_seconds=0;
		
}

void loop() {
	
	WDT_Init();	// keep system alive
	
	if(ControlType==1){
		doManualReflow();
	}else{
		doAutoReflow();
	}

	uint16_t smoothADC = digitalSmooth(collectADCraw(TEMPSENSOR_PIN), BSmoothArray);
	currentTemp = analog2temp(smoothADC);
	// use PID only when difference is small (to prevent windup of I)
	if(abs(setPoint-currentTemp)<10){
		myPID.Compute();
	}else{
		outputVal = (setPoint > currentTemp ? PID_WINDOWSIZE : 0); // ON/OFF control
	}
	doSoftwarePWM((uint16_t)outputVal);	// make slow PWM to prevent unnecessary mosfet heating 
	
	// debug 
	#ifdef DEBUG
	if (serial_mosfet_hyst_ms+1000 < millis()) {
		serial_mosfet_hyst_ms=millis();
		serial_mosfet_hyst_ms = millis();
		Serial.print("Set: ");Serial.print(setPoint);
		Serial.print(", Actual: ");Serial.print(currentTemp);
		Serial.print(", PWM: ");Serial.print(outputVal);
		Serial.print(", Timer: ");Serial.println(timer_seconds);
	}	
	#endif	
	
	// increment timer
	if(timer_millis+1000<millis()) {
		uint16_t seconds_passed=(millis()-timer_millis)/1000;
		timer_millis=millis();
		if(timer_active){timer_seconds+=seconds_passed;}
	}
	#ifdef OLED
	printHeaterState(); //print icon of the heater ON/OFF state
	#endif

}


// PWM output 
void doSoftwarePWM(uint16_t pwm_val){
	if (millis() - soft_pwm_millis > PID_WINDOWSIZE)
	  { //time to shift the Relay Window
		soft_pwm_millis += PID_WINDOWSIZE;
	  }
	if (pwm_val < millis() - soft_pwm_millis){H_OFF}else{H_ON}
		
}

void doManualReflow(){
	// Exit from manual state by holding button for 2 seconds...
	// Adjust preset temperature by rotating the knob
	char encVal = 127;  // signed value - just enter to the loop
	while (encVal == 127) { //loop here while button is pressed (waiting longer than 2 seconds will reset the board (Exit to the init menu).
		encVal = rotaryEncRead();
		if(encVal!=127 && encVal!=0) {
			manual_temp = constrain(manual_temp+encVal,20,300);
			setPoint = manual_temp;
			#ifdef DEBUG
			Serial.print("manual_temp changed to: ");Serial.println(manual_temp);
			#endif
		}else if(encVal==127){
			H_OFF	// turn off heater, because we will freeze here for some time...
			// if button is pressed - store manual temp setting
			writeEEPROMint(EEPROM_MANUAL_TEMP,manual_temp);
			#ifdef DEBUG
			Serial.print("Store manual_temp to EEPROM: ");Serial.println(manual_temp);
			#endif
			waitUntilButtonReleased();

		}
	}
	// draw screen
	#if defined (OLED) && !defined (LOGGER)
	u8g2.clearBuffer();
	printManual();
	printPresetTemperature();
	printTime(timer_seconds);
	printCurrentTemperature();
	u8g2.sendBuffer();
	#endif
	#ifdef LOGGER
	logger128secOLED();
	#endif
	
	if(currentTemp>=(setPoint-5)){timer_active=true;} else {timer_active=false;}	// Timer running if temperature near or reached preset temp.
}

void doAutoReflow(){
	#ifdef OLED
	u8g2.clearBuffer();

	printPresetTemperature();
	printTime(timer_seconds);
	printCurrentTemperature();

	u8g2.sendBuffer();
	#endif
}
