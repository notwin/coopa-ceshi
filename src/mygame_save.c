// src/mygame_save.c —— 见 mygame_save.h。
#include "mygame_save.h"

#include <string.h>

void mygame_save_reset(mygame_save_t *s) {
    memset(s, 0, sizeof(*s));
    s->magic = MYGAME_MAGIC;
    s->ver = MYGAME_SAVE_VER;
}

bool mygame_save_valid(const void *blob) {
    const mygame_save_t *s = blob;
    return s->magic == MYGAME_MAGIC && s->ver == MYGAME_SAVE_VER && s->best <= MYGAME_BEST_MAX;
}

bool mygame_save_record(mygame_save_t *s, uint16_t score) {
    bool changed = false;
    if (s->plays < 0xFFFFu) {
        s->plays++;
        changed = true;
    }
    if (score > s->best) {
        s->best = score > MYGAME_BEST_MAX ? MYGAME_BEST_MAX : score;
        changed = true;
    }
    return changed;
}

uint8_t mygame_milestones_of(const mygame_save_t *s) {
    return (uint8_t)((s->best >= 10 ? 1u : 0u) | (s->best >= 30 ? 2u : 0u));
}
