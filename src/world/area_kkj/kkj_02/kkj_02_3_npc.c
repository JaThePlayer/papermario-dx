#include "kkj_02.h"

#include "hud_element.h"
#include "pause/pause_common.h"

#include "world/common/npc/Toad_Wander.inc.c"
#include "world/common/npc/Toad_Stationary.inc.c"

#include "sprite/npc/Nomadimouse.h"

#define NPCVar_NomadimouseMet 0

EvtScript N(EVS_NpcInteract_Toad) = {
    Call(SpeakToPlayer, NPC_SELF, ANIM_Toad_Red_Talk, ANIM_Toad_Red_Idle, 0, MSG_Intro_0049)
    Return
    End
};

EvtScript N(EVS_NpcInteract_ToadGuard) = {
    Call(SpeakToPlayer, NPC_SELF, ANIM_ToadGuard_Red_Talk, ANIM_ToadGuard_Red_Idle, 0, MSG_Intro_004A)
    Return
    End
};

EvtScript N(EVS_NpcInit_Toad) = {
    Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_Toad)))
    Return
    End
};

EvtScript N(EVS_NpcInit_ToadGuard) = {
    Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_ToadGuard)))
    Return
    End
};

NpcSettings N(NpcSettings_Nomadimouse) = {
    .height = 22,
    .radius = 25,
    .level = ACTOR_LEVEL_NONE,
};

s32 N(ChallengeItems)[] = {
    ITEM_ORB_OF_LUG,
    ITEM_MIDAS_STONE,
    ITEM_LONE_IDOL,
    ITEM_ASCETIC_NOTE,
};

extern s32 gPopupState;

s32 N(count_chosen_challenges)() {
    s32 count = 0;
    for (s32 i = 0; i < ARRAY_COUNT(N(ChallengeItems)); i++) {
        s32 itemId = N(ChallengeItems)[i];
        if (find_item(itemId) != -1) {
            count++;
        }
    }

    return count;
}

API_CALLABLE(N(CountChosenChallenges)) {
    evt_set_variable(script, *script->ptrReadPos, N(count_chosen_challenges)());

    return ApiStatus_DONE2;
}

void N(PopulateChallengeSelectorPopup)(PopupMenu* menu, Evt* script) {
    script->functionTemp[1] = 0;
    s32 menuPos = 0;
    for (s32 i = 0; i < ARRAY_COUNT(N(ChallengeItems)); i++) {
        s32 itemId = N(ChallengeItems)[i];

        if (find_item(itemId) == -1) {
            ItemData* item = &gItemTable[itemId];
            IconHudScriptPair* itemHudScripts = &gItemHudScripts[item->hudElemID];

            menu->userIndex[menuPos] = i;
            menu->nameMsg[menuPos] = item->nameMsg;
            menu->ptrIcon[menuPos] = itemHudScripts->enabled;

            menu->enabled[menuPos] = TRUE;

            menu->descMsg[menuPos] = item->fullDescMsg;
            menuPos++;
        } else {
            script->functionTemp[1] += 1;
        }
    }

    menu->numEntries = menuPos;
}

API_CALLABLE(N(ChallengeSelectorPopup)) {
    PlayerData* playerData = &gPlayerData;
    PopupMenu* menu;
    Enemy* enemy = script->owner1.enemy;

    if (isInitialCall) {
        // ret: how many challenges are picked
        script->functionTemp[1] = 0;

        script->functionTempPtr[2] = heap_malloc(sizeof(*menu));
        menu = script->functionTempPtr[2];
        N(PopulateChallengeSelectorPopup)(menu, script);
        menu->popupType = POPUP_MENU_CLAIM_ITEM;
        menu->initialPos = 0;
        create_standard_popup_menu(menu);
        script->functionTemp[0] = 0;
    }

    menu = script->functionTempPtr[2];

    if (script->functionTemp[0] == 0) {
        switch(menu->result) {
            case POPUP_RESULT_CHOOSING:
                return ApiStatus_BLOCK;
            case POPUP_RESULT_CANCEL:
                hide_popup_menu();
                break;
            default:
                add_item(N(ChallengeItems)[menu->userIndex[menu->result - 1]]);
                sfx_play_sound(SOUND_JUMP_SCARE);
                script->functionTemp[1]++;

                N(PopulateChallengeSelectorPopup)(menu, script);
                if (menu->numEntries == 0) {
                    hide_popup_menu();
                    break;
                }
                // Pickup next item
                gPopupState = POPUP_STATE_CHOOSING;
                menu->result = POPUP_RESULT_CHOOSING;
                return ApiStatus_BLOCK;
        }
    }
    script->functionTemp[0]++;
    if (script->functionTemp[0] < 20) {
        return ApiStatus_BLOCK;
    }

    destroy_popup_menu();

    script->varTable[0xE] = script->functionTemp[1];


    heap_free(script->functionTempPtr[2]);
    return ApiStatus_DONE2;
}

