#include "custom_status.h"

extern void on_apply_atk_down(Actor* target, Vec3f position);
extern void on_apply_def_down(Actor* target, Vec3f position);

StatusType gCustomStatusTypes[CUSTOM_STATUS_AMT] = {
    [NONE_CUSTOM_STATUS] = {},
    [ATK_DOWN_TEMP_STATUS] = {
        .onApply = &on_apply_atk_down,
    },
    [DEF_DOWN_TEMP_STATUS] = {
        .onApply = &on_apply_def_down,
    }
};

// Gets the potency of the given status for the given actor. 0 if actor doesn't have this status
u8 custom_status_get_potency(Actor* actor, s8 customStatusId) {
    StatusInfo* status = &actor->customStatuses[customStatusId];

    if (status->turns > 0)
        return status->potency;
    return 0;
}

// Decrements all custom statuses for the given actor
void custom_status_decrement(Actor* actor) {
    for (s32 i = 0; i < ARRAY_COUNT(actor->customStatuses); i++)
    {
        StatusInfo* status = &actor->customStatuses[i];

        if (status->turns > 0) {
            status->turns--;
            if (status->turns == 0) {
                // status just got removed now
                status->potency = 0;
            }
        }
    }
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

extern EvtScript EVS_PlaySleepHitFX;

void on_apply_atk_down(Actor* target, Vec3f position) {
    Evt* evt = start_script(&EVS_PlaySleepHitFX, EVT_PRIORITY_A, 0);
    evt->varTable[0] = position.x;
    evt->varTable[1] = position.y;
    evt->varTable[2] = position.z;
    sfx_play_sound_at_position(SOUND_INFLICT_SLEEP, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);

    create_status_debuff(target->hudElementDataIndex, STATUS_KEY_POISON);
}

void on_apply_def_down(Actor* target, Vec3f position) {
    Evt* evt = start_script(&EVS_PlaySleepHitFX, EVT_PRIORITY_A, 0);
    evt->varTable[0] = position.x;
    evt->varTable[1] = position.y;
    evt->varTable[2] = position.z;
    sfx_play_sound_at_position(SOUND_INFLICT_SLEEP, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);

    create_status_debuff(target->hudElementDataIndex, STATUS_KEY_POISON);
}

// from inflict_status:
// set_actor_pal_adjustment(target, ACTOR_PAL_ADJUST_POISON);
// create_status_debuff(target->hudElementDataIndex, STATUS_KEY_POISON);
