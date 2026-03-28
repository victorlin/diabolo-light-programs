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
const static Color WHITE_2   = Color(64, 64, 64 );
const static Color WHITE_3   = Color(255, 255, 255);
const static Color BLUE_1    = Color(0, 0, 20);
const static Color BLUE_2    = Color(0, 0, 64);
const static Color BLUE_3    = Color(0, 0, 255);
const static Color RED_3     = Color(255, 0, 0  );
const static Color ORANGE_3  = Color(255, 32, 0  );
const static Color OFF       = Color(0, 0, 0  );

struct Scene {
    const Color start;
    const Color end;
    uint32_t duration;
    // TODO: add types of transitions. R/G/B balanced transition or R+G+B combined transition

    Scene(const Color start, const Color end, double duration)
        : start(start), end(end), duration(duration) {}
};

const uint32_t STROBE_TOTAL_DURATION = 2235;
const uint32_t STROBE_PULSE_DURATION = 50;
const uint8_t STROBE_COUNT = STROBE_TOTAL_DURATION / STROBE_PULSE_DURATION;


Scene get_scene(uint8_t index) {
    uint8_t i = 0;

    if (index == i++) return Scene(WHITE_1, WHITE_1, 7991);
    
    // Strobe sequence
    if (index >= i && index < i + STROBE_COUNT) {
        // uncomment to debug other scenes
        // return Scene(WHITE_1, WHITE_1, 10);

        uint8_t strobe_idx = index - i;
        double progress = static_cast<double>(strobe_idx) / (STROBE_COUNT - 1);
        bool is_white = strobe_idx % 2;
        
        if (is_white) {
            DiodeBrightness c_val = 20 + static_cast<DiodeBrightness>((255 - 20) * progress);
            Color c(c_val, c_val, c_val);
            return Scene(c, c, STROBE_PULSE_DURATION);
        } else {
            DiodeBrightness b_val = 20 + static_cast<DiodeBrightness>((255 - 20) * progress);
            Color c(0, 0, b_val);
            return Scene(c, c, STROBE_PULSE_DURATION);
        }
    }
    i += STROBE_COUNT;
    
    if (index == i++) return Scene(WHITE_3, WHITE_3, 1634);
    if (index == i++) return Scene(WHITE_3, OFF, 1634);
    if (index == i++) return Scene(OFF, OFF, 14413);
    if (index == i++) return Scene(OFF, ORANGE_3, 817);
    if (index == i++) return Scene(ORANGE_3, RED_3, 817);
    if (index == i++) return Scene(RED_3, RED_3, 1000);
    
    return Scene(OFF, OFF, 0); // End of animation
}

uint8_t scene_num = 0;
unsigned long past_scene_durations = 0;
unsigned int prev_mode = 0;

void set_all_pixels(uint32_t color) {
    for (unsigned int i = 0; i < Diabolo_Light::NUM_LEDS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

void setup() {
    pixels.begin();

    Diabolo_Light::begin(2, 0, [](){
        scene_num = 0;
        past_scene_durations = 0;
    });
}

void loop() {
    Diabolo_Light::handle_button();

    unsigned int mode = Diabolo_Light::get_current_mode();

    if (mode > 2) return;

    // 2nd press - straight to red
    if (mode == 2) {
        if (mode != prev_mode) {
            set_all_pixels(pixels.Color(255, 0, 0));
            prev_mode = mode;
        }
        return;
    }
    prev_mode = mode;

    const Scene scene = get_scene(scene_num);

    // Progress within current scene, from 0-1
    double progress =
        static_cast<double>(Diabolo_Light::awake_time() - past_scene_durations)
        / scene.duration;

    // Go to next scene
    if (progress >= 1) {
        const Scene next_scene = get_scene(scene_num + 1);
        if (next_scene.duration > 0) {
            past_scene_durations = past_scene_durations + scene.duration;
            scene_num++;
            // recalculate progress for the new scene
            progress = static_cast<double>(Diabolo_Light::awake_time() - past_scene_durations)
                       / next_scene.duration;
        } else {
            // Last scene reached, hold indefinitely
            progress = 1.0;
        }
    }

    const Scene render_scene = get_scene(scene_num);

    if (render_scene.start == render_scene.end) {
        set_all_pixels(pixels.Color(
            render_scene.start.r,
            render_scene.start.g,
            render_scene.start.b
        ));
    }
    else {
        set_all_pixels(pixels.Color(
            render_scene.start.r + static_cast<DiodeBrightnessOffset>((render_scene.end.r - render_scene.start.r) * progress),
            render_scene.start.g + static_cast<DiodeBrightnessOffset>((render_scene.end.g - render_scene.start.g) * progress),
            render_scene.start.b + static_cast<DiodeBrightnessOffset>((render_scene.end.b - render_scene.start.b) * progress)
        ));
    }
}
