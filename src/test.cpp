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
const static Color S3 = Color(255, 32 , 0  ); // orange
const static Color S4 = Color(0  , 255, 0  ); // green
const static Color S5 = Color(0  , 0  , 255); // blue
const static Color S6 = Color(255, 128, 0  ); // warm yellow
const static Color S7 = Color(64 , 255, 0  ); // glowing green
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
        case 1: return Scene(S1, S1, 101);
        case 2: return Scene(S1, S1, 102);
        case 3: return Scene(S1, S1, 103);
        case 4: return Scene(S1, S1, 104);
        case 5: return Scene(S1, S1, 105);
        case 6: return Scene(S1, S1, 106);
        case 7: return Scene(S1, S1, 107);
        case 8: return Scene(S1, S1, 108);
        case 9: return Scene(S1, S1, 109);
        case 10: return Scene(S1, S1, 110);
        case 11: return Scene(S1, S1, 111);
        case 12: return Scene(S1, S1, 112);
        case 13: return Scene(S2, S2, 113);
        case 14: return Scene(S2, S2, 114);
        case 15: return Scene(S2, S2, 115);
        case 16: return Scene(S2, S2, 116);
        case 17: return Scene(S2, S2, 117);
        case 18: return Scene(S2, S2, 118);
        case 19: return Scene(S2, S2, 119);
        case 20: return Scene(S2, S2, 120);
        case 21: return Scene(S2, S2, 121);
        case 22: return Scene(S2, S2, 122);
        case 23: return Scene(S2, S2, 123);
        case 24: return Scene(S2, S2, 124);
        case 25: return Scene(S3, S3, 125);
        case 26: return Scene(S3, S3, 126);
        case 27: return Scene(S3, S3, 127);
        case 28: return Scene(S3, S3, 128);
        case 29: return Scene(S3, S3, 129);
        case 30: return Scene(S3, S3, 130);
        case 31: return Scene(S3, S3, 131);
        case 32: return Scene(S3, S3, 132);
        case 33: return Scene(S3, S3, 133);
        case 34: return Scene(S3, S3, 134);
        case 35: return Scene(S3, S3, 135);
        case 36: return Scene(S3, S3, 136);
        case 37: return Scene(S3, S3, 137);
        case 38: return Scene(S4, S4, 138);
        case 39: return Scene(S4, S4, 139);
        case 40: return Scene(S4, S4, 140);
        case 41: return Scene(S4, S4, 141);
        case 42: return Scene(S4, S4, 142);
        case 43: return Scene(S4, S4, 143);
        case 44: return Scene(S4, S4, 144);
        case 45: return Scene(S4, S4, 145);
        case 46: return Scene(S4, S4, 146);
        case 47: return Scene(S4, S4, 147);
        case 48: return Scene(S4, S4, 148);
        case 49: return Scene(S4, S4, 149);
        case 50: return Scene(S5, S5, 150);
        case 51: return Scene(S5, S5, 151);
        case 52: return Scene(S5, S5, 152);
        case 53: return Scene(S5, S5, 153);
        case 54: return Scene(S5, S5, 154);
        case 55: return Scene(S5, S5, 155);
        case 56: return Scene(S5, S5, 156);
        case 57: return Scene(S5, S5, 157);
        case 58: return Scene(S5, S5, 158);
        case 59: return Scene(S5, S5, 159);
        case 60: return Scene(S5, S5, 160);
        case 61: return Scene(S5, S5, 161);
        case 62: return Scene(S5, S5, 162);
        case 63: return Scene(S6, S6, 163);
        case 64: return Scene(S6, S6, 164);
        case 65: return Scene(S6, S6, 165);
        case 66: return Scene(S6, S6, 166);
        case 67: return Scene(S6, S6, 167);
        case 68: return Scene(S6, S6, 168);
        case 69: return Scene(S6, S6, 169);
        case 70: return Scene(S6, S6, 170);
        case 71: return Scene(S6, S6, 171);
        case 72: return Scene(S6, S6, 172);
        case 73: return Scene(S6, S6, 173);
        case 74: return Scene(S6, S6, 174);
        case 75: return Scene(S7, S7, 175);
        case 76: return Scene(S7, S7, 176);
        case 77: return Scene(S7, S7, 177);
        case 78: return Scene(S7, S7, 178);
        case 79: return Scene(S7, S7, 179);
        case 80: return Scene(S7, S7, 180);
        case 81: return Scene(S7, S7, 181);
        case 82: return Scene(S7, S7, 182);
        case 83: return Scene(S7, S7, 183);
        case 84: return Scene(S7, S7, 184);
        case 85: return Scene(S7, S7, 185);
        case 86: return Scene(S7, S7, 186);
        case 87: return Scene(S7, S7, 187);
        case 88: return Scene(S8, S8, 188);
        case 89: return Scene(S8, S8, 189);
        case 90: return Scene(S8, S8, 190);
        case 91: return Scene(S8, S8, 191);
        case 92: return Scene(S8, S8, 192);
        case 93: return Scene(S8, S8, 193);
        case 94: return Scene(S8, S8, 194);
        case 95: return Scene(S8, S8, 195);
        case 96: return Scene(S8, S8, 196);
        case 97: return Scene(S8, S8, 197);
        case 98: return Scene(S8, S8, 198);
        case 99: return Scene(S8, S8, 199);
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
