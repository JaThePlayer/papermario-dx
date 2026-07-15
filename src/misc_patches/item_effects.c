#include "item_effects.h"
#include "PR/ultratypes.h"
#include "evt.h"
#include "functions.h"
#include "item_enum.h"
#include "macros.h"
#include "script_api/macros.h"

#define NAMESPACE item_effects

#include "item_effects_definitions.c"

#define NONE { .type = nullptr, .userdata.s32 = 0 }
#define HEAL(amt) { .type = &N(heal), .userdata.s32 = (amt), }

#define INFLICT(status, turns, enemyTurns, potency) { .type = &N(status), .userdata.s32 = ITEM_EFFECTS_PACK_TURNS_POTENCY(turns, enemyTurns, potency), }
// increase turn count for enemies, as the first turn is wasted
#define DEF_UP(turns, potency) INFLICT(def_up, turns, turns + 1, potency)
// increase turn count for both mario and enemies, as the first turn is wasted
#define ATK_UP(turns, potency) INFLICT(atk_up, turns + 1, turns + 1, potency)

#define FP_COST(turns, potency) INFLICT(fp_cost, turns, turns + 1, potency)

#define ELECTRIFY(turns) { .type = &N(electrify), .userdata.s32 = (turns), }

#define POISON(turns, potency) INFLICT(poison, turns, turns, potency)

#define CHARGE(amt) INFLICT(charge, 99, 99, amt)

#define RECOVER_FP(amt) RecoverFpFromCustomItemEffect(actor, amt)

#define CLEAR_STATUS() { .type = &N(clear_status), .userdata.s32 = 0, }

#define MUSH_POWER_PROC() { .type = &N(mush_power), .userdata.s32 = 0, }

#define DEFINE_EFFECTS(item, effects...) ItemEffect effects_##item[] = { \
    effects, \
    NONE \
}
#define IMPL_EFFECTS(item) [item] = effects_##item

DEFINE_EFFECTS(ITEM_DRIED_SHROOM, MUSH_POWER_PROC());
DEFINE_EFFECTS(ITEM_MUSHROOM, MUSH_POWER_PROC());
DEFINE_EFFECTS(ITEM_SUPER_SHROOM, MUSH_POWER_PROC());
DEFINE_EFFECTS(ITEM_ULTRA_SHROOM, MUSH_POWER_PROC());
DEFINE_EFFECTS(ITEM_LIFE_SHROOM, MUSH_POWER_PROC());
DEFINE_EFFECTS(ITEM_VOLT_SHROOM, ELECTRIFY(3), MUSH_POWER_PROC());

DEFINE_EFFECTS(ITEM_GOOMNUT, DEF_UP(1, 2));
DEFINE_EFFECTS(ITEM_NUTTY_CAKE, DEF_UP(2, 1));
DEFINE_EFFECTS(ITEM_KOOPA_LEAF, FP_COST(2, -2));
DEFINE_EFFECTS(ITEM_KOOPA_TEA, FP_COST(4, -1));
DEFINE_EFFECTS(ITEM_MAPLE_CREAM, FP_COST(2, +1));
DEFINE_EFFECTS(ITEM_DRIED_FRUIT, FP_COST(2, +2));
DEFINE_EFFECTS(ITEM_MISTAKE, POISON(3, 1));
DEFINE_EFFECTS(ITEM_POISON_SHROOM, POISON(4, 1));
DEFINE_EFFECTS(ITEM_SPICY_SOUP, ATK_UP(1, 2));
DEFINE_EFFECTS(ITEM_HOT_SHROOM, ELECTRIFY(2), CHARGE(1));
DEFINE_EFFECTS(ITEM_SUPER_SODA, CLEAR_STATUS());
DEFINE_EFFECTS(ITEM_TASTY_TONIC, { .type = &N(tasty_tonic), .userdata.s32 = 0, });

ItemEffect* gItemEffectTable[NUM_ITEMS] = {
    IMPL_EFFECTS(ITEM_DRIED_SHROOM),
    IMPL_EFFECTS(ITEM_MUSHROOM),
    IMPL_EFFECTS(ITEM_SUPER_SHROOM),
    IMPL_EFFECTS(ITEM_ULTRA_SHROOM),
    IMPL_EFFECTS(ITEM_LIFE_SHROOM),
    IMPL_EFFECTS(ITEM_VOLT_SHROOM),

    IMPL_EFFECTS(ITEM_GOOMNUT),
    IMPL_EFFECTS(ITEM_NUTTY_CAKE),
    IMPL_EFFECTS(ITEM_KOOPA_LEAF),
    IMPL_EFFECTS(ITEM_KOOPA_TEA),
    IMPL_EFFECTS(ITEM_MAPLE_CREAM),
    IMPL_EFFECTS(ITEM_DRIED_FRUIT),
    IMPL_EFFECTS(ITEM_MISTAKE),
    IMPL_EFFECTS(ITEM_POISON_SHROOM),
    IMPL_EFFECTS(ITEM_SPICY_SOUP),
    IMPL_EFFECTS(ITEM_HOT_SHROOM),
    IMPL_EFFECTS(ITEM_SUPER_SODA),
    IMPL_EFFECTS(ITEM_TASTY_TONIC),
};

#undef NONE
#undef HEAL

static API_CALLABLE(get_effect) {
    Bytecode* args = script->ptrReadPos;
    s32 itemIdx = evt_get_variable(script, *args++);
    s32 i = evt_get_variable(script, *args++);

    ItemEffect* effects = gItemEffectTable[itemIdx];
    if (effects == nullptr) {
        evt_set_variable(script, *args++, nullptr);
        evt_set_variable(script, *args++, nullptr);
        return ApiStatus_DONE2;
    }

    ItemEffect* effect = &effects[i];
    if (effect->type == nullptr) {
        evt_set_variable(script, *args++, nullptr);
        evt_set_variable(script, *args++, nullptr);
        return ApiStatus_DONE2;
    }

    evt_set_variable(script, *args++, (Bytecode) effect->type->apply);
    evt_set_variable(script, *args++, effect->userdata.s32);
    return ApiStatus_DONE2;
}

#define LVarA_Item LVarA
#define LVarB_ApplyEvt LVarB
#define LVarC_Userdata LVarC
#define LVarD_I LVarD
#define LVarE_CurrEvt LVarE
EvtScript ApplyItemEffects = {
    Set(LVarD_I, 0)

    Loop(0)
        Call(get_effect, LVarA_Item, LVarD_I, LVarB, LVarC)
        IfEq(LVarB_ApplyEvt, nullptr)
            BreakLoop
        EndIf

        // Exec to make sure the executed script won't mess with our lvars.
        ExecGetTID(LVarB_ApplyEvt, LVarE_CurrEvt)
        Loop(0)
            IsThreadRunning(LVarE_CurrEvt, LVar0)
            IfEq(LVar0, 0)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop

        Add(LVarD_I, 1)
    EndLoop

    Return
    End
};
#undef LVarItem
#undef LVarB_ApplyEvt
#undef LVarC_Userdata
#undef LVarD_I
#undef LVarE_CurrEvt

#undef NAMESPACE
