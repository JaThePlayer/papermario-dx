INCLUDE_IMG("ui/battle/status/defdown_0.png", ui_battle_status_defdown_0_png);
INCLUDE_PAL("ui/battle/status/defdown_0.pal", ui_battle_status_defdown_0_pal);
INCLUDE_IMG("ui/battle/status/defdown_1.png", ui_battle_status_defdown_1_png);
INCLUDE_PAL("ui/battle/status/defdown_1.pal", ui_battle_status_defdown_1_pal);

#define NAMESPACE status_temp_def_down

HudScript N(HES_Begin) = DEFAULT_STATUS_ICON_BEGIN(ui_battle_status_defdown_0, ui_battle_status_defdown_1);

HudScript N(HES_End) = DEFAULT_STATUS_ICON_END(ui_battle_status_defdown_0, ui_battle_status_defdown_1);


INCLUDE_IMG("ui/battle/status/defup_0.png", ui_battle_status_defup_0_png);
INCLUDE_PAL("ui/battle/status/defup_0.pal", ui_battle_status_defup_0_pal);
INCLUDE_IMG("ui/battle/status/defup_1.png", ui_battle_status_defup_1_png);
INCLUDE_PAL("ui/battle/status/defup_1.pal", ui_battle_status_defup_1_pal);
/*
INCLUDE_IMG("ui/battle/status/defdown_0.png", ui_battle_status_defup_0_png);
INCLUDE_PAL("ui/battle/status/defdown_0.pal", ui_battle_status_defup_0_pal);
INCLUDE_IMG("ui/battle/status/defdown_1.png", ui_battle_status_defup_1_png);
INCLUDE_PAL("ui/battle/status/defdown_1.pal", ui_battle_status_defup_1_pal);*/

#define NAMESPACE status_temp_def_up

HudScript N(HES_Begin) = DEFAULT_STATUS_ICON_BEGIN(ui_battle_status_defup_1, ui_battle_status_defup_0);

HudScript N(HES_End) = DEFAULT_STATUS_ICON_END(ui_battle_status_defup_1, ui_battle_status_defup_0);

