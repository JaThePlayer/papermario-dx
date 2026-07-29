#include "food_poisoning.h"
#include "misc_patches/item_effects.h"
#include "misc_patches/misc_patches.h"

FOOD_POISON_ITEM_ID usedItems[20];

s32 food_poison_was_used(FOOD_POISON_ITEM_ID itemId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(usedItems); i++) {
        if (usedItems[i] == itemId) {
            return true;
        }
    }

    return false;
}

void food_poison_mark_used(FOOD_POISON_ITEM_ID itemId) {
    s32 i;

    if (food_poison_was_used(itemId))
        return;

    ItemEffect* effect = gItemEffectTable[itemId];
    while (effect != nullptr && effect->type != nullptr) {
        // Mush Power makes food poisoning not trigger on mushroom badges.
        if (effect->type == &item_effects_mush_power && player_count_badges_with_move_id(MOVE_MUSH_POWER) > 0) {
            return;
        }

        effect++;
    }

    for (i = 0; i < ARRAY_COUNT(usedItems); i++) {
        if (usedItems[i] == ITEM_NONE) {
            usedItems[i] = itemId;
            return;
        }
    }
}

void food_poison_clear() {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(usedItems); i++) {
        usedItems[i] = ITEM_NONE;
    }
}
