#pragma once

#include "hud_element.h"

struct ItemEffect;

void draw_icon_tag(s32 value, s32* posX, s32 posY, s32 msgId, WindowStyleCustom* windowStyle, HudScriptPtr hudScript);

typedef void (*ItemTagDraw)(struct ItemEffect* effect, s32* posX, s32 posY);

/// Represents a type of tag rendered on items which have certain effects.
/// The draw function is responsible for rendering its box and advancing posX as well.
typedef struct ItemTag {
    ItemTagDraw draw;
} ItemTag;


void draw_fp_tag(s32 fp, s32* posX, s32 posY);

void draw_bp_tag(s32 bp, s32* posX, s32 posY);

void draw_hp_tag(s32 hp, s32* posX, s32 posY);

extern ItemTag item_tag_electrify;

extern ItemTag item_tag_charge;
