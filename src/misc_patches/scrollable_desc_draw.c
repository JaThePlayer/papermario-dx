#include "scrollable_desc_draw.h"
#include "include_asset.h"
#include "hud_element.h"

static s32 descTextPos;
static s32 descTextOffset;
static s32 prevMsgHudElementId = -1;
static s32 nextMsgHudElementId = -1;
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
INCLUDE_IMG("ui/desc_msg_prev.png", prev_msg_png);
INCLUDE_PAL("ui/desc_msg_prev.pal", prev_msg_pal);
INCLUDE_IMG("ui/desc_msg_next.png", next_msg_png);
INCLUDE_PAL("ui/desc_msg_next.pal", next_msg_pal);

static HudScript HES_DescMsgPrev2 = {
    hs_SetVisible
    hs_SetCustomSize(16, 16)
    hs_Loop
        hs_ClearFlags(HUD_ELEMENT_FLAG_200000)
        hs_SetCI(9, prev_msg)
        hs_SetFlags(HUD_ELEMENT_FLAG_200000)
        hs_SetCI(6, prev_msg)
    hs_Restart
    hs_End
};

static HudScript HES_DescMsgNext2 = {
    hs_SetVisible
    hs_SetCustomSize(16, 16)
    hs_Loop
        hs_ClearFlags(HUD_ELEMENT_FLAG_200000)
        hs_SetCI(9, next_msg)
        hs_SetFlags(HUD_ELEMENT_FLAG_200000)
        hs_SetCI(6, next_msg)
    hs_Restart
    hs_End
};

static void draw_item_setup(void) {
    prevMsgHudElementId = hud_element_create(&HES_DescMsgPrev2);
    nextMsgHudElementId = hud_element_create(&HES_DescMsgNext2);
    descTextPos = 0;
    descTextOffset = 0;
}

static void draw_item_cleanup(void) {
    hud_element_free(prevMsgHudElementId);
    hud_element_free(nextMsgHudElementId);

    prevMsgHudElementId = -1;
    nextMsgHudElementId = -1;
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

    get_msg_properties(itemMsg, NULL, NULL, NULL, &numLines, NULL, NULL, 0);
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

    descTextOffset += interp_text_scroll(descTextPos * 16 - descTextOffset); //= descTextPos * 16; // todo: interpolate

    gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, posX + 1, posY + 1, posX + width - 1, posY + height - 1);
    draw_msg(itemMsg, posX + 8, posY - descTextOffset, opacity, palette, style);

    if (descTextPos != 0) {
        hud_element_set_flags(nextMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_clear_flags(prevMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);

        s32 id = prevMsgHudElementId;
        hud_element_set_flags(id, HUD_ELEMENT_FLAG_80);

        hud_element_set_render_pos(id, posX + width - 8, posY + 8);
        hud_element_draw_without_clipping(id);
    } else if (descTextPos < textMaxPos) {
        hud_element_set_flags(prevMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_clear_flags(nextMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);

        hud_element_set_flags(nextMsgHudElementId, HUD_ELEMENT_FLAG_80);
        hud_element_set_render_pos(nextMsgHudElementId, posX + width - 8, posY + height - 8);
        hud_element_draw_without_clipping(nextMsgHudElementId);
    } else {
        hud_element_set_flags(prevMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);
        hud_element_set_flags(nextMsgHudElementId, HUD_ELEMENT_FLAG_DISABLED);
    }
}

void draw_scrollable_item_desc(ItemEntity* item, s32 posX, s32 posY, s32 width, s32 height, s32 opacity, s32 palette, u8 style) {
    ItemData* itemData = &gItemTable[item->itemID];
    s32 itemMsg = itemData->fullDescMsg;

    switch (item->state) {
        case ITEM_PICKUP_STATE_SHOW_GOT_ITEM:
        case ITEM_PICKUP_STATE_HIDE_GOT_ITEM:
        case ITEM_PICKUP_STATE_SHOW_TOO_MANY:
        case ITEM_PICKUP_STATE_HIDE_TOO_MANY:
            draw_scrollable_desc(itemMsg, posX, posY, width, height, opacity, palette, style);
            break;
    }
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
