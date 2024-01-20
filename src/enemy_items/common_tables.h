#ifndef _ENEMY_ITEM_COMMON_ITEMS_H_
#define _ENEMY_ITEM_COMMON_ITEMS_H_

#include "enemy_items/api.h"

// Used when defining weighted arrays to quickly create an element.
#define WEIGHTED(item, itemWeight) { .itemID = item, .weight = itemWeight }

extern ItemChance prologue_ch1_items[];

#endif
