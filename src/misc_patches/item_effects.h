#pragma once

#include "item_enum.h"
#define NAMESPACE item_effects

#include "common_structs.h"

typedef struct ItemEffectType {
    /// Script called and awaited upon applying this effect.
    /// Args:
    /// LVar9 actorId
    /// LVarC userdata
    EvtScript* apply;
} ItemEffectType;

typedef struct ItemEffect {
    ItemEffectType* type;
    union {
        s32 s32;
        void* ptr;
    } userdata;
} ItemEffect;

extern ItemEffectType N(heal);

extern ItemEffect* gItemEffectTable[NUM_ITEMS];

/// Applies the item effects for the given item.
/// Args:
/// - LVar9 - actorId
/// - LVarA - itemId
extern EvtScript ApplyItemEffects;

#define ITEM_EFFECTS_PACK_TURNS_POTENCY(turns, enemyTurns, potency) ((u32)(turns) | (((u32)(enemyTurns) << 8)) | ((u32)(potency) << 16))
#define ITEM_EFFECTS_UNPACK_TURNS(userdata) ((userdata) & 0x000000ff)
#define ITEM_EFFECTS_UNPACK_ENEMY_TURNS(userdata) ((userdata) & 0x0000ff00)
#define ITEM_EFFECTS_UNPACK_POTENCY(userdata) (((userdata) & 0x00ff0000) >> 16)

#undef NAMESPACE
