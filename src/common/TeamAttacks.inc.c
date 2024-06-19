#include "common.h"

// (ref null-terminated ActorType[], out lvar Actor)
// Returns the next enemy after the script owner, if it's type is present in the input type array.
API_CALLABLE(N(NextEnemyIfTypeIsOneOf)) {
    Bytecode* args = script->ptrReadPos;

    s32* types = evt_get_variable(script, *args++);

    s32 ownerId = script->owner1.actorID;
    s32 nextActorId = ownerId + 1;

    s32 ret = FALSE;
    Actor* nextActor = get_actor(nextActorId);
    if (nextActor != NULL) {
        while (*types != NULL) {
            if (*types == nextActor->actorType) {
                ret = TRUE;
                break;
            }
            types++;
        }
    }

    evt_set_variable(script, *args++, ret ? nextActor : NULL);

    return ApiStatus_DONE2;
}
