#ifdef NTC
	#define filterSamples   25
#else
	#define filterSamples   9
#endif
uint16_t BSmoothArray[filterSamples];