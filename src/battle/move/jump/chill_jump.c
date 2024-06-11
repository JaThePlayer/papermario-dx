#include "common.h"
#include "script_api/battle.h"
#include "battle/action_cmd/jump.h"
#include "misc_patches/custom_status.h"

#define NAMESPACE battle_move_chill_jump

#include "battle/common/move/JumpSupport.inc.c"

extern EvtScript N(EVS_UseMove_ImplA);
extern EvtScript N(EVS_UseMove_ImplB);
extern EvtScript N(EVS_UseMove_ImplC);

API_CALLABLE(N(setup_status)) {
    switch (gBattleStatus.selectedMoveID)
    {
        case MOVE_CHILL_JUMP:
            set_next_attack_custom_status(ATK_DOWN_TEMP_STATUS, 3, 1, 100);
            break;
    }

    return ApiStatus_DONE2;
}

EvtScript N(EVS_UseMove) = {
    Call(EnablePlayerBlur, ACTOR_BLUR_ENABLE)
    Call(ShowActionHud, TRUE)
    Call(GetMenuSelection, LVar0, LVar1, LVar2)
    Switch(LVar1)
        CaseEq(0)
            ExecWait(N(EVS_UseMove_ImplA))
        CaseEq(1)
            ExecWait(N(EVS_UseMove_ImplB))
        CaseEq(2)
            ExecWait(N(EVS_UseMove_ImplC))
    EndSwitch
    Call(EnablePlayerBlur, ACTOR_BLUR_DISABLE)
    Return
    End
};

EvtScript N(EVS_802A26B4) = {
    Call(StartRumble, BTL_RUMBLE_PLAYER_HEAVY)
    ChildThread
        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(0.5))
        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.5))
        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(0.5))
        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(0.2))
    EndChildThread
    Return
    End
};

EvtScript N(EVS_UseMove_ImplA) = {
    ExecWait(N(EVS_JumpSupport_InitCommand))
    Call(PlayerTestEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 1, 0)
    IfEq(LVar0, HIT_RESULT_MISS)
        ExecWait(N(EVS_JumpSupport_Miss))
        Return
    EndIf
    ExecWait(N(EVS_802A26B4))
    Wait(1)
    Call(GetPlayerActionSuccess, LVar0)
    Switch(LVar0)
        CaseGt(FALSE)
            Call(SetActorSounds, ACTOR_PLAYER, ACTOR_SOUND_HURT, SOUND_ACTOR_JUMPED_1, SOUND_NONE)
            Call(N(setup_status))
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS | BS_FLAGS1_NICE_HIT)
        CaseDefault
            Call(SetActorSounds, ACTOR_PLAYER, ACTOR_SOUND_HURT, SOUND_ACTOR_JUMPED_1, SOUND_NONE)
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS)
    EndSwitch
    Switch(LVar0)
        CaseGt(HIT_RESULT_HIT)
            ExecWait(N(EVS_JumpSupport_E))
        CaseDefault
            ExecWait(N(EVS_JumpSupport_F))
    EndSwitch
    Return
    End
};

EvtScript N(EVS_UseMove_ImplB) = {
    ExecWait(N(EVS_JumpSupport_InitCommand))
    Call(PlayerTestEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 1, 0)
    IfEq(LVar0, HIT_RESULT_MISS)
        ExecWait(N(EVS_JumpSupport_Miss))
        Return
    EndIf
    ExecWait(N(EVS_802A26B4))
    Wait(1)
    Call(GetPlayerActionSuccess, LVar0)
    Switch(LVar0)
        CaseGt(FALSE)
            Call(SetActorSounds, ACTOR_PLAYER, ACTOR_SOUND_HURT, SOUND_ACTOR_JUMPED_2, SOUND_NONE)
            Call(N(setup_status))
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 4, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS | BS_FLAGS1_NICE_HIT)
        CaseDefault
            Call(SetActorSounds, ACTOR_PLAYER, ACTOR_SOUND_HURT, SOUND_ACTOR_JUMPED_2, SOUND_NONE)
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS)
    EndSwitch
    Switch(LVar0)
        CaseGt(HIT_RESULT_HIT)
            ExecWait(N(EVS_JumpSupport_E))
        CaseDefault
            ExecWait(N(EVS_JumpSupport_F))
    EndSwitch
    Return
    End
};

EvtScript N(EVS_UseMove_ImplC) = {
    ExecWait(N(EVS_JumpSupport_InitCommand))
    Call(PlayerTestEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 1, 0)
    IfEq(LVar0, HIT_RESULT_MISS)
        ExecWait(N(EVS_JumpSupport_Miss))
        Return
    EndIf
    ExecWait(N(EVS_802A26B4))
    Wait(1)
    Call(GetPlayerActionSuccess, LVar0)
    Switch(LVar0)
        CaseGt(FALSE)
            Call(SetActorSounds, ACTOR_PLAYER, ACTOR_SOUND_HURT, SOUND_ACTOR_JUMPED_3, SOUND_NONE)
            Call(N(setup_status))
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 6, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS | BS_FLAGS1_NICE_HIT)
        CaseDefault
            Call(SetActorSounds, ACTOR_PLAYER, ACTOR_SOUND_HURT, SOUND_ACTOR_JUMPED_3, SOUND_NONE)
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_JUMP, 0, 0, 3, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS)
    EndSwitch
    Switch(LVar0)
        CaseGt(HIT_RESULT_HIT)
            ExecWait(N(EVS_JumpSupport_E))
        CaseDefault
            ExecWait(N(EVS_JumpSupport_F))
    EndSwitch
    Return
    End
};
