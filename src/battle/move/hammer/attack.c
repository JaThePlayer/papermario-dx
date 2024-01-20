#include "common.h"
#include "script_api/battle.h"
#include "sprite/npc/WorldBombette.h"
#include "battle/action_cmd/hammer.h"
#include "sprite/player.h"

#define NAMESPACE battle_move_hammer_attack

#include "battle/common/move/HammerSupport.inc.c"

extern EvtScript N(EVS_UseMove_Impl);

EvtScript N(EVS_UseMove) = {
    Call(ShowActionHud, TRUE)
    Call(GetMenuSelection, LVar0, LVar1, LVar2)
    Switch(LVar1)
        CaseEq(0)
            Set(LVarD, 45)
            Set(LVarE, 1)
            Set(LVarF, 2)
            ExecWait(N(EVS_UseMove_Impl))
        CaseEq(1)
            Set(LVarD, 45)
            Set(LVarE, 2)
            Set(LVarF, 4)
            ExecWait(N(EVS_UseMove_Impl))
        CaseEq(2)
            Set(LVarD, 45)
            Set(LVarE, 3)
            Set(LVarF, 6)
            ExecWait(N(EVS_UseMove_Impl))
    EndSwitch
    Return
    End
};

EvtScript N(EVS_UseMove_Impl) = {
    Call(GetMenuSelection, LVar0, LVar1, LVar2)
    Switch(LVar1)
        CaseEq(0)
            ExecWait(N(EVS_UseBasicHammer))
        CaseEq(1)
            ExecWait(N(EVS_UseSuperHammer))
        CaseEq(2)
            ExecWait(N(EVS_UseUltraHammer))
    EndSwitch
    Call(GetActionSuccessCopy, LVar0)
    Switch(LVar0)
        CaseGt(0)
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_MIN)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(1.3))
                    EndThread
                CaseEq(1)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_LIGHT)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(1.6))
                    EndThread
                CaseEq(2)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_HEAVY)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(1.9))
                    EndThread
            EndSwitch
        CaseDefault
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_MIN)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.3))
                    EndThread
                CaseEq(1)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_LIGHT)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.6))
                    EndThread
                CaseEq(2)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_HEAVY)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.9))
                    EndThread
            EndSwitch
    EndSwitch
    Call(UseBattleCamPreset, BTL_CAM_PLAYER_HAMMER_STRIKE)
    Wait(1)
    Call(PlayerTestEnemy, LVar0, DAMAGE_TYPE_SMASH, 25, 0, 0, 16)
    IfEq(LVar0, HIT_RESULT_MISS)
        ExecWait(N(EVS_Hammer_ReturnHome_C))
        Return
    EndIf
    Call(GetPlayerActionSuccess, LVar0)
    Switch(LVar0)
        CaseGt(FALSE)
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_NORMAL)
                CaseEq(1)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
                CaseEq(2)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
            EndSwitch
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_SMASH, SUPPRESS_EVENTS_HAMMER, 0, LVarF, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS | BS_FLAGS1_NICE_HIT)
        CaseDefault
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_NORMAL)
                CaseEq(1)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
                CaseEq(2)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
            EndSwitch
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_SMASH, SUPPRESS_EVENTS_HAMMER, 0, LVarE, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS)
    EndSwitch
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_NICE)
        CaseOrEq(HIT_RESULT_NICE_NO_DAMAGE)
            ExecWait(N(EVS_Hammer_ReturnHome_A))
        EndCaseGroup
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            ExecWait(N(EVS_Hammer_ReturnHome_C))
        EndCaseGroup
    EndSwitch
    Return
    End
};

