#include <stdint.h>
#include <stdbool.h>
#include "../app_api.h"

#define TICK_MS 20
#define NOTE_MS 70
#define OCT_MIN (-2)
#define OCT_MAX (2)

static const app_api_t *A;
static int8_t octave;
static uint8_t prev_key;
static bool running;
static bool expert;
static bool sharp_pending;
static char note_name[12];

static const uint16_t chromatic[] = {
    262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494
};


static char *put(char *o, const char *s){
    while (*s) *o++ = *s++;
    return o;
}

static char *puti(char *o, int8_t v){
    if (v < 0) { *o++='-'; v=(int8_t)-v; }
    *o++=(char)('0'+v);
    return o;
}

static void chrome(void){
    char buf[20];
    char mode[16];

    A->display_clear();
    A->status_clear();
    A->print_inverse(expert ? "PIANO EXPERTO" : "PIANO", 2, 0, true, true, 70);
    A->draw_battery();

    if (!expert) {
        A->print_normal("1  2  3  4  5  6  7", 0, 127, 2);
        A->print_normal("DO RE MI FA SOL LA SI", 14, 127, 3);
        A->print_normal("8  9  0  *", 0, 127, 4);
        A->print_normal("DO RE MI FA", 14, 127, 5);
    } else {
        A->print_normal("1  2  3  4  5  6  7", 0, 127, 2);
        A->print_normal("DO RE MI FA SOL LA SI", 14, 127, 3);
        A->print_normal("F = SOSTENIDO", 0, 127, 4);
        A->print_normal("F+1 F+2 F+4 F+5 F+6", 14, 127, 5);
    }

    {
        char *o = put(buf, "OCT ");
        o = puti(o, octave);
        *o = '\0';
        A->print_normal(buf, 2, 50, 1);
    }

    if (expert) {
        char *m = mode;
        m = put(m, sharp_pending ? "SOSTENIDO" : "NORMAL");
        *m = '\0';
        A->print_normal(mode, 55, 127, 1);
    }

    if (note_name[0])
        A->print_normal(note_name, 94, 127, 1);
}

static void blit(void){
    A->blit_status();
    A->blit_full();
}

static void make_note_name(uint8_t semitone){
    static const char *const names[] = {
        "DO", "DO#", "RE", "RE#", "MI", "FA",
        "FA#", "SOL", "SOL#", "LA", "LA#", "SI"
    };

    note_name[0] = '\0';
    put(note_name, names[semitone]);
}

static int8_t expert_semitone(uint8_t key){
    switch (key) {
        case APP_KEY_1: return sharp_pending ? 1  : 0;
        case APP_KEY_2: return sharp_pending ? 3  : 2;
        case APP_KEY_3: return sharp_pending ? -1 : 4;  
        case APP_KEY_4: return sharp_pending ? 6  : 5;
        case APP_KEY_5: return sharp_pending ? 8  : 7;
        case APP_KEY_6: return sharp_pending ? 10 : 9;
        case APP_KEY_7: return sharp_pending ? -1 : 11;
        default: return -1;
    }
}

static int8_t normal_semitone(uint8_t key){
    switch (key) {
        case APP_KEY_1: return 0;
        case APP_KEY_2: return 2;
        case APP_KEY_3: return 4;
        case APP_KEY_4: return 5;
        case APP_KEY_5: return 7;
        case APP_KEY_6: return 9;
        case APP_KEY_7: return 11;
        case APP_KEY_8: return 12;
        case APP_KEY_9: return 14;
        case APP_KEY_0: return 16;
        case APP_KEY_STAR: return 17;
        default: return -1;
    }
}

static void play_semitone(uint8_t semitone){
    uint8_t base = (uint8_t)(semitone % 12u);
    int16_t octave_shift = octave + (int16_t)(semitone / 12u);
    uint32_t f = chromatic[base];

    while (octave_shift > 0) { f *= 2u; octave_shift--; }
    while (octave_shift < 0) { f /= 2u; octave_shift++; }
    if (f > 20000u) f = 20000u;
    if (f < 1u) f = 1u;

    make_note_name(base);
    chrome();
    blit();
    A->play_tone((uint16_t)f, NOTE_MS);
}

static void play_key(uint8_t key){
    int8_t semitone;

    if (expert)
        semitone = expert_semitone(key);
    else
        semitone = normal_semitone(key);

    if (semitone < 0 || semitone > 17) {
        if (expert) {
            note_name[0] = '-';
            note_name[1] = '\0';
            chrome();
            blit();
        }
        return;
    }

    play_semitone((uint8_t)semitone);
    if (expert) {
        sharp_pending = false;
        chrome();
        blit();
    }
}

static void handle(uint8_t key){
    int8_t dir;

    if (key == APP_KEY_EXIT) {
        running = false;
        return;
    }

    if (key == APP_KEY_MENU) {
        expert = !expert;
        sharp_pending = false;
        note_name[0] = '-';
        note_name[1] = '\0';
        chrome();
        blit();
        return;
    }

    dir = A->nav_dir(key);
    if (dir > 0) {
        if (octave < OCT_MAX) octave++;
        chrome();
        blit();
        return;
    }
    if (dir < 0) {
        if (octave > OCT_MIN) octave--;
        chrome();
        blit();
        return;
    }

    if (expert && key == APP_KEY_F) {
        sharp_pending = !sharp_pending;
        note_name[0] = sharp_pending ? '#' : '-';
        note_name[1] = '\0';
        chrome();
        blit();
        return;
    }

    if (expert) {
        if (key >= APP_KEY_1 && key <= APP_KEY_7)
            play_key(key);
    } else {
        play_key(key);
    }
}

__attribute__((section(".text.entry"),used))
void app_main(const app_api_t *api){
    A = api;
    octave = 0;
    prev_key = APP_KEY_INVALID;
    expert = false;
    sharp_pending = false;
    note_name[0] = '-';
    note_name[1] = '\0';
    running = true;

    A->backlight_on();
    chrome();
    blit();

    while (running) {
        uint8_t key = A->get_key();

        if (key != APP_KEY_INVALID && key != prev_key) {
            prev_key = key;
            A->backlight_on();
            handle(key);
        } else if (key == APP_KEY_INVALID) {
            prev_key = APP_KEY_INVALID;
        }

        A->battery_sample();
        A->delay_ms(TICK_MS);
        A->backlight_update();
    }

    A->audio_path(false);
}
