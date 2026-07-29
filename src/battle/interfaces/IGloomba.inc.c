#include "battle/interfaces/IGloomba.h"

#include "misc_patches/actor_interfaces.h"
#include "misc_patches/custom_status.h"
#include "misc_patches/anim_bank.h"

extern EvtScript N(EVS_HeadbonkPlayer);
extern EvtScript N(EVS_HeadbonkGloomba);

#define PRT_MAIN 1
#define LVarF_Enemy LVarF
#define LVarD_NextEnemy LVarD
#define LVarC_BestScore LVarC
#define LVar6_NextScore LVar6

EvtScript N(EVS_TakeTurn) = {
    STANDARD_ITEM_USE_AI()

    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)

    #define LABEL_END_SCORE_CALC 2
    FIND_BEST_ENEMY(LVarF_Enemy, LVarC_BestScore, LVarD_NextEnemy, LVar6_NextScore, 1, LABEL_END_SCORE_CALC, TARGET_FLAG_GROUND | TARGET_FLAG_PRIMARY_ONLY,
        // Enemies need to be to the left
        Call(GetActorPos, LVarD_NextEnemy, LVar3, LVar4, LVar5)
        IfLe(LVar0, LVar3)
            Goto(LABEL_END_SCORE_CALC)
        EndIf

        // Enemies need to implement IGroundedGloomba
        DoesActorImplement(LVarD_NextEnemy, IGroundedGloomba, LVarE)
        IfFalse(LVarE)
            Goto(LABEL_END_SCORE_CALC)
        EndIf

        // score = hp (deprioritize almost dead enemies, they won't benefit presumably)
        Call(GetActorHP, LVarD_NextEnemy, LVar6_NextScore)
        // ... but deprioritize inflicting healing on undamaged enemies
        Call(GetEnemyMaxHP, LVarD_NextEnemy, LVar7)
        IfEq(LVar6_NextScore, LVar7)
            Sub(LVar6_NextScore, 2)
        EndIf

        // deprioritize already poisoned enemies
        Call(GetCustomStatusTurns, LVarD_NextEnemy, POISON_STATUS, LVar7)
        IfGt(LVar7, 0)
            Div(LVar6_NextScore, 2)
        EndIf
    )

    IfNe(LVarF_Enemy, 0)
        ExecWait(N(EVS_HeadbonkGloomba))
        Return
    EndIf

    Set(LVarF_Enemy, 0)
    ExecWait(N(EVS_HeadbonkPlayer))

    Return
    End
};
#undef LVarD_NextEnemy
#undef LVar6_NextScore

// uses LVarF_Enemy
EvtScript N(EVS_HeadbonkPlayer) = {
    #define LVarA_Bank LVarA
    GetRefFromInterface(ACTOR_SELF, IGroundedGloomba, anims, LVarA_Bank)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)

    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 10)
            Set(LVar1, 10)
            Add(LVar2, 3)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
            Thread
                Call(GetActorPos, ACTOR_SELF, LVar1, LVar2, LVar0)
                Set(LVar0, 0)
                Loop(16)
                    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                    Call(CalcActorRotation, LVar0, LVar1, LVar2, LVar4, LVar5)
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Set(LVar1, LVar4)
                    Set(LVar2, LVar5)
                    Set(LVar3, LVar6)
                    Wait(1)
                EndLoop
            EndThread
            Thread
                Wait(6)
                SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            EndThread
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, sleep)
            Call(SetActorScale, ACTOR_SELF, Float(1.1), Float(0.8), Float(1.0))
            Call(SetActorDispOffset, ACTOR_SELF, 0, 5, 0)
            Wait(1)
            Call(SetActorScale, ACTOR_SELF, Float(1.3), Float(0.5), Float(1.0))
            Call(SetActorDispOffset, ACTOR_SELF, 0, -2, 0)
            Wait(1)
            Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
            Call(SetActorDispOffset, ACTOR_SELF, 0, 7, 0)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, hurt)
            Wait(HIT_RESULT_LUCKY)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(5)
            Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 20)
            Set(LVar1, 0)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(2.0))
            Thread
                Wait(4)
                Set(LVar0, 180)
                Loop(4)
                    Sub(LVar0, 45)
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Wait(1)
                EndLoop
                SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            EndThread
            Call(JumpToGoal, ACTOR_SELF, 15, false, true, false)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, dizzy)
            Wait(5)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, false)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            Call(SetActorYaw, ACTOR_SELF, 0)
            Wait(5)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, idle)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.6))
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
        CaseDefault
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
            Thread
                Call(GetActorPos, ACTOR_SELF, LVar1, LVar2, LVar0)
                Set(LVar0, 0)
                Loop(16)
                    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                    Call(CalcActorRotation, LVar0, LVar1, LVar2, LVar4, LVar5)
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Set(LVar1, LVar4)
                    Set(LVar2, LVar5)
                    Set(LVar3, LVar6)
                    Wait(1)
                EndLoop
            EndThread
            Thread
                Wait(6)
                SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            EndThread
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, tense)
            Call(SetActorScale, ACTOR_SELF, Float(1.1), Float(0.8), Float(1.0))
            Wait(1)
            Call(SetActorScale, ACTOR_SELF, Float(1.3), Float(0.5), Float(1.0))
            Wait(1)
    EndSwitch
    Call(SetNextAttackCustomStatus, POISON_STATUS, 3, 1, 100)
    IfNe(LVarF_Enemy, 0)
        GetFromInterface(ACTOR_SELF, IGroundedGloomba, buffedHeadbonkDamage, LVarE)
    Else
        GetFromInterface(ACTOR_SELF, IGroundedGloomba, headbonkDamage, LVarE)
    EndIf
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, LVarE, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetActorScale, ACTOR_SELF, Float(1.1), Float(0.8), Float(1.0))
            Wait(1)
            Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
            Wait(1)
            Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
            Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, idle)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 40)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 6, false, true, false)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, idle)
            Wait(3)
            Call(YieldTurn)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, run)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, false)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
    #undef LVarA_Bank
};

