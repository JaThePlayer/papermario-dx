#include "common.h"
#include "script_api/battle.h"
#include "effects.h"
#include "entity.h"
#include "sprite/player.h"
#include "enemy_items/api.h"

#define NAMESPACE battle_item_life_shroom

extern EntityModelScript EMS_StarIcon;

#include "battle/common/move/ItemRefund.inc.c"

API_CALLABLE(N(func_802A123C_72E76C)) {
    Bytecode* args = script->ptrReadPos;
    s32 a = evt_get_variable(script, *args++);
    s32 b = evt_get_variable(script, *args++);
    s32 c = evt_get_variable(script, *args++);
    ItemEntity* item = get_item_entity(script->varTable[14]);

    item->pos.x = a;
    item->pos.y = b;
    item->pos.z = c;

    return ApiStatus_DONE2;
}

API_CALLABLE(N(func_802A12EC_72E81C)) {
    BattleStatus* battleStatus = &gBattleStatus;
    Actor* partner = battleStatus->partnerActor;
    Bytecode* args = script->ptrReadPos;

    if (partner->koDuration == 0) {
        return ApiStatus_DONE2;
    }

    partner->koDuration -= evt_get_variable(script, *args++);
    if (partner->koDuration < 0) {
        partner->koDuration = 0;
    }
    if (partner->koDuration > 0) {
        partner->disableEffect->data.disableX->koDuration = partner->koDuration;
    } else {
        partner->koStatus = 0;
        dispatch_event_partner(EVENT_RECOVER_FROM_KO);
        partner->disableEffect->data.disableX->koDuration = 0;
    }

    return ApiStatus_DONE2;
}

API_CALLABLE(N(SpawnHeartRecoveryFX)) {
    Bytecode* args = script->ptrReadPos;
    s32 a = evt_get_variable(script, *args++);
    s32 b = evt_get_variable(script, *args++);
    s32 c = evt_get_variable(script, *args++);
    s32 d = evt_get_variable(script, *args++);

    fx_recover(0, a, b, c, d);

    return ApiStatus_DONE2;
}

API_CALLABLE(N(func_802A1438_72E968)) {
    ItemData* item = &gItemTable[ITEM_LIFE_SHROOM];
    PlayerData* playerData = &gPlayerData;

    playerData->curHP += item->potencyA;
    if (playerData->curHP > playerData->curMaxHP) {
        playerData->curHP = playerData->curMaxHP;
    }

    script->varTable[3] = item->potencyA;

    return ApiStatus_DONE2;
}

API_CALLABLE(N(func_802A1484_72E9B4)) {
    Bytecode* args = script->ptrReadPos;
    s32 itemIdx = evt_get_variable(script, *args++);
    ItemData* itemTable = gItemTable;
    ItemData* item = &itemTable[itemIdx];

    script->varTable[11] = item->potencyA;
    script->varTable[12] = item->potencyB;

    itemIdx = script->varTable[11] < 0 || (script->varTable[11] <= 0 && script->varTable[12] < 0);
    script->varTable[15] = itemIdx;

    return ApiStatus_DONE2;
}

#include "battle/common/move/UseItem.inc.c"

EvtScript N(EVS_UseOnPartner) = {
    Call(SetActorYaw, ACTOR_PLAYER, 30)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 60)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 90)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 120)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 150)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 180)
    Wait(10)
    Set(LVar1, LVarF)
    ExecWait(N(UseItem))
    Set(LVarE, LVarA)
    Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Throw)
    Thread
        Wait(20)
        Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Idle)
    EndThread
    Call(CreateVirtualEntity, LVarA, Ref(EMS_StarIcon))
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, 30)
    Add(LVar2, 5)
    Call(SetVirtualEntityPosition, LVarA, LVar0, LVar1, LVar2)
    Call(SetVirtualEntityFlags, LVarA, 32)
    Thread
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Add(LVar1, 45)
        Call(MakeItemEntity, LVarE, LVar0, LVar1, LVar2, ITEM_SPAWN_MODE_DECORATION, 0)
        Set(LVarE, LVar0)
        Loop(25)
            Call(GetVirtualEntityPosition, LVarA, LVar0, LVar1, LVar2)
            Call(N(func_802A123C_72E76C), LVar0, LVar1, LVar2)
            Wait(1)
        EndLoop
        Call(RemoveItemEntity, LVarE)
    EndThread
    Call(InitTargetIterator)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetBattleCamTarget, -125, 1, 0)
    Call(SetBattleCamOffsetZ, 41)
    Call(SetBattleCamZoom, 248)
    Call(MoveBattleCamOver, 25)
    Call(SetVirtualEntityJumpGravity, LVarA, Float(0.4))
    Add(LVar2, 5)
    Call(VirtualEntityJumpTo, LVarA, LVar0, LVar1, LVar2, 25)
    Call(AddBattleCamZoom, 100)
    Call(MoveBattleCamOver, 30)
    Call(GetActorPos, ACTOR_PARTNER, LVar0, LVar1, LVar2)
    Add(LVar1, 25)
    Call(ShowStartRecoveryShimmer, LVar0, LVar1, LVar2, LVarB)
    IfGt(LVarB, 0)
        Call(N(func_802A12EC_72E81C), LVarB)
    EndIf
    Wait(30)
    Call(GetActorPos, ACTOR_PARTNER, LVar0, LVar1, LVar2)
    Call(ShowRecoveryShimmer, LVar0, LVar1, LVar2, LVarB)
    Wait(20)
    Call(SetActorYaw, ACTOR_PLAYER, 150)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 120)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 90)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 60)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 30)
    Wait(1)
    Call(SetActorYaw, ACTOR_PLAYER, 0)
    Wait(10)
    Return
    End
};

EvtScript N(EVS_UseItem) = {
    STANDARD_HEALING_ITEM_ENEMY_USE()

    EVT_SET(LVarF, LVar1)
    EVT_CALL(GetMenuSelection, LVar0, LVar1, LVar2)
    EVT_SET(LVarA, LVar1)
    EVT_CALL(N(func_802A1484_72E9B4), LVarA)
    EVT_CALL(InitTargetIterator)
    EVT_CALL(GetOwnerTarget, LVar0, LVar1)
    EVT_IF_EQ(LVar0, ACTOR_PARTNER)
        EVT_EXEC_WAIT(N(EVS_UseOnPartner))
        EVT_RETURN
    EVT_END_IF
    EVT_SET_CONST(LVarA, ITEM_LIFE_SHROOM)
    EVT_SET(LVar1, LVarF)
    EVT_EXEC_WAIT(N(UseItemWithEffect))
    EVT_EXEC_WAIT(N(EatItem))
    EVT_CALL(N(func_802A1438_72E968))
    EVT_CALL(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    EVT_ADD(LVar0, 0)
    EVT_ADD(LVar1, 35)
    EVT_CALL(N(SpawnHeartRecoveryFX), LVar0, LVar1, LVar2, LVar3)
    EVT_CALL(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    EVT_ADD(LVar1, 25)
    EVT_CALL(ShowStartRecoveryShimmer, LVar0, LVar1, LVar2, LVar3)
    EVT_WAIT(10)
    EVT_CALL(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_ThumbsUp)
    EVT_WAIT(30)
    EVT_CALL(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    EVT_CALL(ShowRecoveryShimmer, LVar0, LVar1, LVar2, LVar3)
    EVT_CALL(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Idle)
    EVT_WAIT(20)
    EVT_EXEC_WAIT(N(PlayerGoHome))
    EVT_RETURN
    EVT_END
};
