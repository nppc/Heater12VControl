//#define DEBUG

#define OVERSAMPLENR 	16

#define MOSFET_PIN 		A1
#define TEMPSENSOR_PIN 	A0
#define KNOB_PIN 		A2	// Potentiometer for setting the desired temperature

#include <U8g2lib.h>
#include "temptable.h"

static void* heater_ttbl_map = (void*)TEMPTABLE;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
//U8G2_SSD1306_96X16_ER_F_HW_I2C u8g2(U8G2_R0);   // EastRising 0.69"

// ADC=1023 - temp sensor is not connected

// mosfet (HIGH - OFF, LOW - ON)
#define H_ON 	digitalWrite(MOSFET_PIN,LOW)
#define H_OFF 	digitalWrite(MOSFET_PIN,HIGH)

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

bool stateHeater;
bool stateAdjustment=HIGH;	// Is temperature adjusted?
int presetTemp;
float currentTemp;
int last_presetTemp = 0;
unsigned long disp_hyst_ms = 0;
unsigned long adjust_hyst_ms = 0;
unsigned long switch_mosfet_hyst_ms = 0;
#ifdef DEBUG
unsigned long serial_mosfet_hyst_ms = 0;
#endif

void setup(){
	
	H_OFF;
	pinMode(MOSFET_PIN,OUTPUT);
	
	u8g2.begin();
	
	#ifdef DEBUG
	Serial.begin(57600);
	#endif
	
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_inb16_mr);

	// test temoerature sensor presense
	//while (analog2temp(collectADCraw(TEMPSENSOR_PIN))>999.0) {
	//	delay(1000);
	//}
	#ifdef DEBUG
	Serial.print("Temperature Sensor: ");
	Serial.println(analog2temp(collectADCraw(TEMPSENSOR_PIN)));
	#endif
	
}

void loop() {
	
	presetTemp = map(collectADCraw(KNOB_PIN),0,1023*OVERSAMPLENR,20,300);
	currentTemp = analog2temp(collectADCraw(TEMPSENSOR_PIN));

	// Switch Heater ON/OFF
	if (switch_mosfet_hyst_ms+500 < millis()) {
		// if preset temperature is near 20, consider allways OFF
		if ((float)presetTemp>=currentTemp && presetTemp>21) {
			if(!stateHeater){H_ON;stateHeater=HIGH;screenRedraw();}
		} else {
			if(stateHeater){H_OFF;stateHeater=LOW;screenRedraw();}
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
	
	// Display
	if (disp_hyst_ms+700 < millis()) {
		disp_hyst_ms=millis();
		screenRedraw();
	}	
	
	// debug 
	#ifdef DEBUG
	if (serial_mosfet_hyst_ms+1000 < millis()) {
		serial_mosfet_hyst_ms = millis();
		Serial.print("Set: ");
		Serial.print(presetTemp);
		Serial.print(", Actual: ");
		Serial.println(currentTemp);
	}	
	#endif

}

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

#define PGM_RD_W(x)   (short)pgm_read_word(&x)

int collectADCraw(uint8_t sensor) {
	int sumADC;
	sumADC = analogRead(sensor);	// dummy read
	sumADC = 0;
	for (uint8_t i = 0; i < OVERSAMPLENR; i++) {
		sumADC+=analogRead(sensor);
	}
	return sumADC;
}

float analog2temp(int raw) {
	if (raw==1023) {
		#ifdef DEBUG
		Serial.println("No Sensor!");
		#endif
		return 999.9;
	}
    float celsius = 0;
    uint8_t i;
    short(*tt)[][2] = (short(*)[][2])(heater_ttbl_map);

    for (i = 1; i < tt_len; i++) {
      if (PGM_RD_W((*tt)[i][0]) > raw) {
        celsius = PGM_RD_W((*tt)[i - 1][1]) +
                  (raw - PGM_RD_W((*tt)[i - 1][0])) *
                  (float)(PGM_RD_W((*tt)[i][1]) - PGM_RD_W((*tt)[i - 1][1])) /
                  (float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i - 1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == tt_len) celsius = PGM_RD_W((*tt)[i - 1][1]);

    return celsius;
}

