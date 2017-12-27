#define PGM_RD_W(x)   (short)pgm_read_word(&x)


float analog2temp(uint16_t raw) {
	if (raw==(1023 * OVERSAMPLENR)) {
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
