#ifndef _ENEMY_ITEM_API_H_
#define _ENEMY_ITEM_API_H_

#include "common.h"

typedef struct ItemChance {
    s32 itemID;
    s32 weight; // the weight of the item during randomisation. an item with a higher weight will have a higher chance of getting selected.
} ItemChance;

#include "enemy_items/config.h"

// EnemyItems_AddItem(actorID, itemID, f32 xOffset, f32 yOffset, f32 zOffset)
// adds a new item to the actor
API_CALLABLE(EnemyItems_AddItem);

// EnemyItems_AddRandomItem(actorID, &ItemChance[], f32 xOffset, f32 yOffset, f32 zOffset)
API_CALLABLE(EnemyItems_AddRandomItem);

// EnemyItems_IsCalledByEnemy(out var)
// sets 'var' to 1 if the item script was called by an enemy
API_CALLABLE(EnemyItems_IsCalledByEnemy);

// EnemyItems_SetCalledByEnemy(value)
// sets a value indicating whether the item script was called by an enemy.
// for internal use.
API_CALLABLE(EnemyItems_SetCalledByEnemy);

// EnemyItems_GetHeldItem(actorID, index, out var itemID, out var itemEntityID)
// gets the item at the given index held by the enemy
API_CALLABLE(EnemyItems_GetHeldItem);

// EnemyItems_GetFirstHeldItem(ActorID, out var index, out var itemID, out var entityID)
API_CALLABLE(EnemyItems_GetFirstHeldItem);

// EnemyItems_GetFirstHeldItemWithID(ActorID, itemID, out var index, out var entityID)
API_CALLABLE(EnemyItems_GetFirstHeldItemWithID);

// EnemyItems_RemoveHeldItem(actorID)
// removes the item held by the given actor
API_CALLABLE(EnemyItems_RemoveHeldItem);

// picks an item from a weighted list
void enemy_items_add_random_item(Actor* actor, ItemChance* items, f32 xOffset, f32 yOffset, f32 zOffset);

// adds an item to the enemy
void enemy_items_add_item(Actor* actor, s32 itemID, f32 xOffset, f32 yOffset, f32 zOffset);

// removes the item held by this actor at the given index, if it has one
void enemy_items_remove_item(Actor* actor, s32 index);

// changes the base position for all held items
void enemy_items_set_base_pos(Actor* actor, f32 x, f32 y, f32 z);

s32 get_enemy_items_called_by_enemy(void);

void set_enemy_items_called_by_enemy(s32 value);

// removes all held items
void enemy_items_clear_items(Actor* actor);

// zero-initializes all held items
void enemy_items_zero_initialize(Actor* actor);

// finds the first full item slot
s32 enemy_items_find_full_slot(Actor* actor);

// finds the first full item slot that doesn't have a badge
s32 enemy_items_find_full_non_badge_slot(Actor* actor);

// counts the amount of items with a given itemID
s32 enemy_items_count_items_with_id(Actor* actor, s32 itemID);

// counts the amount of items with a given moveID
s32 enemy_items_count_items_with_move_id(Actor* actor, s32 moveID);

// Script, which handles the usage of a healing item by an enemy.
extern EvtScript EnemyItems_UseHealingItem;

// Script which handles the usage of an item by an enemy, loading the right script etc.
// Expects LVar8 to store the index of the item to use
// Might crash if the enemy doesn't have an item at this index.
// might clobber a *lot* of LVar's, so just assume all of them got messed up after calling this.
extern EvtScript EnemyItems_UseHeldItem;

// Script which makes the enemy use an item if it has one, with no AI.
// Returns 0 on LVar0 if the enemy didn't use an item (because it did not have any), 1 if it did.
// might clobber a *lot* of LVar's, so just assume all of them got messed up after calling this.
extern EvtScript EnemyItems_TryUseHeldItem;

// Script which makes the enemy use an item if it has one, with a simple AI that will check whether its worth to use the item first.
// Returns 0 on LVar0 if the enemy didn't use an item, 1 if it did.
// might clobber a *lot* of LVar's, so just assume all of them got messed up after calling this.
extern EvtScript EnemyItems_TryUseHeldItem_WithAI;

// applies effects for Armageddon items
extern API_CALLABLE(ApplyCustomItemEffects);

// Handles calling the default item use AI, and returns from the Evt if an item was used
#define STANDARD_ITEM_USE_AI() \
    Call(GetBattlePhase, LVar0) \
    IfNe(LVar0, PHASE_FIRST_STRIKE) \
        ExecWait(EnemyItems_TryUseHeldItem_WithAI) \
        IfNe(LVar0, 0) \
            Return \
        EndIf \
    EndIf

#define GIVE_ITEM(pool) Call(EnemyItems_AddRandomItem, ACTOR_SELF, (s32)&pool, Float(14.0), Float(0.0), Float(-1.0))

// Handles calling EnemyItems_UseHealingItem, should be the first thing called in a healing item script
#define STANDARD_HEALING_ITEM_ENEMY_USE() \
    Call(EnemyItems_IsCalledByEnemy, LVarD) \
    IfEq(LVarD, 1) \
        ExecWait(EnemyItems_UseHealingItem) \
        Return \
    EndIf \

#endif
