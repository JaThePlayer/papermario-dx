#include "custom_status.h"

#include "statuses/temp_def_down.c"
#include "statuses/temp_atk_down.c"
#include "statuses/temp_def_up.c"
#include "statuses/temp_atk_up.c"

#define STATUS_ENTRY(namespace) { \
        .onApply = &namespace##_on_apply, \
        .drawIcon = &namespace##_create_icon, \
        .onRemoveIcon = &namespace##_remove_icon, \
        .decrementLate = namespace##_DECREMENT_LATE, \
    }

StatusType gCustomStatusTypes[CUSTOM_STATUS_AMT] = {
    [NONE_CUSTOM_STATUS] = {},
    [ATK_DOWN_TEMP_STATUS] = STATUS_ENTRY(temp_atk_down),
    [DEF_DOWN_TEMP_STATUS] = STATUS_ENTRY(temp_def_down),
    [ATK_UP_TEMP_STATUS] = STATUS_ENTRY(temp_atk_up),
    [DEF_UP_TEMP_STATUS] = STATUS_ENTRY(temp_def_up),
};

// Gets the potency of the given status for the given actor. 0 if actor doesn't have this status
s8 custom_status_get_potency(Actor* actor, s8 customStatusId) {
    StatusInfo* status = &actor->customStatuses[customStatusId];

    if (status->turns > 0)
        return status->potency;
    return 0;
}

static void custom_status_decrement_impl(Actor* actor, s8 isLate) {
    for (s32 i = 0; i < ARRAY_COUNT(actor->customStatuses); i++)
    {
        StatusInfo* status = &actor->customStatuses[i];
        StatusType* statusType = &gCustomStatusTypes[i];

        if (statusType->decrementLate == isLate && status->turns > 0) {
            status->turns--;
            if (status->turns == 0) {
                // status just got removed now
                status->potency = 0;
                if (statusType->onRemoveIcon) {
                    statusType->onRemoveIcon(actor->hudElementDataIndex);
                }
            }
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

    status->turns = turns;
    status->potency = potency;

    if (statusType->onApply) {
        statusType->onApply(actor, position);
    }

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