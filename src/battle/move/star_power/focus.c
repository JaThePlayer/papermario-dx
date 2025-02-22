#include "common.h"
#include "script_api/battle.h"
#include "sprite/player.h"
#include "misc_patches/misc_patches.h"
#include "misc_patches/custom_status.h"

#define NAMESPACE battle_move_focus

#include "battle/common/move/StarPowerSupport.inc.c"

enum {
    RESTORE_NOT_FULL        = 0,
    RESTORE_ALREADY_FULL    = 1,
    RESTORE_NOW_FULL        = 2,
};

s32 count_enemies() {
    BattleStatus* battleStatus = &gBattleStatus;
    s32 i;
    s32 count = 0;

    for (i = 0; i < ARRAY_COUNT(battleStatus->enemyActors); i++) {
        Actor* targetActor = battleStatus->enemyActors[i];
        if (targetActor == NULL)
            continue;
        if ((targetActor->flags & (ACTOR_FLAG_NO_DMG_APPLY | ACTOR_FLAG_TARGET_ONLY)))
            continue;

        count++;
    }

    return count;
}

s32 get_base_sp_recovery(b32 isPartner) {
    s32 sp = 0;
    switch (get_focus_move_id())
    {
        case MOVE_THREAT_FOCUS:
            sp += count_enemies() * SP_PER_SEG * 2;
            break;
        case MOVE_PAIN_FOCUS:
            try_inflict_custom_status(gBattleStatus.playerActor, gBattleStatus.playerActor->curPos, PAIN_FOCUS_STATUS, 1, 1, 100);
            break;
        default:
            sp += SP_PER_SEG * 4;
            break;
    }

    return sp;
}

ApiStatus RestoreStarPowerImpl(Evt* script, s32 isInitialCall, b32 isPartner) {
    PlayerData* playerData = &gPlayerData;
    s32 sp;

    script->varTable[0] = RESTORE_NOT_FULL;

    gBattleStatus.focusUses++;

    if (playerData->starPower >= playerData->maxStarPower * SP_PER_BAR
        || gBattleStatus.focusUses > get_focus_cap()) {
        script->varTable[0] = RESTORE_ALREADY_FULL;
        return ApiStatus_DONE2;
    }

    sp = get_base_sp_recovery(isPartner);
    if (!isPartner) {
        sp += is_ability_active(ABILITY_DEEP_FOCUS) * SP_PER_SEG * 2;
        sp += is_ability_active(ABILITY_SUPER_FOCUS) * SP_PER_SEG * 4;
    }

    add_star_power(sp);

    if (playerData->starPower == playerData->maxStarPower * SP_PER_BAR) {
        script->varTable[0] = RESTORE_NOW_FULL;
    }

    return ApiStatus_DONE2;
}

API_CALLABLE(N(RestoreStarPowerFromPlayer)) {
    return RestoreStarPowerImpl(script, isInitialCall, FALSE);
}

API_CALLABLE(N(RestoreStarPowerFromPartner)) {
    return RestoreStarPowerImpl(script, isInitialCall, TRUE);
}

EvtScript N(EVS_UsePower) = {
    ExecWait(N(EVS_StarPower_WishForSpirit))
    Call(EnableBattleStatusBar, TRUE)
    Call(GetOwnerID, LVarA)
    IfEq(LVarA, ACTOR_PLAYER)
        Call(PlaySound, SOUND_STAR_POWER_RECOVERY)
        Call(N(RestoreStarPowerFromPlayer))
        Wait(30)
        Call(N(FadeBackgroundLighten))
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Switch(LVar0)
            CaseEq(RESTORE_NOT_FULL)
                Call(ShowMessageBox, BTL_MSG_STAR_POWER_RECHARGED, 60)
            CaseEq(RESTORE_ALREADY_FULL)
                Call(ShowMessageBox, BTL_MSG_STAR_POWER_MAXED, 60)
            CaseEq(RESTORE_NOW_FULL)
                Call(ShowMessageBox, BTL_MSG_STAR_POWER_FILLED, 60)
        EndSwitch
        Call(WaitForMessageBoxDone)
        Call(PlayerYieldTurn)
        Call(SetGoalToHome, ACTOR_PLAYER)
        Call(SetActorSpeed, ACTOR_PLAYER, Float(8.0))
        Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Run)
        Call(PlayerRunToGoal, 0)
        Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Idle)
    Else
        Call(PlaySound, SOUND_STAR_POWER_RECOVERY)
        Call(N(RestoreStarPowerFromPartner))
        Wait(30)
        Call(N(FadeBackgroundLighten))
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Switch(LVar0)
            CaseEq(RESTORE_NOT_FULL)
                Call(ShowMessageBox, BTL_MSG_STAR_POWER_RECHARGED, 60)
            CaseEq(RESTORE_ALREADY_FULL)
                Call(ShowMessageBox, BTL_MSG_STAR_POWER_MAXED, 60)
            CaseEq(RESTORE_NOW_FULL)
                Call(ShowMessageBox, BTL_MSG_STAR_POWER_FILLED, 60)
        EndSwitch
        Call(WaitForMessageBoxDone)
        Call(PartnerYieldTurn)
        Call(SetGoalToHome, ACTOR_PARTNER)
        Call(SetActorSpeed, ACTOR_PARTNER, Float(8.0))
        Call(N(SetPartnerWishAnim), PARTNER_WISH_ANIM_RETURN)
        Call(RunToGoal, ACTOR_PARTNER, 0)
        Call(N(SetPartnerWishAnim), PARTNER_WISH_ANIM_IDLE)
    EndIf
    Return
    End
};
