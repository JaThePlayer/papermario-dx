#include "common.h"
#include "enemy_items/api.h"
#include "script_api/battle.h"

static s32 isCalledByEnemy = FALSE;

static Actor* get_actor_from_evt_var(Evt* script, s32 var) {
    s32 actorID = evt_get_variable(script, var);
    Actor* actor;
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }

    actor = get_actor(actorID);

    return actor;
}

s32 enemy_items_find_empty_slot(Actor* actor) {
    s32 i;
    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];

        if (held->itemID == ITEM_NONE)
            return i;
    }

    return -1;
}

s32 enemy_items_find_full_slot(Actor* actor) {
    s32 i;
    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];

        if (held->itemID != ITEM_NONE)
            return i;
    }

    return -1;
}

s32 enemy_items_find_full_non_badge_slot(Actor* actor) {
    s32 i;
    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];
        ItemData* itemData = &gItemTable[held->itemID];

        if (itemData->typeFlags & ITEM_TYPE_FLAG_BADGE) {
            continue;
        }

        if (held->itemID != ITEM_NONE)
            return i;
    }

    return -1;
}

s32 enemy_items_count_items_with_id(Actor* actor, s32 itemID) {
    s32 sum = 0;
    s32 i;

    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];

        if (held->itemID == itemID)
            sum++;
    }

    return sum;
}

s32 enemy_items_count_items_with_move_id(Actor* actor, s32 moveID) {
    s32 sum = 0;
    s32 i;

    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];
        ItemData* itemData = &gItemTable[held->itemID];

        if (itemData->moveID == moveID)
            sum++;
    }

    return sum;
}

void enemy_items_add_item(Actor* actor, s32 itemID, f32 xOffset, f32 yOffset, f32 zOffset) {
    s32 slot = enemy_items_find_empty_slot(actor);
    ActorHeldItem* held;

    if (slot == -1)
        return;

    held = &actor->heldItems[slot];

    held->itemID = itemID;

    held->itemEntityID = make_item_entity_nodelay(held->itemID,
        actor->homePos.x + xOffset + (slot / 5) * 24.f, actor->homePos.y + yOffset + ((slot % 5) * 24.f), actor->homePos.z + zOffset,
        ITEM_SPAWN_MODE_DECORATION, 0
    );
}

API_CALLABLE(EnemyItems_AddItem) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor = get_actor_from_evt_var(script, *args++);
    s32 item = evt_get_variable(script, *args++);
    f32 x = evt_get_float_variable(script, *args++);
    f32 y = evt_get_float_variable(script, *args++);
    f32 z = evt_get_float_variable(script, *args++);

    enemy_items_add_item(actor, item, x, y, z);

    return ApiStatus_DONE2;
}

void enemy_items_add_random_item(Actor* actor, ItemChance* items, f32 xOffset, f32 yOffset, f32 zOffset) {
    s32 totalWeight, randNum, weightSoFar;
    ItemChance* item;

    totalWeight = 0;
    item = items;
    while(TRUE) {
        totalWeight += item->weight;

        if (item->itemID == ITEM_NONE)
            break;
        item++;
    }

    randNum = rand_int(totalWeight - 1);
    weightSoFar = 0;
    item = items;
    while (TRUE) {
        weightSoFar += item->weight;

        if (randNum < weightSoFar) {
            if (item->itemID != ITEM_NONE)
                enemy_items_add_item(actor, item->itemID, xOffset, yOffset, zOffset);
            return;
        }

        if (item->itemID == ITEM_NONE)
            break;
        item++;
    }

    enemy_items_add_item(actor, ITEM_MISTAKE, xOffset, yOffset, zOffset);
}

