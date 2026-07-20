#include "item_tags.h"
#include "message_ids.h"
#include "misc_patches/item_effects.h"
#include "misc_patches/scrollable_desc_draw.h"
#include "include_asset.h"

extern u8 ui_box_bg_tile_png[];
extern u8 ui_box_corners4_png[];

void draw_icon_tag(s32 value, s32* posX, s32 posY, s32 msgId, WindowStyleCustom* windowStyle, HudScriptPtr hudScript) {
    s32 padding = 2;
    s32 tabWidth, tabHeight;
    gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (value != 0) {
        tabWidth = (value > 9 ? 2 : 1) * 9;
        tabHeight = 16;
        tabWidth += 16 + padding;
        draw_box(DRAW_FLAG_NO_CLIP, windowStyle, *posX, posY - 1, 1, tabWidth, tabHeight + 2, 255,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        draw_number(value, *posX + 2, posY + 1, 1, MSG_PAL_WHITE, 255, DRAW_NUMBER_STYLE_MONOSPACE);

        hud_element_clear_flags(tagIconElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_set_render_pos(tagIconElementId, *posX + tabWidth - 8, posY + 8);
        hud_element_set_script(tagIconElementId, hudScript);
        hud_element_set_flags(tagIconElementId, HUD_ELEMENT_FLAG_DROP_SHADOW);
        hud_element_clear_flags(tagIconElementId, HUD_ELEMENT_FLAG_FILTER_TEX);
        hud_element_set_scale(tagIconElementId, 1.0f);
        hud_element_draw_without_clipping(tagIconElementId);

        *posX += tabWidth + padding;
    } else {
        tabHeight = 16;
        tabWidth = 16 + padding;
        draw_box(DRAW_FLAG_NO_CLIP, windowStyle, *posX, posY - 1, 1, tabWidth, tabHeight + 2, 255,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        hud_element_clear_flags(tagIconElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_set_render_pos(tagIconElementId, *posX + tabWidth - 8, posY + 8);
        hud_element_set_script(tagIconElementId, hudScript);
        hud_element_set_flags(tagIconElementId, HUD_ELEMENT_FLAG_DROP_SHADOW);
        hud_element_clear_flags(tagIconElementId, HUD_ELEMENT_FLAG_FILTER_TEX);
        hud_element_set_scale(tagIconElementId, 1.0f);
        hud_element_draw_without_clipping(tagIconElementId);

        *posX += tabWidth + padding;
    }
}

#define RGBA(r, g, b, a) { r, g, b, a }
#define CREATE_TAG_WINDOW_STYLE(brightColor, darkColor) { \
    .background = { \
        .imgData = ui_box_bg_tile_png, \
        .fmt = G_IM_FMT_I, \
        .bitDepth = G_IM_SIZ_4b, \
        .width = 16, \
        .height = 16, \
        .size = 128 \
    }, \
    .corners = { \
        .imgData = ui_box_corners4_png, \
        .fmt = G_IM_FMT_IA, \
        .bitDepth = G_IM_SIZ_8b, \
        .size1 = { .x = 8, .y = 8 }, \
        .size2 = { .x = 8, .y = 8 }, \
        .size3 = { .x = 8, .y = 8 }, \
        .size4 = { .x = 8, .y = 8 }, \
    }, \
    .opaqueCombineMode = gsDPSetCombineMode(PM_CC_WINDOW_2, PM_CC_WINDOW_1), \
    .transparentCombineMode = gsDPSetCombineMode(PM_CC_WINDOW_3, PM_CC_WINDOW_1), \
    .color1 = brightColor, \
    .color2 = darkColor, \
}


INCLUDE_IMG("ui/stat_heart.png", stat_hp_png);
INCLUDE_IMG("assets/us/ui/pause/stat_fp.png", stat_fp_png);
INCLUDE_PAL("assets/us/ui/pause/stat_fp.pal", stat_fp_pal);
INCLUDE_IMG("assets/us/ui/pause/stat_bp.png", stat_bp_png);
INCLUDE_PAL("assets/us/ui/pause/stat_bp.pal", stat_bp_pal);
INCLUDE_IMG("ui/tags/electric.png", electric_png);
INCLUDE_PAL("ui/tags/electric.pal", electric_pal);
INCLUDE_IMG("ui/tags/charge.png", charge_png);
INCLUDE_PAL("ui/tags/charge.pal", charge_pal);

HudScript HES_Tag_StatHp =
{
    hs_SetTileSize(HUD_ELEMENT_SIZE_16x16)
    hs_Loop
    hs_SetTexelOffset(-2, -2)
    hs_ClearFlags(HUD_ELEMENT_FLAG_FMT_CI4)
    hs_SetRGBA(60, stat_hp_png)
    hs_Restart
    hs_End
};

HudScript HES_Tag_StatFp = HES_TEMPLATE_CI_ENUM_SIZE(stat_fp, 16, 16);
HudScript HES_Tag_StatBp = HES_TEMPLATE_CI_ENUM_SIZE(stat_bp, 16, 16);
HudScript HES_Tag_Electric = HES_TEMPLATE_CI_ENUM_SIZE(electric, 16, 16);
HudScript HES_Tag_Charge = HES_TEMPLATE_CI_ENUM_SIZE(charge, 16, 16);

WindowStyleCustom WindowStyle_BpCostTag = CREATE_TAG_WINDOW_STYLE(RGBA(204, 155, 71, 255), RGBA(230, 205, 163, 0));
WindowStyleCustom WindowStyle_FpCostTag = CREATE_TAG_WINDOW_STYLE(RGBA(71, 200, 204, 255), RGBA(163, 228, 230, 0));
WindowStyleCustom WindowStyle_HpCostTag = CREATE_TAG_WINDOW_STYLE(RGBA(210, 91, 91, 255), RGBA(230, 163, 163, 0));
WindowStyleCustom WindowStyle_ElectricTag = CREATE_TAG_WINDOW_STYLE(RGBA(245, 238, 39, 255), RGBA(250, 246, 143, 0));
WindowStyleCustom WindowStyle_ChargeTag = CREATE_TAG_WINDOW_STYLE(RGBA(245, 238, 39, 255), RGBA(250, 246, 143, 0));

void draw_fp_tag(s32 fp, s32* posX, s32 posY) {
    if (fp != 0)
        draw_icon_tag(fp, posX, posY, MSG_MenuTip_FpCost, &WindowStyle_FpCostTag, HES_Tag_StatFp);
}

void draw_bp_tag(s32 bp, s32* posX, s32 posY) {
    if (bp != 0)
        draw_icon_tag(bp, posX, posY, MSG_MenuTip_BpCost, &WindowStyle_BpCostTag, HES_Tag_StatBp);
}

void draw_hp_tag(s32 hp, s32* posX, s32 posY) {
    if (hp != 0)
        draw_icon_tag(hp, posX, posY, MSG_MenuTip_BpCost, &WindowStyle_HpCostTag, HES_Tag_StatHp);
}

void draw_electric_tag(ItemEffect* effect, s32* posX, s32 posY) {
    draw_icon_tag(ITEM_EFFECTS_UNPACK_TURNS(effect->userdata.s32), posX, posY, MSG_MenuTip_BpCost, &WindowStyle_ElectricTag, HES_Tag_Electric);
}

void draw_charge_tag(ItemEffect* effect, s32* posX, s32 posY) {
    draw_icon_tag(ITEM_EFFECTS_UNPACK_POTENCY(effect->userdata.s32), posX, posY, MSG_MenuTip_BpCost, &WindowStyle_ChargeTag, HES_Tag_Charge);
}

ItemTag item_tag_electrify = {
    .draw = draw_electric_tag,
};

ItemTag item_tag_charge = {
    .draw = draw_charge_tag,
};
