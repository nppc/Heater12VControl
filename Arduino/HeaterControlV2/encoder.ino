// Encoder routines. Uses one Interrupt.

void initEncoder() {    
	pinMode(encoderPinA, INPUT_PULLUP); 
    pinMode(encoderPinB, INPUT_PULLUP); 
    pinMode(BUTTON_PIN, INPUT_PULLUP); 
    attachInterrupt(1, INT_doEncoder, FALLING);  // encoder pin on interrupt 1 - pin 3
}

char rotaryEncRead() {
  noInterrupts();
  char tmp = encoderPos;
  encoderPos=0;  // reset encoder
  interrupts();
  if(!digitalRead(BUTTON_PIN)){tmp=127;} // read button
  return tmp;
}

void INT_doEncoder() {
    // If interrupts come faster than 5ms, assume it's a bounce and ignore
    if (millis() - encodermillis > 5) {
        if (!digitalRead(encoderPinB))
            encoderPos++;
        else
            encoderPos--;

		has_encoderChange = HIGH;
        }
    encodermillis = millis();
}

// determine if rotary encoder was pressed longer than 3 seconds
bool is_rotaryEncLongPress() {
	 if(digitalRead(BUTTON_PIN)){
		 // button is not pressed
		 encoderLongPressmillis=millis();
	 }else{
		 // button is pressed
		 // how long?
		 if((encoderLongPressmillis+3000)<millis()){
			 // Long press engaged
			 return true;
			 encoderLongPressmillis=millis();	// reset counter
		 }
	 }
	 return false;
}