#include "hud_element.h"
#include "mac_04.h"

#include "common.h"
#include "pause/pause_common.h"
#include "misc_patches/misc_patches.h"

API_CALLABLE(N(GetPlayerCoins)) {
    script->varTable[0] = gPlayerData.coins;
    return ApiStatus_DONE2;
}

extern s32 gPopupState;

API_CALLABLE(N(SetNpcShadowScale)) {
    Bytecode* args = script->ptrReadPos;
    s32 npcID = evt_get_variable(script, *args++);
    f32 newShadowScale = evt_get_float_variable(script, *args++);

    resolve_npc(script, npcID)->shadowScale = newShadowScale;
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetStatusBarIgnoreChanges)) {
    status_bar_ignore_changes();
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetStatusBarRespondToChanges)) {
    status_bar_respond_to_changes();
    return ApiStatus_DONE2;
}

API_CALLABLE(N(ForceStatusBarToAppear)) {
    status_bar_respond_to_changes();
    open_status_bar_slowly();
    return ApiStatus_DONE2;
}

static s32 N(get_max_tokens)(PlayerData* playerData) {
    return playerData->level - 1;
};

static s32 N(get_spent_tokens)(PlayerData* playerData) {
    s32 spent = 0;
    spent += (playerData->hardMaxHP - 10) / 5;
    spent += (playerData->hardMaxFP - 5) / 5;
    spent += (playerData->maxBP - 3) / 3;

    return spent;
};

static s32 N(get_available_tokens)(PlayerData* playerData) {
    return N(get_max_tokens)(playerData) - N(get_spent_tokens)(playerData);
};

typedef void(*MenuEntryOnSelected)();

typedef struct MenuEntry {
    s32 nameMsg;
    s32 descMsg;
    s32 previewItemId;
    s32 cost;

    b8 closeMenuOnSelected;
    s8 hpChange;
    s8 fpChange;
    s8 bpChange;
} MenuEntry;

static MenuEntry N(Entries)[] = {
    {
        .nameMsg = MSG_ChetRippo_HpUp_Name,
        .descMsg = MSG_ChetRippo_HpUp_Desc,
        .previewItemId = ITEM_HP_PLUS_A,
        .cost = 1,
        .closeMenuOnSelected = false,
        .hpChange = 5,
        .fpChange = 0,
        .bpChange = 0,
    },
    {
        .nameMsg = MSG_ChetRippo_FpUp_Name,
        .descMsg = MSG_ChetRippo_FpUp_Desc,
        .previewItemId = ITEM_FP_PLUS_A,
        .cost = 1,
        .closeMenuOnSelected = false,
        .hpChange = 0,
        .fpChange = 5,
        .bpChange = 0,
    },
    {
        .nameMsg = MSG_ChetRippo_BpUp_Name,
        .descMsg = MSG_ChetRippo_BpUp_Desc,
        .previewItemId = ITEM_SP_PLUS, // TODO: BP_PLUS
        .cost = 1,
        .closeMenuOnSelected = false,
        .hpChange = 0,
        .fpChange = 0,
        .bpChange = 3,
    },
    {
        .nameMsg = MSG_ChetRippo_HpDown_Name,
        .descMsg = MSG_ChetRippo_HpDown_Desc,
        .previewItemId = ITEM_HP_PLUS_A,
        .cost = -1,
        .closeMenuOnSelected = false,
        .hpChange = -5,
        .fpChange = 0,
        .bpChange = 0,
    },
    {
        .nameMsg = MSG_ChetRippo_FpDown_Name,
        .descMsg = MSG_ChetRippo_FpDown_Desc,
        .previewItemId = ITEM_FP_PLUS_A,
        .cost = -1,
        .closeMenuOnSelected = false,
        .hpChange = 0,
        .fpChange = -5,
        .bpChange = 0,
    },
    {
        .nameMsg = MSG_ChetRippo_BpDown_Name,
        .descMsg = MSG_ChetRippo_BpDown_Desc,
        .previewItemId = ITEM_SP_PLUS, // TODO: BP_PLUS
        .cost = -1,
        .closeMenuOnSelected = false,
        .hpChange = 0,
        .fpChange = 0,
        .bpChange = -3,
    }
};

