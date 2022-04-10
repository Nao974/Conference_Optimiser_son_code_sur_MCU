#include <Arduino.h>

const uint8_t NB_LED = 8;

uint8_t pinLed[] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t led = 0;

uint32_t lastMillis = 0;

void setup() {

	for (led = 0; led < NB_LED; led++)
		pinMode(pinLed[led], OUTPUT);

	lastMillis = millis();
}

void loop() {

	if (millis() - lastMillis > 75) {

		digitalWrite(pinLed[led++], LOW);
		led %=  NB_LED; // Permet de repasser de la Led 7++ à la Led 0
		digitalWrite(pinLed[led],HIGH);

		lastMillis = millis();
  }

	// Lecture des Capteurs

	// Machine à état: état précedent + Capteurs => prochain état

	// Commandes des Actionneurs suivant nouvel état

}