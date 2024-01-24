#ifndef _H_CUSTOM_STATUS_ICONS_
#define _H_CUSTOM_STATUS_ICONS_

#define CUSTOM_ICON_AMT 4

#define DEF_DOWN_ICON_ID 0

typedef struct CustomHudComplexStatusIcon {
    /* 0x0 */ s8 active;
    /* 0x1 */ s8 removing;
    /* 0x2 */ s8 activeTask;
    /* 0x3 */ s8 removingTask;
    /* 0x4 */ s8 unk_04;
    /* 0x5 */ s8 frameCounter;
    /* 0x8 */ s32 activeElementID;
    /* 0xC */ s32 removingElementID;
} CustomHudComplexStatusIcon; // size = 0x10

void custom_status_icons_init(CustomHudComplexStatusIcon customIcons[CUSTOM_ICON_AMT]);
void custom_icons_update(CustomHudComplexStatusIcon customIcons[CUSTOM_ICON_AMT]);

void create_custom_status_icon(s32 iconId, s32 customIconId);
void remove_custom_status_icon(s32 iconId, s32 customIconId);

#endif
