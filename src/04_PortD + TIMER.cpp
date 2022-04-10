#include <Arduino.h>

const uint8_t NB_LED = 8;
volatile uint8_t led = 0;

volatile uint8_t compteurTimer = 0;

void setup() {

	cli(); // Arrêt des interruptions

		// Paramétrage des broches du port D en mode OUTPUT
		DDRD  = B11111111; 
		PORTD = B00000000; // On éteint toutes les Leds
		
		// TIMER2: passage en mode CTC
		TCCR2A &= B11111110; // WGM20 = 0
    	TCCR2A |= B00000010; // WGM21 = 1
		TCCR2B &= B11110111; // WGM22 = 0

		// Prédiviseur à 64 => 1/16MHz / 64 => 4µs
		TCCR2B &= B1111110; //CS20 = 0
		TCCR2B |= B0000100; //CS21 = 1 , CS22 = 0

		// Autorisatiion de l'interruption sur comparateur A
		TIMSK2 = B00000010; //OCIE2A = 1 et désactivation autres interruptions du TIMER2
    	OCR2A = 250; // Initialisation du comparateur 1 pour compter 250 tics => 4µs * 250 * 125 => 75 ms

	sei(); // Relance des Interruptions



}

// Vecteur d'interruption sur comparateur A
ISR(TIMER2_COMPA_vect) {

	// 4µs * 250 * 125 => 75 ms
	if ( ++compteurTimer >= 75 ) {
		/* Le port D est une image de notre chenillard, chaque bit représente une Led
			Nous décalons à chaque itération d'un bit permettant de passer à la Led suivante
			Le port D commande l'ensemble des Leds, les autres bits étant à zéro,
			Cela permet d'éteindre automatiquement la Led precedente  
		*/
		PORTD = B00000001  << led++;
		led %=  NB_LED; // Permet de repasser de la Led 7++ à la Led 0

		compteurTimer = 0;
	}

}


void loop() {

	// Lecture des Capteurs

	// Machine à état: état précedent + Capteurs => prochain état

	// Commandes des Actionneurs suivant nouvel état

}