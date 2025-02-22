#ifndef _H_CUSTOM_STATUS_ICONS_
#define _H_CUSTOM_STATUS_ICONS_

#define CUSTOM_ICON_AMT 16

#define DEF_DOWN_ICON_ID 0
#define ATK_DOWN_ICON_ID 1
#define DEF_UP_ICON_ID 2
#define ATK_UP_ICON_ID 3
#define BURN_ICON_ID 4
#define FP_COST_DOWN_ICON_ID 5
#define CHARGE_ICON_ID 6
#define POISON_ICON_ID 7
#define PAIN_FOCUS_ICON_ID 8

typedef struct CustomHudComplexStatusIcon {
    /* 0x0 */ s8 active;
    /* 0x1 */ s8 removing;
    /* 0x2 */ s8 activeTask;
    /* 0x3 */ s8 removingTask;
    /* 0x4 */ s8 turns;
    /* 0x5 */ s8 frameCounter;
              s8 palette;
    /* 0x8 */ s32 activeElementID;
    /* 0xC */ s32 removingElementID;
} CustomHudComplexStatusIcon;

void custom_status_icons_init(CustomHudComplexStatusIcon customIcons[CUSTOM_ICON_AMT]);
void custom_icons_update(CustomHudComplexStatusIcon customIcons[CUSTOM_ICON_AMT]);

void create_custom_status_icon(Actor* target, s32 customIconId);
void remove_custom_status_icon(s32 iconId, s32 customIconId);

#define DEFAULT_STATUS_ICON_BEGIN(frame1, frame2) { \
    hs_SetVisible \
    hs_SetCustomSize(16, 16) \
    hs_SetAlpha(30) \
    hs_SetTexelOffset(10, -10) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(50) \
    hs_SetTexelOffset(8, -8) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(80) \
    hs_SetTexelOffset(6, -6) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(120) \
    hs_SetTexelOffset(5, -5) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(170) \
    hs_SetTexelOffset(4, -4) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(200) \
    hs_SetTexelOffset(3, -3) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(220) \
    hs_SetTexelOffset(2, -2) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(240) \
    hs_SetTexelOffset(1, -1) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(255) \
    hs_SetTexelOffset(0, 0) \
    hs_SetCI(1, frame1) \
    hs_Loop \
        hs_SetCI(6, frame1) \
        hs_SetCI(6, frame2) \
    hs_Restart \
    hs_End \
};

#define DEFAULT_STATUS_ICON_END(frame1, frame2) { \
    hs_SetVisible \
    hs_SetCustomSize(16, 16) \
    hs_SetAlpha(255) \
    hs_SetTexelOffset(0, 0) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(240) \
    hs_SetTexelOffset(1, -1) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(220) \
    hs_SetTexelOffset(2, -2) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(200) \
    hs_SetTexelOffset(3, -3) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(170) \
    hs_SetTexelOffset(4, -4) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(120) \
    hs_SetTexelOffset(5, -5) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(80) \
    hs_SetTexelOffset(6, -6) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(50) \
    hs_SetTexelOffset(8, -8) \
    hs_SetCI(1, frame1) \
    hs_SetAlpha(30) \
    hs_SetTexelOffset(10, -10) \
    hs_SetCI(1, frame1) \
    hs_End \
};

#endif
