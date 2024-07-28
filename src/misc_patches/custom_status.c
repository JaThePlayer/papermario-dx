#include "custom_status.h"

#include "status_rework_helpers.c"

#include "statuses/temp_def_down.c"
#include "statuses/temp_atk_down.c"
#include "statuses/temp_def_up.c"
#include "statuses/temp_atk_up.c"
#include "statuses/close_call_status.c"
#include "statuses/burn.c"
#include "statuses/fp_cost.c"
#include "statuses/charge.c"
#include "statuses/poison.c"

#define STATUS_ENTRY(namespace, _isDebuff, _hasTurnCount, _stacking) { \
        .onApply = &namespace##_on_apply, \
        .drawIcon = &namespace##_create_icon, \
        .onRemoveIcon = &namespace##_remove_icon, \
        .decrementLate = namespace##_DECREMENT_LATE, \
        .onDecrement = &namespace##_on_decrement, \
        .isDebuff = _isDebuff, \
        .hasTurnCount = _hasTurnCount, \
        .stackingBehaviour = _stacking, \
    }

StatusType gCustomStatusTypes[CUSTOM_STATUS_AMT] = {
    [NONE_CUSTOM_STATUS] = {},
    [ATK_DOWN_TEMP_STATUS] = STATUS_ENTRY(temp_atk_down, TRUE, TRUE, STATUS_STACKING_OVERRIDE),
    [DEF_DOWN_TEMP_STATUS] = STATUS_ENTRY(temp_def_down, TRUE, TRUE, STATUS_STACKING_OVERRIDE),
    [ATK_UP_TEMP_STATUS] = STATUS_ENTRY(temp_atk_up, FALSE, TRUE, STATUS_STACKING_OVERRIDE),
    [DEF_UP_TEMP_STATUS] = STATUS_ENTRY(temp_def_up, FALSE, TRUE, STATUS_STACKING_OVERRIDE),
    [CLOSE_CALL_STATUS] = STATUS_ENTRY(close_call, FALSE, TRUE, STATUS_STACKING_OVERRIDE),
    [BURN_STATUS] = STATUS_ENTRY(burn_status, TRUE, TRUE, STATUS_STACKING_BURN),
    [FP_COST_STATUS] = STATUS_ENTRY(fp_cost_status, FALSE, TRUE, STATUS_STACKING_OVERRIDE),
    [CHARGE_STATUS] = STATUS_ENTRY(charge_status, FALSE, FALSE, STATUS_STACKING_ADD_POTENCY),
    [POISON_STATUS] = STATUS_ENTRY(poison_status, TRUE, TRUE, STATUS_STACKING_OVERRIDE),
};

StatusInfo* custom_status_get_info(Actor* actor, s8 customStatusId) {
    return &actor->customStatuses[customStatusId];
}

// Gets the potency of the given status for the given actor. 0 if actor doesn't have this status
s8 custom_status_get_potency(Actor* actor, s8 customStatusId) {
    StatusInfo* status = &actor->customStatuses[customStatusId];

    if (status->turns > 0)
        return status->potency;
    return 0;
}

static void custom_status_decrease_turn_count_impl(Actor* actor, u8 newTurns, StatusInfo* status, StatusType* statusType,
                                                   b32 callOnDecrement) {
    if (callOnDecrement && statusType->onDecrement) {
        statusType->onDecrement(actor);
    }

    status->turns = newTurns;

    if (status->turns == 0) {
        // status just got removed now
        status->potency = 0;
        if (statusType->onRemoveIcon) {
            statusType->onRemoveIcon(actor->hudElementDataIndex);
        }
    }
}

static void custom_status_decrement_impl(Actor* actor, s8 isLate) {
    for (s32 i = 0; i < ARRAY_COUNT(actor->customStatuses); i++)
    {
        StatusInfo* status = &actor->customStatuses[i];
        StatusType* statusType = &gCustomStatusTypes[i];

        if (statusType->hasTurnCount && statusType->decrementLate == isLate && status->turns > 0) {
            s32 decrement = 1;

            if (i == BURN_STATUS && badge_count_by_move_id_in_opposing_team(actor, MOVE_EMBER_EMBLEM) > 0) {
                decrement = 2;
            }

            if (status->turns < decrement)
                decrement = status->turns;

            custom_status_decrease_turn_count_impl(actor, status->turns - decrement, status, statusType, TRUE);
        }
    }
}

