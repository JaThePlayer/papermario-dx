#include "enemy_items/api.h"
#include "enemy_items/common_tables.h"
#include "common.h"
#include "script_api/battle.h"

ItemChance prologue_ch1_items[] = {
    WEIGHTED(ITEM_MUSHROOM, 70),
    WEIGHTED(ITEM_THUNDER_BOLT, 5),
    WEIGHTED(ITEM_LIFE_SHROOM, 5),
    WEIGHTED(ITEM_NONE, 20),
};