// EnemyItems_AddRandomItem(actorID, &ItemChance[], f32 xOffset, f32 yOffset, f32 zOffset)
API_CALLABLE(EnemyItems_AddRandomItem) {
    Bytecode* args = script->ptrReadPos;

    Actor* actor = get_actor_from_evt_var(script, *args++);
    ItemChance* items = (ItemChance*)evt_get_variable(script, *args++);
    f32 x = evt_get_float_variable(script, *args++);
    f32 y = evt_get_float_variable(script, *args++);
    f32 z = evt_get_float_variable(script, *args++);

    enemy_items_add_random_item(actor, items, x, y, z);

    return ApiStatus_DONE2;
}

s32 get_enemy_items_called_by_enemy() {
    return isCalledByEnemy;
}

void set_enemy_items_called_by_enemy(s32 value) {
    isCalledByEnemy = value;
}

API_CALLABLE(EnemyItems_IsCalledByEnemy) {
    Bytecode* args = script->ptrReadPos;
    s32 outVar = *args++;

    evt_set_variable(script, outVar, get_enemy_items_called_by_enemy());

    return ApiStatus_DONE2;
}

API_CALLABLE(EnemyItems_SetCalledByEnemy) {
    Bytecode* args = script->ptrReadPos;
    s32 val = evt_get_variable(script, *args++);

    set_enemy_items_called_by_enemy(val);

    return ApiStatus_DONE2;
}

API_CALLABLE(_LoadItemStats) {
    Bytecode* args = script->ptrReadPos;
    ItemData* item = &gItemTable[evt_get_variable(script, *args++)];

    script->varTable[11] = item->potencyA;
    script->varTable[12] = item->potencyB;
    script->varTable[15] = item->potencyA == 1;

    return ApiStatus_DONE2;
}

API_CALLABLE(_GetTargetActorId) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    s32 targetActorID;
    Actor* actor;

    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }

    actor = get_actor(actorID);
    evt_set_variable(script, *args++, actor->targetActorID);
    return ApiStatus_DONE2;
}

EvtScript EnemyItems_UseHealingItem = {
    // load up stats
    EVT_CALL(_LoadItemStats, LVarA)
    EVT_CALL(_GetTargetActorId, ACTOR_SELF, LVar9)

    EVT_IF_NE(LVarB, 0)
        EVT_THREAD
            EVT_WAIT(5)
            EVT_CALL(PlaySoundAtActor, LVar9, SOUND_RECOVER_HEART)
            EVT_CALL(PlaySoundAtActor, LVar9, SOUND_HEART_BOUNCE)
            EVT_WAIT(30)
            EVT_CALL(PlaySoundAtActor, LVar9, SOUND_STAR_BOUNCE_A)
        EVT_END_THREAD
        EVT_THREAD
            EVT_CALL(FreezeBattleState, 1)
            EVT_CALL(HealActor, LVar9, LVarB, FALSE)
            EVT_CALL(FreezeBattleState, 0)
        EVT_END_THREAD
    EVT_END_IF

    EVT_RETURN
    EVT_END
};

API_CALLABLE(EnemyItems_GetHeldItem) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor = get_actor_from_evt_var(script, *args++);
    s32 index = evt_get_variable(script, *args++);

    s32 outItemID, outitemEntityID;
    s32 itemID, itemEntityID;

    outItemID = *args++;
    outitemEntityID = *args++;

    itemID = actor->heldItems[index].itemID;
    itemEntityID = actor->heldItems[index].itemEntityID;

    evt_set_variable(script, outItemID, itemID);
    evt_set_variable(script, outitemEntityID, itemEntityID);

    return ApiStatus_DONE2;
}

// EnemyItems_GetFirstHeldItem(ActorID, out var index, out var itemID, out var entityID)
API_CALLABLE(EnemyItems_GetFirstHeldItem) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor = get_actor_from_evt_var(script, *args++);
    s32 index = enemy_items_find_full_non_badge_slot(actor);
    s32 outItemID, outitemEntityID, outIndex;
    s32 itemID, itemEntityID;

    if (index != -1) {
        itemID = actor->heldItems[index].itemID;
        itemEntityID = actor->heldItems[index].itemEntityID;
    } else {
        itemID = 0;
        itemEntityID = 0;
    }

    outIndex = *args++;
    outItemID = *args++;
    outitemEntityID = *args++;

    evt_set_variable(script, outIndex, index);
    evt_set_variable(script, outItemID, itemID);
    evt_set_variable(script, outitemEntityID, itemEntityID);

    return ApiStatus_DONE2;
}