// Decrements all custom statuses for the given actor
void custom_status_decrement(Actor* actor) {
    custom_status_decrement_impl(actor, FALSE);
}

void custom_status_decrement_late(Actor* actor) {
    custom_status_decrement_impl(actor, TRUE);
}

void custom_status_zero_initialize(Actor* actor) {
    for (s32 i = 0; i < ARRAY_COUNT(actor->customStatuses); i++)
    {
        StatusInfo* status = &actor->customStatuses[i];
        status->turns = 0;
        status->potency = 0;
    }
}

s32 try_inflict_custom_status(Actor* actor, Vec3f position, s8 customStatusId, u8 turns, u8 potency, s32 chance) {
    StatusInfo* status = &actor->customStatuses[customStatusId];
    StatusType* statusType = &gCustomStatusTypes[customStatusId];

    // Read status table
    s32 actorChance = 100;
    s32 actorTurnMod = 0;
    s32* statusTable = actor->statusTable;

    if (chance <= 100) { // allow passing >100 chance to prevent status table from doing anything (like for copying status)
        while (statusTable[DICTIONARY_KEY] != STATUS_END) {
            if (statusTable[DICTIONARY_KEY] == STATUS_KEY_CUSTOM(customStatusId)) {
                actorChance = statusTable[DICTIONARY_VALUE];
            }

            if (statusTable[DICTIONARY_KEY] == STATUS_TURN_MOD_CUSTOM(customStatusId)) {
                actorTurnMod = statusTable[DICTIONARY_VALUE];
            }
            statusTable += DICTIONARY_SIZE;
        }
    }

    chance = (chance * actorChance) / 100;

    if (chance <= 0 || chance < rand_int(100)) {
        return FALSE;
    }

    turns += actorTurnMod;

    s8 stackBehaviour = statusType->stackingBehaviour;

    // Handle stack behaviours that toggle different behaviours based on some condition (like badges)
    switch (stackBehaviour) {
        case STATUS_STACKING_BURN:
            stackBehaviour = badge_count_by_move_id_in_opposing_team(actor, MOVE_EMBER_EMBLEM) > 0 ? STATUS_STACKING_ADD_TURNS : STATUS_STACKING_OVERRIDE;
            break;
    }

    switch (stackBehaviour)
    {
        case STATUS_STACKING_OVERRIDE:
            status->turns = turns;
            status->potency = potency;
            break;
        case STATUS_STACKING_ADD_POTENCY:
            status->turns = turns;
            status->potency += potency;
            if (status->potency > 99 || status->potency < 0) {
                status->potency = 99;
            }
            break;
        case STATUS_STACKING_ADD_TURNS:
            if (status->potency == potency) {
                status->turns += turns;
            }
            if (status->potency < potency) {
                status->turns = turns;
                status->potency = potency;
            }
    }


    if (statusType->onApply) {
        statusType->onApply(actor, position, potency);
    }

    if (statusType->drawIcon) {
        statusType->drawIcon(actor);
    }

    actor->flags |= ACTOR_FLAG_SHOW_STATUS_ICONS;

    return TRUE;
}

NextAttackStatus gNextAttackStatuses[CUSTOM_STATUS_AMT];
s32 gNextAttackStatusCount;

void set_next_attack_custom_status(s8 customStatusId, u8 turns, u8 potency, u8 chance) {
    if (customStatusId == NONE_CUSTOM_STATUS) {
        gNextAttackStatusCount = 0;
        return;
    }

    if (gNextAttackStatusCount < ARRAY_COUNT(gNextAttackStatuses)) {
        gNextAttackStatuses[gNextAttackStatusCount].id = customStatusId;
        gNextAttackStatuses[gNextAttackStatusCount].turns = turns;
        gNextAttackStatuses[gNextAttackStatusCount].potency = potency;
        gNextAttackStatuses[gNextAttackStatusCount].chance = chance;

        gNextAttackStatusCount += 1;
    }
}

