#include "custom_status_icons.h"
#include "include_asset.h"

typedef struct StatusIconType {
    HudScript* beginHudScript;
    HudScript* endHudScript;
} StatusIconType;

#define STATUS_ICON_ENTRY(namespace) { \
        .beginHudScript = &namespace##_HES_Begin, \
        .endHudScript = &namespace##_HES_End, \
    }

#include "status_icons/temp_def_down.c"

StatusIconType gCustomStatusIconTypes[CUSTOM_ICON_AMT] = {
    [DEF_DOWN_ICON_ID] = STATUS_ICON_ENTRY(status_temp_def_down),
};

void custom_status_icons_init(CustomHudComplexStatusIcon customIcons[CUSTOM_ICON_AMT]) {
    for (s32 i = 0; i < CUSTOM_ICON_AMT; i++) {
        CustomHudComplexStatusIcon* icon = &customIcons[i];

        icon->active = 0;
        icon->removing = 0;
        icon->activeTask = 0;
        icon->removingTask = 0;
    }
}

void custom_icons_update(CustomHudComplexStatusIcon customIcons[CUSTOM_ICON_AMT]) {
    for (s32 i = 0; i < CUSTOM_ICON_AMT; i++) {
        CustomHudComplexStatusIcon* icon = &customIcons[i];
        StatusIconType* iconType = &gCustomStatusIconTypes[i];

        if (icon->activeTask == STATUS_ICON_TASK_LOAD) {
            s32 elementID = icon->activeElementID = hud_element_create(iconType->beginHudScript);
            hud_element_set_flags(elementID, HUD_ELEMENT_FLAG_DISABLED);
            hud_element_set_flags(elementID, HUD_ELEMENT_FLAG_80);
            icon->activeTask = STATUS_ICON_TASK_DRAW;
        }

        switch (icon->removingTask) {
            case STATUS_ICON_TASK_NONE:
                break;
            case STATUS_ICON_TASK_LOAD:
                hud_element_set_script(icon->removingElementID, iconType->endHudScript);
                icon->frameCounter = 20;
                icon->removingTask = STATUS_ICON_TASK_DRAW;
                break;
            case STATUS_ICON_TASK_DRAW:
                if (icon->frameCounter != 0) {
                    icon->frameCounter--;
                } else {
                    icon->removing = 0;
                    hud_element_free(icon->removingElementID);
                    icon->removingTask = STATUS_ICON_TASK_NONE;
                }
                break;
        }
    }
}

extern CustomHudComplexStatusIcon* get_custom_status_icons(s32 iconID);

void create_custom_status_icon(s32 iconId, s32 customIconId) {
    CustomHudComplexStatusIcon* icons = get_custom_status_icons(iconId);
    CustomHudComplexStatusIcon* icon = &icons[customIconId];

    if (!icon->active) {
        icon->active = TRUE;
        icon->activeTask = TRUE;
    }
}

void remove_custom_status_icon(s32 iconId, s32 customIconId) {
    CustomHudComplexStatusIcon* icons = get_custom_status_icons(iconId);
    CustomHudComplexStatusIcon* statusIcon = &icons[customIconId];

    if (statusIcon->active && !statusIcon->removing) {
        statusIcon->removing = statusIcon->active;
        statusIcon->removingTask = TRUE;
        statusIcon->active = FALSE;
        statusIcon->activeTask = FALSE;
        statusIcon->frameCounter = 10;
        statusIcon->removingElementID = statusIcon->activeElementID;
    }
}