// EnemyItems_GetFirstHeldItemWithID(ActorID, itemID, out var index, out var entityID)
API_CALLABLE(EnemyItems_GetFirstHeldItemWithID) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor = get_actor_from_evt_var(script, *args++);
    s32 targetItemID = evt_get_variable(script, *args++);
    s32 outIndex = *args++;
    s32 outEntityID = *args++;
    s32 i;

    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];
        if (held->itemID == targetItemID) {
            evt_set_variable(script, outIndex, i);
            evt_set_variable(script, outEntityID, held->itemEntityID);

            return ApiStatus_DONE2;
        }
    }

    evt_set_variable(script, outIndex, -1);
    evt_set_variable(script, outEntityID, 0);

    return ApiStatus_DONE2;
}

void enemy_items_remove_item(Actor* actor, s32 index) {
    ActorHeldItem* held = &actor->heldItems[index];

    if (held->itemID != ITEM_NONE) {
        remove_item_entity_by_index(held->itemEntityID);
    }

    held->itemID = ITEM_NONE;
    held->itemEntityID = 0;
}

void enemy_items_clear_items(Actor* actor) {
    s32 i;
    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        enemy_items_remove_item(actor, i);
    }
}

void enemy_items_zero_initialize(Actor* actor) {
    s32 i;
    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];

        held->itemID = ITEM_NONE;
        held->itemEntityID = 0;
    }
}

void enemy_items_set_base_pos(Actor* actor, f32 x, f32 y, f32 z) {
    s32 i;
    for (i = 0; i < MAX_ENEMY_ITEMS; i++)
        if (actor->heldItems[i].itemID != ITEM_NONE) {
            // adjust the location of the held item as well
            ItemEntity* item = get_item_entity(actor->heldItems[i].itemEntityID);
            s32 offX = item->pos.x - actor->homePos.x;
            s32 offY = item->pos.y - actor->homePos.y;
            s32 offZ = item->pos.z - actor->homePos.z;

            item->pos.x = x + offX;
            item->pos.y = y + offY;
            item->pos.z = z + offZ;
        }
}

// todo: added arg
API_CALLABLE(EnemyItems_RemoveHeldItem) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor = get_actor_from_evt_var(script, *args++);
    s32 index = evt_get_variable(script, *args++);

    enemy_items_remove_item(actor, index);

    return ApiStatus_DONE2;
}

extern API_CALLABLE(LoadItemScriptForEnemy);

EvtScript EnemyItems_UseHeldItem = {
    EVT_CALL(GetActorPos, ACTOR_SELF, LVar7, LVar1, LVar2)
    EVT_CALL(PlaySoundAtActor, ACTOR_SELF, SOUND_USE_ITEM)

    EVT_WAIT(4)
    EVT_ADD(LVar1, 45)
    EVT_SET(LVar3, LVar1)
    EVT_ADD(LVar3, 10)
    EVT_ADD(LVar3, 2)

    // remove the existing item
    EVT_CALL(EnemyItems_GetHeldItem, ACTOR_SELF, LVar8, LVarA, LVar9)
    EVT_CALL(EnemyItems_RemoveHeldItem, ACTOR_SELF, LVar8)

    // create new item
    EVT_PLAY_EFFECT(EFFECT_RADIAL_SHIMMER, 1, LVar7, LVar3, LVar2, EVT_FLOAT(1.0), 30, 0)
    EVT_CALL(MakeItemEntity, LVarA, LVar7, LVar1, LVar2, ITEM_SPAWN_MODE_DECORATION, 0)
    EVT_SET(LVarB, LVar0)
    EVT_WAIT(15)
    EVT_CALL(RemoveItemEntity, LVarB)

    // load the right script for the item
    EVT_CALL(LoadItemScriptForEnemy, LVarA)

    // so that scripts know they're being used by the enemy
    EVT_CALL(EnemyItems_SetCalledByEnemy, 1)

    // call the right script for the item
    EVT_EXEC_WAIT(LVar0)

    EVT_CALL(EnemyItems_SetCalledByEnemy, 0)

    EVT_RETURN
    EVT_END
};

