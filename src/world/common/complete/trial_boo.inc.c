#include "common.h"
#include "message_ids.h"
#include "effects.h"
#include "model.h"
#include "sprite/npc/Boo.h"
#include "sprite/player.h"
#include "hud_element.h"
#include "pause/pause_common.h"

#ifndef TRIAL_BOO_NPC_ID
    #error TRIAL_BOO_NPC_ID must be defined for trial_boo.inc.c
#endif

#include "world/common/complete/GiveReward.inc.c"

typedef struct Trial {
    s32 gameFlag;
    s32 nameMsg;
    s32 descMsg;
    s32 previewItemId;
    s32 storyProgress;
    s32 coinCost;
    s32 rewardItemId;
    u8 rewardStarPieces;
    u8 rewardSp;
    s16* formations;
} Trial;

extern void* gCurrentTrial;
extern s32 gCurrentTrialFormationId;

s16 N(Ch1Formations)[] = { BTL_NOK_FORMATION_00, BTL_NOK_FORMATION_00, 0 };

Trial N(BooTrials)[] = {
    {
        .gameFlag = GF_BooTrial_Ch1,
        .formations = &N(Ch1Formations),
        .nameMsg = MSG_BooTrial_Ch1_Name,
        .descMsg = MSG_BooTrial_Ch1_Desc,
        .previewItemId = ITEM_MUSHROOM,
        .storyProgress = STORY_CH1_STAR_SPRIT_DEPARTED,
        .coinCost = 10,
        .rewardItemId = ITEM_ATTACK_FX_C,
        .rewardStarPieces = 1,
        .rewardSp = 20,
    },
    {
        .gameFlag = GF_BooTrial_Ch1,
        .formations = &N(Ch1Formations),
        .nameMsg = MSG_BooTrial_Ch1_Name,
        .descMsg = MSG_BooTrial_Ch1_Desc,
        .previewItemId = ITEM_MUSHROOM,
        .storyProgress = STORY_CH1_STAR_SPRIT_DEPARTED,
        .coinCost = 333,
        .rewardItemId = ITEM_ATTACK_FX_C,
        .rewardStarPieces = 1,
        .rewardSp = 20,
    }
};

b8 is_trial_completed(Trial* trial) {
    return evt_get_variable(NULL, trial->gameFlag);
}

API_CALLABLE(N(BooTrials_TrialSelectorPopup)) {
    PlayerData* playerData = &gPlayerData;
    PopupMenu* menu;
    Enemy* enemy = script->owner1.enemy;
    s32 selected, menuPos, i;

    if (isInitialCall) {
        s32 storyProgress = evt_get_variable(NULL, GB_StoryProgress);

        script->functionTempPtr[2] = heap_malloc(sizeof(*menu));
        menu = script->functionTempPtr[2];
        menuPos = 0;
        for (i = 0; i < ARRAY_COUNT(N(BooTrials)); i++) {
            Trial* trial = &N(BooTrials)[i];

            if (storyProgress >= trial->storyProgress) {
                ItemData* item = &gItemTable[trial->previewItemId];
                IconHudScriptPair* itemHudScripts = &gItemHudScripts[item->hudElemID];

                menu->userIndex[menuPos] = i;
                menu->nameMsg[menuPos] = trial->nameMsg;
                menu->ptrIcon[menuPos] = itemHudScripts->enabled;

                menu->enabled[menuPos] = TRUE;
                s32 cost = is_trial_completed(trial) ? 0 : trial->coinCost;
                if (playerData->coins < cost) {
                    menu->ptrIcon[menuPos] = itemHudScripts->disabled;
                    menu->enabled[menuPos] = FALSE;
                }

                menu->descMsg[menuPos] = trial->descMsg;
                menu->value[menuPos] = cost;
                menu->userData[menuPos] = is_trial_completed(trial) ? MSG_PAL_YELLOW : MSG_PAL_STANDARD;
                menuPos++;
            }
        }
        menu->popupType = POPUP_MENU_TRIAL_SELECT;
        menu->numEntries = menuPos;
        menu->initialPos = 0;
        create_standard_popup_menu(menu);
        script->functionTemp[0] = 0;
    }

    menu = script->functionTempPtr[2];
    if (script->functionTemp[0] == 0) {
        script->functionTemp[1] = menu->result;
        if (script->functionTemp[1] != POPUP_RESULT_CHOOSING) {
            hide_popup_menu();
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
        Trial* selectedItem = &N(BooTrials)[i];

        script->varTable[0xE] = selectedItem;

        gCurrentTrialFormationId = 0;
        gCurrentEncounter.encounterList[enemy->encounterIndex]->battle = selectedItem->formations[0];
        sp_pool_setup_fake_pool(is_trial_completed(selectedItem) ? 0 : selectedItem->rewardSp);

        playerData->coins -= menu->value[selected - 1];
    } else {
        script->varTable[0xE] = NULL;
    }

    gCurrentTrial = script->varTable[0xE];

    heap_free(script->functionTempPtr[2]);
    return ApiStatus_DONE2;
}

#define TRIAL_BOO_ANIMS \
{ \
    .idle   = ANIM_Boo_Idle, \
    .walk   = ANIM_Boo_Walk, \
    .run    = ANIM_Boo_Run, \
    .chase  = ANIM_Boo_Run, \
    .anim_4 = ANIM_Boo_Idle, \
    .anim_5 = ANIM_Boo_Idle, \
    .death  = ANIM_Boo_Still, \
    .hit    = ANIM_Boo_Still, \
    .anim_8 = ANIM_Boo_Run, \
    .anim_9 = ANIM_Boo_Run, \
    .anim_A = ANIM_Boo_Run, \
    .anim_B = ANIM_Boo_Run, \
    .anim_C = ANIM_Boo_Run, \
    .anim_D = ANIM_Boo_Run, \
    .anim_E = ANIM_Boo_Run, \
    .anim_F = ANIM_Boo_Run, \
}

extern EvtScript N(EVS_TrialBoo_Conversation);

EvtScript N(EVS_TrialBoo_NPC_Aux) = {
    Return
    End
};

EvtScript N(EVS_TrialBoo_NPC_OtherAI) = {
    /*
    Call(N(Quizmo_ShouldAppear))
    IfEq(LVar0, 0)
        Return
    EndIf
    */
    Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_HAS_NO_SPRITE, FALSE)
    Call(SetNpcSprite, -1, ANIM_Boo_Idle)
    Return
    End
};

