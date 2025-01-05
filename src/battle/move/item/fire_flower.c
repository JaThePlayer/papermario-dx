#include "common.h"
#include "script_api/battle.h"
#include "effects.h"
#include "model.h"
#include "sprite/player.h"
#include "misc_patches/custom_status.h"

#define NAMESPACE battle_item_fire_flower

#include "battle/common/move/ItemRefund.inc.c"

API_CALLABLE(N(func_802A123C_716E9C)) {
    Bytecode* args = script->ptrReadPos;
    s32 a = evt_get_variable(script, *args++);
    s32 b = evt_get_variable(script, *args++);
    s32 c = evt_get_variable(script, *args++);
    s32 isFlipped = evt_get_variable(script, *args++);

    fx_fire_flower(isFlipped, a, b, c, 0);

    return ApiStatus_DONE2;
}

#include "common/FadeBackgroundDarken.inc.c"
#include "common/FadeBackgroundLighten.inc.c"

#include "battle/common/move/UseItem.inc.c"

EvtScript N(EVS_UseItem_Enemy) = {
    SetConst(LVarA, ITEM_FIRE_FLOWER)
    Call(N(FadeBackgroundDarken))
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Wait(10)
    Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
    Set(LVar0, -40)
    Call(MultiplyByActorScale, LVar0)
    Add(LVar3, LVar0)

    Thread
        Call(N(func_802A123C_716E9C), LVar3, LVar4, LVar5, TRUE)
        Wait(25)
        Loop(12)
            Wait(1)
            Call(PlaySound, SOUND_FIRE_FLOWER_A)
            Wait(2)
            Call(PlaySound, SOUND_FIRE_FLOWER_B)
            Wait(1)
        EndLoop
    EndThread
    Wait(80)

    // hit player
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyTestTarget, ACTOR_SELF, LVarA, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarA)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Goto(1)
        EndCaseGroup
        CaseDefault
    EndSwitch
    Call(GetItemPower, ITEM_FIRE_FLOWER, LVar0, LVar1)
    Call(SetNextAttackCustomStatus, BURN_STATUS, 4, 1, 100) // 4 turn t1 burn
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_FIRE | DAMAGE_TYPE_IGNORE_DEFENSE | DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_MULTIPLE_POPUPS, 0, 0, LVar0, BS_FLAGS1_TRIGGER_EVENTS)
    Label(1)
    Wait(8)
    // hit partner
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PARTNER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyTestTarget, ACTOR_SELF, LVarA, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarA)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Goto(2)
        EndCaseGroup
        CaseDefault
    EndSwitch
    // no burn, 1 atk always
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_FIRE | DAMAGE_TYPE_IGNORE_DEFENSE | DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_MULTIPLE_POPUPS, 0, 0, 1, BS_FLAGS1_TRIGGER_EVENTS)
    Label(2)

    Call(N(FadeBackgroundLighten))
    Wait(30)
    Return
    End
};

EvtScript N(EVS_UseItem) = {
    Call(EnemyItems_IsCalledByEnemy, LVarD) // the first arg specifies which LVar to store the output value of the function to.
    IfEq(LVarD, 1)
        ExecWait(N(EVS_UseItem_Enemy))
        Return
    EndIf

    SetConst(LVarA, ITEM_FIRE_FLOWER)
    ExecWait(N(UseItemWithEffect))
    Call(N(FadeBackgroundDarken))
    Call(PlaySound, SOUND_PLANT_FIRE_FLOWER)
    Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_PlantFireFlower)
    Thread
        Wait(50)
        Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Idle)
    EndThread
    Wait(35)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Wait(10)
    Call(GetActorPos, ACTOR_PLAYER, LVar3, LVar4, LVar5)
    Set(LVar0, 40)
    Call(MultiplyByActorScale, LVar0)
    Add(LVar3, LVar0)
    Thread
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Sub(LVar0, 10)
        Call(SetActorSpeed, ACTOR_PLAYER, Float(2.0))
        Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Run)
        Call(SetGoalPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Call(PlayerRunToGoal, 0)
        Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Idle)
    EndThread
    Thread
        Call(N(func_802A123C_716E9C), LVar3, LVar4, LVar5, FALSE)
        Wait(25)
        Loop(12)
            Wait(1)
            Call(PlaySound, SOUND_FIRE_FLOWER_A)
            Wait(2)
            Call(PlaySound, SOUND_FIRE_FLOWER_B)
            Wait(1)
        EndLoop
    EndThread
    Wait(80)
    Call(UseBattleCamPreset, BTL_CAM_VIEW_ENEMIES)
    Call(MoveBattleCamOver, 20)
    Call(InitTargetIterator)
    Label(0)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(ItemCheckHit, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, LVar0, 0)
        IfEq(LVar0, HIT_RESULT_MISS)
            Goto(1)
        EndIf
        Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(GetItemPower, ITEM_FIRE_FLOWER, LVar0, LVar1)
        Call(SetNextAttackCustomStatus, BURN_STATUS, 4, 1, 100) // 4 turn t1 burn
        Call(ItemDamageEnemy, LVar0, DAMAGE_TYPE_FIRE | DAMAGE_TYPE_IGNORE_DEFENSE | DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_MULTIPLE_POPUPS, 0, LVar0, BS_FLAGS1_TRIGGER_EVENTS)
        Label(1)
        Wait(5)
        Call(ChooseNextTarget, ITER_NEXT, LVar0)
        IfNe(LVar0, ITER_NO_MORE)
            Goto(0)
        EndIf
    Call(N(FadeBackgroundLighten))
    Wait(30)
    ExecWait(N(PlayerGoHome))
    Return
    End
};
