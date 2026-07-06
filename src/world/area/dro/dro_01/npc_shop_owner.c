#include "dro_01.h"

EvtScript N(EVS_NpcIdle_ShopOwner) = {
    // Skip the item purchase sequence
    IfLt(GB_StoryProgress, STORY_CH2_BOUGHT_SECRET_ITEMS)
        Set(GB_StoryProgress, STORY_CH2_BOUGHT_SECRET_ITEMS)
    EndIf
    Call(SetNpcAnimation, NPC_SELF, ANIM_Mouser_Purple_Idle)
    Call(SetNpcPos, NPC_SELF, 20, 0, -375)
    Return
    End
};

LetterDelivery N(LetterDelivery_ShopOwner) = {
    .recipientID = NPC_Mouser_ShopOwner,
    .recipientTalk = ANIM_Mouser_Purple_Talk,
    .recipientIdle = ANIM_Mouser_Purple_Idle,
    .msgGreeting = MSG_CH2_0089,
    .msgCancelled = MSG_CH2_008A,
    .msgDelivered = MSG_CH2_008B,
    .msgRecieved = MSG_CH2_008C,
    .letters = { ITEM_LETTER_CHAIN_LITTLE_MOUSER },
    .reward = ITEM_LETTER_CHAIN_FRANKY,
};

EvtScript N(EVS_NpcInteract_ShopOwner) = {
    ExecWait(EVS_ShopOwnerDialog)
    Set(LVar0, Ref(N(LetterDelivery_ShopOwner)))
    ExecWait(EVS_TryLetterDelivery)
    Return
    End
};

EvtScript N(EVS_NpcInit_ShopOwner) = {
    Call(BindNpcIdle, NPC_SELF, Ref(N(EVS_NpcIdle_ShopOwner)))
    Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_ShopOwner)))
    Return
    End
};
