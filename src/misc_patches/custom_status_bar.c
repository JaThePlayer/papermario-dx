#include "custom_status_bar.h"
#include "PR/ultratypes.h"
#include "common_structs.h"
#include "enums.h"
#include "hud_element.h"
#include "inventory.h"
#include "misc_patches/misc_patches.h"

enum BlinkModes {
    BLINK_OFF   = 0,
    BLINK_ON    = 1,
};

enum ShimmerStates {
    STATUS_SHIMMER_OFF      = 0,
    STATUS_SHIMMER_BEGIN    = 1,
    STATUS_SHIMMER_ON       = 2,
};

#define TEXT_X_OFFSET 4
#define ICON_Y_OFFSET 4
#define TEXT_Y_OFFSET 7

static void draw_stat(s32 id, s32 x, s32 y, s32 timesHid, s32 current, s32 max) {
    hud_element_set_render_pos(id, x+4, y+8 + ICON_Y_OFFSET);
    hud_element_draw_next(id);
    status_bar_draw_stat(timesHid, x + 8 + TEXT_X_OFFSET, y + TEXT_Y_OFFSET, current, max);
}

static void draw_hp_display(StatusBar* statusBar, s32 x, s32 y) {
    s32 showStat = true;
    if (statusBar->hpBlinkTimeLeft > 0) {
        statusBar->hpBlinkTimeLeft--;
        if (statusBar->hpBlinkTimeLeft == 0) {
            status_bar_stop_blinking_hp();
        }
    }

    if (statusBar->hpBlinking != BLINK_OFF) {
        if (statusBar->hpBlinkAnimTime > 8) {
            showStat = false;
            if (statusBar->hpBlinkAnimTime > 12) {
                statusBar->hpBlinkAnimTime = 0;
                showStat = true;
            }
        }
        statusBar->hpBlinkAnimTime++;
    }

    if (showStat) {
        draw_stat(statusBar->hpIconHIDs[1], x, y, statusBar->hpTimesHID, statusBar->displayHP, gPlayerData.curMaxHP);
    }
}

static void draw_fp_display(StatusBar* statusBar, s32 x, s32 y) {
    s32 showStat = true;
    if (statusBar->fpBlinkTimeLeft > 0) {
        statusBar->fpBlinkTimeLeft--;
        if (statusBar->fpBlinkTimeLeft == 0) {
            status_bar_stop_blinking_hp();
        }
    }

    if (statusBar->fpBlinking != BLINK_OFF) {
        if (statusBar->fpBlinkAnimTime > 8) {
            showStat = false;
            if (statusBar->fpBlinkAnimTime > 12) {
                statusBar->fpBlinkAnimTime = 0;
                showStat = true;
            }
        }
        statusBar->fpBlinkAnimTime++;
    }

    if (showStat) {
        draw_stat(statusBar->fpIconHIDs[1], x, y, statusBar->fpTimesHID, statusBar->displayFP, gPlayerData.curMaxFP);
    }
}

static void draw_sp_display(StatusBar* statusBar, s32 x, s32 y) {
    s32 showStat = true;
    if (statusBar->starpointsBlinking != BLINK_OFF) {
        if (statusBar->starpointsBlinkAnimTime > 8) {
            if (statusBar->starpointsBlinkAnimTime <= 12) {
                showStat = false;
            } else {
                statusBar->starpointsBlinkAnimTime = 0;
            }
        }
        statusBar->starpointsBlinkAnimTime++;
    }

    if (showStat) {
        hud_element_set_render_pos(statusBar->spIconHID, x + 4, y+8 + ICON_Y_OFFSET + 1);
        hud_element_draw_next(statusBar->spIconHID);

        hud_element_set_render_pos(statusBar->spShineHID, x + 4, y+8 + ICON_Y_OFFSET + 1 - 5);
        hud_element_draw_next(statusBar->spShineHID);

        status_bar_draw_number(statusBar->spTimesHID, x + 8 + TEXT_X_OFFSET - 4, y + TEXT_Y_OFFSET, gPlayerData.starPoints, 2);
    }
}

extern void status_bar_stop_blinking_coins(void);

static void draw_coin_display(StatusBar* statusBar, s32 x, s32 y) {
    if (statusBar->coinsBlinkTimeLeft > 0) {
        statusBar->coinsBlinkTimeLeft--;
        if (statusBar->coinsBlinkTimeLeft == 0) {
            status_bar_stop_blinking_coins();
        }
    }

    s32 showStat = true;
    if (statusBar->coinsBlinking != BLINK_OFF) {
        if (statusBar->coinsBlinkAnimTime > 8) {
            showStat = false;
            if (statusBar->coinsBlinkAnimTime > 12) {
                statusBar->coinsBlinkAnimTime = 0;
                showStat = true;
            }
        }
        statusBar->coinsBlinkAnimTime++;
    }

    if (showStat) {
        hud_element_set_render_pos(statusBar->coinIconHID, x + 4, y+8 + ICON_Y_OFFSET + 1);
        hud_element_draw_next(statusBar->coinIconHID);

        hud_element_set_render_pos(statusBar->coinSparkleHID, x + 4, y+8 + ICON_Y_OFFSET + 1 - 5);
        hud_element_draw_next(statusBar->coinSparkleHID);

        status_bar_draw_number(statusBar->coinTimesHID, x + 8 + TEXT_X_OFFSET - 4, y + TEXT_Y_OFFSET, statusBar->displayCoins, 4);
    }
}

