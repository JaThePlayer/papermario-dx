#include "actor_interfaces.h"
#include "common_structs.h"
#include "dx/overlay.h"
#include "evt.h"
#include "functions.h"

API_CALLABLE(_ExecWaitInterface) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) actorID = script->owner1.actorID;
    const char* interfaceImplName = (const char*)evt_get_variable(script, *args++);
    s32 offset = evt_get_variable(script, *args++);

    Actor* actor = get_actor(actorID);
    if (actor == nullptr) {
        PANIC_MSG("ExecWaitInterface: no such actor %ld\n", actorID);
        return ApiStatus_DONE2;
    }
    if (actor->overlay == nullptr) {
        PANIC_MSG("ExecWaitInterface: actor %ld has no overlay\n", actorID);
        return ApiStatus_DONE2;
    }
    u8* data = (u8*)ovl_import(actor->overlay, interfaceImplName);
    if (data == nullptr) {
        PANIC_MSG("ExecWaitInterface: overlay does not export '%s'\n", interfaceImplName);
        return ApiStatus_DONE2;
    }
    EvtScript* scriptSource = *(EvtScript**)&data[offset];

    Evt* newScript = start_child_script(script, scriptSource, 0);
    newScript->owner1.actorID = actorID;

    script->curOpcode = EVT_OP_INTERNAL_FETCH;
    return ApiStatus_FINISH;
}

u8* get_from_interface_ptr(Evt* script, Bytecode* outVar) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) actorID = script->owner1.actorID;
    const char* interfaceImplName = (const char*)evt_get_variable(script, *args++);
    s32 offset = evt_get_variable(script, *args++);
    *outVar = *args++;

    Actor* actor = get_actor(actorID);
    if (actor == nullptr) {
        PANIC_MSG("GetFromInterface: no such actor %ld\n", actorID);
    }
    if (actor->overlay == nullptr) {
        PANIC_MSG("GetFromInterface: actor %ld has no overlay\n", actorID);
    }
    u8* data = (u8*)ovl_import(actor->overlay, interfaceImplName);
    if (data == nullptr) {
        PANIC_MSG("GetFromInterface: overlay does not export '%s'\n", interfaceImplName);
    }

    return &data[offset];
}

API_CALLABLE(_GetRefFromInterface) {
    Bytecode outVar;
    s32 offset;
    u8* data = get_from_interface_ptr(script, &outVar);

    evt_set_variable(script, outVar, (Bytecode)data);
    return ApiStatus_DONE2;
}

API_CALLABLE(_GetFromInterface_s32) {
    Bytecode outVar;
    s32 offset;
    s32* data = (s32*)get_from_interface_ptr(script, &outVar);

    evt_set_variable(script, outVar, (Bytecode)*data);
    return ApiStatus_DONE2;
}

EvtScript IYieldable_DontYieldIfWillUseItem = {
    Call(EnemyItems_ItemUseAI, ACTOR_SELF, LVarF, 0)
    // LVarF is the actor ID to use the item on, return `false` if its non-0.
    IfEq(LVarF, 0)
        Set(LVarF, true)
        Return
    EndIf
    Set(LVarF, false)
    Return
    End
};

EvtScript YieldIfNextActorImplements = {
    #define LVar_Actor LVarA
    Call(GetOwnerID, LVar_Actor)
    Label(0)
  	Add(LVar_Actor, 1)
  	Call(ActorExists, LVar_Actor, LVarF)
    IfFalse(LVarF)
        IfLt(LVar_Actor, ACTOR_ENEMY23)
            Goto(0)
        EndIf
        Set(LVarA, 0)
        Return
    EndIf


    Call(DoesActorExport, LVar_Actor, LVar0, LVarF)
    IfFalse(LVarF)
        Set(LVarA, 0)
        Return
    EndIf

    DoesActorImplement(LVar_Actor, IYieldable, LVarF)
    IfTrue(LVarF)
        ExecWaitInterface(LVar_Actor, IYieldable, isYieldable)
        IfFalse(LVarF)
            Set(LVarA, 0)
            Return
        EndIf
    EndIf

    IfEq(LVar1, 0)
        Call(YieldTurn)
    Else
        Thread
            Wait(LVar1)
            Call(YieldTurn)
        EndThread
    EndIf
    Return
    End

    #undef LVar_Actor
};
