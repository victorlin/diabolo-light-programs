#include <Arduino.h>
#include <Diabolo_Light.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(
    Diabolo_Light::NUM_LEDS,
    Diabolo_Light::LED_PIN,
    Diabolo_Light::LED_TYPE
);

void set_all_pixels(uint32_t color) {
    for (unsigned int i = 0; i < Diabolo_Light::NUM_LEDS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

void setup() {
    pixels.begin();
    Diabolo_Light::begin(1, 0);
}

void loop() {
    Diabolo_Light::handle_button();
    if (Diabolo_Light::get_current_mode() != 1) return;
    set_all_pixels(pixels.Color(255, 255, 255));
}
