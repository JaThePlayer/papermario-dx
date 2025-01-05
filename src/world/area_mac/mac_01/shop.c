#include "mac_01.h"

typedef struct RowfBadgeOption {
    u32 itemID;
    s32 price;
    s32 story;
} RowfBadgeOption;

RowfBadgeOption N(RowfBadgeInventory)[] = {
    { .itemID = ITEM_SPIN_SMASH,     .price =  50, .story = STORY_CH1_STAR_SPRIT_DEPARTED },
    { .itemID = ITEM_FIRST_ATTACK,   .price = 100, .story = STORY_CH1_STAR_SPRIT_DEPARTED  },
    { .itemID = ITEM_MONEY_MONEY,    .price = 100, .story = STORY_CH1_STAR_SPRIT_DEPARTED  },
    { .itemID = ITEM_D_DOWN_POUND,   .price =  75, .story = STORY_CH1_STAR_SPRIT_DEPARTED  },

    { .itemID = ITEM_DODGE_MASTER,   .price = 100, .story = STORY_CH2_STAR_SPRIT_DEPARTED },
    { .itemID = ITEM_SLEEP_STOMP,    .price =  75, .story = STORY_CH2_STAR_SPRIT_DEPARTED },
    { .itemID = ITEM_DOUBLE_DIP,     .price = 100, .story = STORY_CH2_STAR_SPRIT_DEPARTED },

    { .itemID = ITEM_JUMP_CHARGE,    .price =  50, .story = STORY_CH3_STAR_SPRIT_DEPARTED },
//    { .itemID = ITEM_SPIN_SMASH,     .price =  75, .story = STORY_CH3_STAR_SPRIT_DEPARTED },
    { .itemID = ITEM_GROUP_FOCUS,    .price = 100, .story = STORY_CH3_STAR_SPRIT_DEPARTED },

    { .itemID = ITEM_ALLOR_NOTHING,  .price = 100, .story = STORY_CH4_STAR_SPRIT_DEPARTED },
    { .itemID = ITEM_HP_PLUS_C,      .price = 150, .story = STORY_CH4_STAR_SPRIT_DEPARTED },
    { .itemID = ITEM_FP_PLUS_C,      .price = 150, .story = STORY_CH4_STAR_SPRIT_DEPARTED },

    { .itemID = ITEM_S_SMASH_CHG,    .price = 100, .story = STORY_CH5_RETURNED_TO_TOAD_TOWN },
    { .itemID = ITEM_DAMAGE_DODGE_A, .price = 150, .story = STORY_CH5_RETURNED_TO_TOAD_TOWN },
    { .itemID = ITEM_MEGA_QUAKE,     .price = 200, .story = STORY_CH5_RETURNED_TO_TOAD_TOWN },
};

API_CALLABLE(N(func_80244984_805204)) {
    s32 count = 0;

    for (s32 i = 0; i < ARRAY_COUNT(N(RowfBadgeInventory)); i++) {
        if (find_item(N(RowfBadgeInventory)[i].itemID) != -1) {
            count++;
        }
    }
    script->varTable[0] = count;
    return ApiStatus_DONE2;
}

