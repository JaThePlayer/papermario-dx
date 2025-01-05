#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/Swooper.h"
#include "sprite/npc/BuzzyBeetle.h"

#define NAMESPACE A(swooper)

extern s32 N(CeilingAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
};

enum N(ActorVars) {
    AVAR_IsFlying           = 0, // false when attached to ceiling
    AVAR_IN_SnapToIndexPos  = 1, // -1 = no, 0 = yes
};

enum N(ActorParams) {
    DMG_SWOOP           = 3,
    DMG_CEILING_DROP    = 6, // from buzzy beetle
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              80,
    STATUS_KEY_POISON,            100,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,             100,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,             80,
    STATUS_KEY_PARALYZE,           90,
    STATUS_KEY_SHRINK,             90,
    STATUS_KEY_STOP,               90,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          1,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       1,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

ActorPartBlueprint N(ActorParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 29 },
        .opacity = 255,
        .idleAnimations = N(CeilingAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 1, 15 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = ACTOR_FLAG_UPSIDE_DOWN,
    .type = ACTOR_TYPE_SWOOPER,
    .level = ACTOR_LEVEL_SWOOPER,
    .maxHP = 5,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 50,
    .airLiftChance = 100,
    .hurricaneChance = 95,
    .spookChance = 90,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 80,
    .coinReward = 1,
    .size = { 31, 24 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, -20 },
    .statusTextOffset = { 10, -20 },
    .spPool = CURRENT_SP_POOL,
};

s32 N(CeilingAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_Swooper_Anim0C,
    STATUS_KEY_STONE,     ANIM_Swooper_Anim0B,
    STATUS_KEY_SLEEP,     ANIM_Swooper_Anim0F,
    STATUS_KEY_POISON,    ANIM_Swooper_Anim0C,
    STATUS_KEY_STOP,      ANIM_Swooper_Anim0B,
    STATUS_KEY_STATIC,    ANIM_Swooper_Anim0C,
    STATUS_KEY_PARALYZE,  ANIM_Swooper_Anim0B,
    STATUS_KEY_DIZZY,     ANIM_Swooper_Anim10,
    STATUS_KEY_FEAR,      ANIM_Swooper_Anim10,
    STATUS_END,
};

s32 N(FlyingAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_Swooper_Anim02,
    STATUS_KEY_STONE,     ANIM_Swooper_Anim01,
    STATUS_KEY_SLEEP,     ANIM_Swooper_Anim13,
    STATUS_KEY_POISON,    ANIM_Swooper_Anim02,
    STATUS_KEY_STOP,      ANIM_Swooper_Anim01,
    STATUS_KEY_STATIC,    ANIM_Swooper_Anim02,
    STATUS_KEY_PARALYZE,  ANIM_Swooper_Anim01,
    STATUS_KEY_DIZZY,     ANIM_Swooper_Anim14,
    STATUS_KEY_FEAR,      ANIM_Swooper_Anim14,
    STATUS_END,
};

#include "common/battle/SetAbsoluteStatusOffsets.inc.c"

EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))

    // read avar0 from formation
    Call(GetActorVar, ACTOR_SELF, AVAR_IsFlying, LVar0)
    Switch(LVar0)
        CaseEq(1)
            Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, 0, -10)
            Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 22)
            Call(N(SetAbsoluteStatusOffsets), -10, 20, 10, 20)
            Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(FlyingAnims)))
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_UPSIDE_DOWN, FALSE)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, TRUE)
            Call(HPBarToHome, ACTOR_SELF)
    EndSwitch
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Return
    End
};

EvtScript N(EVS_ReturnHome) = {
    Call(GetActorVar, ACTOR_SELF, AVAR_IsFlying, LVar0)
    Switch(LVar0)
        CaseEq(0)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetGoalToHome, ACTOR_SELF)
            Call(AddGoalPos, ACTOR_SELF, 0, -24, 0)
            Call(FlyToGoal, ACTOR_SELF, 0, 1, EASING_SIN_OUT)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_UPSIDE_DOWN, TRUE)
            Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(CeilingAnims)))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim0C)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar1, 24)
            Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(1)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetGoalToHome, ACTOR_SELF)
            Call(FlyToGoal, ACTOR_SELF, 0, 1, EASING_SIN_OUT)
    EndSwitch
    Return
    End
};

