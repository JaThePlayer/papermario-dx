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

EvtScript N(EVS_NpcInteract_ShopOwner) = {
    ExecWait(EVS_ShopOwnerDialog)
    ExecWait(N(EVS_LetterPrompt_ShopOwner))
    IfNe(LVarC, DELIVERY_NOT_POSSIBLE)
        Return
    EndIf
    Return
    End
};

EvtScript N(EVS_NpcInit_ShopOwner) = {
    Call(BindNpcIdle, NPC_SELF, Ref(N(EVS_NpcIdle_ShopOwner)))
    Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_ShopOwner)))
    Return
    End
};
