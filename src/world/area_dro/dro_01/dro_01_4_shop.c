#include "dro_01.h"
#define NAMESPACE dro_01_Shop
#include "sprite/player.h"

s32 N(ShopMessages)[] = {
    [SHOP_MSG_BUY_CONFIRM      ] MSG_Shop_00_DR001,
    [SHOP_MSG_NOT_ENOUGH_COINS ] MSG_Shop_01_DR001,
    [SHOP_MSG_NOT_ENOUGH_ROOM  ] MSG_Shop_02_DR001,
    [SHOP_MSG_BUY_THANK_YOU    ] MSG_Shop_03_DR001,
    [SHOP_MSG_GREETING         ] MSG_Shop_04_DR001,
    [SHOP_MSG_INSTRUCTIONS     ] MSG_Shop_05_DR001,
    [SHOP_MSG_NOTHING_TO_SELL  ] MSG_Shop_06_DR001,
    [SHOP_MSG_SELL_WHICH       ] MSG_Shop_07_DR001,
    [SHOP_MSG_SELL_CONFIRM     ] MSG_Shop_08_DR001,
    [SHOP_MSG_SELL_CANCEL      ] MSG_Shop_09_DR001,
    [SHOP_MSG_SELL_MORE        ] MSG_Shop_0A_DR001,
    [SHOP_MSG_SELL_THANKS      ] MSG_Shop_0B_DR001,
    [SHOP_MSG_NOTHING_TO_CHECK ] MSG_Shop_0C_DR001,
    [SHOP_MSG_NO_CHECK_ROOM    ] MSG_Shop_0D_DR001,
    [SHOP_MSG_CHECK_WHICH      ] MSG_Shop_0E_DR001,
    [SHOP_MSG_CHECK_ACCEPTED   ] MSG_Shop_0F_DR001,
    [SHOP_MSG_CHECK_MORE       ] MSG_Shop_10_DR001,
    [SHOP_MSG_NOTHING_TO_CLAIM ] MSG_Shop_11_DR001,
    [SHOP_MSG_NO_CLAIM_ROOM    ] MSG_Shop_12_DR001,
    [SHOP_MSG_CLAIM_WHICH      ] MSG_Shop_13_DR001,
    [SHOP_MSG_CLAIM_ACCEPTED   ] MSG_Shop_14_DR001,
    [SHOP_MSG_CLAIM_MORE       ] MSG_Shop_15_DR001,
    [SHOP_MSG_FAREWELL         ] MSG_Shop_16_DR001,
};

ShopItemData N(Inventory)[] = {
    { .itemID = ITEM_THUNDER_BOLT,   .price = 5, .descMsg = MSG_ItemFullDesc_ThunderBolt },
    { .itemID = ITEM_DRIED_FRUIT,    .price = 11, .descMsg = MSG_ItemFullDesc_DriedFruit },
    { .itemID = ITEM_POISON_SHROOM,  .price = 4, .descMsg = MSG_ItemFullDesc_PoisonShroom },
    { .itemID = ITEM_DRIED_SHROOM,   .price = 2, .descMsg = MSG_ItemFullDesc_DriedShroom },
    { .itemID = ITEM_DRIED_PASTA,    .price = 3, .descMsg = MSG_ItemFullDesc_DriedPasta },
    { .itemID = ITEM_MUSHROOM,       .price = 3, .descMsg = MSG_ItemFullDesc_Mushroom },
    {},
};

ShopSellPriceData N(PriceList)[] = {
    { .itemID = ITEM_SNOWMAN_DOLL,  .sellPrice = 12 },
    { .itemID = ITEM_MELON,         .sellPrice = 10 },
    { .itemID = ITEM_ICED_POTATO,   .sellPrice = 10 },
    { .itemID = ITEM_TASTY_TONIC,   .sellPrice =  5 },
    { .itemID = ITEM_SUPER_SODA,    .sellPrice =  6 },
    { .itemID = ITEM_SPECIAL_SHAKE, .sellPrice = 15 },
    { .itemID = ITEM_DRIED_PASTA,   .sellPrice =  2 },
    {},
};

#include "world/common/complete/GiveReward.inc.c"

API_CALLABLE(N(HideCoinCounter)) {
    hide_coin_counter_immediately();
    return ApiStatus_DONE2;
}

EvtScript N(EVS_JumpToPlayer) = {
    Call(NpcJump0, NPC_Mouser_ShopOwner, -31, 0, -283, 10)
    Call(NpcJump0, NPC_Mouser_ShopOwner, -30, 0, -283, 8)
    Return
    End
};

EvtScript N(EVS_JumpAway) = {
    Call(NpcJump0, NPC_Mouser_ShopOwner, 20, 0, -259, 10)
    Call(NpcJump0, NPC_Mouser_ShopOwner, 20, 0, -259, 4)
    Return
    End
};

EvtScript N(EVS_OnBuy) = {
    Return
    End
};

ShopItemLocation N(ItemPositions)[] = {
    { .posModelID = MODEL_o741, .triggerColliderID = COLLIDER_mono1 },
    { .posModelID = MODEL_o740, .triggerColliderID = COLLIDER_mono2 },
    { .posModelID = MODEL_o738, .triggerColliderID = COLLIDER_mono3 },
    { .posModelID = MODEL_o739, .triggerColliderID = COLLIDER_mono4 },
    { .posModelID = MODEL_o737, .triggerColliderID = COLLIDER_mono5 },
    { .posModelID = MODEL_o736, .triggerColliderID = COLLIDER_mono6 },
};

ShopOwner N(Owner) = {
    .npcID = NPC_Mouser_ShopOwner,
    .idleAnim = ANIM_Mouser_Purple_Idle,
    .talkAnim = ANIM_Mouser_Purple_Talk,
    .onBuyEvt = &N(EVS_OnBuy),
    .shopMsgIDs = N(ShopMessages),
};

#define NAMESPACE dro_01
