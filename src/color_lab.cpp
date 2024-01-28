#include <Arduino.h>
#include <Diabolo_Light.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(
    Diabolo_Light::NUM_LEDS,
    Diabolo_Light::LED_PIN,
    Diabolo_Light::LED_TYPE
);

typedef uint32_t Color;

static Color colors[] = {
    pixels.Color(255, 0  , 0  ), // red
    pixels.Color(255, 64 , 0  ), // orange
    pixels.Color(255, 128, 0  ), // warm yellow
    pixels.Color(255, 192, 0  ), // yellow-green
    pixels.Color(255, 255, 0  ), // light green
    pixels.Color(192, 255, 0  ), // green
    pixels.Color(128, 255, 0  ), // green
    pixels.Color(64 , 255, 0  ), // glowing green
    pixels.Color(0  , 255, 0  ), // glowing green
    pixels.Color(0  , 255, 64 ), // cyan
    pixels.Color(0  , 255, 128), // sky blue
    pixels.Color(0  , 255, 192), // neon blue
    pixels.Color(0  , 255, 255), // neon blue
    pixels.Color(0  , 192, 255), // blue
    pixels.Color(0  , 128, 255), // blue
    pixels.Color(0  , 64 , 255), // blue
    pixels.Color(0  , 0  , 255), // blue
    pixels.Color(64 , 0  , 255), // purple
    pixels.Color(128, 0  , 255), // purple
    pixels.Color(192, 0  , 255), // purple-pink
    pixels.Color(255, 0  , 255), // pink
    pixels.Color(255, 0  , 192), // pink
    pixels.Color(255, 0  , 128), // pink
    pixels.Color(255, 0  , 64 ), // hot pink
};

void show_color(Color color) {
    for (unsigned int i = 0; i < Diabolo_Light::NUM_LEDS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

void setup() {
    pixels.begin();
    unsigned int num_colors = sizeof(colors) / sizeof(colors[0]);
    Diabolo_Light::begin(num_colors, 0);
}

void loop() {
    Diabolo_Light::handle_button();
    unsigned int mode = Diabolo_Light::get_current_mode();
    if (mode > 0) {
        show_color(colors[mode - 1]);
    }
}
