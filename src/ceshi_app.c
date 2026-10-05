// src/ceshi_app.c —— 示范游戏「拍拖鞋」的入口表(给开发者照着写的模板)。
// 按键:标题页 ▲▼ 选、● 确定;游戏中 ▲▼ 移动拖鞋、● 拍;结束页 ● 再来、▼ 回标题。
// 存档:saves 分区的 "ceshi"/"save"(ceshi_save.h;新游戏都用 kit_sv_*,命名空间 = coopa.toml 的 id)。里程碑由最高分推出(ceshi_milestones_of)。
#include "ceshi_logic.h"
#include "ceshi_save.h"
#include "ceshi_texts.h"
#include "ceshi_ui.h"
#include "esp_random.h"
#include "kit_app.h"
#include "kit_sfx.h"
#include "kit_sprite.h"
#include "kit_store.h"
#include "ui_kit.h"

#include <stdio.h>

#define FLASH_MS 150
#define OVER_LOCK_MS 500  // 结束页先挡 500 ms 的 ●:狂按拍东西时,别一下就跳过结束页开新局

typedef enum { PG_TITLE, PG_PLAY, PG_OVER } page_t;

static ceshi_save_t s_save;
static ceshi_game_t s_game;
static page_t s_page;
static uint8_t s_sel;
static uint32_t s_flash;
static uint32_t s_over_lock;  // 结束页还要挡 ● 多少毫秒

// 开机读存档。读不到或校验不过(kit_sv_blob_load 不动 s_save)就从零开始;这里不取随机数。
static void ceshi_init(void) {
    static ceshi_save_t scratch;
    if (!kit_sv_blob_load(CESHI_NS, CESHI_KEY, &s_save, sizeof(s_save), ceshi_save_valid, &scratch)) {
        ceshi_save_reset(&s_save);
    }
}

static void to_title(void) {
    s_page = PG_TITLE;
    s_sel = 0;
    ceshi_ui_title(s_sel, s_save.best);
}

static void to_play(void) {
    s_page = PG_PLAY;
    s_flash = 0;
    ceshi_start(&s_game, esp_random());  // 种子只在开局取,不在 init 里取(见计划 Review Focus 1)
    ceshi_ui_play();
    ceshi_ui_play_update(&s_game, 0);
    sfx_play(SFX_START);
}

static void to_over(void) {
    s_page = PG_OVER;
    s_over_lock = OVER_LOCK_MS;
    if (ceshi_save_record(&s_save, s_game.score)) {
        kit_sv_blob_store(CESHI_NS, CESHI_KEY, &s_save, sizeof(s_save));
    }
    sfx_play(SFX_GAME_OVER);
    ceshi_ui_over(s_game.score, s_save.best);
}

static void ceshi_enter(const kit_links_t *links) {
    (void)links;
    to_title();
}

static void react(dm_event_t e) {
    if (e == DM_HIT) {
        s_flash = FLASH_MS;
        sfx_play(SFX_HIT);
    } else if (e == DM_OOPS || e == DM_LOST) {
        sfx_play(SFX_DODGE);
    }
    if (ceshi_over(&s_game)) to_over();
}

static bool ceshi_input(const kit_input_t *in) {
    if (in->long_press) return false;
    switch (s_page) {
        case PG_TITLE:
            if (in->key == KIT_KEY_OK) {
                if (s_sel == 1) return true;  // 回菜单
                to_play();
            } else {
                s_sel ^= 1;
                ceshi_ui_title(s_sel, s_save.best);
            }
            break;
        case PG_PLAY:
            if (in->key == KIT_KEY_UP) ceshi_move(&s_game, -1);
            else if (in->key == KIT_KEY_DOWN) ceshi_move(&s_game, +1);
            else react(ceshi_swat(&s_game));
            if (s_page == PG_PLAY) ceshi_ui_play_update(&s_game, s_flash);
            break;
        case PG_OVER:
            if (in->key == KIT_KEY_OK) {
                if (!s_over_lock) to_play();
            }
            else if (in->key == KIT_KEY_DOWN) to_title();
            break;
    }
    return false;
}

static void ceshi_frame(uint32_t dt) {
    if (s_page == PG_OVER) uk_countdown(&s_over_lock, dt);
    if (s_page != PG_PLAY) return;
    s_flash = s_flash > dt ? s_flash - dt : 0;
    react(ceshi_tick(&s_game, dt));
    if (s_page == PG_PLAY) ceshi_ui_play_update(&s_game, s_flash);
}

static unsigned ceshi_busy(void) {
    return s_page == PG_PLAY ? KIT_BUSY_TIMING : 0;
}

// won / milestones / status 会被外壳在任意时刻调用:只读状态,不取随机数。
static bool ceshi_won(void) {
    return ceshi_milestones_of(&s_save) == 3;
}

static uint8_t ceshi_milestones(void) {
    return ceshi_milestones_of(&s_save);
}

// 调试通道状态行:DM <页> <分> <命> <最高>
static size_t ceshi_status(char *buf, size_t len) {
    int n = snprintf(buf, len, "DM %d %u %u %u\n", (int)s_page, (unsigned)s_game.score, (unsigned)s_game.lives,
                     (unsigned)s_save.best);
    return n < 0 ? 0 : ((size_t)n < len ? (size_t)n : len - 1);
}

static void ceshi_save_now(void) {
    if (s_page == PG_PLAY) kit_sv_blob_store(CESHI_NS, CESHI_KEY, &s_save, sizeof(s_save));
}

static const kit_menu_card_t CESHI_CARD = {
    .title = DT_TITLE,
    .sub = DT_SUB,
    .art = { SPR_SLIPPER, SPR_KID },
};

const kit_app_t ceshi_app = {
    .id = "ceshi",
    .sprite_budget = KIT_SPR_BUDGET(4 * 16 * 16 * 4, 4),  // 奶奶、拖鞋、库巴、猫,都 ≤ 16×16
    .init = ceshi_init,
    .enter = ceshi_enter,
    .input = ceshi_input,
    .frame = ceshi_frame,
    .busy = ceshi_busy,
    .won = ceshi_won,
    .milestones = ceshi_milestones,
    .status = ceshi_status,
    .save_now = ceshi_save_now,
    .card = &CESHI_CARD,
};