static void draw_se_display(StatusBar* statusBar, s32 x, s32 y) {
    x += TEXT_X_OFFSET + 8;
    y += TEXT_Y_OFFSET + 4;

    PlayerData* playerData = &gPlayerData;
    s32 id = statusBar->starIconHID;
    s32 showStat = true;
    if (statusBar->starPowerBlinking != BLINK_OFF) {
        if (statusBar->starPowerBlinkCounter > 5) {
            if (statusBar->starPowerBlinkCounter <= 8) {
                showStat = false;
            } else {
                statusBar->starPowerBlinkCounter = 0;
                showStat = true;
            }
        }
        statusBar->starPowerBlinkCounter++;
    }

    s32 displayStarPower = statusBar->displayStarPower;
    s32 spBars = displayStarPower / SP_PER_BAR;
    s32 limit = displayStarPower % SP_PER_BAR;
    limit /= 32;
    limit += spBars * 8;
    if (statusBar->shimmerState == STATUS_SHIMMER_BEGIN) {
        spBars = playerData->starPower / SP_PER_BAR;
        limit = playerData->starPower % SP_PER_BAR;
        limit = limit / 32;
        limit += spBars * 8;
        star_power_shimmer_start(0, x + limit * 25 / 10, y, 1.0f);
        statusBar->shimmerState = STATUS_SHIMMER_ON;
    }

    s32 s7;
    s32 showAddedBar = false;
    if (statusBar->shimmerState != STATUS_SHIMMER_OFF) {
        if (statusBar->shimmerTime != 0) {
            statusBar->shimmerTime--;
        } else {
            statusBar->shimmerState = STATUS_SHIMMER_OFF;
        }
        // creates a stipple pattern repeating 5x zero, 5x one, ...
        if ((statusBar->shimmerTime / 5) & 1) {
            showAddedBar = true;
        }
        s7 = statusBar->shimmerLimit % 8;
        s7 += statusBar->shimmerLimit / 8 * 8;
    } else {
        s7 = limit;
    }

    s32 i = 0;

    if (statusBar->starPowerBlinking != BLINK_OFF) {
        if (!showStat) {
            // start rendering after the blinking SP bars
            i = statusBar->powBarsToBlink * (SP_PER_BAR / SP_PER_SEG);
        }
    }

    if (!showAddedBar) {
        limit = MIN(limit, s7);
    }

    render_se_bar(id, x, y, i, limit + 6);

    star_power_shimmer_update();
    star_power_shimmer_draw();
}

extern HudScript HES_Item_BattleFireflower;

static void draw_item_display(StatusBar* statusBar, s32 x, s32 y) {
    s32 showStat = true;

    if (showStat) {
        s32 id = statusBar->hpIconHIDs[0];
        hud_element_set_script(id, HES_Item_BattleFireflower);
        hud_element_set_scale(id, 0.5f);
        hud_element_set_render_pos(id, x + 4, y+8 + ICON_Y_OFFSET + 1);
        hud_element_draw_next(id);
        hud_element_set_scale(id, 1.f);

        status_bar_draw_number(statusBar->spTimesHID, x + 8 + TEXT_X_OFFSET - 4, y + TEXT_Y_OFFSET, get_consumables_count(), 2);
    }
}

extern u8 ui_box_bg_flat_png[];
extern u8 ui_box_corners9_png[];

#define RGBA(r, g, b, a) { r, g, b, a }
#define CREATE_BAR_WINDOW_STYLE(brightColor, darkColor) { \
    .background = { \
        .imgData = ui_box_bg_flat_png, \
        .fmt = G_IM_FMT_I, \
        .bitDepth = G_IM_SIZ_4b, \
        .width = 16, \
        .height = 1, \
        .size = 8 \
    }, \
    .corners = { \
        .imgData = ui_box_corners9_png, \
        .fmt = G_IM_FMT_IA, \
        .bitDepth = G_IM_SIZ_8b, \
        .size1 = { .x = 16, .y = 16 }, \
        .size2 = { .x = 16, .y = 16 }, \
        .size3 = { .x = 16, .y = 16 }, \
        .size4 = { .x = 16, .y = 16 }, \
    }, \
    .opaqueCombineMode = gsDPSetCombineMode(PM_CC_BOX2_OPAQUE, PM_CC_BOX2_CYC2), \
    .transparentCombineMode = gsDPSetCombineMode(PM_CC_BOX2_TRANSPARENT, PM_CC_BOX2_CYC2), \
    .color1 = brightColor, \
    .color2 = darkColor, \
}

