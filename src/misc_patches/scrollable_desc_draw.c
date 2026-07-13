#include "scrollable_desc_draw.h"
#include "PR/ultratypes.h"
#include "common_structs.h"
#include "enums.h"
#include "include_asset.h"
#include "hud_element.h"
#include "message_ids.h"
#include "move_enum.h"

static s32 descTextPos;
static s32 descTextOffset;
static s32 prevMsgHudElementId = -1;
static s32 nextMsgHudElementId = -1;
static s32 tagIconElementId = -1;
static s32 lastDrawnMsgId = ITEM_NONE;
static s8 framesSinceLastDraw = 0;

static u8 scrollInterpEasingLUT[] = { 0, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8};

static s32 interp_text_scroll(s32 deltaBefore) {
    s32 val;
    s32 db = abs(deltaBefore);
    s32 s = sign(deltaBefore);

    if (db >= 16) {
        val = 8;
    } else {
        val = scrollInterpEasingLUT[db];
    }

    return val * s;
}

// copy of ui_pause_desc_msg_prev
INCLUDE_IMG("assets/us/ui/pause/desc_msg_prev.png", prev_msg_png);
INCLUDE_PAL("assets/us/ui/pause/desc_msg_prev.pal", prev_msg_pal);
INCLUDE_IMG("assets/us/ui/pause/desc_msg_next.png", next_msg_png);
INCLUDE_PAL("assets/us/ui/pause/desc_msg_next.pal", next_msg_pal);

INCLUDE_IMG("ui/stat_heart.png", stat_hp_png);
INCLUDE_IMG("assets/us/ui/pause/stat_fp.png", stat_fp_png);
INCLUDE_PAL("assets/us/ui/pause/stat_fp.pal", stat_fp_pal);
INCLUDE_IMG("assets/us/ui/pause/stat_bp.png", stat_bp_png);
INCLUDE_PAL("assets/us/ui/pause/stat_bp.pal", stat_bp_pal);

HudScript HES_ScrollableDesc_StatHp =
{
    hs_SetTileSize(HUD_ELEMENT_SIZE_16x16)
    hs_Loop
    hs_SetTexelOffset(-2, -2)
    hs_ClearFlags(HUD_ELEMENT_FLAG_FMT_CI4)
    hs_SetRGBA(60, stat_hp_png)
    hs_Restart
    hs_End
};
HudScript HES_ScrollableDesc_StatFp = HES_TEMPLATE_CI_ENUM_SIZE(stat_fp, 16, 16);
HudScript HES_ScrollableDesc_StatBp = HES_TEMPLATE_CI_ENUM_SIZE(stat_bp, 16, 16);

static HudScript HES_DescMsgPrev2 = {
    hs_SetVisible
    hs_SetCustomSize(16, 16)
    hs_Loop
        hs_ClearFlags(HUD_ELEMENT_FLAG_INVISIBLE)
        hs_SetCI(9, prev_msg)
        hs_SetFlags(HUD_ELEMENT_FLAG_INVISIBLE)
        hs_SetCI(6, prev_msg)
    hs_Restart
    hs_End
};

static HudScript HES_DescMsgNext2 = {
    hs_SetVisible
    hs_SetCustomSize(16, 16)
    hs_Loop
        hs_ClearFlags(HUD_ELEMENT_FLAG_INVISIBLE)
        hs_SetCI(9, next_msg)
        hs_SetFlags(HUD_ELEMENT_FLAG_INVISIBLE)
        hs_SetCI(6, next_msg)
    hs_Restart
    hs_End
};

static HudScript HES_TagIcon = {
    hs_SetVisible
    hs_SetCustomSize(16, 16)
    hs_Loop
        hs_SetCI(9, next_msg)
    hs_Restart
    hs_End
};

static void draw_item_setup(void) {
    prevMsgHudElementId = hud_element_create(&HES_DescMsgPrev2);
    nextMsgHudElementId = hud_element_create(&HES_DescMsgNext2);
    tagIconElementId = hud_element_create(&HES_TagIcon);

    descTextPos = 0;
    descTextOffset = 0;
    
    hud_element_set_flags(tagIconElementId, HUD_ELEMENT_FLAG_DISABLED);
    hud_element_set_flags(tagIconElementId, HUD_ELEMENT_FLAG_MANUAL_RENDER);
}