static b32 N(can_select)(MenuEntry* entry) {
    PlayerData* playerData = &gPlayerData;

    if (entry->hpChange < 0 && playerData->hardMaxHP <= -entry->hpChange)
        return false;
    if (entry->fpChange < 0 && playerData->hardMaxFP < -entry->fpChange)
        return false;
    if (entry->bpChange < 0 && playerData->maxBP < -entry->bpChange)
        return false;

    if (entry->hpChange > 0 && playerData->hardMaxHP + entry->hpChange > MAX_HP)
        return false;
    if (entry->fpChange > 0 && playerData->hardMaxFP + entry->fpChange > MAX_FP)
        return false;
    if (entry->bpChange > 0 && playerData->maxBP + entry->bpChange > MAX_BP)
        return false;

    return true;
}

static void N(adjust_stat_maxima)(s32 hp, s32 fp, s32 bp) {
    PlayerData* playerData = &gPlayerData;

    playerData->hardMaxHP += hp;
    playerData->curMaxHP += hp;
    if (playerData->curMaxHP > MAX_CURR_HP) {
        playerData->curMaxHP = MAX_CURR_HP;
    }
    if (playerData->curHP > playerData->curMaxHP) {
        playerData->curHP = playerData->curMaxHP;
    }

    playerData->hardMaxFP += fp;
    playerData->curMaxFP += fp;
    if (playerData->curMaxFP > MAX_CURR_FP) {
        playerData->curMaxFP = MAX_CURR_FP;
    }
    if (playerData->curFP > playerData->curMaxFP) {
        playerData->curFP = playerData->curMaxFP;
    }

    playerData->maxBP += bp;
    if (playerData->maxBP > MAX_BP) {
        playerData->maxBP = MAX_BP;
    }
}

static void N(ChetRippo_PopulatePopupEntries)(PopupMenu* menu, Evt* script) {
    s32 available_tokens = N(get_available_tokens)(&gPlayerData);
    s32 menuPos = 0;

    for (s32 i = 0; i < ARRAY_COUNT(N(Entries)); i++) {
        MenuEntry* trial = &N(Entries)[i];

        ItemData* item = &gItemTable[trial->previewItemId];
        IconHudScriptPair* itemHudScripts = &gItemHudScripts[item->hudElemID];

        menu->userIndex[menuPos] = i;
        menu->nameMsg[menuPos] = trial->nameMsg;
        menu->ptrIcon[menuPos] = itemHudScripts->enabled;

        menu->enabled[menuPos] = true;
        s32 cost = trial->cost;

        if (available_tokens < cost || !N(can_select)(trial)) {
            menu->ptrIcon[menuPos] = itemHudScripts->disabled;
            menu->enabled[menuPos] = false;
        }

        menu->descMsg[menuPos] = trial->descMsg;
        menu->value[menuPos] = cost;
        menuPos++;
    }

    menu->popupType = POPUP_MENU_TRADE_FOR_BADGE;
    menu->numEntries = menuPos;
    menu->userData[0] = (void*)(available_tokens == 0 ? 0xffffffff : available_tokens);
    menu->userData[1] = (void*)&HES_StatusStarPoint;
    menu->userData[2] = (void*)MSG_Menus_ChetRippo_UI_Left;
    menu->userData[3] = (void*)MSG_Menus_ChetRippo_UI_Title;
}

