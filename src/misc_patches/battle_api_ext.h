#pragma once

#include "common.h"
#include "macros.h"

extern ActorPart* get_actor_part_for_def(Actor* actor);

/// Calls get_defense(enemy, get_actor_part_for_def(enemy)->defenseTable, element)
/// @evtapi
/// @param actorId
/// @param element
/// @param outVarDefense
API_CALLABLE(GetActorDefense);

/// @evtapi
/// @param actorId
/// @param outVarMaxHp
API_CALLABLE(GetActorMaxHP);
