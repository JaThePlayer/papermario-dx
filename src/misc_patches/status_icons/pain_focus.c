#include "misc_patches/custom_status_icons.h"
#include "include_asset.h"
#include "common.h"
#include "hud_element.h"

INCLUDE_IMG("ui/battle/status/pain_focus_0.png", ui_battle_status_pain_focus_0_png);
INCLUDE_PAL("ui/battle/status/pain_focus_0.pal", ui_battle_status_pain_focus_0_pal);
INCLUDE_IMG("ui/battle/status/pain_focus_1.png", ui_battle_status_pain_focus_1_png);
INCLUDE_PAL("ui/battle/status/pain_focus_1.pal", ui_battle_status_pain_focus_1_pal);

#define NAMESPACE status_icon_pain_focus

HudScript N(HES_Begin) = DEFAULT_STATUS_ICON_BEGIN(ui_battle_status_pain_focus_0, ui_battle_status_pain_focus_1);

HudScript N(HES_End) = DEFAULT_STATUS_ICON_END(ui_battle_status_pain_focus_0, ui_battle_status_pain_focus_1);
