

#define OVERSAMPLENR 	16

#define MOSFET_PIN 		A1
#define TEMPSENSOR_PIN 	A0
#define KNOB_PIN 		A2	// Potentsiomenter for setting the desired temperature

#include <U8g2lib.h>
#include "temptable.h"

static void* heater_ttbl_map = (void*)TEMPTABLE;

U8G2_SSD1306_96X16_ER_F_HW_I2C u8g2(U8G2_R0);   // EastRising 0.69"


// ADC=1023 - temp sensor is not connected

// mosfet (HIGH - OFF, LOW - ON)
#define H_ON 	digitalWrite(MOSFET_PIN,LOW)
#define H_OFF 	digitalWrite(MOSFET_PIN,HIGH)

void setup(){
	
	H_OFF;
	pinMode(MOSFET_PIN,OUTPUT);
	
	u8g2.begin();
	
	Serial.begin(57600);
	
	// test temoerature sensor presense
	while (analog2temp(collectADCraw(TEMPSENSOR_PIN))>999.0) {
		delay(1000);
	}
	Serial.print("Temperature Sensor: ");
	Serial.println(analog2temp(collectADCraw(TEMPSENSOR_PIN)));
	
}

int last_presetTemp = 0;
unsigned long disp_hyst_ms = 0;
unsigned long adjust_hyst_ms = 0;
unsigned long switch_mosfet_hyst_ms = 0;
unsigned long serial_mosfet_hyst_ms = 0;

void loop() {
	
	int presetTemp = map(collectADCraw(KNOB_PIN),0,1023*OVERSAMPLENR,20,300);
	float currentTemp = analog2temp(collectADCraw(TEMPSENSOR_PIN));

	if (switch_mosfet_hyst_ms+500 < millis()) {
		// if preset temperature is near 20, consider allways OFF
		if ((float)presetTemp>=currentTemp && presetTemp>21) {H_ON;} else {H_OFF;}
		switch_mosfet_hyst_ms = millis();
	}	

	if (disp_hyst_ms+700 < millis()) {
		disp_hyst_ms=millis();
		u8g2.clearBuffer();
		u8g2.setFont(u8g2_font_inb16_mr);
		u8g2.setCursor(0, 15);
		if (presetTemp < (last_presetTemp-2) || presetTemp > (last_presetTemp+2)) { // add small hysterysis
			// value is adjusted
			last_presetTemp = presetTemp;
			adjust_hyst_ms = millis();
			u8g2.print("<");
			u8g2.print(presetTemp);
			u8g2.print(">");
		} else if(adjust_hyst_ms+2000 < millis()){
			u8g2.print(currentTemp);
		}
		u8g2.sendBuffer();
	}	
	
	// debug 
	if (serial_mosfet_hyst_ms+1000 < millis()) {
		serial_mosfet_hyst_ms = millis();
		Serial.print("Set: ");
		Serial.print(presetTemp);
		Serial.print(", Actual: ");
		Serial.println(currentTemp);
	}	

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
		Serial.println("No Sensor!");
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