EvtScript N(EVS_FirstStrike) = {
    EVT_CALL(GetMenuSelection, LVar0, LVar1, LVar2)
    EVT_SWITCH(LVar1)
        EVT_CASE_EQ(0)
            EVT_SET(LVar9, 2)
            EVT_SET_CONST(LVarA, ANIM_MarioB1_Smash1_Hold1)
            EVT_SET_CONST(LVarB, ANIM_MarioB1_Smash1_PreSwing)
            EVT_SET_CONST(LVarC, ANIM_MarioB1_Smash1_Swing)
        EVT_CASE_EQ(1)
            EVT_SET(LVar9, 4)
            EVT_SET_CONST(LVarA, ANIM_MarioB1_Smash2_Hold1)
            EVT_SET_CONST(LVarB, ANIM_MarioB1_Smash2_Hold2)
            EVT_SET_CONST(LVarC, ANIM_MarioB1_Smash2_Swing)
        EVT_CASE_EQ(2)
            EVT_SET(LVar9, 6)
            EVT_SET_CONST(LVarA, ANIM_MarioB1_Smash3_Hold1)
            EVT_SET_CONST(LVarB, ANIM_MarioB1_Smash3_Hold2)
            EVT_SET_CONST(LVarC, ANIM_MarioB1_Smash3_Swing)
    EVT_END_SWITCH
    EVT_CALL(SetGoalToTarget, ACTOR_PLAYER)
    EVT_CALL(AddGoalPos, ACTOR_PLAYER, 0, 0, 0)
    EVT_CALL(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    EVT_SUB(LVar0, 32)
    EVT_SET(LVar1, 0)
    EVT_CALL(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    EVT_CALL(UseBattleCamPresetImmediately, BTL_CAM_PLAYER_AIM_HAMMER)
    EVT_CALL(SetAnimation, ACTOR_PLAYER, 0, LVarA)
    EVT_WAIT(8)
    EVT_CALL(SetAnimation, ACTOR_PLAYER, 0, LVarB)
    EVT_WAIT(3)
    EVT_CALL(UseBattleCamPreset, BTL_CAM_PLAYER_HAMMER_STRIKE)
    EVT_CALL(SetAnimation, ACTOR_PLAYER, 0, LVarC)
    EVT_CALL(GetMenuSelection, LVar0, LVar1, LVar2)
    EVT_SWITCH(LVar1)
        EVT_CASE_EQ(0)
            EVT_CALL(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HAMMER_SWING_1)
            EVT_CALL(StartRumble, BTL_RUMBLE_PLAYER_MIN)
            EVT_THREAD
                EVT_CALL(ShakeCam, CAM_BATTLE, 0, 10, EVT_FLOAT(1.3))
            EVT_END_THREAD
        EVT_CASE_EQ(1)
            EVT_CALL(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HAMMER_SWING_2)
            EVT_CALL(StartRumble, BTL_RUMBLE_PLAYER_LIGHT)
            EVT_THREAD
                EVT_CALL(ShakeCam, CAM_BATTLE, 0, 10, EVT_FLOAT(1.6))
            EVT_END_THREAD
        EVT_CASE_EQ(2)
            EVT_CALL(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HAMMER_SWING_3)
            EVT_CALL(StartRumble, BTL_RUMBLE_PLAYER_HEAVY)
            EVT_THREAD
                EVT_CALL(ShakeCam, CAM_BATTLE, 0, 10, EVT_FLOAT(1.9))
            EVT_END_THREAD
    EVT_END_SWITCH
    EVT_CALL(UseBattleCamPreset, BTL_CAM_PLAYER_HAMMER_STRIKE)
    EVT_WAIT(1)
    EVT_CALL(PlayerTestEnemy, LVar0, DAMAGE_TYPE_SMASH, 25, 0, 0, 16)
    EVT_IF_EQ(LVar0, HIT_RESULT_MISS)
        EVT_EXEC_WAIT(N(EVS_Hammer_ReturnHome_C))
        EVT_RETURN
    EVT_END_IF
    EVT_CALL(GetMenuSelection, LVar0, LVar1, LVar2)
    EVT_SWITCH(LVar1)
        EVT_CASE_EQ(0)
            EVT_CALL(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_NORMAL)
        EVT_CASE_EQ(1)
            EVT_CALL(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
        EVT_CASE_EQ(2)
            EVT_CALL(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
    EVT_END_SWITCH
    EVT_CALL(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_SMASH, SUPPRESS_EVENTS_HAMMER, 0, LVar9, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS)
    EVT_EXEC_WAIT(N(EVS_Hammer_ReturnHome_A))
    EVT_RETURN
    EVT_END
};

extern EvtScript N(EVS_UseBerserker_Impl);

EvtScript N(EVS_UseBerserker) = {
    Call(ShowActionHud, TRUE)
    Call(GetMenuSelection, LVar0, LVar1, LVar2)
    Switch(LVar1)
        CaseEq(0)
            Set(LVarD, 75)
            Set(LVarE, 1)
            Set(LVarF, 2)
            ExecWait(N(EVS_UseBerserker_Impl))
        CaseEq(1)
            Set(LVarD, 66)
            Set(LVarE, 2)
            Set(LVarF, 4)
            ExecWait(N(EVS_UseBerserker_Impl))
        CaseEq(2)
            Set(LVarD, 57)
            Set(LVarE, 3)
            Set(LVarF, 6)
            ExecWait(N(EVS_UseBerserker_Impl))
    EndSwitch
    Return
    End
};

EvtScript N(EVS_UseBerserker_Impl) = {
    Call(GetMenuSelection, LVar0, LVar1, LVar2)
    Switch(LVar1)
        CaseEq(0)
            ExecWait(N(EVS_UseBasicHammer))
        CaseEq(1)
            ExecWait(N(EVS_UseSuperHammer))
        CaseEq(2)
            ExecWait(N(EVS_UseUltraHammer))
    EndSwitch
    Call(PlayerTestEnemy, LVar0, DAMAGE_TYPE_SMASH, 25, 0, LVar9, 16)
    IfEq(LVar0, HIT_RESULT_MISS)
        Call(GetMenuSelection, LVar0, LVar1, LVar2)
        Switch(LVar1)
            CaseEq(0)
                Call(StartRumble, BTL_RUMBLE_PLAYER_LIGHT)
                Thread
                    Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.3))
                EndThread
            CaseEq(1)
                Call(StartRumble, BTL_RUMBLE_PLAYER_HEAVY)
                Thread
                    Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.6))
                EndThread
            CaseEq(2)
                Call(StartRumble, BTL_RUMBLE_PLAYER_EXTREME)
                Thread
                    Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.9))
                EndThread
        EndSwitch
        Call(UseBattleCamPreset, BTL_CAM_PLAYER_HAMMER_STRIKE)
        ExecWait(N(EVS_Hammer_ReturnHome_C))
        Return
    EndIf
    Call(GetActionSuccessCopy, LVar0)
    Switch(LVar0)
        CaseGt(0)
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_LIGHT)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(1.3))
                    EndThread
                CaseEq(1)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_HEAVY)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(1.6))
                    EndThread
                CaseEq(2)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_EXTREME)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(1.9))
                    EndThread
            EndSwitch
        CaseDefault
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_LIGHT)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.3))
                    EndThread
                CaseEq(1)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_HEAVY)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.6))
                    EndThread
                CaseEq(2)
                    Call(StartRumble, BTL_RUMBLE_PLAYER_EXTREME)
                    Thread
                        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(1.9))
                    EndThread
            EndSwitch
    EndSwitch
    Call(UseBattleCamPreset, BTL_CAM_PLAYER_HAMMER_STRIKE)
    Wait(1)
    Call(PlayerTestEnemy, LVar0, DAMAGE_TYPE_SMASH, 25, 0, 0, 16)
    IfEq(LVar0, HIT_RESULT_MISS)
        ExecWait(N(EVS_Hammer_ReturnHome_C))
        Return
    EndIf
    Call(GetPlayerActionSuccess, LVar0)
    Switch(LVar0)
        CaseGt(FALSE)
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_NORMAL)
                CaseEq(1)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
                CaseEq(2)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
            EndSwitch
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_SMASH, SUPPRESS_EVENTS_HAMMER, 0, LVarF, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS | BS_FLAGS1_NICE_HIT)
        CaseDefault
            Call(GetMenuSelection, LVar0, LVar1, LVar2)
            Switch(LVar1)
                CaseEq(0)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_NORMAL)
                CaseEq(1)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
                CaseEq(2)
                    Call(PlaySoundAtActor, ACTOR_PLAYER, SOUND_HIT_SILENT)
            EndSwitch
            Call(PlayerDamageEnemy, LVar0, DAMAGE_TYPE_SMASH, SUPPRESS_EVENTS_HAMMER, 0, LVarE, BS_FLAGS1_INCLUDE_POWER_UPS | BS_FLAGS1_TRIGGER_EVENTS)
    EndSwitch
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_NICE)
        CaseOrEq(HIT_RESULT_NICE_NO_DAMAGE)
            ExecWait(N(EVS_Hammer_ReturnHome_A))
        EndCaseGroup
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            ExecWait(N(EVS_Hammer_ReturnHome_C))
        EndCaseGroup
    EndSwitch
    Return
    End
};
