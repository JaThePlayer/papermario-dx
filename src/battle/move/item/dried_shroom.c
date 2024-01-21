// Used for food poisoning, not actually for dried shrooms

#include "common.h"
#include "script_api/battle.h"
#include "effects.h"
#include "sprite/player.h"
#include "enemy_items/api.h"

#define NAMESPACE battle_item_dried_shroom

#include "battle/common/move/ItemRefund.inc.c"

#include "battle/common/move/UseItem.inc.c"

EvtScript N(EVS_UseItem) = {
    STANDARD_HEALING_ITEM_ENEMY_USE()

    Call(GetMenuSelection, LVar0, LVarA, LVar2)
    ExecWait(N(UseItemWithEffect))
    ExecWait(N(EatItem))
    Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_StickOutTongue)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar1, 25)
    Add(LVar2, 5)
    Call(ShowStartRecoveryShimmer, LVar0, LVar1, LVar2, 1)
    Wait(30)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar2, 5)
    Call(ShowRecoveryShimmer, LVar0, LVar1, LVar2, 1)
    Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_Mario1_Idle)
    Wait(20)
    ExecWait(N(PlayerGoHome))
    Return
    End
};
