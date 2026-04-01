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
const static Color YELLOW_3  = Color(255, 128, 0  );
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

const uint32_t STROBE_TOTAL_DURATION = 2235;
const uint32_t STROBE_PULSE_DURATION = 50;
const uint8_t STROBE_COUNT = STROBE_TOTAL_DURATION / STROBE_PULSE_DURATION;

Segment get_chidori_segment(uint8_t index) {
    uint8_t i = 0;

    if (index == i++) return Segment(WHITE_1, WHITE_1, 7991);

    // Strobe sequence
    if (index >= i && index < i + STROBE_COUNT) {
        // uncomment to debug other segments
        // return Segment(WHITE_1, WHITE_1, 10);

        uint8_t strobe_idx = index - i;
        double progress = static_cast<double>(strobe_idx) / (STROBE_COUNT - 1);
        bool is_white = strobe_idx % 2;

        if (is_white) {
            DiodeBrightness c_val = 20 + static_cast<DiodeBrightness>((255 - 20) * progress);
            Color c(c_val, c_val, c_val);
            return Segment(c, c, STROBE_PULSE_DURATION);
        } else {
            DiodeBrightness b_val = 20 + static_cast<DiodeBrightness>((255 - 20) * progress);
            Color c(0, 0, b_val);
            return Segment(c, c, STROBE_PULSE_DURATION);
        }
    }
    i += STROBE_COUNT;

    if (index == i++) return Segment(WHITE_3, WHITE_3, 13797);
    if (index == i++) return Segment(RED_3, RED_3, 4404);
    if (index == i++) return Segment(RED_3, RED_3, 103570);
    if (index == i++) return Segment(RED_3, YELLOW_3, 2286);
    if (index == i++) return Segment(YELLOW_3, YELLOW_3, 1000);

    return Segment(OFF, OFF, 0); // End of animation
}


/*!
    @brief   Advances the animation based on elapsed time and updates the LEDs.
             Segments are played in sequence; each segment interpolates between
             a start and end color over its duration. Has side effects:
             mutates segment_num, past_segment_durations.
*/
void chidori() {
    const Segment segment = get_chidori_segment(segment_num);

    // Progress within current segment, from 0-1
    double progress =
        static_cast<double>(Diabolo_Light::awake_time() - past_segment_durations)
        / segment.duration;

    // Go to next segment
    if (progress >= 1) {
        const Segment next_segment = get_chidori_segment(segment_num + 1);
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

    const Segment render_segment = get_chidori_segment(segment_num);

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
        chidori();
    }
    if (mode == 2) {
        set_all_pixels(pixels.Color(255, 0, 0));
    }
}
