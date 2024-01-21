#include "food_poisoning.h"

FOOD_POISON_ITEM_ID usedItems[20];

s32 food_poison_was_used(FOOD_POISON_ITEM_ID itemId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(usedItems); i++) {
        if (usedItems[i] == itemId) {
            return TRUE;
        }
    }

    return FALSE;
}

void food_poison_mark_used(FOOD_POISON_ITEM_ID itemId) {
    s32 i;

    if (food_poison_was_used(itemId))
        return;

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