API_CALLABLE(N(ChetRippo_CreatePopup)) {
    PopupMenu* menu;
    s32 selected, i;

    if (isInitialCall) {
        script->functionTempPtr[2] = heap_malloc(sizeof(*menu));
        menu = script->functionTempPtr[2];
        mem_clear(menu, sizeof(*menu));

        N(ChetRippo_PopulatePopupEntries)(menu, script);

        menu->initialPos = 0;
        create_standard_popup_menu(menu);
        script->functionTemp[0] = 0;
    }

    menu = script->functionTempPtr[2];
    if (script->functionTemp[0] == 0) {
        script->functionTemp[1] = menu->result;
        if (script->functionTemp[1] != POPUP_RESULT_CHOOSING) {
            i = menu->userIndex[menu->result - 1];
            MenuEntry* selectedItem = &N(Entries)[i];

            if (selectedItem->closeMenuOnSelected || script->functionTemp[1] == POPUP_RESULT_CANCEL) {
                hide_popup_menu();
            } else {
                N(adjust_stat_maxima)(selectedItem->hpChange, selectedItem->fpChange, selectedItem->bpChange);
                N(ChetRippo_PopulatePopupEntries)(menu, script);
                if (menu->numEntries == 0) {
                    hide_popup_menu();
                } else {
                    gPopupState = POPUP_STATE_CHOOSING;
                    menu->result = POPUP_RESULT_CHOOSING;
                    return ApiStatus_BLOCK;
                }
            }

        } else {
            return ApiStatus_BLOCK;
        }
    }
    script->functionTemp[0]++;
    if (script->functionTemp[0] < 20) {
        return ApiStatus_BLOCK;
    }

    destroy_popup_menu();
    selected = script->functionTemp[1];
    if (selected != POPUP_RESULT_CANCEL) {
        i = menu->userIndex[selected - 1];
        MenuEntry* selectedItem = &N(Entries)[i];

        script->varTablePtr[0xE] = selectedItem;
    } else {
        script->varTable[0xE] = nullptr;
    }

    heap_free(script->functionTempPtr[2]);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(HideCoinCounterImmediately)) {
    hide_coin_counter_immediately();
    return ApiStatus_DONE2;
}

