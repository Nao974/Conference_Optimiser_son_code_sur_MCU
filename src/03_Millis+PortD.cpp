#include <Arduino.h>

const uint8_t NB_LED = 8;
uint8_t led = 0;

uint32_t lastMillis = 0;

void setup() {

	DDRD  = B11111111; // Paramétrage des broches du port D en mode OUTPUT
    PORTD = B00000000; // On éteint toutes les Leds

	lastMillis = millis();
}

void loop() {

	if (millis() - lastMillis > 75) {
		/* Le port D est une image de notre chenillard, chaque bit représente une Led
			Nous décalons à chaque itération d'un bit permettant de passer à la Led suivante
			Le port D commande l'ensemble des Leds, les autres bits étant à zéro,
			Cela permet d'éteindre automatiquement la Led precedente  
		*/
		PORTD = B00000001  << led++; 
		led %=  NB_LED; // Permet de repasser de la Led 7++ à la Led 0

		lastMillis = millis();
  }

	// Lecture des Capteurs

	// Machine à état: état précedent + Capteurs => prochain état

	// Commandes des Actionneurs suivant nouvel état

}