EvtScript N(EVS_Flying_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim17)
            SetConst(LVar2, ANIM_Swooper_Anim18)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim17)
            SetConst(LVar2, ANIM_Swooper_Anim18)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim18)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            Call(func_80269470)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_ShockHit)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            ExecWait(N(EVS_ReturnHome))
        CaseEq(EVENT_SHOCK_DEATH)
            Call(func_80269470)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_ShockHit)
            Thread
                Wait(30)
                Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            EndThread
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim02)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim02)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, FALSE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim02)
            SetConst(LVar2, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim02)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_BlowAway)
            Return
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_FallFromCeiling) = {
    Call(HideHealthBar, ACTOR_SELF)
    Call(GetLastDamage, ACTOR_SELF, LVar0)
    IfLe(LVar0, 0)
        Return
    EndIf
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_UPSIDE_DOWN, FALSE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Sub(LVar1, 24)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetActorVar, ACTOR_SELF, AVAR_IN_SnapToIndexPos, LVarA)
    IfEq(LVarA, -1)
        Call(SetHomePos, ACTOR_SELF, LVar0, 45, LVar2)
        Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetGoalToHome, ACTOR_SELF)
        Call(JumpToGoal, ACTOR_SELF, 15, FALSE, FALSE, FALSE)
        Call(JumpToGoal, ACTOR_SELF, 10, FALSE, FALSE, FALSE)
    Else
        Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetGoalPos, ACTOR_SELF, LVar0, 45, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 15, FALSE, FALSE, FALSE)
        Call(GetIndexFromPos, ACTOR_SELF, LVarA)
        Mod(LVarA, 4)
        Add(LVarA, 4)
        Call(SetGoalToIndex, ACTOR_SELF, LVarA)
        Call(GetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
        Call(SetHomePos, ACTOR_SELF, LVarA, LVarB, LVarC)
        Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetGoalToHome, ACTOR_SELF)
        Call(JumpToGoal, ACTOR_SELF, 10, FALSE, FALSE, FALSE)
    EndIf
    Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, 0, -10)
    Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 22)
    Call(N(SetAbsoluteStatusOffsets), -10, 20, 10, 20)
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(FlyingAnims)))
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, TRUE)
    Call(HPBarToHome, ACTOR_SELF)
    Call(SetActorVar, ACTOR_SELF, AVAR_IsFlying, 1)
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(GetActorVar, ACTOR_SELF, AVAR_IsFlying, LVar0)
    IfEq(LVar0, 1)
        ExecWait(N(EVS_Flying_HandleEvent))
        Return
    EndIf
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim15)
                ExecWait(EVS_Enemy_Hit)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim0E)
                ExecWait(EVS_Enemy_Hit)
            EndIf
        CaseEq(EVENT_HIT)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim15)
                ExecWait(EVS_Enemy_Hit)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim0E)
                ExecWait(EVS_Enemy_Hit)
                ExecWait(N(EVS_FallFromCeiling))
            EndIf
        CaseEq(EVENT_BURN_HIT)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim17)
                SetConst(LVar2, ANIM_Swooper_Anim18)
                ExecWait(EVS_Enemy_BurnHit)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim11)
                SetConst(LVar2, ANIM_Swooper_Anim12)
                ExecWait(EVS_Enemy_BurnHit)
                ExecWait(N(EVS_FallFromCeiling))
            EndIf
        CaseEq(EVENT_BURN_DEATH)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim17)
                SetConst(LVar2, ANIM_Swooper_Anim18)
                ExecWait(EVS_Enemy_BurnHit)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim18)
                ExecWait(EVS_Enemy_Death)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim11)
                SetConst(LVar2, ANIM_Swooper_Anim12)
                ExecWait(EVS_Enemy_BurnHit)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim12)
                ExecWait(EVS_Enemy_Death)
            EndIf
            Return
        CaseEq(EVENT_SHOCK_HIT)
            Call(func_80269470)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_ShockHit_Impl)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            ExecWait(N(EVS_ReturnHome))
        CaseEq(EVENT_SHOCK_DEATH)
            Call(func_80269470)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_ShockHit_Impl)
            Thread
                Wait(30)
                Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            EndThread
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim0C)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim15)
                ExecWait(EVS_Enemy_Hit)
                Wait(10)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim15)
                ExecWait(EVS_Enemy_Death)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim0E)
                ExecWait(EVS_Enemy_Hit)
                Wait(10)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Swooper_Anim0E)
                ExecWait(EVS_Enemy_Death)
            EndIf
            Return
        CaseEq(EVENT_BEGIN_FIRST_STRIKE)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_UPSIDE_DOWN, FALSE)
            Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(FlyingAnims)))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
            Call(SetActorPos, ACTOR_SELF, 20, 0, 0)
            Call(HPBarToCurrent, ACTOR_SELF)
        CaseEq(EVENT_END_FIRST_STRIKE)
            ExecWait(N(EVS_ReturnHome))
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, TRUE)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_UPSIDE_DOWN, FALSE)
            Call(SetActorDispOffset, ACTOR_SELF, 0, -24, 0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Swooper_Anim02)
            SetConst(LVar2, ANIM_Swooper_Anim15)
            ExecWait(EVS_Enemy_ScareAway)
            Return
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

