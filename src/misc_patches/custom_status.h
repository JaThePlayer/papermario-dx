#ifndef _H_CUSTOM_STATUS
#define _H_CUSTOM_STATUS

#include "custom_status_defs.h"
#include "common.h"
#include "battle/battle.h"

#define NONE_CUSTOM_STATUS 0
#define ATK_DOWN_TEMP_STATUS 1
#define DEF_DOWN_TEMP_STATUS 2
#define ATK_UP_TEMP_STATUS 3
#define DEF_UP_TEMP_STATUS 4
#define CLOSE_CALL_STATUS 5
#define BURN_STATUS 6

// A function which gets called when a custom status gets applied
typedef void(*StatusFxApplyFunc)(Actor* target, Vec3f position, u8 potency);

typedef void(*StatusFxDrawIconFunc)(Actor* target);

typedef void(*StatusFxRemoveIconFunc)(s32 iconId);

typedef void(*StatusFxOnDecrementFunc)(Actor* target);

typedef struct StatusType {
    StatusFxApplyFunc onApply;
    StatusFxDrawIconFunc drawIcon;
    StatusFxRemoveIconFunc onRemoveIcon;
    StatusFxOnDecrementFunc onDecrement;
    s8 decrementLate; // whether the status should be decremented after enemies attack, like Chill Out
} StatusType;

extern StatusType gCustomStatusTypes[CUSTOM_STATUS_AMT];

// (id, turns, potency, chance)
API_CALLABLE(SetNextAttackCustomStatus);

// Gets the potency of the given status for the given actor. 0 if actor doesn't have this status
s8 custom_status_get_potency(Actor* actor, s8 customStatusId);

// Decrements all custom statuses for the given actor. For enemies, this is called before they attack.
void custom_status_decrement(Actor* actor);

// Decrements all custom statuses for the given actor. For enemies, this is called after they attack.
void custom_status_decrement_late(Actor* actor);

s32 try_inflict_custom_status(Actor* actor, Vec3f position, s8 customStatusId, u8 turns, u8 potency, u8 chance);

void set_next_attack_custom_status(s8 customStatusId, u8 turns, u8 potency, u8 chance);

void custom_status_zero_initialize(Actor* actor);

void custom_status_render_all_icons(Actor* actor);

void custom_status_remove_icons(s32 iconId);

Vec3f get_expected_arrow_pos(Actor* actor);

#endif
