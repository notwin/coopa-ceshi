// src/ceshi_save.c —— 见 ceshi_save.h。
#include "ceshi_save.h"

#include <string.h>

void ceshi_save_reset(ceshi_save_t *s) {
    memset(s, 0, sizeof(*s));
    s->magic = CESHI_MAGIC;
    s->ver = CESHI_SAVE_VER;
}

bool ceshi_save_valid(const void *blob) {
    const ceshi_save_t *s = blob;
    return s->magic == CESHI_MAGIC && s->ver == CESHI_SAVE_VER && s->best <= CESHI_BEST_MAX;
}

bool ceshi_save_record(ceshi_save_t *s, uint16_t score) {
    bool changed = false;
    if (s->plays < 0xFFFFu) {
        s->plays++;
        changed = true;
    }
    if (score > s->best) {
        s->best = score > CESHI_BEST_MAX ? CESHI_BEST_MAX : score;
        changed = true;
    }
    return changed;
}

uint8_t ceshi_milestones_of(const ceshi_save_t *s) {
    return (uint8_t)((s->best >= 10 ? 1u : 0u) | (s->best >= 30 ? 2u : 0u));
}
