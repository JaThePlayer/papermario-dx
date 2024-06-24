#include "common.h"
#include "message_ids.h"
#include "world/common/npc/Boo.inc.c"

#ifndef NPC_PINBoo
    #error NPC_PINBoo must be defined for portable_item_network_boo.inc.c
#endif

#ifndef NPC_PINBoo_Pos
    #error NPC_PINBoo_Pos must be defined for portable_item_network_boo.inc.c
#endif

AnimID N(ExtraAnims_PINBoo)[] = {
    ANIM_Boo_Still,
    ANIM_LIST_END
};

EvtScript N(EVS_PINBoo_Empty) = {
    Return
    End
};

EvtScript N(EVS_PINBoo_OtherAI) = {
    Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_HAS_NO_SPRITE, FALSE)
    Call(SetNpcSprite, -1, ANIM_Boo_Idle)
    Return
    End
};

s32 N(PINBoo_ShopMessages)[] = {
    [SHOP_MSG_BUY_CONFIRM      ] MSG_NONE,
    [SHOP_MSG_NOT_ENOUGH_COINS ] MSG_NONE,
    [SHOP_MSG_NOT_ENOUGH_ROOM  ] MSG_Shop_PortableStorage_NotEnoughRoom,
    [SHOP_MSG_BUY_THANK_YOU    ] MSG_NONE,
    [SHOP_MSG_GREETING         ] MSG_Shop_PortableStorage_Greeting_FirstTime,
    [SHOP_MSG_INSTRUCTIONS     ] MSG_NONE,
    [SHOP_MSG_NOTHING_TO_SELL  ] MSG_Shop_PortableStorage_NothingToSell,
    [SHOP_MSG_SELL_WHICH       ] MSG_Shop_PortableStorage_WhatToSell,
    [SHOP_MSG_SELL_CONFIRM     ] MSG_NONE,
    [SHOP_MSG_SELL_CANCEL      ] MSG_NONE,
    [SHOP_MSG_SELL_MORE        ] MSG_NONE,
    [SHOP_MSG_SELL_THANKS      ] MSG_NONE,
    [SHOP_MSG_NOTHING_TO_CHECK ] MSG_Shop_PortableStorage_NothingToCheck,
    [SHOP_MSG_NO_CHECK_ROOM    ] MSG_Shop_PortableStorage_NoCheckRoom,
    [SHOP_MSG_CHECK_WHICH      ] MSG_Shop_PortableStorage_StoreWhich,
    [SHOP_MSG_CHECK_ACCEPTED   ] MSG_NONE,
    [SHOP_MSG_CHECK_MORE       ] MSG_NONE,
    [SHOP_MSG_NOTHING_TO_CLAIM ] MSG_Shop_PortableStorage_NothingToClaim,
    [SHOP_MSG_NO_CLAIM_ROOM    ] MSG_Shop_PortableStorage_NoClaimSpace,
    [SHOP_MSG_CLAIM_WHICH      ] MSG_Shop_PortableStorage_ClaimWhich,
    [SHOP_MSG_CLAIM_ACCEPTED   ] MSG_NONE,
    [SHOP_MSG_CLAIM_MORE       ] MSG_NONE,
    [SHOP_MSG_FAREWELL         ] MSG_Shop_PortableStorage_Farewell,
};

API_CALLABLE(N(PINBoo_SetupGreeting)) {
    s32 hasSeen = evt_get_variable(NULL, GF_PortableItemNetworkIntro);

    N(PINBoo_ShopMessages)[SHOP_MSG_GREETING] = hasSeen ? MSG_Shop_PortableStorage_Greeting_Normal : MSG_Shop_PortableStorage_Greeting_FirstTime;

    evt_set_variable(NULL, GF_PortableItemNetworkIntro, TRUE);

    return ApiStatus_DONE2;
}

EvtScript N(EVS_PINBoo_Interact) = {
    Call(N(PINBoo_SetupGreeting))
    ExecWait(EVS_ShopOwnerDialog)
    Return
    End
};

ShopItemLocation N(ItemPositions)[] = {
};
ShopItemData N(Inventory)[] = {
    {}
};
ShopSellPriceData N(PriceList)[] = {
    {}
};

ShopOwner N(PINOwner) = {
    .npcID = NPC_PINBoo,
    .idleAnim = ANIM_Boo_Idle,
    .talkAnim = ANIM_Boo_Walk,
    .onBuyEvt = &N(EVS_PINBoo_Empty),
    .shopMsgIDs = N(PINBoo_ShopMessages),
};

NpcSettings N(PINBooNpcSettings) = {
    .defaultAnim = ANIM_Boo_Idle,
    .height = 35,
    .radius = 28,
    .otherAI = &N(EVS_PINBoo_OtherAI),
    .onInteract = &N(EVS_PINBoo_Interact),
    .aux = &N(EVS_PINBoo_Empty),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING,
    .level = ACTOR_LEVEL_NONE,
};

NpcData N(NpcData_PINBoo)[] = {
    {
        .id = NPC_PINBoo,
        .pos = { NPC_PINBoo_Pos },
        .yaw = 263,
        .settings = &N(PINBooNpcSettings),
        .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_4,
        .drops = NO_DROPS,
        .animations = NORMAL_BOO_ANIMS,
        .tattle = MSG_NpcTattle_TrialBoo,
    },
};

EvtScript N(EVS_SetupPortableItemNetworkBoo) = {
    Call(MakeShop, Ref(N(ItemPositions)), Ref(N(Inventory)), Ref(N(PriceList)), 0)
    Call(MakeShopOwner, Ref(N(PINOwner)))
    Return
    End
};