s32 inflict_next_attack_statuses(Actor* attacker, Actor* target, Vec3f position) {
    s32 inflicted = FALSE;
    while (gNextAttackStatusCount > 0) {
        NextAttackStatus* st = &gNextAttackStatuses[gNextAttackStatusCount - 1];
        inflicted |= try_inflict_custom_status(target,
            position,
            st->id,
            st->turns,
            st->potency,
            st->chance
        );

        gNextAttackStatusCount--;
    }

    if (attacker != NULL) {
        // Koopa Curse - poison spreading
        if (badge_count_by_move_id_in_both_teams(MOVE_SLOW_GO) > 0 && !(gBattleStatus.curAttackElement & DAMAGE_TYPE_NO_CONTACT)) {
            StatusInfo* attackerPoison = custom_status_get_info(attacker, POISON_STATUS);
            if (attackerPoison->potency > 0) {
                try_inflict_custom_status(target, position, POISON_STATUS, attackerPoison->turns, attackerPoison->potency, 100);
            }
        }
    }

    return inflicted;
}

// (id, turns, potency, chance)
API_CALLABLE(SetNextAttackCustomStatus) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;
    s32 id, turns, potency, chance;

    id = evt_get_variable(script, *args++);
    turns = evt_get_variable(script, *args++);
    potency = evt_get_variable(script, *args++);
    chance = evt_get_variable(script, *args++);

    set_next_attack_custom_status(id, turns, potency, chance);

    return ApiStatus_DONE2;
}

// (actorId, customStatusId, turns, potency, chance)
API_CALLABLE(InflictCustomStatus) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    s32 id = evt_get_variable(script, *args++);
    s32 turns = evt_get_variable(script, *args++);
    s32 potency = evt_get_variable(script, *args++);
    s32 chance = evt_get_variable(script, *args++);

    try_inflict_custom_status(actor, actor->curPos, id, turns, potency, chance);

    return ApiStatus_DONE2;
}

// (actorId, customStatusId, out var ret)
API_CALLABLE(GetCustomStatusTurns) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    s32 id = evt_get_variable(script, *args++);

    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    evt_set_variable(script, *args++, custom_status_get_info(actor, id)->turns);
    return ApiStatus_DONE2;
}

// (actorId, customStatusId, out var ret)
API_CALLABLE(GetCustomStatusPotency) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    s32 id = evt_get_variable(script, *args++);

    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    evt_set_variable(script, *args++, custom_status_get_potency(actor, id));
    return ApiStatus_DONE2;
}

void custom_status_render_all_icons(Actor* actor) {
    for (s32 i = 0; i < ARRAY_COUNT(actor->customStatuses); i++)
    {
        StatusInfo* status = &actor->customStatuses[i];

        if (status->turns > 0) {
            StatusType* statusType = &gCustomStatusTypes[i];
            if (statusType->drawIcon) {
                statusType->drawIcon(actor);
            }
        }
    }
}

void custom_status_remove_icons(s32 iconId) {
    for (s32 i = 0; i < ARRAY_COUNT(gCustomStatusTypes); i++)
    {
        StatusType* statusType = &gCustomStatusTypes[i];

        if (statusType->onRemoveIcon) {
            statusType->onRemoveIcon(iconId);
        }
    }
}

Vec3f get_expected_arrow_pos(Actor* actor) {
    Vec3f res = {};
    s32 x, y, z;

    s32 flags = actor->flags;
    x = actor->curPos.x + actor->headOffset.x + actor->size.x / 2;
    if (flags & ACTOR_FLAG_UPSIDE_DOWN) {
        y = actor->curPos.y + actor->headOffset.y - actor->size.y;
    } else if (!(flags & ACTOR_FLAG_HALF_HEIGHT)) {
        y = actor->curPos.y + actor->headOffset.y + actor->size.y;
    } else {
        y = actor->curPos.y + actor->headOffset.y + actor->size.y * 2;
    }
    z = actor->curPos.z + actor->headOffset.z + 10.0f;

    res.x = x;
    res.y = y;
    res.z = z;

    return res;
}

void custom_status_clear(Actor* actor, s8 customStatusId) {
    StatusInfo* status = &actor->customStatuses[customStatusId];
    StatusType* statusType = &gCustomStatusTypes[customStatusId];

    if (status->turns > 0)
        custom_status_decrease_turn_count_impl(actor, 0, status, statusType, FALSE);
}

s32 custom_status_clear_debuffs(Actor* actor) {
    s32 amt = 0;

    for (s32 i = 0; i < ARRAY_COUNT(actor->customStatuses); i++)
    {
        StatusInfo* status = &actor->customStatuses[i];
        StatusType* statusType = &gCustomStatusTypes[i];

        if (status->turns > 0 && statusType->isDebuff) {
            amt += 1;
            custom_status_decrease_turn_count_impl(actor, 0, status, statusType, FALSE);
        }
    }

    return amt;
}