API_CALLABLE(N(HideRowfBadge)) {
    s32 itemIndex = evt_get_variable(script, *script->ptrReadPos);

    set_item_entity_flags(gGameStatusPtr->shopItemEntities[itemIndex].index, ITEM_ENTITY_FLAG_HIDDEN);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetRowfBadgeBought)) {
    s32 itemIndex = evt_get_variable(script, *script->ptrReadPos);

    set_item_entity_flags(gGameStatusPtr->shopItemEntities[itemIndex].index, ITEM_ENTITY_FLAG_HIDDEN);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(CreateShopInventory)) {
    s32 varBaseUnlocked = GF_MAC01_UnlockedRowfBadge_00;
    s32 varBaseHasBought = GF_MAC01_RowfBadge_00;
    s32 options[ARRAY_COUNT(N(RowfBadgeInventory))];
    s32 itemID;
    s32 shopIdx;
    s32 randIdx;
    s32 available;
    s32 count;
    s32 i;

    ShopItemData* inventory = heap_malloc(4 * sizeof(ShopItemData));
    inventory[0].itemID = 0;
    inventory[1].itemID = 0;
    inventory[2].itemID = 0;
    inventory[3].itemID = 0;

    s32 storyProgress = evt_get_variable(NULL, GB_StoryProgress);
    if (!evt_get_variable(script, GF_MAC01_RowfBadgesChosen)) {
        available = 0;
        for (i = 0; i < (u32) ARRAY_COUNT(options); i++) {
            RowfBadgeOption* opt = &N(RowfBadgeInventory)[i];
            s32 isUnlocked = storyProgress >= opt->story;
            s32 hasBought = find_item(opt->itemID) != -1;
            if ((isUnlocked == 1) && (hasBought == 0)) {
                options[available++] = i;
            }
        }

        count = 0;
        while (available != 0) {
            randIdx = rand_int(available - 1);
            shopIdx = options[randIdx];
            RowfBadgeOption* opt = &N(RowfBadgeInventory)[shopIdx];

            inventory[count].itemID = opt->itemID;
            inventory[count].price = opt->price;
            inventory[count].descMsg = gItemTable[opt->itemID].fullDescMsg;

            evt_set_variable(script, GB_MAC01_Rowf_Badge0 + count, shopIdx);

            count++;
            if (count >= 3)
                break;

            for (i = randIdx; i < available - 1; i++) {
                options[i] = options[i + 1];
            }
            available--;
        }
        evt_set_variable(script, GB_MAC01_Rowf_NumBadges, count);
        evt_set_variable(script, GF_MAC01_RowfBadgesChosen, TRUE);
        script->varTable[3] = FALSE;
    } else {
        count = evt_get_variable(script, GB_MAC01_Rowf_NumBadges);
        for (i = 0; i < count; i++) {
            shopIdx = evt_get_variable(script, GB_MAC01_Rowf_Badge0 + i);
            RowfBadgeOption* opt = &N(RowfBadgeInventory)[shopIdx];
            inventory[i].itemID = opt->itemID;
            inventory[i].price = opt->price;
            inventory[i].descMsg = gItemTable[opt->itemID].fullDescMsg;
        }
        script->varTable[3] = TRUE;
    }

    script->varTable[0] = count;
    script->varTablePtr[2] = inventory;
    return ApiStatus_DONE2;
}

s32 N(ShopMessages)[] = {
    [SHOP_MSG_BUY_CONFIRM      ] MSG_Shop_00_MAC01,
    [SHOP_MSG_NOT_ENOUGH_COINS ] MSG_Shop_01_MAC01,
    [SHOP_MSG_NOT_ENOUGH_ROOM  ] MSG_Shop_02_MAC00,
    [SHOP_MSG_BUY_THANK_YOU    ] MSG_Shop_03_MAC01,
    [SHOP_MSG_GREETING         ] MSG_Shop_04_MAC00,
    [SHOP_MSG_INSTRUCTIONS     ] MSG_Shop_05_MAC00,
    [SHOP_MSG_NOTHING_TO_SELL  ] MSG_Shop_06_MAC00,
    [SHOP_MSG_SELL_WHICH       ] MSG_Shop_07_MAC00,
    [SHOP_MSG_SELL_CONFIRM     ] MSG_Shop_08_MAC00,
    [SHOP_MSG_SELL_CANCEL      ] MSG_Shop_09_MAC00,
    [SHOP_MSG_SELL_MORE        ] MSG_Shop_0A_MAC00,
    [SHOP_MSG_SELL_THANKS      ] MSG_Shop_0B_MAC00,
    [SHOP_MSG_NOTHING_TO_CHECK ] MSG_Shop_0C_MAC00,
    [SHOP_MSG_NO_CHECK_ROOM    ] MSG_Shop_0D_MAC00,
    [SHOP_MSG_CHECK_WHICH      ] MSG_Shop_0E_MAC00,
    [SHOP_MSG_CHECK_ACCEPTED   ] MSG_Shop_0F_MAC00,
    [SHOP_MSG_CHECK_MORE       ] MSG_Shop_10_MAC00,
    [SHOP_MSG_NOTHING_TO_CLAIM ] MSG_Shop_11_MAC00,
    [SHOP_MSG_NO_CLAIM_ROOM    ] MSG_Shop_12_MAC00,
    [SHOP_MSG_CLAIM_WHICH      ] MSG_Shop_13_MAC00,
    [SHOP_MSG_CLAIM_ACCEPTED   ] MSG_Shop_14_MAC00,
    [SHOP_MSG_CLAIM_MORE       ] MSG_Shop_15_MAC00,
    [SHOP_MSG_FAREWELL         ] MSG_Shop_16_MAC01,
};

