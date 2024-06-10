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
    [BURN_STATUS] = STATUS_ENTRY(burn_status, TRUE, TRUE, STATUS_STACKING_OVERRIDE),
    [FP_COST_STATUS] = STATUS_ENTRY(fp_cost_status, FALSE, TRUE, STATUS_STACKING_OVERRIDE),
    [CHARGE_STATUS] = STATUS_ENTRY(charge_status, FALSE, FALSE, STATUS_STACKING_ADD_POTENCY),
};

// Gets the potency of the given status for the given actor. 0 if actor doesn't have this status
s8 custom_status_get_potency(Actor* actor, s8 customStatusId) {
    StatusInfo* status = &actor->customStatuses[customStatusId];

    if (status->turns > 0)
        return status->potency;
    return 0;
}

static void custom_status_decrease_turn_count_impl(Actor* actor, u8 newTurns, StatusInfo* status, StatusType* statusType) {
    if (statusType->onDecrement) {
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
            custom_status_decrease_turn_count_impl(actor, status->turns - 1, status, statusType);
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

s32 try_inflict_custom_status(Actor* actor, Vec3f position, s8 customStatusId, u8 turns, u8 potency, u8 chance) {
    StatusInfo* status = &actor->customStatuses[customStatusId];
    StatusType* statusType = &gCustomStatusTypes[customStatusId];
    // todo: resistance

    switch (statusType->stackingBehaviour)
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

void set_next_attack_custom_status(s8 customStatusId, u8 turns, u8 potency, u8 chance) {
    gBattleStatus.curAttackCustomStatusId = customStatusId;
    gBattleStatus.curAttackCustomStatusTurns = turns;
    gBattleStatus.curAttackCustomStatusPotency = potency;
    gBattleStatus.curAttackCustomStatusChance = chance;
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
        custom_status_decrease_turn_count_impl(actor, 0, status, statusType);
}

s32 custom_status_clear_debuffs(Actor* actor) {
    s32 amt = 0;

    for (s32 i = 0; i < ARRAY_COUNT(actor->customStatuses); i++)
    {
        StatusInfo* status = &actor->customStatuses[i];
        StatusType* statusType = &gCustomStatusTypes[i];

        if (status->turns > 0 && statusType->isDebuff) {
            amt += 1;
            custom_status_decrease_turn_count_impl(actor, 0, status, statusType);
        }
    }

    return amt;
}
