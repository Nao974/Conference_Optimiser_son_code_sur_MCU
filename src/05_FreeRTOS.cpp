#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const uint8_t NUM_PIXELS= 1;         // Nombre de NeoPixel
Adafruit_NeoPixel pixel(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void taskLedBlink(void *pvParameters) {
    bool state = false;
    while (true) {
        state = !state;
        state ? pixel.setPixelColor(0, pixel.Color(0, 50, 0)) : pixel.setPixelColor(0, 0);
        pixel.show();
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

void taskSerialPrint(void *pvParameters) {
    uint32_t lastMicros= micros();
    while (true) {
        Serial.printf("Temps écoulé : %lu µs\n", micros() - lastMicros);
        lastMicros = micros();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second

    }
}

void setup() {
    Serial.begin(9600);
    delay(500);

    pixel.begin();
    pixel.show();

    Serial.println("Starting FreeRTOS tasks SerialPrint\n");
    xTaskCreate(
        taskSerialPrint,   // Fonction de la tâche
        "taskSerialPrint", // Nom
        2048,              // Stack size
        NULL,              // Paramètres
        1,                 // Priorité
        NULL               // Handle
    );

    Serial.println("Starting FreeRTOS tasks LedBlink\n");
    xTaskCreate(
        taskLedBlink,    // Fonction de la tâche
        "taskLedBlink",  // Nom
        2048,            // Stack size
        NULL,            // Paramètres
        2,               // Priorité
        NULL             // Handle
    );
}

void loop() {

	// Lecture des Capteurs

	// Machine à état: état précedent + Capteurs => prochain état

	// Commandes des Actionneurs suivant nouvel état

}