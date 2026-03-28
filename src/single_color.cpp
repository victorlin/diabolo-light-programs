#include <Arduino.h>
#include <Diabolo_Light.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(
    Diabolo_Light::NUM_LEDS,
    Diabolo_Light::LED_PIN,
    Diabolo_Light::LED_TYPE
);

uint32_t prev_color = UINT32_MAX; // "null"

void on_wake_up() {
    prev_color = UINT32_MAX;
}

void set_all_pixels(uint32_t color) {
    if (color == prev_color) return;
    for (unsigned int i = 0; i < Diabolo_Light::NUM_LEDS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
    prev_color = color;
}


void setup() {
    pixels.begin();
    Diabolo_Light::begin(1, 0, on_wake_up);
}

void loop() {
    Diabolo_Light::handle_button();
    unsigned int mode = Diabolo_Light::get_current_mode();

    if (mode == 1) {
        set_all_pixels(pixels.Color(255, 0, 0));
    }
}