EvtScript N(EVS_NpcInteract_Nomadimouse) = {
    Call(GetNpcVar, NPC_Nomadimouse, NPCVar_NomadimouseMet, LVar0)
    IfEq(LVar0, FALSE)
        Call(SpeakToPlayer, NPC_SELF, ANIM_Nomadimouse_Talk, ANIM_Nomadimouse_Idle, 0, MSG_CH0_ChallengeIntroduction)
        Call(SetNpcVar, NPC_Nomadimouse, NPCVar_NomadimouseMet, TRUE)
    Else
        Call(N(CountChosenChallenges), LVar0)
        Switch(LVar0)
            CaseEq(0)
                Call(SpeakToPlayer, NPC_SELF, ANIM_Nomadimouse_Talk, ANIM_Nomadimouse_Idle, 0, MSG_CH0_ChallengeIntroductionAgainWithoutAny)
            CaseEq(ARRAY_COUNT(N(ChallengeItems)))
                Call(SpeakToPlayer, NPC_SELF, ANIM_Nomadimouse_Talk, ANIM_Nomadimouse_Idle, 0, MSG_CH0_ChallengeIntroductionAgainHasAll)
                Return
            CaseDefault
                Call(SpeakToPlayer, NPC_SELF, ANIM_Nomadimouse_Talk, ANIM_Nomadimouse_Idle, 0, MSG_CH0_ChallengeIntroductionAgainHasAny)
        EndSwitch
    EndIf

    Call(N(ChallengeSelectorPopup))
    Switch(LVarE)
        CaseEq(0)
            Set(LVar0, MSG_CH0_ChallengeRejected)
        CaseEq(1)
            Set(LVar0, MSG_CH0_ChallengeAccepted)
        CaseDefault
            Set(LVar0, MSG_CH0_ChallengeMoreThanOne)
    EndSwitch
    Call(SpeakToPlayer, NPC_SELF, ANIM_Nomadimouse_Talk, ANIM_Nomadimouse_Idle, 0, LVar0)

    Return
    End
};

EvtScript N(EVS_NpcInit_Nomadimouse) = {
    Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_Nomadimouse)))
    Call(SetNpcAnimation, NPC_SELF, ANIM_Nomadimouse_Idle)
    Call(SetNpcVar, NPC_Nomadimouse, NPCVar_NomadimouseMet, FALSE)
    Return
    End
};

EvtScript N(EVS_NpcInit_Sack) = {
    Call(SetNpcAnimation, NPC_SELF, ANIM_Nomadimouse_Sack)
    Return
    End
};

NpcData N(NpcData_Toads)[] = {
    {
        .id = NPC_ToadGuard,
        .pos = { 1175.0f, 110.0f, 60.0f },
        .yaw = 270,
        .init = &N(EVS_NpcInit_ToadGuard),
        .settings = &N(NpcSettings_Toad_Stationary),
        .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_4 | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING,
        .drops = NO_DROPS,
        .animations = TOAD_GUARD_RED_ANIMS,
    },
        {
        .id = NPC_Nomadimouse,
        .pos = { 0.0f, 0.0f, -100.0f },
        .yaw = 90,
        .init = &N(EVS_NpcInit_Nomadimouse),
        .settings = &N(NpcSettings_Nomadimouse),
        .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_NO_SHADOW_RAYCAST | ENEMY_FLAG_400000,
        .drops = NO_DROPS,
        .animations = {
            .idle   = ANIM_Nomadimouse_Idle,
            .walk   = ANIM_Nomadimouse_Idle,
            .run    = ANIM_Nomadimouse_Idle,
            .chase  = ANIM_Nomadimouse_Idle,
            .anim_4 = ANIM_Nomadimouse_Idle,
            .anim_5 = ANIM_Nomadimouse_Idle,
            .death  = ANIM_Nomadimouse_Idle,
            .hit    = ANIM_Nomadimouse_Idle,
            .anim_8 = ANIM_Nomadimouse_Idle,
            .anim_9 = ANIM_Nomadimouse_Idle,
            .anim_A = ANIM_Nomadimouse_Idle,
            .anim_B = ANIM_Nomadimouse_Idle,
            .anim_C = ANIM_Nomadimouse_Idle,
            .anim_D = ANIM_Nomadimouse_Idle,
            .anim_E = ANIM_Nomadimouse_Idle,
            .anim_F = ANIM_Nomadimouse_Idle,
        },
    },
    {
        .id = NPC_Sack,
        .pos = { 20.0f, 0.0f, -100.0f },
        .yaw = 90,
        .init = &N(EVS_NpcInit_Sack),
        .settings = &N(NpcSettings_Nomadimouse),
        .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_NO_SHADOW_RAYCAST | ENEMY_FLAG_400000,
        .drops = NO_DROPS,
        .animations = {
            .idle   = ANIM_Nomadimouse_Idle,
            .walk   = ANIM_Nomadimouse_Idle,
            .run    = ANIM_Nomadimouse_Idle,
            .chase  = ANIM_Nomadimouse_Idle,
            .anim_4 = ANIM_Nomadimouse_Idle,
            .anim_5 = ANIM_Nomadimouse_Idle,
            .death  = ANIM_Nomadimouse_Idle,
            .hit    = ANIM_Nomadimouse_Idle,
            .anim_8 = ANIM_Nomadimouse_Idle,
            .anim_9 = ANIM_Nomadimouse_Idle,
            .anim_A = ANIM_Nomadimouse_Idle,
            .anim_B = ANIM_Nomadimouse_Idle,
            .anim_C = ANIM_Nomadimouse_Idle,
            .anim_D = ANIM_Nomadimouse_Idle,
            .anim_E = ANIM_Nomadimouse_Idle,
            .anim_F = ANIM_Nomadimouse_Idle,
        },
    },
};

NpcGroupList N(DefaultNPCs) = {
    NPC_GROUP(N(NpcData_Toads)),
    {}
};


