#include "evt.h"
#include "functions.h"
#include "item_effects.h"
#include "macros.h"
#include "misc_patches/custom_status.h"
#include "script_api/battle.h"
#include "script_api/macros.h"

static API_CALLABLE(apply_status) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    Actor* actor = get_actor(actorID);
    b32 isPlayer = actor == gBattleStatus.playerActor;

    s32 userdata = evt_get_variable(script, *args++);
    s32 status = evt_get_variable(script, *args++);

    u8 potency = ITEM_EFFECTS_UNPACK_POTENCY(userdata);

    try_inflict_custom_status(actor, actor->curPos, status, isPlayer ? ITEM_EFFECTS_UNPACK_TURNS(userdata) : ITEM_EFFECTS_UNPACK_ENEMY_TURNS(userdata), potency, 100);

    return ApiStatus_DONE2;
}

static API_CALLABLE(apply_charge) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    Actor* actor = get_actor(actorID);

    s32 potency = evt_get_variable(script, *args++);

    try_inflict_custom_status(actor, actor->curPos, CHARGE_STATUS, 99, potency, 100);

    return ApiStatus_DONE2;
}

#define LVar9_Actor LVar9
#define LVarC_Userdata LVarC

#define CREATE_STANDARD_CUSTOM_EFFECT_APPLY(name, statusId) \
    EvtScript N(name##_apply) = { \
        Call(apply_status, LVar9_Actor, LVarC_Userdata, statusId) \
        Return \
        End \
    }; \
    ItemEffectType N(name) = { \
        .apply = &N(name##_apply), \
    }

CREATE_STANDARD_CUSTOM_EFFECT_APPLY(def_up, DEF_UP_TEMP_STATUS);
CREATE_STANDARD_CUSTOM_EFFECT_APPLY(atk_up, ATK_UP_TEMP_STATUS);
CREATE_STANDARD_CUSTOM_EFFECT_APPLY(def_down, DEF_DOWN_TEMP_STATUS);
CREATE_STANDARD_CUSTOM_EFFECT_APPLY(atk_down, ATK_DOWN_TEMP_STATUS);
CREATE_STANDARD_CUSTOM_EFFECT_APPLY(fp_cost, FP_COST_STATUS);
CREATE_STANDARD_CUSTOM_EFFECT_APPLY(poison, POISON_STATUS);
CREATE_STANDARD_CUSTOM_EFFECT_APPLY(charge, CHARGE_STATUS);

EvtScript N(heal_apply) = {
    IfNe(LVarC, 0)
        Thread
            Wait(5)
            Call(PlaySoundAtActor, LVar9_Actor, SOUND_RECOVER_HEART)
            Call(PlaySoundAtActor, LVar9_Actor, SOUND_HEART_BOUNCE)
            Wait(30)
            Call(PlaySoundAtActor, LVar9_Actor, SOUND_STAR_BOUNCE_A)
        EndThread
        Thread
            Call(FreezeBattleState, true)
            Call(HealActorNoPopupsOrEvents, LVar9_Actor, LVarC_Userdata, false)
            Call(FreezeBattleState, false)
        EndThread
    EndIf
    Call(WaitForBuffDone)

    Return
    End
};

ItemEffectType N(heal) = {
    .apply = &N(heal_apply),
};

EvtScript N(electrify_apply) = {
    Thread
        Wait(10)
        Loop(4)
            Call(PlaySoundAtActor, LVar9_Actor, SOUND_ELECTRIC_BUZZ)
            Call(RandInt, 3, LVar0)
            Add(LVar0, 3)
            Wait(LVar0)
        EndLoop
    EndThread
    Thread
        Call(FreezeBattleState, true)
        Call(ElectrifyActor, LVar9_Actor, LVarC_Userdata)
        Call(FreezeBattleState, false)
    EndThread
    Call(WaitForBuffDone)
    Wait(10)

    Return
    End
};

ItemEffectType N(electrify) = {
    .apply = &N(electrify_apply),
};

static API_CALLABLE(ItemClearStatus) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    Actor* actor = get_actor(actorID);

    s32 amt = 0;

    if (actor->debuff != 0) {
        actor->debuffDuration = 0;
        actor->debuff = 0;
        remove_status_debuff(actor->hudElementDataIndex);
        amt += 1;
    }

    if (actor->koStatus != 0) {
        actor->koDuration = 0;
        actor->koStatus = 0;
        actor->disableEffect->data.disableX->koDuration = 0;
        //amt += 1; Handled already by actor->debuff check above
    }

    amt += custom_status_clear_debuffs(actor);

    btl_update_ko_status();

    evt_set_variable(script, *args++, amt);

    return ApiStatus_DONE2;
}

static s32 get_actor_head_y_pos(Actor* actor) {
    if (!(actor->flags & ACTOR_FLAG_HALF_HEIGHT)) {
        return actor->curPos.y + actor->headOffset.y + actor->size.y;
    } else {
        return actor->curPos.y + actor->headOffset.y + actor->size.y / 2;
    }
}

static API_CALLABLE(RecoverFpFromCustomItemEffect) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    Actor* actor = get_actor(actorID);

    s32 amt = evt_get_variable(script, *args++);

    PlayerData* playerData = &gPlayerData;

    fx_recover(1, actor->curPos.x + 20, get_actor_head_y_pos(actor), actor->curPos.z, amt);

    if (actor == gBattleStatus.playerActor) {
        s32 newFP = playerData->curFP + amt;

        if (newFP > playerData->curMaxFP) {
            newFP = playerData->curMaxFP;
        }

        playerData->curFP = newFP;
    }

    return ApiStatus_DONE2;
}

EvtScript N(clear_status_apply) = {
    Call(ItemClearStatus, LVar9_Actor, LVar0)
    Return
    End
};

ItemEffectType N(clear_status) = {
    .apply = &N(clear_status_apply),
};

EvtScript N(tasty_tonic_apply) = {
    Call(ItemClearStatus, LVar9_Actor, LVar0)
    Mul(LVar0, 5)
    Call(RecoverFpFromCustomItemEffect, LVar9_Actor, LVar0)

    Return
    End
};

ItemEffectType N(tasty_tonic) = {
    .apply = &N(tasty_tonic_apply),
};

static API_CALLABLE(count_mush_power) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    Actor* actor = get_actor(actorID);

    evt_set_variable(script, *args++, badge_count_by_move_id(actor, MOVE_MUSH_POWER));

    return ApiStatus_DONE2;
}

EvtScript N(mush_power_apply) = {
    Call(count_mush_power, LVar9_Actor, LVar0)
    IfGt(LVar0, 0)
        Call(apply_charge, LVar9_Actor, LVar0)
    EndIf

    Return
    End
};

ItemEffectType N(mush_power) = {
    .apply = &N(mush_power_apply),
};

#undef LVar9_Actor
#undef LVarC_Userdata
