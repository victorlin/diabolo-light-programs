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

unsigned int prev_mode = 0;

void loop() {
    Diabolo_Light::handle_button();
    unsigned int mode = Diabolo_Light::get_current_mode();

    if (mode == prev_mode) return;

    if (mode == 1) {
        set_all_pixels(pixels.Color(255, 0, 0));
    }

    prev_mode = mode;
}
