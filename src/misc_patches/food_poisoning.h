#ifndef _FOOD_POISON_H_
#define _FOOD_POISON_H_

#include "common.h"

#define FOOD_POISON_ITEM_ID s16

s32 food_poison_was_used(FOOD_POISON_ITEM_ID itemId);

void food_poison_mark_used(FOOD_POISON_ITEM_ID itemId);

void food_poison_clear();

#endif