EvtScript N(EVS_NpcInteract_ChetRippo) = {
    IfEq(GF_MAC04_Met_ChetRippo, false)
        Set(LVar0, MSG_MAC_Housing_00A8)
        //Set(GF_MAC04_Met_ChetRippo, true)
    Else
        Set(LVar0, MSG_MAC_Housing_00A9)
    EndIf
    Call(N(SetStatusBarIgnoreChanges))
    Call(SpeakToPlayer, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, LVar0)
    Call(ShowCoinCounter, true)
    Call(ShowChoice, MSG_Choice_000E)
    IfNe(LVar0, 0)
        Call(ShowCoinCounter, false)
        Call(ContinueSpeech, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, MSG_MAC_Housing_00AA)
        Call(N(SetStatusBarRespondToChanges))
        Return
    EndIf
    Call(N(GetPlayerCoins))
    IfLt(LVar0, 39)
        Call(ShowCoinCounter, false)
        Call(ContinueSpeech, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, MSG_MAC_Housing_00AB)
        Call(N(SetStatusBarRespondToChanges))
        Return
    EndIf

    Call(AddCoin, -39)
    Wait(10)
    Call(N(HideCoinCounterImmediately))
    Wait(3)
    IfEq(GF_MAC04_Met_ChetRippo, false)
        Set(LVar0, MSG_MAC_Housing_00AC_FirstTime)
        Set(GF_MAC04_Met_ChetRippo, true)
    Else
        Set(LVar0, MSG_MAC_Housing_00AC)
    EndIf
    Call(ContinueSpeech, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, LVar0)

    Call(N(ChetRippo_CreatePopup))

    Call(N(EnforceNewStatLimits), LVar6)
    IfEq(LVar6, 0)
        Set(LVar7, MSG_MAC_Housing_00AF)
    Else
        Set(LVar7, MSG_MAC_Housing_00B0)
    EndIf
    Call(SpeakToPlayer, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, LVar7)

    Wait(10)
    Call(GetNpcPos, NPC_ChetRippo, LVar0, LVar1, LVar2)
    Call(PlaySoundAtNpc, NPC_ChetRippo, SOUND_VANISH_IN_SMOKE, SOUND_SPACE_DEFAULT)
    PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar0, LVar1, LVar2, 1, 1, 1, 1)
    Call(SetNpcPos, NPC_ChetRippo, NPC_DISPOSE_LOCATION)
    //Set(AF_MAC_32, true)
    Call(N(ForceStatusBarToAppear))
    /*
    Call(N(GetCurrentStatValues))
    Call(SetMessageValue, LVar0, 0)
    Call(SetMessageValue, LVar1, 1)
    Call(SetMessageValue, LVar2, 2)
    Call(ContinueSpeech, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, MSG_MAC_Housing_00AC)
    Call(ShowChoice, MSG_Choice_000B)
    IfEq(LVar0, 3)
        Call(ShowCoinCounter, false)
        Call(ContinueSpeech, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, MSG_MAC_Housing_00AD)
        Call(N(SetStatusBarRespondToChanges))
        Return
    EndIf
    Set(LVar2, 0)
    Call(N(GetCurrentStatMaxima), LVar3, LVar4, LVar5)
    Switch(LVar0)
        CaseEq(0)
            IfGt(LVar3, 40)
                Set(LVar2, 1)
            EndIf
            Set(LVar1, MSG_Menus_00D1)
        CaseEq(1)
            IfGt(LVar4, 40)
                Set(LVar2, 1)
            EndIf
            Set(LVar1, MSG_Menus_00D2)
        CaseEq(2)
            IfGt(LVar5, 24)
                Set(LVar2, 1)
            EndIf
            Set(LVar1, MSG_Menus_00D3)
    EndSwitch
    Set(LVarA, LVar0)
    IfEq(LVar2, 1)
        Call(ShowCoinCounter, false)
        Call(ContinueSpeech, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, MSG_MAC_Housing_00B1)
        Call(N(SetStatusBarRespondToChanges))
        Return
    EndIf
    Call(AddCoin, -39)
    Call(ShowCoinCounter, false)
    Call(SetMessageText, LVar1, 0)
    Call(ContinueSpeech, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, MSG_MAC_Housing_00AE)
    Wait(10)
    Call(SetNpcAnimation, NPC_ChetRippo, ANIM_ChetRippo_Spread)
    Wait(10)
    Call(PlaySoundAtNpc, NPC_ChetRippo, SOUND_MERLEE_APPEAR, SOUND_SPACE_DEFAULT)
    Call(GetNpcPos, NPC_ChetRippo, LVar6, LVar7, LVar8)
    Add(LVar7, 26)
    PlayEffect(EFFECT_SHIMMER_BURST, 0, LVar6, LVar7, LVar8, 1, 60)
    Call(SetNpcAnimation, NPC_ChetRippo, ANIM_ChetRippo_ShakeSlow)
    Wait(80)
    Call(SetNpcAnimation, NPC_ChetRippo, ANIM_ChetRippo_Idle)
    Call(N(AdjustStatMaxima))
    Call(N(EnforceNewStatLimits), LVar6)
    IfEq(LVar6, 0)
        Set(LVar7, MSG_MAC_Housing_00AF)
    Else
        Set(LVar7, MSG_MAC_Housing_00B0)
    EndIf
    Call(SpeakToPlayer, NPC_ChetRippo, ANIM_ChetRippo_Talk, ANIM_ChetRippo_Idle, 0, LVar7)
    Wait(10)
    Call(GetNpcPos, NPC_ChetRippo, LVar0, LVar1, LVar2)
    Call(PlaySoundAtNpc, NPC_ChetRippo, SOUND_VANISH_IN_SMOKE, SOUND_SPACE_DEFAULT)
    PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar0, LVar1, LVar2, 1, 1, 1, 1)
    Call(SetNpcPos, NPC_ChetRippo, NPC_DISPOSE_LOCATION)
    Set(AF_MAC04_UsedChetRippo, true)
    Call(N(ForceStatusBarToAppear))
    */
    Return
    End
};

EvtScript N(EVS_NpcInit_ChetRippo) = {
    IfEq(AF_MAC04_UsedChetRippo, false)
        Set(LVar0, 0)
    Else
        Set(LVar0, 1)
    EndIf
    IfEq(LVar0, 0)
        Call(SetNpcCollisionSize, NPC_SELF, 38, 52)
        Call(N(SetNpcShadowScale), -1, Float(2.0))
        Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_DIRTY_SHADOW, true)
        Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_ChetRippo)))
    Else
        Call(RemoveNpc, NPC_SELF)
    EndIf
    Return
    End
};
