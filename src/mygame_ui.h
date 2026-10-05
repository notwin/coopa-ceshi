// src/mygame_ui.h —— 「拍拖鞋」画面(LVGL,只在持有 LVGL 锁时调用)。
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "mygame_logic.h"

// 标题页:sel 0 = 开始,1 = 回菜单。
void mygame_ui_title(uint8_t sel, uint16_t best);
// 进一局时建好游戏画面(三行洞、奶奶、拖鞋、HUD)。
void mygame_ui_play(void);
// 每帧按状态更新画面(位置、冒出来的东西、分数、命)。flash_ms > 0 时洞口闪一下(拍到的反馈)。
void mygame_ui_play_update(const mygame_game_t *g, uint32_t flash_ms);
// 结束页。
void mygame_ui_over(uint16_t score, uint16_t best);