extern IGroundedGloomba IGroundedGloombaImpl;

EvtScript N(EVS_HeadbonkGloomba) = {
    #define LVarA_Bank LVarA
    GetRefFromInterface(ACTOR_SELF, IGroundedGloomba, anims, LVarA_Bank)

    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, LVarF_Enemy)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, idle)
    Call(SetActorDispOffset, ACTOR_SELF, 0, -1, 0)
    Wait(1)
    Call(SetActorDispOffset, ACTOR_SELF, 0, -2, 0)
    Wait(5)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 10)
            Set(LVar1, 10)
            Add(LVar2, 3)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
            Thread
                Call(GetActorPos, ACTOR_SELF, LVar1, LVar2, LVar0)
                Set(LVar0, 0)
                Loop(16)
                    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                    Call(CalcActorRotation, LVar0, LVar1, LVar2, LVar4, LVar5)
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Set(LVar1, LVar4)
                    Set(LVar2, LVar5)
                    Set(LVar3, LVar6)
                    Wait(1)
                EndLoop
            EndThread
            Thread
                Wait(6)
                SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            EndThread
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, sleep)
            Call(SetActorScale, ACTOR_SELF, Float(1.1), Float(0.8), Float(1.0))
            Call(SetActorDispOffset, ACTOR_SELF, 0, 5, 0)
            Wait(1)
            Call(SetActorScale, ACTOR_SELF, Float(1.3), Float(0.5), Float(1.0))
            Call(SetActorDispOffset, ACTOR_SELF, 0, -2, 0)
            Wait(1)
            Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
            Call(SetActorDispOffset, ACTOR_SELF, 0, 7, 0)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, hurt)
            Wait(HIT_RESULT_LUCKY)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(5)
            Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 20)
            Set(LVar1, 0)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(2.0))
            Thread
                Wait(4)
                Set(LVar0, 180)
                Loop(4)
                    Sub(LVar0, 45)
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Wait(1)
                EndLoop
                SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            EndThread
            Call(JumpToGoal, ACTOR_SELF, 15, false, true, false)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, dizzy)
            Wait(5)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, false)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            Call(SetActorYaw, ACTOR_SELF, 0)
            Wait(5)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, idle)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.6))
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
        CaseDefault
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
            Thread
                Call(GetActorPos, ACTOR_SELF, LVar1, LVar2, LVar0)
                Set(LVar0, 0)
                Loop(16)
                    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                    Call(CalcActorRotation, LVar0, LVar1, LVar2, LVar4, LVar5)
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Set(LVar1, LVar4)
                    Set(LVar2, LVar5)
                    Set(LVar3, LVar6)
                    Wait(1)
                EndLoop
            EndThread
            Thread
                Wait(6)
                SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, midair)
            EndThread
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            SetTypedBankAnimation(ACTOR_SELF, PRT_MAIN, LVarA_Bank, GroundedGloombaAnims, tense)
            Call(SetActorScale, ACTOR_SELF, Float(1.1), Float(0.8), Float(1.0))
            Wait(1)
            Call(SetActorScale, ACTOR_SELF, Float(1.3), Float(0.5), Float(1.0))
            Wait(1)
    EndSwitch
    Call(SetNextAttackCustomStatus, POISON_STATUS, 3, 1, 100)
    GetFromInterface(ACTOR_SELF, IGroundedGloomba, friendlyHeadbonkDamage, LVarE)

    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, LVarE, BS_FLAGS1_TRIGGER_EVENTS)
    // Now attack Mario
    ExecWait(N(EVS_HeadbonkPlayer))
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End

    #undef LVarA_Bank
};

#undef LVarF_Enemy
#undef PRT_MAIN
