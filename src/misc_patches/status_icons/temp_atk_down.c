//INCLUDE_IMG("ui/battle/status/pdown_0.png", ui_battle_status_pdown_0_png);
//INCLUDE_PAL("ui/battle/status/pdown_0.pal", ui_battle_status_pdown_0_pal);
//INCLUDE_IMG("ui/battle/status/pdown_1.png", ui_battle_status_pdown_1_png);
//INCLUDE_PAL("ui/battle/status/pdown_1.pal", ui_battle_status_pdown_1_pal);

extern unsigned short ui_battle_status_pdown_0_png[];
extern unsigned short ui_battle_status_pdown_0_pal[];
extern unsigned short ui_battle_status_pdown_1_png[];
extern unsigned short ui_battle_status_pdown_1_pal[];

#define NAMESPACE status_temp_atk_down

HudScript N(HES_Begin) = DEFAULT_STATUS_ICON_BEGIN(ui_battle_status_pdown_0, ui_battle_status_pdown_1);

HudScript N(HES_End) = DEFAULT_STATUS_ICON_END(ui_battle_status_pdown_0, ui_battle_status_pdown_1);
