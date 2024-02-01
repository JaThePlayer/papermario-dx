#ifndef _H_ITEM_DESC_DRAW
#define _H_ITEM_DESC_DRAW

#include "common.h"

// draws the description given by the given msgId, supporting C-down/C-up etc.
void draw_scrollable_desc(s32 msgId, s32 posX, s32 posY, s32 width, s32 height, s32 opacity, s32 palette, u8 style);

// draws the item description for the given item, supporting C-down/C-up etc.
void draw_scrollable_item_desc(ItemEntity* item, s32 posX, s32 posY, s32 width, s32 height, s32 opacity, s32 palette, u8 style);

// should be called each frame for automatic unloading of HudElements related to item description drawing.
void draw_item_gc(void);

// Whether a scrollable description was drawn this frame.
// Used to disable using partner abilities when checking a shop inventory.
b8 was_description_drawn_this_frame(void);

#endif
