#include <Arduino.h>

const uint8_t NB_LED = 8;

uint8_t pinLed[] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t led = 0;

void setup() {

	for (led = 0; led < NB_LED; led++)
		pinMode(pinLed[led], OUTPUT);

}

void loop() {

	for (led = 0; led < NB_LED; led++) {
		digitalWrite(pinLed[led], HIGH);
		delay(75);
		digitalWrite(pinLed[led],LOW);
	}

	// Lecture des Capteurs

	// Machine à état: état précedent + Capteurs => prochain état

	// Commandes des Actionneurs suivant nouvel état
 
}