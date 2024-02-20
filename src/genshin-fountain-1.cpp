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

const static Color S1 = Color(255, 0  , 0  ); // red
const static Color S2 = Color(128, 0  , 255); // purple
const static Color S3 = Color(64 , 255, 0  ); // glowing green
const static Color S4 = Color(0  , 255, 0  ); // green
const static Color S5 = Color(255, 32 , 0  ); // orange
const static Color S6 = Color(255, 128, 0  ); // warm yellow
const static Color S7 = Color(0  , 0  , 255); // blue
const static Color S8 = Color(255, 0  , 255); // pink

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
        case 1:  return Scene(S1, S1, 6683); 
        case 2:  return Scene(S2, S2, 1166); 
        case 3:  return Scene(S3, S3, 20966); 
        case 4:  return Scene(S4, S4, 816);  // toss up
        case 5:  return Scene(S5, S5, 816);  // toss down
        case 6:  return Scene(S6, S6, 1016);  // before umbrella
        case 7:  return Scene(S7, S7, 816);  // toss up
        case 8:  return Scene(S8, S8, 816);  // toss down
        case 9:  return Scene(S1, S1, 933); 
        case 10: return Scene(S2, S2, 1283);  // before umbrella
        case 11: return Scene(S3, S3, 1250);  // before umbrella
        case 12: return Scene(S4, S4, 816);  // toss up
        case 13: return Scene(S5, S5, 816);  // toss down
        case 14: return Scene(S6, S6, 950);  // before umbrella
        case 15: return Scene(S7, S7, 5333);  // flower
        case 16: return Scene(S8, S8, 333);  // umbrella
        case 17: return Scene(S1, S1, 316);  // umbrella
        case 18: return Scene(S2, S2, 333);  // umbrella
        case 19: return Scene(S3, S3, 316);  // umbrella
        case 20: return Scene(S4, S4, 333);  // umbrella
        case 21: return Scene(S5, S5, 316);  // umbrella
        case 22: return Scene(S6, S6, 333);  // umbrella
        case 23: return Scene(S7, S7, 316);  // umbrella
        case 24: return Scene(S8, S8, 333);  // umbrella
        case 25: return Scene(S1, S1, 316);  // umbrella
        case 26: return Scene(S2, S2, 333);  // umbrella
        case 27: return Scene(S3, S3, 316);  // umbrella
        case 28: return Scene(S4, S4, 333);  // umbrella
        case 29: return Scene(S5, S5, 316);  // umbrella
        case 30: return Scene(S6, S6, 650);  // umbrella
        case 31: return Scene(S7, S7, 21200);  // after umbrella
        case 32: return Scene(S8, S8, 2733);  // transition
        case 33: return Scene(S1, S1, 17950);  // around the world
        case 34: return Scene(S2, S2, 2733);  // transition
        case 35: return Scene(S3, S3, 1483);  // transition
        case 36: return Scene(S4, S4, 17850);  // dragon
        case 37: return Scene(S5, S5, 2916);  // transition
        case 38: return Scene(S6, S6, 251016); 
    }
}

const static uint8_t TOTAL_SCENES = 38;
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
        if (scene_num > TOTAL_SCENES) {
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
