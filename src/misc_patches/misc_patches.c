#include "common.h"
#include "custom_status.h"

// returns how much fp costs are changed by
s32 getFpCostChange(Actor* player) {
    s32 fpCost = 0;

    fpCost -= player_team_is_ability_active(player, ABILITY_FLOWER_SAVER);
    fpCost -= player_team_is_ability_active(player, ABILITY_FLOWER_FANATIC) * 2;
    fpCost += custom_status_get_potency(player, FP_COST_STATUS);

    return fpCost;
}

s32 getDamageChangeFromStatus(Actor* actor) {
    s32 change = 0;

    change += custom_status_get_potency(actor, ATK_UP_TEMP_STATUS);
    change -= custom_status_get_potency(actor, ATK_DOWN_TEMP_STATUS);
    change += custom_status_get_potency(actor, CHARGE_STATUS);

    return change;
}

void clearChargesFrom(Actor* actor) {
    custom_status_clear(actor, CHARGE_STATUS);
}

API_CALLABLE(GetPartnerKoDuration) {
    evt_set_variable(script, *script->ptrReadPos, gBattleStatus.partnerActor->koDuration);
    return ApiStatus_DONE2;
}

// actor, chanceToTargetPlayer(in percent), out isPlayer
API_CALLABLE(TargetPlayerOrPartner) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    s32 chanceToTargetPlayer = evt_get_variable(script, *args++);

    actor->targetPartID = 1;
    if (gBattleStatus.partnerActor->koDuration > 0 || (rand_int(100) < chanceToTargetPlayer)) {
        actor->targetActorID = ACTOR_PLAYER;
        evt_set_variable(script, *args++, TRUE);
    } else {
        actor->targetActorID = ACTOR_PARTNER;
        evt_set_variable(script, *args++, FALSE);
    }

    return ApiStatus_DONE2;
}

// Used by Trial Boo, needs to be somewhere global
void* gCurrentTrial = NULL;
s32 gCurrentTrialFormationId = 0;

void _onActorCtor(Actor* actor) {
    enemy_items_zero_initialize(actor);
    custom_status_zero_initialize(actor);
    actor->attackedThisTurn = FALSE;
    actor->overridenLevel = -1;
}

API_CALLABLE(OverrideActorLevel) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    s32 level = evt_get_variable(script, *args++);

    actor->overridenLevel = level;

    return ApiStatus_DONE2;
}
