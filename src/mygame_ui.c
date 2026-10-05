// src/mygame_ui.c —— 见 mygame_ui.h。只用 ui_kit.h 的小工具;精灵都 ≤ 16×16,放大 3 倍。
#include "mygame_ui.h"

#include <stdio.h>

#include "mygame_texts.h"
#include "ui_kit.h"

#define ROW_Y0   70
#define ROW_H    70
#define HOLE_X   140
#define GRANNY_X 30
#define SCALE    3

static lv_obj_t *s_slipper;
static lv_obj_t *s_thing;
static lv_obj_t *s_holes[DM_ROWS];
static lv_obj_t *s_score;
static lv_obj_t *s_lives;
static lv_obj_t *s_hint;

static lv_obj_t *centered(lv_obj_t *scr, const lv_font_t *f, uint32_t color, const char *s, int y) {
    lv_obj_t *t = uk_text(scr, f, color, s);
    lv_obj_align(t, LV_ALIGN_TOP_MID, 0, y);
    return t;
}

void mygame_ui_title(uint8_t sel, uint16_t best) {
    lv_obj_t *scr = uk_screen_new(UK_WALL);
    centered(scr, UK_F36, UK_INK, DT_TITLE, 40);
    centered(scr, UK_F12, UK_INK, DT_SUB, 90);
    lv_obj_t *k = uk_sprite(scr, SPR_KID, SCALE);
    uk_spos(k, (UK_W - 16 * SCALE) / 2, 115);
    char buf[24];
    snprintf(buf, sizeof(buf), DT_BEST, (unsigned)best);
    centered(scr, UK_F12, UK_INK, buf, 175);
    lv_obj_t *a = uk_pill(scr, UK_F24, sel == 0 ? UK_RED : UK_LINE, UK_WHITE, DT_START);
    lv_obj_align(a, LV_ALIGN_TOP_MID, 0, 205);
    lv_obj_t *b = uk_pill(scr, UK_F24, sel == 1 ? UK_RED : UK_LINE, UK_WHITE, DT_BACK);
    lv_obj_align(b, LV_ALIGN_TOP_MID, 0, 250);
    uk_screen_swap(scr);
}

void mygame_ui_play(void) {
    lv_obj_t *scr = uk_screen_new(UK_WALL);
    for (int i = 0; i < DM_ROWS; i++) {
        s_holes[i] = uk_rect(scr, HOLE_X - 10, ROW_Y0 + i * ROW_H + 40, 70, 12, UK_FLOOR_D);
    }
    lv_obj_t *g = uk_sprite(scr, SPR_GRANNY, SCALE);
    uk_spos(g, GRANNY_X, ROW_Y0 + ROW_H - 4);
    s_slipper = uk_sprite(scr, SPR_SLIPPER, SCALE);
    s_thing = uk_sprite(scr, SPR_KID, SCALE);
    s_score = uk_text(scr, UK_F24, UK_INK, "");
    lv_obj_align(s_score, LV_ALIGN_TOP_LEFT, 12, 14);
    s_lives = uk_text(scr, UK_F24, UK_RED, "");
    lv_obj_align(s_lives, LV_ALIGN_TOP_RIGHT, -12, 14);
    s_hint = centered(scr, UK_F12, UK_INK, DT_HELP, UK_H - 26);
    uk_screen_swap(scr);
}

void mygame_ui_play_update(const mygame_game_t *g, uint32_t flash_ms) {
    uk_spos(s_slipper, HOLE_X - 16 * SCALE - 8, ROW_Y0 + g->row * ROW_H);
    bool up = g->who != DM_EMPTY;
    uk_show(s_thing, up);
    if (up) {
        uk_sprite_set(s_thing, g->who == DM_CAT ? SPR_MON_CAT : SPR_KID);
        uk_spos(s_thing, HOLE_X, ROW_Y0 + g->hole * ROW_H);
    }
    for (int i = 0; i < DM_ROWS; i++) {
        lv_obj_set_style_bg_color(s_holes[i], lv_color_hex(flash_ms && i == g->row ? UK_GOLD : UK_FLOOR_D), 0);
    }
    char buf[24];
    snprintf(buf, sizeof(buf), "%u", (unsigned)g->score);
    uk_set_text(s_score, buf);
    // 命数用 ASCII「+」画,不依赖字库里有没有心形
    static const char *const HEARTS[DM_LIVES + 1] = { "", "+", "++", "+++" };
    uk_set_text(s_lives, HEARTS[g->lives <= DM_LIVES ? g->lives : DM_LIVES]);
    uk_set_text(s_hint, up && g->who == DM_CAT ? DT_NO_CAT : DT_HELP);
}

void mygame_ui_over(uint16_t score, uint16_t best) {
    lv_obj_t *scr = uk_screen_new(UK_WALL);
    centered(scr, UK_F36, UK_RED, DT_OVER, 60);
    char buf[24];
    snprintf(buf, sizeof(buf), DT_SCORE, (unsigned)score);
    centered(scr, UK_F24, UK_INK, buf, 130);
    snprintf(buf, sizeof(buf), DT_BEST, (unsigned)best);
    centered(scr, UK_F24, UK_INK, buf, 170);
    centered(scr, UK_F12, UK_INK, DT_AGAIN, 240);
    uk_screen_swap(scr);
}
