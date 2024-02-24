#include <Arduino.h>
#include <Diabolo_Light.h>
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>

Adafruit_NeoPixel pixels(
    Diabolo_Light::NUM_LEDS,
    Diabolo_Light::LED_PIN,
    Diabolo_Light::LED_TYPE
);

typedef uint8_t DiodeBrightness;
typedef int8_t DiodeBrightnessOffset;

struct Color {
    DiodeBrightness r;
    DiodeBrightness g;
    DiodeBrightness b;

    // Idk you can uncomment this if you want
    // bool operator==(const Color& other) const {
    //     return r == other.r && g == other.g && b == other.b;
    // }
};

#define WHITE_1   {20 , 20 , 20 }
#define WHITE_3   {255, 255, 255}
#define RED_3     {255, 0  , 0  }
#define GREEN_3   {128, 255, 0  }
#define BLUE_3    {0  , 64 , 255}
#define YELLOW_3  {255, 128, 0  }
#define ORANGE_3  {255, 32 , 0  }
#define BLUE_2    {0  , 128, 255}

struct Scene {
    const Color& start;
    const Color& end;
    uint32_t duration;
    // TODO: add types of transitions. R/G/B balanced transition or R+G+B combined transition
};

const static Scene scenes[] PROGMEM = {
    {WHITE_1, WHITE_1, 6683}, 
    {WHITE_1, YELLOW_3, 1166}, 
    {YELLOW_3, YELLOW_3, 20966}, 
    {YELLOW_3, BLUE_2, 816},  // toss up
    {BLUE_2, YELLOW_3, 816},  // toss down
    {YELLOW_3, YELLOW_3, 1016},  // before umbrella
    {YELLOW_3, BLUE_2, 816},  // toss up
    {BLUE_2, YELLOW_3, 816},  // toss down
    {YELLOW_3, YELLOW_3, 933}, 
    {YELLOW_3, WHITE_3, 1283},  // before umbrella
    {WHITE_3, WHITE_3, 1250},  // before umbrella
    {WHITE_3, BLUE_2, 816},  // toss up
    {BLUE_2, WHITE_3, 816},  // toss down
    {WHITE_3, WHITE_3, 950},  // before umbrella
    {WHITE_3, WHITE_3, 5333},  // flower
    {RED_3, RED_3, 333},  // umbrella
    {YELLOW_3, YELLOW_3, 316},  // umbrella
    {RED_3, RED_3, 333},  // umbrella
    {YELLOW_3, YELLOW_3, 316},  // umbrella
    {RED_3, RED_3, 333},  // umbrella
    {YELLOW_3, YELLOW_3, 316},  // umbrella
    {RED_3, RED_3, 333},  // umbrella
    {YELLOW_3, YELLOW_3, 316},  // umbrella
    {RED_3, RED_3, 333},  // umbrella
    {YELLOW_3, YELLOW_3, 316},  // umbrella
    {RED_3, RED_3, 333},  // umbrella
    {YELLOW_3, YELLOW_3, 316},  // umbrella
    {RED_3, RED_3, 333},  // umbrella
    {YELLOW_3, YELLOW_3, 316},  // umbrella
    {RED_3, ORANGE_3, 650},  // umbrella
    {ORANGE_3, ORANGE_3, 21200},  // after umbrella
    {ORANGE_3, BLUE_3, 2733},  // transition
    {BLUE_3, BLUE_3, 17950},  // around the world
    {BLUE_3, YELLOW_3, 2733},  // transition
    {YELLOW_3, RED_3, 1483},  // transition
    {RED_3, RED_3, 17850},  // dragon
    {RED_3, WHITE_3, 2916},  // transition
    {WHITE_3, WHITE_3, 251016}
};

uint8_t scene_num = 0;
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
        scene_num = 0;
        past_scene_durations = 0;
    });
}

void loop() {
    Diabolo_Light::handle_button();

    if (Diabolo_Light::get_current_mode() != 1) return;

    // This is basically just Scene scene = scenes[scene_num] but since we stored
    // scenes inside flash instead of RAM we need to access it in a special way
    Scene scene;
    memcpy_P(&scene, &scenes[scene_num], sizeof(Scene));

    // Progress within current scene, from 0-1
    double progress =
        static_cast<double>(Diabolo_Light::awake_time() - past_scene_durations)
        / scene.duration;

    // Go to next scene
    if (progress >= 1) {
        past_scene_durations = past_scene_durations + scene.duration;
        scene_num++;

        // Turn off after last scene
        if (scene_num > sizeof(scenes) / sizeof(scenes[0])) {
            Diabolo_Light::set_current_mode(0);
        }
    }

    // idk you can uncomment this if you want
    // if (scene.start == scene.end) {
    //     set_all_pixels(pixels.Color(
    //         scene.start.r,
    //         scene.start.g,
    //         scene.start.b
    //     ));
    // }
    // else {
        set_all_pixels(pixels.Color(
            scene.start.r + static_cast<DiodeBrightnessOffset>((scene.end.r - scene.start.r) * progress),
            scene.start.g + static_cast<DiodeBrightnessOffset>((scene.end.g - scene.start.g) * progress),
            scene.start.b + static_cast<DiodeBrightnessOffset>((scene.end.b - scene.start.b) * progress)
        ));
    // }
}
