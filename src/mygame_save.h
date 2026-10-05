// src/mygame_save.h —— 「拍拖鞋」的存档(saves 分区的 "mygame"/"save",12 字节)和里程碑。
// 纯数据,不碰 NVS;读写由 mygame_app.c 用 kit_store 做。
#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MYGAME_NS        "mygame"        // = coopa.toml 的 id
#define MYGAME_KEY       "save"
#define MYGAME_MAGIC     0x4F4D4544u   // "DEMO"(小端)
#define MYGAME_SAVE_VER  1
#define MYGAME_BEST_MAX  9999          // 和 DM_SCORE_MAX 一样

typedef struct {
    uint32_t magic;
    uint16_t best;    // 单局最高分(只增不减)
    uint16_t plays;   // 玩过几局(封顶 65535)
    uint8_t ver;
    uint8_t pad[3];
} mygame_save_t;

void mygame_save_reset(mygame_save_t *s);
bool mygame_save_valid(const void *blob);
// 一局结束:记局数和最高分。返回 true = 有变化,要写回存档。
bool mygame_save_record(mygame_save_t *s, uint16_t score);
// 里程碑位(coopa.toml 的顺序):位 0 = 最高分 ≥ 10,位 1 = 最高分 ≥ 30。
uint8_t mygame_milestones_of(const mygame_save_t *s);
