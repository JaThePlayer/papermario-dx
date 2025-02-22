#include "custom_status_icons.h"
#include "include_asset.h"
#include "common.h"
#include "hud_element.h"
#include "custom_status.h"

extern HudScript HES_PoisonedBegin;
extern HudScript HES_PoisonedEnd;

typedef struct StatusIconType {
    HudScript* beginHudScript;
    HudScript* endHudScript;
    u8 customStatusid;
} StatusIconType;

#define STATUS_ICON_ENTRY(namespace, status) { \
        .beginHudScript = &namespace##_HES_Begin, \
        .endHudScript = &namespace##_HES_End, \
        .customStatusid = status, \
    }

#include "status_icons/temp_def_down.c"
#include "status_icons/temp_atk_down.c"
#include "status_icons/burn.c"
#include "status_icons/fp_cost.c"
#include "status_icons/charge.c"
#include "status_icons/poison.c"
#include "status_icons/pain_focus.c"

StatusIconType gCustomStatusIconTypes[CUSTOM_ICON_AMT] = {
    [DEF_DOWN_ICON_ID] = STATUS_ICON_ENTRY(status_temp_def_down, DEF_DOWN_TEMP_STATUS),
    [ATK_DOWN_ICON_ID] = STATUS_ICON_ENTRY(status_temp_atk_down, ATK_DOWN_TEMP_STATUS),
    [DEF_UP_ICON_ID] = STATUS_ICON_ENTRY(status_temp_def_up, DEF_UP_TEMP_STATUS),
    [ATK_UP_ICON_ID] = STATUS_ICON_ENTRY(status_temp_atk_up, ATK_UP_TEMP_STATUS),
    [BURN_ICON_ID] = STATUS_ICON_ENTRY(status_icon_burn, BURN_STATUS),
    [FP_COST_DOWN_ICON_ID] = STATUS_ICON_ENTRY(status_icon_fp_cost_down, FP_COST_STATUS),
    [CHARGE_ICON_ID] = STATUS_ICON_ENTRY(status_icon_charge, CHARGE_STATUS),
    [POISON_ICON_ID] = {
        .beginHudScript = &HES_PoisonedBegin,
        .endHudScript = &HES_PoisonedEnd,
        .customStatusid = POISON_STATUS,
    },
    [PAIN_FOCUS_ICON_ID] = STATUS_ICON_ENTRY(status_icon_pain_focus, PAIN_FOCUS_STATUS),
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

void create_custom_status_icon(Actor* target, s32 customIconId) {
    CustomHudComplexStatusIcon* icons = get_custom_status_icons(target->hudElementDataIndex);
    CustomHudComplexStatusIcon* icon = &icons[customIconId];
    s32 customStatusId = gCustomStatusIconTypes[customIconId].customStatusid;
    StatusInfo* customStatus = &target->customStatuses[customStatusId];

    if (!icon->active) {
        icon->active = TRUE;
        icon->activeTask = TRUE;
    }

    if (gCustomStatusTypes[customStatusId].hasTurnCount &&
        (gGameStatus.frameCounter % 120 < 60 || customStatus->potency == 1 || customStatus->potency == -1)) {
        icon->turns = customStatus->turns;
        icon->palette = MSG_PAL_WHITE;
    }
    else {
        icon->turns = customStatus->potency;
        icon->palette = MSG_PAL_YELLOW;
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
