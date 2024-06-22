#include "iwa_03.h"

SlideParams N(SlideData) = {
    .heading = 89.0f,
    .maxDescendAccel = 0.41f,
    .launchVel = -0.25f,
    .maxDescendVel = 16.0f,
    .integrator = { 9.612, -0.452, 0.003, -0.023 },
};

API_CALLABLE(N(SetPlayerSliding)) {
    gPlayerStatus.slideParams = script->varTablePtr[0];
    gPlayerStatus.flags |= PS_FLAG_SLIDING;
    return ApiStatus_DONE2;
}

EvtScript N(EVS_UpdateSliding) = {
    Set(LVar0, Ref(N(SlideData)))
    Call(N(SetPlayerSliding))
    Return
    End
};

EvtScript N(EVS_UseSlide) = {
    Call(DisablePlayerInput, TRUE)
    Wait(10)
    Thread
        Call(PlaySoundAtCollider, COLLIDER_st1, SOUND_IWA_SLIDE_GATE, SOUND_SPACE_DEFAULT)
        Call(MakeLerp, 0, -80, 10, EASING_LINEAR)
        Loop(0)
            Call(UpdateLerp)
            Call(RotateModel, MODEL_se1, LVar0, 0, -1, 0)
            Wait(1)
            IfEq(LVar1, 0)
                BreakLoop
            EndIf
        EndLoop
    EndThread
    Wait(15)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_st1, COLLIDER_FLAGS_UPPER_MASK)
    Call(SetPlayerSpeed, Float(3.0))
    Call(PlayerMoveTo, -1460, -60, 0)
    Call(DisablePlayerInput, FALSE)
    Wait(100)
    Thread
        Call(PlaySoundAtCollider, COLLIDER_st1, SOUND_IWA_SLIDE_GATE, SOUND_SPACE_DEFAULT)
        Call(MakeLerp, -80, 0, 30, EASING_LINEAR)
        Loop(0)
            Call(UpdateLerp)
            Call(RotateModel, MODEL_se1, LVar0, 0, -1, 0)
            Wait(1)
            IfEq(LVar1, 0)
                BreakLoop
            EndIf
        EndLoop
    EndThread
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_CLEAR_BITS, COLLIDER_st1, COLLIDER_FLAGS_UPPER_MASK)
    Return
    End
};

b32 N(should_player_be_sliding)(void) {
    Shadow* shadow = get_shadow_by_index(gPlayerStatus.shadowID);
    f32 angle = shadow->rot.z + 180.0;

    return (angle != 0.0f) && (fabsf(angle) >= 20.0f);
}

API_CALLABLE(N(SetupSlidingCheck)) {
    phys_set_player_sliding_check(N(should_player_be_sliding));
    return ApiStatus_DONE2;
}

EvtScript N(EVS_BindSlideTriggers) = {
    Call(N(SetupSlidingCheck))
    BindTrigger(Ref(N(EVS_UseSlide)), TRIGGER_WALL_PRESS_A, COLLIDER_st1, 1, 0)
    BindTrigger(Ref(N(EVS_UpdateSliding)), TRIGGER_FLOOR_TOUCH, COLLIDER_suberi0, 1, 0)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_SURFACE, COLLIDER_suberi0, SURFACE_TYPE_SLIDE)
    Return
    End
};
