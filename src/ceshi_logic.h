// src/ceshi_logic.h —— 「拍拖鞋」玩法(纯 C,不碰 LVGL / NVS,主机可测)。
// 三行洞,库巴(80%)或猫(20%)从某一行冒出来;▲▼ 移动拖鞋,● 拍。
// 拍到库巴 +1;拍到猫丢命;库巴停留超时跑掉也丢命;拍空不罚。3 条命。
#pragma once

#include <stdbool.h>
#include <stdint.h>

#define DM_ROWS 3
#define DM_LIVES 3
#define DM_GAP_MS 400         // 洞清空后多久冒下一个
#define DM_SCORE_MAX 9999

typedef enum { DM_EMPTY = 0, DM_KOOPA, DM_CAT } dm_who_t;
typedef enum { DM_NONE = 0, DM_HIT, DM_OOPS, DM_MISS, DM_LOST } dm_event_t;

typedef struct {
    uint32_t rng;        // xorshift32 状态,不能为 0
    uint16_t score;
    uint8_t lives;       // 0 = 结束
    uint8_t row;         // 拖鞋光标 0..DM_ROWS-1
    uint8_t hole;        // 冒出来的那一行(who == DM_EMPTY 时无意义)
    dm_who_t who;
    uint32_t left_ms;    // who != EMPTY:还能停多久;EMPTY:还要等多久才冒
} ceshi_game_t;

// 开一局。seed 由调用方给(卡上是 esp_random()),0 会换成固定的非 0 值。
void ceshi_start(ceshi_game_t *g, uint32_t seed);
// dir = -1 往上、+1 往下,到头循环。
void ceshi_move(ceshi_game_t *g, int dir);
// 拍一下:DM_HIT(库巴,+1)、DM_OOPS(猫,丢命)、DM_MISS(空的或行不对,什么都不变)。
dm_event_t ceshi_swat(ceshi_game_t *g);
// 过 dt 毫秒:返回 DM_LOST(库巴跑了,丢命)或 DM_NONE。
dm_event_t ceshi_tick(ceshi_game_t *g, uint32_t dt);
// 冒出来的东西停留多久:1500 − 30×分数,最少 500。
uint32_t ceshi_show_ms(uint16_t score);
bool ceshi_over(const ceshi_game_t *g);