WindowStyleCustom WindowStyle_HpBar = CREATE_BAR_WINDOW_STYLE(RGBA(0xed, 0x87, 0x87, 255), RGBA(0x69, 0x29, 0x29, 255));
WindowStyleCustom WindowStyle_FpBar = CREATE_BAR_WINDOW_STYLE(RGBA(0xe8, 0xe7, 0xa2, 255), RGBA(0x61, 0x60, 0x38, 255));
WindowStyleCustom WindowStyle_SpBar = CREATE_BAR_WINDOW_STYLE(RGBA(0xef, 0xc4, 0xa8, 255), RGBA(0x4d, 0x89, 0x8d, 255));
WindowStyleCustom WindowStyle_CoinBar = CREATE_BAR_WINDOW_STYLE(RGBA(0x98, 0xc8, 0xa6, 255), RGBA(0x42, 0x4d, 0x3f, 255));
WindowStyleCustom WindowStyle_ItemBar = CREATE_BAR_WINDOW_STYLE(RGBA(0xef, 0xc4, 0xa8, 255), RGBA(0x4d, 0x89, 0x8d, 255));

#define DISPLAY_PAD 4
#define STAT_DISPLAY_HEIGHT 27

#define HP_DISPLAY_X 12
#define HP_DISPLAY_WIDTH 80
#define HP_DISPLAY_RIGHT HP_DISPLAY_X + HP_DISPLAY_WIDTH + DISPLAY_PAD

#define FP_DISPLAY_X HP_DISPLAY_RIGHT
#define FP_DISPLAY_WIDTH 80
#define FP_DISPLAY_RIGHT FP_DISPLAY_X + FP_DISPLAY_WIDTH + DISPLAY_PAD

#define SP_DISPLAY_X FP_DISPLAY_RIGHT
#define SP_DISPLAY_WIDTH 50
#define SP_DISPLAY_RIGHT FP_DISPLAY_X + FP_DISPLAY_WIDTH + DISPLAY_PAD

#define ITEM_DISPLAY_X FP_DISPLAY_RIGHT
#define ITEM_DISPLAY_WIDTH 50
#define ITEM_DISPLAY_RIGHT FP_DISPLAY_X + FP_DISPLAY_WIDTH + DISPLAY_PAD

#define COIN_DISPLAY_X (SP_DISPLAY_X + SP_DISPLAY_WIDTH + DISPLAY_PAD)
#define COIN_DISPLAY_WIDTH 62

#define SE_DISPLAY_X HP_DISPLAY_X
#define SE_DISPLAY_WIDTH 160

StatusBarElement hp_display = {
    .x = HP_DISPLAY_X,
    .y = 1,
    .width = HP_DISPLAY_WIDTH,
    .height = STAT_DISPLAY_HEIGHT,
    .windowStyle = { .customStyle = &WindowStyle_HpBar }, //WINDOW_STYLE_20,
    .draw = draw_hp_display,
};

StatusBarElement fp_display = {
    .x = FP_DISPLAY_X,
    .y = 1,
    .width = FP_DISPLAY_WIDTH,
    .height = STAT_DISPLAY_HEIGHT,
    .windowStyle = { .customStyle = &WindowStyle_FpBar },
    .draw = draw_fp_display,
};

StatusBarElement sp_display = {
    .x = SP_DISPLAY_X,
    .y = 1,
    .width = SP_DISPLAY_WIDTH,
    .height = STAT_DISPLAY_HEIGHT,
    .windowStyle = { .customStyle = &WindowStyle_SpBar },
    .draw = draw_sp_display,
};

StatusBarElement coin_display = {
    .x = COIN_DISPLAY_X,
    .y = 1,
    .width = COIN_DISPLAY_WIDTH,
    .height = STAT_DISPLAY_HEIGHT,
    .windowStyle = { .customStyle = &WindowStyle_CoinBar },
    .draw = draw_coin_display,
};

StatusBarElement se_display = {
    .x = SE_DISPLAY_X,
    .y = 1 + STAT_DISPLAY_HEIGHT - 6,
    .width = SE_DISPLAY_WIDTH,
    .height = 22,
    .windowStyle = { .customStyle = &WindowStyle_CoinBar },
    .draw = draw_se_display,
};

StatusBarElement item_display = {
    .x = ITEM_DISPLAY_X,
    .y = 1 + STAT_DISPLAY_HEIGHT - 6,
    .width = ITEM_DISPLAY_WIDTH,
    .height = STAT_DISPLAY_HEIGHT,
    .windowStyle = { .customStyle = &WindowStyle_ItemBar },
    .draw = draw_item_display,
};

StatusBarElement* status_bar_elements[] = {
    &hp_display,
    &fp_display,
    &sp_display,
    &coin_display,
    &se_display,
    &item_display,
    nullptr,
};

void draw_custom_status_bar(StatusBar* statusBar) {
    StatusBarElement* el;
    s32 i = 0;
    s32 sx = statusBar->drawPosX;
    s32 sy = statusBar->drawPosY;

    while ((el = status_bar_elements[i]) != nullptr) {
        s32 x = sx + el->x;
        s32 y = sy + el->y;
        gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX);
        draw_box(0, el->windowStyle, x, y, 0, el->width, el->height, 255, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, nullptr, nullptr, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);
        el->draw(statusBar, x, y);

        i++;
    }
}