ShopSellPriceData N(RowfDummyPriceList)[] = {
    { .itemID = ITEM_SHOOTING_STAR,  .sellPrice = 0 },
    {}
};

EvtScript N(EVS_OnBuy) = {
    Switch(LVar0)
        CaseEq(SHOP_BUY_RESULT_CANCEL)
        CaseEq(SHOP_BUY_RESULT_4)
        CaseEq(SHOP_BUY_RESULT_OK)
            Switch(LVar2)
                CaseEq(0) // left badge shop slot
                    Set(GF_MAC01_RowfBadgeAvailableA, TRUE)
                    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_b3, COLLIDER_FLAGS_UPPER_MASK)
                CaseEq(1) // middle badge shop slot
                    Set(GF_MAC01_RowfBadgeAvailableB, TRUE)
                    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_b2, COLLIDER_FLAGS_UPPER_MASK)
                CaseEq(2) // right badge shop slot
                    Set(GF_MAC01_RowfBadgeAvailableC, TRUE)
                    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_b1, COLLIDER_FLAGS_UPPER_MASK)
            EndSwitch
            Call(N(SetRowfBadgeBought), LVar2)
        CaseEq(SHOP_BUY_RESULT_2)
    EndSwitch
    Return
    End
};

ShopItemLocation N(RowfItemPositions)[] = {
    { .posModelID = MODEL_b3, .triggerColliderID = COLLIDER_b3 },
    { .posModelID = MODEL_b2, .triggerColliderID = COLLIDER_b2 },
    { .posModelID = MODEL_b1, .triggerColliderID = COLLIDER_b1 },
};

ShopOwner N(ShopOwnerRowf) = {
    .npcID = NPC_Rowf,
    .idleAnim = ANIM_Rowf_Idle,
    .talkAnim = ANIM_Rowf_Talk,
    .onBuyEvt = &N(EVS_OnBuy),
    .shopMsgIDs = N(ShopMessages),
};

EvtScript N(EVS_SetupBadgeShop) = {
    Call(N(CreateShopInventory))
    IfEq(LVar3, 0)
        Set(GF_MAC01_RowfBadgeAvailableA, FALSE)
        Set(GF_MAC01_RowfBadgeAvailableB, FALSE)
        Set(GF_MAC01_RowfBadgeAvailableC, FALSE)
        IfLt(LVar0, 3)
            Set(GF_MAC01_RowfBadgeAvailableC, TRUE)
        EndIf
        IfLt(LVar0, 2)
            Set(GF_MAC01_RowfBadgeAvailableB, TRUE)
        EndIf
        IfLt(LVar0, 1)
            Set(GF_MAC01_RowfBadgeAvailableA, TRUE)
        EndIf
    EndIf
    Call(MakeShop, Ref(N(RowfItemPositions)), LVar2, Ref(N(RowfDummyPriceList)), 0)
    Call(MakeShopOwner, Ref(N(ShopOwnerRowf)))
    IfEq(LVar3, 1)
        IfGe(LVar0, 3)
            IfEq(GF_MAC01_RowfBadgeAvailableC, TRUE)
                Call(N(SetRowfBadgeBought), 2)
            EndIf
        EndIf
        IfGe(LVar0, 2)
            IfEq(GF_MAC01_RowfBadgeAvailableB, TRUE)
                Call(N(SetRowfBadgeBought), 1)
            EndIf
        EndIf
        IfGe(LVar0, 1)
            IfEq(GF_MAC01_RowfBadgeAvailableA, TRUE)
                Call(N(SetRowfBadgeBought), 0)
            EndIf
        EndIf
    EndIf
    Return
    End
};
