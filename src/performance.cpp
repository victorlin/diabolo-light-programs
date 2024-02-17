#include <Arduino.h>
#include <Diabolo_Light.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(
    Diabolo_Light::NUM_LEDS,
    Diabolo_Light::LED_PIN,
    Diabolo_Light::LED_TYPE
);

typedef uint8_t DiodeBrightness;
typedef int8_t DiodeBrightnessOffset;

struct Color {
    const DiodeBrightness r;
    const DiodeBrightness g;
    const DiodeBrightness b;

    Color(DiodeBrightness r, DiodeBrightness g, DiodeBrightness b)
        : r(r), g(g), b(b) {}

    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
};

const static Color WHITE_1   = Color(20, 20, 20 );
const static Color WHITE_3   = Color(255, 255, 255);
const static Color RED_3     = Color(255, 0, 0  );
const static Color GREEN_3   = Color(128, 255, 0  );
const static Color BLUE_3    = Color(0, 0, 255);
const static Color YELLOW_3  = Color(255, 128, 0  );
const static Color ORANGE_3  = Color(255, 32, 0  );

struct Scene {
    const Color& start;
    const Color& end;
    uint32_t duration;
    // TODO: add types of transitions. R/G/B balanced transition or R+G+B combined transition

    Scene(const Color& start, const Color& end, uint32_t duration)
        : start(start), end(end), duration(duration) {}
};

Scene get_current_scene(uint8_t scene_num) {
    switch (scene_num) {
        case 1: return Scene(WHITE_1, WHITE_1, 6683);
        case 2: return Scene(WHITE_1, YELLOW_3, 1166);
        case 3: return Scene(YELLOW_3, YELLOW_3, 36633);
        case 4: return Scene(RED_3, RED_3, 333);         //  // umbrella
        case 5: return Scene(YELLOW_3, YELLOW_3, 316);   //  // umbrella
        case 6: return Scene(RED_3, RED_3, 333);         //  // umbrella
        case 7: return Scene(YELLOW_3, YELLOW_3, 316);   //  // umbrella
        case 8: return Scene(RED_3, RED_3, 333);         //  // umbrella
        case 9: return Scene(YELLOW_3, YELLOW_3, 316);   //  // umbrella
        case 10: return Scene(RED_3, RED_3, 333);         //  // umbrella
        case 11: return Scene(YELLOW_3, YELLOW_3, 316);   //  // umbrella
        case 12: return Scene(RED_3, RED_3, 333);         //  // umbrella
        case 13: return Scene(YELLOW_3, YELLOW_3, 316);   //  // umbrella
        case 14: return Scene(RED_3, RED_3, 333);         //  // umbrella
        case 15: return Scene(YELLOW_3, YELLOW_3, 316);   //  // umbrella
        case 16: return Scene(RED_3, RED_3, 333);         //  // umbrella
        case 17: return Scene(YELLOW_3, YELLOW_3, 316);   //  // umbrella
        case 18: return Scene(RED_3, ORANGE_3, 650);      //  // umbrella
        case 19: return Scene(ORANGE_3, ORANGE_3, 21200); //  // after umbrella
        case 20: return Scene(ORANGE_3, BLUE_3, 2733);    //  // transition
        case 21: return Scene(BLUE_3, BLUE_3, 17950);     //  // around the world
        case 22: return Scene(BLUE_3, YELLOW_3, 2733);    //  // transition
        case 23: return Scene(YELLOW_3, RED_3, 1483);     //  // transition
        case 24: return Scene(RED_3, RED_3, 17850);       //  // dragon
        case 25: return Scene(RED_3, WHITE_3, 2916);      //  // transition
        case 26: return Scene(WHITE_3, WHITE_3, 251016); //
    }
}

const static uint8_t TOTAL_SCENES = 26;
uint8_t scene_num = 1;
unsigned long past_scene_durations = 0;

void set_all_pixels(uint32_t color) {
    for (unsigned int i = 0; i < Diabolo_Light::NUM_LEDS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

void setup() {
    pixels.begin();

    Diabolo_Light::begin(1, 0, [](){
        scene_num = 1;
        past_scene_durations = 0;
    });
}

void loop() {
    Diabolo_Light::handle_button();

    if (Diabolo_Light::get_current_mode() != 1) return;

    Scene scene = get_current_scene(scene_num);

    // Progress within current scene, from 0-1
    double progress =
        static_cast<double>(Diabolo_Light::awake_time() - past_scene_durations)
        / scene.duration;

    // Go to next scene
    if (progress >= 1) {
        past_scene_durations = past_scene_durations + scene.duration;
        scene_num++;

        // Turn off after last scene
        if (scene_num >= TOTAL_SCENES) {
            Diabolo_Light::set_current_mode(0);
        }
    }

    if (scene.start == scene.end) {
        set_all_pixels(pixels.Color(
            scene.start.r,
            scene.start.g,
            scene.start.b
        ));
    }
    else {
        set_all_pixels(pixels.Color(
            scene.start.r + static_cast<DiodeBrightnessOffset>((scene.end.r - scene.start.r) * progress),
            scene.start.g + static_cast<DiodeBrightnessOffset>((scene.end.g - scene.start.g) * progress),
            scene.start.b + static_cast<DiodeBrightnessOffset>((scene.end.b - scene.start.b) * progress)
        ));
    }
}
