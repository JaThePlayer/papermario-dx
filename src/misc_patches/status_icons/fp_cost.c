#include "misc_patches/custom_status_icons.h"
#include "include_asset.h"
#include "common.h"
#include "hud_element.h"

INCLUDE_IMG("ui/battle/status/fp_cost_0.png", ui_battle_status_fp_cost_0_png);
INCLUDE_PAL("ui/battle/status/fp_cost_0.pal", ui_battle_status_fp_cost_0_pal);
INCLUDE_IMG("ui/battle/status/fp_cost_1.png", ui_battle_status_fp_cost_1_png);
INCLUDE_PAL("ui/battle/status/fp_cost_1.pal", ui_battle_status_fp_cost_1_pal);

#define NAMESPACE status_icon_fp_cost_down

HudScript N(HES_Begin) = DEFAULT_STATUS_ICON_BEGIN(ui_battle_status_fp_cost_0, ui_battle_status_fp_cost_1);

HudScript N(HES_End) = DEFAULT_STATUS_ICON_END(ui_battle_status_fp_cost_0, ui_battle_status_fp_cost_1);
