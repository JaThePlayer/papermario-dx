#pragma once

#include "common_structs.h"
#include "enums.h"
#include "macros.h"
#include "script_api/battle.h"
#include "script_api/macros.h"

#define _STR(x) #x

#define INTERFACE(interfaceName) _STR(interfaceName##Impl)

/// Checks whether an actor implements the given interface
/// @evtapi
/// @param actorId
/// @param interfaceName
/// @param outVar
#define DoesActorImplement(actorId, interfaceName, outVar) Call(DoesActorExport, actorId, (Bytecode)_STR(interfaceName##Impl), outVar)

#define IMPLEMENT(interfaceName, args...) export struct interfaceName interfaceName##Impl = { args }

API_CALLABLE(_ExecWaitInterface);
API_CALLABLE(_GetRefFromInterface);
API_CALLABLE(_GetFromInterface_s32);

#define ExecWaitInterface(actorId, interfaceName, field) Call(_ExecWaitInterface, actorId, (Bytecode)_STR(interfaceName##Impl), __builtin_offsetof(interfaceName, field))

#define GetRefFromInterface(actorId, interfaceName, field, outLVar) Call(_GetRefFromInterface, actorId, (Bytecode)_STR(interfaceName##Impl), __builtin_offsetof(interfaceName, field), outLVar)

#define _INTERFACE_FIELD_TYPE(interfaceName, field) _Generic(((interfaceName){{0}}.field), \
    s32:  _GetFromInterface_s32 \
)

// Reads a value from the given field of the interface.
#define GetFromInterface(actorId, interfaceName, field, outLVar) Call((_INTERFACE_FIELD_TYPE(interfaceName, field)), actorId, (Bytecode)_STR(interfaceName##Impl), __builtin_offsetof(interfaceName, field), outLVar)


typedef struct IYieldable {
    /// Script which checks if the actor can be yielded to. Returns a boolean on LVarF.
    EvtScript* isYieldable;
} IYieldable;

typedef struct IGoomba {
    enum {
        GOOMBA_TYPE_Normal,
        GOOMBA_TYPE_Spiked,
        GOOMBA_TYPE_Para,
    } type;
} IGoomba;

/// A script which can implement IYieldable to not allow yielding the turn if the enemy to yield to plans to use an item.
/// Assumes that the default item use AI is used (EnemyItems_ItemUseAI).
extern EvtScript IYieldable_DontYieldIfWillUseItem;

/// Yields the turn if the next actor implements an interface and is currently ready to be yielded to.
/// @evtin LVar0 interfaceName (from INTERFACE macro)
/// @evtin LVar1 delay
/// @evtout LVarA The enemy that was yielded to. 0 if didn't yield.
extern EvtScript YieldIfNextActorImplements;

extern API_CALLABLE(GetBattleCamPreset);
