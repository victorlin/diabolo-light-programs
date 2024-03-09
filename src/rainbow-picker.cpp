// Inspired by ChaosNuggets's Diabolo-Lights-Rainbow:
// <https://github.com/ChaosNuggets/Diabolo-Lights-Rainbow/blob/3b8d74af665ea307387e30cf8260938542734877/src/main.cpp>

#include <Arduino.h>
#include <Diabolo_Light.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(
    Diabolo_Light::NUM_LEDS,
    Diabolo_Light::LED_PIN,
    Diabolo_Light::LED_TYPE
);
uint16_t hue;

void set_all_pixels(uint32_t color) {
    for (unsigned int i = 0; i < Diabolo_Light::NUM_LEDS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

void setup() {
    pixels.begin();
    Diabolo_Light::begin(
        // num_modes. set to maximum possible value
        255,

        // time_to_turn_on
        0,

        // on_wake_up
        [](){ hue = -1000; },

        // time_to_turn_off
        1000
    );
}

void loop() {
    Diabolo_Light::handle_button();
    int mode = Diabolo_Light::get_current_mode();

    if (mode %  2 == 1) {
        set_all_pixels(pixels.ColorHSV(hue, 255, 255));
        hue += 3;
    }
}
