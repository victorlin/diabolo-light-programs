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
const static Color GOLDEN  = Color(255, 64, 0  );
const static Color OFF       = Color(0, 0, 0  );

uint8_t segment_num = 0;
unsigned long past_segment_durations = 0;
uint32_t prev_color = UINT32_MAX; // "null"

void on_wake_up() {
    segment_num = 0;
    past_segment_durations = 0;
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


struct Segment {
    const Color start;
    const Color end;
    uint32_t duration;
    // TODO: add types of transitions. R/G/B balanced transition or R+G+B combined transition

    Segment(const Color start, const Color end, double duration)
        : start(start), end(end), duration(duration) {}
};

Segment get_sequence(uint8_t index) {
    uint8_t i = 0;

    if (index == i++) return Segment(WHITE_1, WHITE_1, 6600);
    if (index == i++) return Segment(WHITE_1, GOLDEN, 1166);
    if (index == i++) return Segment(GOLDEN, GOLDEN, 1000);

    return Segment(OFF, OFF, 0); // End of animation
}


/*!
    @brief   Advances the animation based on elapsed time and updates the LEDs.
             Segments are played in sequence; each segment interpolates between
             a start and end color over its duration. Has side effects:
             mutates segment_num, past_segment_durations.
*/
void run() {
    const Segment segment = get_sequence(segment_num);

    // Progress within current segment, from 0-1
    double progress =
        static_cast<double>(Diabolo_Light::awake_time() - past_segment_durations)
        / segment.duration;

    // Go to next segment
    if (progress >= 1) {
        const Segment next_segment = get_sequence(segment_num + 1);
        if (next_segment.duration > 0) {
            past_segment_durations = past_segment_durations + segment.duration;
            segment_num++;
            // recalculate progress for the new segment
            progress = static_cast<double>(Diabolo_Light::awake_time() - past_segment_durations)
                       / next_segment.duration;
        } else {
            // Last segment reached
            if (segment.end == OFF) {
                Diabolo_Light::set_current_mode(0);
                return;
            }
            // Hold indefinitely
            progress = 1.0;
        }
    }

    const Segment render_segment = get_sequence(segment_num);

    uint32_t color;
    if (render_segment.start == render_segment.end) {
        color = pixels.Color(
            render_segment.start.r,
            render_segment.start.g,
            render_segment.start.b
        );
    }
    else {
        color = pixels.Color(
            render_segment.start.r + static_cast<DiodeBrightnessOffset>((render_segment.end.r - render_segment.start.r) * progress),
            render_segment.start.g + static_cast<DiodeBrightnessOffset>((render_segment.end.g - render_segment.start.g) * progress),
            render_segment.start.b + static_cast<DiodeBrightnessOffset>((render_segment.end.b - render_segment.start.b) * progress)
        );
    }
    set_all_pixels(color);
}

void setup() {
    pixels.begin();
    unsigned int num_modes = 2;
    Diabolo_Light::begin(num_modes, 0, on_wake_up);
}

void loop() {
    Diabolo_Light::handle_button();
    unsigned int mode = Diabolo_Light::get_current_mode();

    if (mode == 1) {
        run();
    }
    if (mode == 2) {
        set_all_pixels(pixels.Color(255, 64, 0));
    }
}
