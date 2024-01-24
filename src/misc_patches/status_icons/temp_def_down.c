INCLUDE_IMG("ui/battle/status/defdown_0.png", ui_battle_status_defdown_0_png);
INCLUDE_PAL("ui/battle/status/defdown_0.pal", ui_battle_status_defdown_0_pal);
INCLUDE_IMG("ui/battle/status/defdown_1.png", ui_battle_status_defdown_1_png);
INCLUDE_PAL("ui/battle/status/defdown_1.pal", ui_battle_status_defdown_1_pal);

#define NAMESPACE status_temp_def_down

HudScript N(HES_Begin) = {
    hs_SetVisible
    hs_SetCustomSize(16, 16)
    hs_SetAlpha(30)
    hs_SetTexelOffset(10, -10)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(50)
    hs_SetTexelOffset(8, -8)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(80)
    hs_SetTexelOffset(6, -6)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(120)
    hs_SetTexelOffset(5, -5)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(170)
    hs_SetTexelOffset(4, -4)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(200)
    hs_SetTexelOffset(3, -3)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(220)
    hs_SetTexelOffset(2, -2)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(240)
    hs_SetTexelOffset(1, -1)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(255)
    hs_SetTexelOffset(0, 0)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_Loop
        hs_SetCI(6, ui_battle_status_defdown_0)
        hs_SetCI(6, ui_battle_status_defdown_1)
    hs_Restart
    hs_End
};

HudScript N(HES_End) = {
    hs_SetVisible
    hs_SetCustomSize(16, 16)
    hs_SetAlpha(255)
    hs_SetTexelOffset(0, 0)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(240)
    hs_SetTexelOffset(1, -1)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(220)
    hs_SetTexelOffset(2, -2)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(200)
    hs_SetTexelOffset(3, -3)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(170)
    hs_SetTexelOffset(4, -4)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(120)
    hs_SetTexelOffset(5, -5)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(80)
    hs_SetTexelOffset(6, -6)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(50)
    hs_SetTexelOffset(8, -8)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_SetAlpha(30)
    hs_SetTexelOffset(10, -10)
    hs_SetCI(1, ui_battle_status_defdown_0)
    hs_End
};
