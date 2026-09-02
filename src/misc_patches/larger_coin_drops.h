#pragma once

#include "common.h"

// Allows for larger coin drops by dropping various coin types of bigger sell value.

// External patches needed:
// - world/enemy_drops.c
// - entity/ItemBlock.c -> entity_ItemBlock_spawn_item
// - item_entity.c -> make_item_entity, make_item_entity_at_player, ...

// Needs to be sorted descending by sell value.
extern enum ItemIDs gCoinDropTypes[3];

/// Whether this item id is considered a coin item, used to replace == ITEM_COIN checks.
b32 is_coin_item(enum ItemIDs itemId);