API_CALLABLE(N(FindBeetle)) {
    Bytecode* args = script->ptrReadPos;
    BattleStatus* battleStatus = &gBattleStatus;
    s32 retActorId = *args++;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(battleStatus->enemyActors); i++) {
        Actor* targetActor = battleStatus->enemyActors[i];
        if (targetActor == NULL)
            continue;
        if (targetActor->actorType != ACTOR_TYPE_BUZZY_BEETLE)
            continue;

        // can't lift up buzzies that aren't toppled
        if (targetActor->state.varTable[8/*AVAR_ToppleState*/] != 2/*AVAL_State_Toppled*/)
            continue;

        evt_set_variable(script, retActorId, ACTOR_ENEMY0 + i);
        return ApiStatus_DONE2;
    }

    evt_set_variable(script, retActorId, FALSE);
    return ApiStatus_DONE2;
}

extern s32 A(buzzy_beetle_UprightDefense)[];
extern s32 A(buzzy_beetle_GroundAnims)[];

#ifdef SWOOPER_NO_BUZZY_BEETLES
s32 A(buzzy_beetle_UprightDefense)[] = {};
s32 A(buzzy_beetle_GroundAnims)[] = {};
#endif

EvtScript N(BeetleDrop_BeetlePart) = {
    Call(SetOwnerID, LVarE)

    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_FALL_QUICK, 0)
    Call(SetActorYaw, ACTOR_SELF, 180)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BuzzyBeetle_Anim08)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Sub(LVar1, 24)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar3, LVar4, LVar5)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar1, LVar4)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
    Call(ResetAllActorSounds, ACTOR_SELF)

    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, SUPPRESS_EVENT_ALL, 0, DMG_CEILING_DROP, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 30)
            Set(LVar1, 0)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
            Thread
                Wait(5)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BuzzyBeetle_Anim00)
            EndThread
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
            Wait(8)
            Thread
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_BuzzyBeetle_Anim04)
                Call(SetActorVar, ACTOR_SELF, 8, 1) // Call(SetActorVar, ACTOR_SELF, AVAR_ToppleState, AVAL_State_Ground)
                Call(SetActorVar, ACTOR_SELF, 9, 0) // Call(SetActorVar, ACTOR_SELF, AVAR_ToppleTurns, 0)
                ExecWait(EVS_Enemy_ReturnHome)
                Call(SetActorYaw, ACTOR_SELF, 0)
                Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 16)
                Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, -1, -9)
                Call(SetDefenseTable, ACTOR_SELF, PRT_MAIN, Ref(A(buzzy_beetle_UprightDefense)))
                Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(A(buzzy_beetle_GroundAnims)))
                Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
                Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLIPPED, FALSE)

                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BuzzyBeetle_Anim01)
                Call(UseIdleAnimation, ACTOR_SELF, FALSE)
                Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
            EndThread
        EndCaseGroup
    EndSwitch

    Return
    End
};

EvtScript N(BeetleDrop_SyncBeetlePos) = {
    Loop(0)
        Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
        Sub(LVar5, 5)
        Call(SetActorPos, LVarE, LVar4, LVar5, LVar6)
        Wait(1)
    EndLoop
    Return
    End
};

