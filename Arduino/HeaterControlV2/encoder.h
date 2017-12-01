volatile char encoderPos = 0;	// accumulate encoder ticks in Interrupt.
volatile bool has_encoderChange = LOW;	// Indicates that Encoder has changed
unsigned long encodermillis = 0;
int encoderVal = 0;
