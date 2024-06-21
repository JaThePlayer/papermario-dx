#ifndef _MISC_PACTHES_H_
#define _MISC_PACTHES_H_

#include "common.h"

#define LEVEL_CAP 27

// returns how much fp costs are changed by
s32 getFpCostChange(Actor* player);

s32 getDamageChangeFromStatus(Actor* actor);

void clearChargesFrom(Actor* actor);

// out koDuration
API_CALLABLE(GetPartnerKoDuration);

/// Targets either the player or partner. Never targets the partner if they are knocked out.
/// actor, chanceToTargetPlayer(in percent), out isPlayer
API_CALLABLE(TargetPlayerOrPartner);

/// (Actor, s8 level)
API_CALLABLE(OverrideActorLevel);

/// (ActorId, ref EvtScript)
/// Binds a script which will be ran each time any enemy in the formation gets damaged.
API_CALLABLE(BindOnEnemyDamaged);

// Challenge Runs
b32 is_midas_stone();
b32 is_ascetic_note();
b32 is_lone_idol();
b32 is_orb_of_lug();

s32 get_bp_cost_of_move(s32 moveId);

// EVENTS

/// Called when an actor gets constructed, used to zero-initialize new fields.
void _onActorCtor(Actor* actor);

/// Called each frame
void _onFrame();

void _on_dispatch_event_actor(Actor* actor, s32 event);

#endif
