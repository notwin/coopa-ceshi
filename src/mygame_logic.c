// src/mygame_logic.c —— 见 mygame_logic.h。
#include "mygame_logic.h"

// xorshift32:只用调用方给的种子,同一个种子出同一串(模拟器 --seed 能复现)。
static uint32_t rnd(mygame_game_t *g) {
    uint32_t x = g->rng;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    g->rng = x;
    return x;
}

static void clear(mygame_game_t *g) {
    g->who = DM_EMPTY;
    g->left_ms = DM_GAP_MS;
}

void mygame_start(mygame_game_t *g, uint32_t seed) {
    g->rng = seed ? seed : 0x9E3779B9u;
    g->score = 0;
    g->lives = DM_LIVES;
    g->row = 1;
    g->hole = 0;
    clear(g);
}

void mygame_move(mygame_game_t *g, int dir) {
    g->row = (uint8_t)((g->row + DM_ROWS + (dir < 0 ? -1 : 1)) % DM_ROWS);
}

uint32_t mygame_show_ms(uint16_t score) {
    uint32_t cut = 30u * score;
    return cut >= 1000u ? 500u : 1500u - cut;
}

bool mygame_over(const mygame_game_t *g) {
    return g->lives == 0;
}

dm_event_t mygame_swat(mygame_game_t *g) {
    if (g->who == DM_EMPTY || g->row != g->hole) return DM_MISS;
    dm_event_t e;
    if (g->who == DM_KOOPA) {
        if (g->score < DM_SCORE_MAX) g->score++;
        e = DM_HIT;
    } else {
        if (g->lives) g->lives--;
        e = DM_OOPS;
    }
    clear(g);
    return e;
}

// 每帧用减法计时(left_ms -= dt),不累加绝对时间,玩多久都不会溢出。
dm_event_t mygame_tick(mygame_game_t *g, uint32_t dt) {
    if (mygame_over(g)) return DM_NONE;
    if (dt < g->left_ms) {
        g->left_ms -= dt;
        return DM_NONE;
    }
    if (g->who == DM_EMPTY) {  // 冒出来
        g->hole = (uint8_t)(rnd(g) % DM_ROWS);
        g->who = rnd(g) % 5u == 0 ? DM_CAT : DM_KOOPA;
        g->left_ms = mygame_show_ms(g->score);
        return DM_NONE;
    }
    dm_event_t e = DM_NONE;
    if (g->who == DM_KOOPA) {  // 库巴跑了
        if (g->lives) g->lives--;
        e = DM_LOST;
    }
    clear(g);
    return e;
}
