#include "larger_coin_drops.h"
#include "item_enum.h"

enum ItemIDs gCoinDropTypes[3] = {
    ITEM_RED_COIN,
    ITEM_BLUE_COIN,
    ITEM_COIN,
};

b32 is_coin_item(enum ItemIDs itemId) {
    return itemId == ITEM_RED_COIN
        || itemId == ITEM_BLUE_COIN
        || itemId == ITEM_COIN;
}