EvtScript N(EVS_BeetleDrop) = {
    // LVarE - beetle
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, LVarE)

    // Move to beetle
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(FlyToGoal, ACTOR_SELF, 0, -4, EASING_LINEAR)

    // Pick up beetle
    Call(UseIdleAnimation, LVarE, FALSE)
    Call(EnableIdleScript, LVarE, IDLE_SCRIPT_DISABLE)
    Wait(24)
    Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim13)

    // Fly with beetle
    ExecGetTID(N(BeetleDrop_SyncBeetlePos), LVarD)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 0, 50, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(FlyToGoal, ACTOR_SELF, 0, -4, EASING_LINEAR)
    Wait(16)

    // drop beetle
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
    Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
    KillThread(LVarD)
    ExecGetTID(N(BeetleDrop_BeetlePart), LVarF)
    Label(0x1e)
    IsThreadRunning(LVarF, LVarD)
    IfEq(LVarD, TRUE)
        Wait(1)
        Goto(0x1e)
    EndIf

    Wait(10)
    Call(YieldTurn)
    ExecWait(N(EVS_ReturnHome))
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)

    Return
    End
};

EvtScript N(EVS_Flying_TakeTurn) = {
    Call(N(FindBeetle), LVarE)
    IfNe(LVarE, FALSE)
        ExecWait(N(EVS_BeetleDrop))
        Return
    EndIf

    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(AddGoalPos, ACTOR_SELF, 50, 10, 0)
        Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
        Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Else
        Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(AddGoalPos, ACTOR_SELF, 50, 10, 0)
        Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
        Call(FlyToGoal, ACTOR_SELF, 0, -4, EASING_LINEAR)
    EndIf
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Wait(10)
            Call(SetActorSpeed, ACTOR_SELF, Float(5.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, FALSE, FALSE, FALSE)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 50)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
            Call(FlyToGoal, ACTOR_SELF, 0, -10, EASING_LINEAR)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(10.0))
            Wait(15)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(YieldTurn)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            ExecWait(N(EVS_ReturnHome))
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
        CaseEq(HIT_RESULT_HIT_STATIC)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfFlag(LVar0, STATUS_FLAG_STATIC)
                Set(LFlag0, FALSE)
            Else
                Set(LFlag0, TRUE)
            EndIf
    EndSwitch
    Wait(10)
    Call(SetActorSpeed, ACTOR_SELF, Float(5.0))
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 10, FALSE, FALSE, FALSE)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
    Call(FlyToGoal, ACTOR_SELF, 0, -10, EASING_LINEAR)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, DMG_SWOOP, BS_FLAGS1_TRIGGER_EVENTS)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 50, 10, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
    Call(FlyToGoal, ACTOR_SELF, 0, -10, EASING_LINEAR)
    Wait(10)
    Call(YieldTurn)
    ExecWait(N(EVS_ReturnHome))
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_TakeTurn) = {
    Call(GetActorVar, ACTOR_SELF, AVAR_IsFlying, LVar0)
    IfEq(LVar0, 1)
        ExecWait(N(EVS_Flying_TakeTurn))
        Return
    EndIf

    Call(N(FindBeetle), LVarE)
    IfNe(LVarE, FALSE)
        ExecWait(N(EVS_BeetleDrop))
        Return
    EndIf

    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_UPSIDE_DOWN, FALSE)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Sub(LVar1, 24)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Wait(10)
            Call(SetActorSpeed, ACTOR_SELF, Float(5.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(AddGoalPos, ACTOR_SELF, -50, 0, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
            Call(FlyToGoal, ACTOR_SELF, 0, -40, EASING_LINEAR)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(10.0))
            Wait(15)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(YieldTurn)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            ExecWait(N(EVS_ReturnHome))
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
        CaseEq(HIT_RESULT_HIT_STATIC)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfFlag(LVar0, STATUS_FLAG_STATIC)
                Set(LFlag0, FALSE)
            Else
                Set(LFlag0, TRUE)
            EndIf
    EndSwitch
    Wait(10)
    Call(SetActorSpeed, ACTOR_SELF, Float(5.0))
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim06)
    Call(FlyToGoal, ACTOR_SELF, 0, -40, EASING_LINEAR)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, DMG_SWOOP, BS_FLAGS1_TRIGGER_EVENTS)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 50, 10, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Swooper_Anim02)
    Call(FlyToGoal, ACTOR_SELF, 0, -10, EASING_LINEAR)
    Wait(10)
    Call(YieldTurn)
    ExecWait(N(EVS_ReturnHome))
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};