EvtScript EnemyItems_TryUseHeldItem = {
    // check if we have any item
    // also loads index onto LVar8, for EnemyItems_UseHeldItem
    EVT_CALL(EnemyItems_GetFirstHeldItem, ACTOR_SELF, LVar8, LVar0, 0)
    EVT_IF_EQ(LVar0, ITEM_NONE)
        EVT_RETURN // no item, early return
    EVT_END_IF

    EVT_EXEC_WAIT(EnemyItems_UseHeldItem)
    EVT_SET(LVar0, 1)
    EVT_RETURN
    EVT_END
};

// simple AI to choose whether to use an item or not
//_HeldItemUseAI(ActorID, out var choice, out var index)
static API_CALLABLE(_HeldItemUseAI) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor = get_actor_from_evt_var(script, *args++);
    BattleStatus* battleStatus = &gBattleStatus;

    s32 outVar = *args++;
    s32 outIndexVar = *args++;

    s32 missingHp, itemHeal;
    s32 itemUsedOn = FALSE;
    s32 i, j;

    for (i = 0; i < MAX_ENEMY_ITEMS; i++) {
        ActorHeldItem* held = &actor->heldItems[i];
        ItemData* itemData = &gItemTable[held->itemID];
        s32 bestHeal = -1;
        Actor* bestHealActor = NULL;

        if (held->itemID == ITEM_NONE) {
            continue;
        }

        if (held->itemID == ITEM_LIFE_SHROOM) {
            // don't waste life shrooms on healing, save them for a revive.
            continue;
        }

        if (itemData->typeFlags & ITEM_TYPE_FLAG_BADGE) {
            // don't use badges
            continue;
        }

        if (!(itemData->typeFlags & ITEM_TYPE_FLAG_FOOD_OR_DRINK)) {
            // this is not food, let's just assume it's something cool and use it.
            itemUsedOn = ACTOR_SELF;
            break;
        }

        for (j = 0; j < ARRAY_COUNT(battleStatus->enemyActors); j++) {
            Actor* targetActor = battleStatus->enemyActors[j];
            if (targetActor == NULL) {
                continue;
            }

            missingHp = targetActor->maxHP - targetActor->curHP;
            // max possible heal
            itemHeal = itemData->potencyA;

            if (itemHeal > targetActor->maxHP - 1) {
                itemHeal = targetActor->maxHP - 1;
            }

            if (itemHeal <= missingHp) {
                // worth to use the item
                // TODO: figure out which enemy is the most worth to heal
                actor->targetActorID = j | ACTOR_ENEMY0;
                actor->targetPartID = 1;
                itemUsedOn = j | ACTOR_ENEMY0;
                break;
            }
        }

        if (itemUsedOn != FALSE)
            break;
    }

    evt_set_variable(script, outVar, itemUsedOn);
    evt_set_variable(script, outIndexVar, i);

    return ApiStatus_DONE2;
}

EvtScript EnemyItems_TryUseHeldItem_WithAI = {
    // ai
    // also loads up LVar8 with the index of the item, for EnemyItems_UseHeldItem
    EVT_CALL(_HeldItemUseAI, ACTOR_SELF, LVar0, LVar8)
    EVT_IF_EQ(LVar0, 0)
        EVT_RETURN // no item, early return
    EVT_END_IF
    EVT_CALL(SetTargetActor, ACTOR_SELF, LVar0)
    EVT_EXEC_WAIT(EnemyItems_UseHeldItem)
    EVT_SET(LVar0, 1)
    EVT_RETURN
    EVT_END
};
