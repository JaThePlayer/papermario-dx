#include "anim_bank.h"
#include "evt.h"
#include "functions.h"


/// Call(SetBankAnimation, ACTOR_SELF, PRT_MAIN, Ref(bank), slotId)
API_CALLABLE(SetBankAnimation) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }

    s32 partID = evt_get_variable(script, *args++);
    s32* bank = (s32*)evt_get_variable(script, *args++);
    s32 slotId = evt_get_variable(script, *args++);

    s32 animID = bank[slotId];

    set_actor_anim(actorID, partID, animID);

    return ApiStatus_DONE2;
}