EvtScript N(EVS_TrialBoo_NPC_Interact) = {
    ExecWait(N(EVS_TrialBoo_Conversation))
    Return
    End
};

NpcSettings N(NpcSettings_TrialBoo) = {
    .defaultAnim = ANIM_Boo_Idle,
    .height = 35,
    .radius = 28,
    .otherAI = &N(EVS_TrialBoo_NPC_OtherAI),
    .onInteract = &N(EVS_TrialBoo_NPC_Interact),
    .aux = &N(EVS_TrialBoo_NPC_Aux),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING,
    .level = ACTOR_LEVEL_NONE,
};

API_CALLABLE(N(CheckForNextBattle)) {
    Trial* trial = gCurrentTrial;
    Enemy* enemy = script->owner1.enemy;

    gCurrentTrialFormationId++;
    if (trial->formations[gCurrentTrialFormationId] == NULL) {
        script->varTable[0x9] = FALSE;
        return ApiStatus_DONE2;
    }

    script->varTable[0x9] = TRUE;
    gCurrentEncounter.encounterList[enemy->encounterIndex]->battle = trial->formations[gCurrentTrialFormationId];

    return ApiStatus_DONE2;
}

API_CALLABLE(N(GetTrialRewardInfoAndMarkAsCompleted)) {
    Trial* trial = gCurrentTrial;

    if (trial == NULL || is_trial_completed(trial)) {
        script->varTable[0x9] = FALSE;
    } else {
        evt_set_variable(NULL, trial->gameFlag, TRUE);
        script->varTable[0x9] = TRUE;
        script->varTable[0xA] = trial->rewardStarPieces;
        script->varTable[0xB] = trial->rewardItemId;
    }

    return ApiStatus_DONE2;
}

EvtScript N(EVS_NpcDefeat_TrialBoo) = {
    Call(GetBattleOutcome, LVar0)
    Call(SetPlayerAnimation, ANIM_Mario1_Idle)

    Call(N(CheckForNextBattle))
    IfTrue(LVar9)
        Call(SpeakToPlayer, NPC_SELF, ANIM_Boo_Talk, ANIM_Boo_Idle, 0, MSG_BooTrial_Intoduction_SwappingFormation)
        Call(DisablePlayerInput, FALSE)
        Call(StartBossBattle)
        Return
    EndIf

    Call(N(GetTrialRewardInfoAndMarkAsCompleted))
    IfTrue(LVar9)
        // first time
        Call(SpeakToPlayer, NPC_SELF, ANIM_Boo_Talk, ANIM_Boo_Idle, 0, MSG_BooTrial_Intoduction_TrialSuccessFirstTime)
        IfGt(LVarA, 0)
            EVT_GIVE_STAR_PIECES(LVarA)
        EndIf
        IfNe(LVarB, ITEM_NONE)
            EVT_GIVE_REWARD(LVarB)
        EndIf
    Else
        // not first time
        Call(SpeakToPlayer, NPC_SELF, ANIM_Boo_Talk, ANIM_Boo_Idle, 0, MSG_BooTrial_Intoduction_TrialSuccess)
    EndIf

    Call(DisablePlayerInput, FALSE)
    Return
    End
};

EvtScript N(EVS_TrialBoo_Conversation) = {
    IfFalse(GF_BooTrial_IntroductionInToadTown)
        Set(GF_BooTrial_IntroductionInToadTown, TRUE)
        Call(SpeakToPlayer, NPC_SELF, ANIM_Boo_Talk, ANIM_Boo_Idle, 0, MSG_BooTrial_Intoduction_BeforeCh3)
    Else
        Call(SpeakToPlayer, NPC_SELF, ANIM_Boo_Talk, ANIM_Boo_Idle, 0, MSG_BooTrial_Intoduction_Again)
    EndIf

    Call(N(BooTrials_TrialSelectorPopup))
    IfEq(LVarE, 0)
        // Cancel
        Return
    EndIf

    // if option to cancel after picking a trial is added, make sure to delay creating a fake SP pool until this line
    Call(BindNpcDefeat, TRIAL_BOO_NPC_ID, Ref(N(EVS_NpcDefeat_TrialBoo)))
    Call(StartBossBattle)
    Return
    End
};
