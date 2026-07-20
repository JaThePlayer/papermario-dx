#pragma once

#include "common_structs.h"

typedef void (*StatusBarElementDraw)(StatusBar* statusBar, s32 x, s32 y);

typedef struct StatusBarElement {
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    WindowStyle windowStyle;
    StatusBarElementDraw draw;
} StatusBarElement;

extern StatusBarElement* status_bar_elements[];

void draw_custom_status_bar(StatusBar* statusBar);