static void draw_item_cleanup(void) {
    if (prevMsgHudElementId != -1 && get_hud_element(prevMsgHudElementId) != nullptr)
        hud_element_free(prevMsgHudElementId);
    if (nextMsgHudElementId != -1 && get_hud_element(nextMsgHudElementId) != nullptr)
        hud_element_free(nextMsgHudElementId);
    if (tagIconElementId != -1 && get_hud_element(tagIconElementId) != nullptr)
        hud_element_free(tagIconElementId);

    prevMsgHudElementId = -1;
    nextMsgHudElementId = -1;
    tagIconElementId = -1;
}

void draw_scrollable_desc(s32 itemMsg, s32 posX, s32 posY, s32 width, s32 height, s32 opacity, s32 palette, u8 style) {
    s32 heldButtons = gGameStatusPtr->heldButtons[0];
    s32 numLines;

    framesSinceLastDraw = 0;

    if (lastDrawnMsgId != itemMsg) {
        lastDrawnMsgId = itemMsg;
        descTextPos = 0;
        descTextOffset = 0;
    }

    if (prevMsgHudElementId == -1) {
        draw_item_setup();
    }
    if (get_hud_element(prevMsgHudElementId) == nullptr) {
        draw_item_cleanup();
        draw_item_setup();
    }

    get_msg_properties(itemMsg, nullptr, nullptr, nullptr, &numLines, nullptr, nullptr, 0);
    if (numLines % 2) {
        numLines++;
    }
    s32 textMaxPos = numLines - 2;
    if (textMaxPos < 0) {
        textMaxPos = 0;
    }

    if (heldButtons & BUTTON_C_UP) {
        descTextPos -= 2;
        if (descTextPos < 0) {
            descTextPos = 0;
        }
    }
    if (heldButtons & BUTTON_C_DOWN) {
        descTextPos += 2;
        if (descTextPos > textMaxPos) {
            descTextPos = textMaxPos;
        }
    }

    descTextOffset += interp_text_scroll(descTextPos * 16 - descTextOffset);

    gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, posX + 1, posY + 1, posX + width - 1, posY + height - 1);
    draw_msg(itemMsg, posX + 2, posY - descTextOffset, opacity, palette, style);

    hud_element_set_flags(prevMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);
    hud_element_set_flags(nextMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);

    if (descTextPos != 0) {
        hud_element_clear_flags(prevMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_set_flags(prevMsgHudElementId, HUD_ELEMENT_FLAG_MANUAL_RENDER);
        hud_element_set_render_pos(prevMsgHudElementId, posX + width - 8, posY + 8);
        hud_element_draw_without_clipping(prevMsgHudElementId);
    } else {
        hud_element_update(get_hud_element(prevMsgHudElementId));
    }

    if (descTextPos < textMaxPos) {
        hud_element_clear_flags(nextMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_set_flags(nextMsgHudElementId, HUD_ELEMENT_FLAG_MANUAL_RENDER);
        hud_element_set_render_pos(nextMsgHudElementId, posX + width - 8, posY + height - 8);
        hud_element_draw_without_clipping(nextMsgHudElementId);
    } else {
        hud_element_update(get_hud_element(nextMsgHudElementId));
    }
}

void draw_scrollable_item_desc(ItemEntity* item, s32 posX, s32 posY, s32 width, s32 height, s32 opacity, s32 palette, u8 style) {
    switch (item->state) {
        case ITEM_PICKUP_STATE_SHOW_GOT_ITEM:
        case ITEM_PICKUP_STATE_HIDE_GOT_ITEM:
        case ITEM_PICKUP_STATE_SHOW_TOO_MANY:
        case ITEM_PICKUP_STATE_HIDE_TOO_MANY:
            draw_scrollable_item_id_desc(item->itemID, posX, posY, width, height, opacity, palette, style);
            break;
    }
}

void draw_scrollable_item_id_desc(s32 itemId, s32 posX, s32 posY, s32 width, s32 height, s32 opacity, s32 palette, u8 style) {
    ItemData* itemData = &gItemTable[itemId];
    s32 itemMsg = itemData->fullDescMsg;

    draw_scrollable_desc(itemMsg, posX, posY, width, height, opacity, palette, style);

    s32 cx = posX + 10;
    s32 cy = posY - 14;
    if (itemData->moveID != MOVE_NONE) {
        gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        MoveData* move = &gMoveTable[itemData->moveID];
        draw_bp_tag(move->costBP, &cx, cy);
        draw_fp_tag(move->costFP, &cx, cy);
    } else if (itemData->typeFlags & ITEM_TYPE_FLAG_FOOD_OR_DRINK) {
        draw_hp_tag(itemData->potencyA, &cx, cy);
        draw_fp_tag(itemData->potencyB, &cx, cy);
    }
}

extern u8 ui_box_bg_tile_png[];
extern u8 ui_box_corners4_png[];

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

WindowStyleCustom WindowStyle_BpCostTag = CREATE_TAG_WINDOW_STYLE(RGBA(204, 155, 71, 255), RGBA(230, 205, 163, 0));
WindowStyleCustom WindowStyle_FpCostTag = CREATE_TAG_WINDOW_STYLE(RGBA(71, 200, 204, 255), RGBA(163, 228, 230, 0));
WindowStyleCustom WindowStyle_HpCostTag = CREATE_TAG_WINDOW_STYLE(RGBA(210, 91, 91, 255), RGBA(230, 163, 163, 0));

void draw_icon_tag(s32 value, s32* posX, s32 posY, s32 msgId, WindowStyleCustom* windowStyle, HudScript* hudScript) {
    s32 padding = 2;
    s32 tabWidth, tabHeight;
    if (value != 0) {
        gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        set_message_int_var(value, 0);
        get_msg_properties(msgId, &tabHeight, &tabWidth, nullptr, nullptr, nullptr, nullptr, 0);
        tabWidth += 16;
        draw_box(DRAW_FLAG_NO_CLIP, windowStyle, *posX, posY - 1, 1, tabWidth, tabHeight + 2, 255,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        draw_msg(msgId, *posX + 2, posY + 1, 255,
                     MSG_PAL_WHITE, DRAW_MSG_STYLE_MENU);

        hud_element_clear_flags(tagIconElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_set_render_pos(tagIconElementId, *posX + tabWidth - 16 + 8, posY + 8);
        hud_element_set_script(tagIconElementId, hudScript);
        hud_element_set_flags(tagIconElementId, HUD_ELEMENT_FLAG_DROP_SHADOW);
        hud_element_clear_flags(tagIconElementId, HUD_ELEMENT_FLAG_FILTER_TEX);
        hud_element_set_scale(tagIconElementId, 1.0f);
        hud_element_draw_without_clipping(tagIconElementId);

        *posX += tabWidth + padding;
    }
}

void draw_fp_tag(s32 fp, s32* posX, s32 posY) {
    draw_icon_tag(fp, posX, posY, MSG_MenuTip_FpCost, &WindowStyle_FpCostTag, &HES_ScrollableDesc_StatFp);
}

void draw_bp_tag(s32 bp, s32* posX, s32 posY) {
    draw_icon_tag(bp, posX, posY, MSG_MenuTip_BpCost, &WindowStyle_BpCostTag, &HES_ScrollableDesc_StatBp);
}

void draw_hp_tag(s32 bp, s32* posX, s32 posY) {
    draw_icon_tag(bp, posX, posY, MSG_MenuTip_BpCost, &WindowStyle_HpCostTag, &HES_ScrollableDesc_StatHp);
}

void draw_item_gc(void) {
    if (prevMsgHudElementId != -1 && framesSinceLastDraw < 2) {
        framesSinceLastDraw++;
        if (framesSinceLastDraw == 2) {
            draw_item_cleanup();
        }
    }
}

b8 was_description_drawn_this_frame(void) {
    return prevMsgHudElementId != -1 && framesSinceLastDraw < 2;
}
