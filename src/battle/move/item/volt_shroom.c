#include "common.h"
#include "script_api/battle.h"
#include "effects.h"
#include "sprite/player.h"
#include "misc_patches/custom_status.h"

#define NAMESPACE battle_item_volt_shroom

#include "battle/common/move/ItemRefund.inc.c"

API_CALLABLE(N(func_802A123C_71AA2C)) {
    BattleStatus* battleStatus = &gBattleStatus;
    Actor* player = battleStatus->playerActor;

    inflict_status(player, STATUS_KEY_STATIC, script->varTable[0]);
    player->statusAfflicted = 0;
    return ApiStatus_DONE2;
}

#include "battle/common/move/UseItem.inc.c"

EvtScript N(EVS_UseItem_Enemy) = {
    Call(GetOwnerTarget, LVar8, 0)

    Call(GetActorPos, LVar8, LVar0, LVar1, LVar2)
    Set(LVar3, 20)
    Call(MultiplyByActorScale, LVar3)
    Add(LVar1, LVar3)
    SetF(LVar3, Float(1.0))
    Call(MultiplyByActorScale, LVar3)
    PlayEffect(EFFECT_SNAKING_STATIC, 0, LVar0, LVar1, LVar2, LVar3, 30, 0)
    Call(PlaySound, SOUND_VOLT_SHROOM_APPLY)

    Thread
        Wait(10)
        Loop(4)
            Call(PlaySoundAtActor, LVar8, SOUND_ELECTRIC_BUZZ)
            Call(RandInt, 3, LVar5)
            Add(LVar5, 3)
            Wait(LVar5)
        EndLoop
    EndThread
    Thread
        Call(FreezeBattleState, TRUE)
        Call(GetItemPower, ITEM_VOLT_SHROOM, LVar0, LVar1)
        Call(ElectrifyActor, LVar8, LVar0)
        Call(FreezeBattleState, FALSE)
    EndThread
    Call(WaitForBuffDone)

    Return
    End
};

EvtScript N(EVS_UseItem) = {
    Call(EnemyItems_IsCalledByEnemy, LVarD) // the first arg specifies which LVar to store the output value of the function to.
    IfEq(LVarD, 1)
        ExecWait(N(EVS_UseItem_Enemy))
        Return
    EndIf

    SetConst(LVarA, ITEM_VOLT_SHROOM)
    ExecWait(N(UseItemWithEffect))
    ExecWait(N(EatItem))
    Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Idle)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Set(LVar3, 20)
    Call(MultiplyByActorScale, LVar3)
    Add(LVar1, LVar3)
    SetF(LVar3, Float(1.0))
    Call(MultiplyByActorScale, LVar3)
    PlayEffect(EFFECT_SNAKING_STATIC, 0, LVar0, LVar1, LVar2, LVar3, 30, 0)
    Call(PlaySound, SOUND_VOLT_SHROOM_APPLY)
    Call(GetItemPower, ITEM_VOLT_SHROOM, LVar0, LVar1)
    Call(N(func_802A123C_71AA2C))
    Wait(20)
    Call(ShowMessageBox, BTL_MSG_PLAYER_CHARGED, 60)
    Call(WaitForMessageBoxDone)
    ExecWait(N(PlayerGoHome))
    Return
    End
};
