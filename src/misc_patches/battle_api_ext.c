#include "battle_api_ext.h"
#include "evt.h"
#include "functions.h"
#include "macros.h"

API_CALLABLE(GetActorDefense) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    s32 element = evt_get_variable(script, *args++);
    Bytecode outVar = *args++;

    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    Actor* actor = get_actor(actorID);

    evt_set_variable(script, outVar, get_defense(actor, get_actor_part_for_def(actor)->defenseTable, element));

    return ApiStatus_DONE2;
}

