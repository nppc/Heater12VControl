
#define OVERSAMPLENR 	16

#define MOSFET_PIN 		13
#define TEMPSENSOR_PIN 	A1
#define KNOB_PIN 		A2	// Potentsiomenter for setting the desired temperature

#include "temptable.h"
static void* heater_ttbl_map = (void*)TEMPTABLE;

// ADC=1023 - temp sensor is not connected

// mosfet (HIGH - OFF, LOW - ON)
#define H_ON 	digitalWrite(MOSFET_PIN,LOW)
#define H_OFF 	digitalWrite(MOSFET_PIN,HIGH)

void setup(){
	
	H_OFF;
	pinMode(MOSFET_PIN,OUTPUT);
	
	Serial.begin(9600);
	
	// test temoerature sensor presense
	while (analog2temp(collectADCraw(TEMPSENSOR_PIN))>999.0) {
		delay(1000);
	}
	Serial.print("Temperature Sensor: ");
	Serial.println(analog2temp(collectADCraw(TEMPSENSOR_PIN)));
	
}


void loop() {
	
	int presetTemp = map(collectADCraw(KNOB_PIN),0,1023*OVERSAMPLENR,20,300);
	float currentTemp = analog2temp(collectADCraw(TEMPSENSOR_PIN));
	
	if ((float)presetTemp>=currentTemp) {H_ON;} else {H_OFF;}
	Serial.print("Set: ");
	Serial.print(presetTemp);
	Serial.print(", Actual: ");
	Serial.println(currentTemp);
	
	delay(1000);
	
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

