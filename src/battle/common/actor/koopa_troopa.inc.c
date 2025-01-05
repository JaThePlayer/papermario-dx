#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/KoopaTroopa.h"
#include "sprite/npc/Bobomb.h"

#define NAMESPACE A(koopa_troopa)

extern s32 N(DefaultAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorPartIDs) {
    PRT_MAIN            = 1,
};

enum N(ActorVars) {
    AVAR_InShellAtStart = 0, // Go into shell at the start of battle, set by formations
    AVAR_IsFlipped      = 8,
    AVAR_FlippedTurns   = 9,
    AVAR_IsInShell      = 10,
};

enum N(ActorParams) {
    DMG_SHELL_TOSS      = 1,
    DMG_CHARGED_SHELL_TOSS = 2,
    DMG_CHARGED_SHELL_TOSS_PARTNER = 1,
};

s32 N(NormalDefense)[] = {
    ELEMENT_NORMAL,   1,
    ELEMENT_SMASH,    1,
    ELEMENT_JUMP,     1,
    ELEMENT_WATER,    1,
    ELEMENT_BLAST,    1,
    ELEMENT_END,
};

s32 N(FlippedDefense)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(InShellDefense)[] = {
    ELEMENT_NORMAL,   2,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              90,
    STATUS_KEY_POISON,             60,
    STATUS_KEY_FROZEN,            100,
    STATUS_KEY_DIZZY,              90,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,             80,
    STATUS_KEY_PARALYZE,           90,
    STATUS_KEY_SHRINK,             90,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
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
        .targetOffset = { -2, 36 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(NormalDefense),
        .eventFlags = ACTOR_EVENT_FLAG_FLIPABLE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -7 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = 0,
    .type = ACTOR_TYPE_KOOPA_TROOPA,
    .level = ACTOR_LEVEL_KOOPA_TROOPA,
    .maxHP = 4,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 70,
    .airLiftChance = 90,
    .hurricaneChance = 90,
    .spookChance = 90,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 1,
    .size = { 28, 36 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -15, 32 },
    .statusTextOffset = { 5, 32 },
    .spPool = CURRENT_SP_POOL,
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_KoopaTroopa_Idle,
    STATUS_KEY_STONE,     ANIM_KoopaTroopa_Still,
    STATUS_KEY_SLEEP,     ANIM_KoopaTroopa_Sleep,
    STATUS_KEY_POISON,    ANIM_KoopaTroopa_Idle,
    STATUS_KEY_STOP,      ANIM_KoopaTroopa_Still,
    STATUS_KEY_STATIC,    ANIM_KoopaTroopa_Idle,
    STATUS_KEY_PARALYZE,  ANIM_KoopaTroopa_Still,
    STATUS_KEY_DIZZY,     ANIM_KoopaTroopa_Stunned,
    STATUS_KEY_FEAR,      ANIM_KoopaTroopa_Stunned,
    STATUS_END,
};

s32 N(ShuffleAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_KoopaTroopa_Walk,
    STATUS_KEY_STONE,     ANIM_KoopaTroopa_Still,
    STATUS_KEY_SLEEP,     ANIM_KoopaTroopa_Sleep,
    STATUS_KEY_POISON,    ANIM_KoopaTroopa_Walk,
    STATUS_KEY_STOP,      ANIM_KoopaTroopa_Still,
    STATUS_KEY_STATIC,    ANIM_KoopaTroopa_Walk,
    STATUS_KEY_PARALYZE,  ANIM_KoopaTroopa_Still,
    STATUS_KEY_DIZZY,     ANIM_KoopaTroopa_Stunned,
    STATUS_KEY_FEAR,      ANIM_KoopaTroopa_Stunned,
    STATUS_END,
};

s32 N(FlippedAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_KoopaTroopa_ToppleStruggle,
    STATUS_KEY_STONE,     ANIM_KoopaTroopa_ToppleStill,
    STATUS_KEY_SLEEP,     ANIM_KoopaTroopa_ToppleSleep,
    STATUS_KEY_POISON,    ANIM_KoopaTroopa_ToppleStruggle,
    STATUS_KEY_STOP,      ANIM_KoopaTroopa_ToppleStill,
    STATUS_KEY_STATIC,    ANIM_KoopaTroopa_ToppleStruggle,
    STATUS_KEY_PARALYZE,  ANIM_KoopaTroopa_ToppleStill,
    STATUS_KEY_DIZZY,     ANIM_KoopaTroopa_ToppleStunned,
    STATUS_KEY_FEAR,      ANIM_KoopaTroopa_ToppleStunned,
    STATUS_END,
};

s32 N(InShellAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_STONE,     ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_SLEEP,     ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_POISON,    ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_STOP,      ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_STATIC,    ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_PARALYZE,  ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_DIZZY,     ANIM_KoopaTroopa_ShellSpin,
    STATUS_KEY_FEAR,      ANIM_KoopaTroopa_ShellSpin,
    STATUS_END,
};

extern EvtScript N(EVS_GoIntoShell);

EvtScript N(EVS_HandlePhase) = {
    // Start of battle popup
    Call(GetBattlePhase, LVar0)
    Switch(LVar0)
        CaseEq(PHASE_PLAYER_BEGIN)
            Call(GetActorVar, ACTOR_SELF, AVAR_InShellAtStart, LVar0)
            IfEq(LVar0, TRUE)
                Call(SetActorVar, ACTOR_SELF, AVAR_InShellAtStart, FALSE)
                ExecWait(N(EVS_GoIntoShell))
            EndIf
    EndSwitch
    Return
    End
};

EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    Call(BindHandlePhase, ACTOR_SELF, Ref(N(EVS_HandlePhase)))
    Call(SetActorVar, ACTOR_SELF, AVAR_IsFlipped, FALSE)
    Call(SetActorVar, ACTOR_SELF, AVAR_IsInShell, FALSE)
    Return
    End
};

#include "common/battle/SetAbsoluteStatusOffsets.inc.c"

EvtScript N(EVS_UpdateTargetOffsets) = {
    Switch(LVar0)
        CaseEq(0)
            IfFlag(LVar1, STATUS_FLAG_SLEEP | STATUS_FLAG_DIZZY)
                Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -5, 15)
                Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 0)
                Call(N(SetAbsoluteStatusOffsets), -10, 20, 10, 20)
            Else
                Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -4, 32)
                Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, -1, -4)
                Call(N(SetAbsoluteStatusOffsets), -15, 32, 5, 32)
            EndIf
        CaseEq(1)
            Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -5, 15)
            Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 0)
            Call(N(SetAbsoluteStatusOffsets), -10, 20, 10, 20)
    EndSwitch
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Label(0)
        Call(RandInt, 80, LVarA)
        Add(LVarA, 80)
        Loop(LVarA)
            Label(1)
                Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                ExecWait(N(EVS_UpdateTargetOffsets))
                IfEq(LVar0, 1)
                    Wait(1)
                    Goto(1)
                EndIf
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(1)
                EndIf
            Wait(1)
        EndLoop
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 5)
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(ShuffleAnims)))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleRunToGoal, ACTOR_SELF, 0)
        Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
        Switch(LVar0)
            CaseEq(0)
                Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(DefaultAnims)))
            CaseEq(1)
                Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(FlippedAnims)))
        EndSwitch
        Loop(20)
            Label(2)
                Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                ExecWait(N(EVS_UpdateTargetOffsets))
                IfEq(LVar0, 1)
                    Wait(1)
                    Goto(2)
                EndIf
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(2)
                EndIf
            Wait(1)
        EndLoop
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Sub(LVar0, 5)
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(ShuffleAnims)))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleRunToGoal, ACTOR_SELF, 0)
        Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
        Switch(LVar0)
            CaseEq(0)
                Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(DefaultAnims)))
            CaseEq(1)
                Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(FlippedAnims)))
        EndSwitch
        Loop(80)
            Label(3)
                Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                ExecWait(N(EVS_UpdateTargetOffsets))
                IfEq(LVar0, 1)
                    Wait(1)
                    Goto(3)
                EndIf
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(3)
                EndIf
            Wait(1)
        EndLoop
        Goto(0)
    Return
    End
};

EvtScript N(EVS_Idle_InShell) = {
    // Idle scripts must always be infinite loops
    Label(0)
        Wait(1000)
    Goto(0)
    Return
    End
};

s32 N(FlipPosOffsets)[] = { 9, 16, 22, 26, 30, 32, 33, 32, 30, 26, 22, 16, 9, 0, 4, 6, 7, 6, 4, 0, 2, 0 };

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetActorYaw, ACTOR_SELF, 0)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_BurnHurt)
            SetConst(LVar2, ANIM_KoopaTroopa_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_BurnHurt)
            SetConst(LVar2, ANIM_KoopaTroopa_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_BurnStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_FLIP_TRIGGER)
            Call(SetActorVar, ACTOR_SELF, AVAR_IsFlipped, TRUE)
            Call(SetActorVar, ACTOR_SELF, AVAR_FlippedTurns, 2)
            Call(SetDefenseTable, ACTOR_SELF, PRT_MAIN, Ref(N(FlippedDefense)))
            Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(FlippedAnims)))
            Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -5, 15)
            Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 0)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLIPPED, TRUE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Hurt)
            Call(GetStatusFlags, ACTOR_SELF, LVarA)
            Call(SetActorRotationOffset, ACTOR_SELF, 0, 12, 0)
            ChildThread
                Wait(4)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 30)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, -30)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, -60)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, -75)
                Wait(1)
            EndChildThread
            UseBuf(Ref(N(FlipPosOffsets)))
            Loop(ARRAY_COUNT(N(FlipPosOffsets)))
                BufRead1(LVar0)
                Call(SetActorDispOffset, ACTOR_SELF, 0, LVar0, 0)
                Wait(1)
            EndLoop
            Call(SetActorRotationOffset, ACTOR_SELF, 0, 0, 0)
            Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
            Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ToppleStruggle)
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_Knockback)
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Run)
            ExecWait(EVS_Enemy_ReturnHome)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.6))
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
            Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
            IfEq(LVar0, 0)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_ShellEnter)
                ExecWait(EVS_Enemy_NoDamageHit)
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellExit)
                    Wait(10)
                EndIf
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_ToppleStruggle)
                ExecWait(EVS_Enemy_NoDamageHit)
            EndIf
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_END_FIRST_STRIKE)
            Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
            IfEq(LVar0, 0)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_Run)
                ExecWait(EVS_Enemy_ReturnHome)
            EndIf
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
            IfEq(LVar0, 0)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_Idle)
                ExecWait(EVS_Enemy_Recover)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_ToppleStruggle)
                ExecWait(EVS_Enemy_NoDamageHit)
            EndIf
        CaseEq(EVENT_SCARE_AWAY)
            Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
            IfEq(LVar0, 0)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_Run)
                SetConst(LVar2, ANIM_KoopaTroopa_Hurt)
                ExecWait(EVS_Enemy_ScareAway)
                Return
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_ToppleStruggle)
                ExecWait(EVS_Enemy_NoDamageHit)
            EndIf
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
            IfEq(LVar0, 0)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_Panic)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_ToppleStruggle)
            EndIf
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
            IfEq(LVar0, 0)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_Panic)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_ToppleStruggle)
            EndIf
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseEq(EVENT_AIR_LIFT_FAILED)
            Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
            IfEq(LVar0, 0)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_Idle)
                ExecWait(EVS_Enemy_NoDamageHit)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_KoopaTroopa_ToppleStruggle)
                ExecWait(EVS_Enemy_NoDamageHit)
            EndIf
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_GetUp) = {
    Call(SetActorVar, ACTOR_SELF, AVAR_FlippedTurns, 0)
    Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
    Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(3.0))
    Wait(20)
    Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
    Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_JUMP)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ToppleStruggle)
    Set(LVar0, 0)
    Call(SetActorRotationOffset, ACTOR_SELF, 0, 12, 0)
    ChildThread
        Loop(5)
            Add(LVar0, 15)
            Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
            Wait(1)
        EndLoop
    EndChildThread
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(3.0))
    Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_STEP_A)
    Call(SetActorRotationOffset, ACTOR_SELF, 0, 0, 0)
    Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Idle)
    Call(SetActorVar, ACTOR_SELF, AVAR_IsFlipped, FALSE)
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(DefaultAnims)))
    Call(SetDefenseTable, ACTOR_SELF, PRT_MAIN, Ref(N(NormalDefense)))
    Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -4, 32)
    Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, -1, -4)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLIPPED, FALSE)
    Call(ResetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP)

    Return
    End
};

EvtScript N(EVS_ShellShot) = {
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellSpin)
    Else
        Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Wait(10)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellEnter)
        Wait(10)
        ChildThread
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar1, 4)
            PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
            Wait(3)
            PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
            Wait(2)
            PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
        EndChildThread
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SHELL_SPIN)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellSpin)
        Wait(10)
        Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
    EndIf

    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SHELL_TOSS)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_WALK, SOUND_NONE, SOUND_NONE)

    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(AddGoalPos, ACTOR_SELF, -40, 0, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(16.0))
            Call(RunToGoal, ACTOR_SELF, 0, FALSE)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                Call(UseBattleCamPreset, BTL_CAM_ACTOR_TARGET_MIDPOINT)
                Call(SetBattleCamDist, 300)
                Call(SetBattleCamOffsetY, 20)
                Call(MoveBattleCamOver, 10)
                Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
            EndIf
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellExit)
            Wait(8)
            Call(ResetAllActorSounds, ACTOR_SELF)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Wait(4)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_BeginPanic1)
            Wait(1)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_BeginPanic2)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar1, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(2.0))
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Panic)
            Wait(6)
            Sub(LVar1, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Panic)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, FALSE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Idle)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetPartYaw, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(16.0))
    Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, SUPPRESS_EVENT_ALL, 0, DMG_SHELL_TOSS, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 40)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, FALSE, TRUE, FALSE)
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 6, FALSE, TRUE, FALSE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellExit)
            Wait(8)
            Call(YieldTurn)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KoopaTroopa_Run)
            ExecWait(EVS_Enemy_ReturnHome)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Idle)
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_GoIntoShell) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)

    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellEnter)
    Wait(10)
    ChildThread
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 4)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
        Wait(3)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
        Wait(2)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
    EndChildThread
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SHELL_SPIN)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellSpin)
    Wait(10)

    Call(SetActorVar, ACTOR_SELF, AVAR_IsInShell, TRUE)
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(InShellAnims)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle_InShell)))
    Call(SetDefenseTable, ACTOR_SELF, PRT_MAIN, Ref(N(InShellDefense)))
    Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -5, 15)
    Call(SetPartEventFlags, ACTOR_SELF, PRT_MAIN, ACTOR_EVENT_FLAGS_NONE)

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)

    Return
    End
};

#define WHIRLWIND_DECOR_IDX 1

EvtScript N(EVS_ChargedShellShot) = {
    #define Lbl_End 0x10
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Wait(10)
    ChildThread
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 4)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
        Wait(3)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
        Wait(2)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
    EndChildThread
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SHELL_SPIN)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellSpin)
    Wait(20)
    Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, WHIRLWIND_DECOR_IDX, ACTOR_DECORATION_WHIRLWIND)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_DIZZY_SHELL)
    Wait(40)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE) // BTL_CAM_YADJ_NONE -> focus cam on y pos of enemy

    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SHELL_TOSS)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_WALK, SOUND_NONE, SOUND_NONE)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(AddGoalPos, ACTOR_SELF, -40, 0, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(16.0))
            Call(RunToGoal, ACTOR_SELF, 0, FALSE)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, WHIRLWIND_DECOR_IDX)

            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                Call(UseBattleCamPreset, BTL_CAM_ACTOR_TARGET_MIDPOINT)
                Call(SetBattleCamDist, 300)
                Call(SetBattleCamOffsetY, 20)
                Call(MoveBattleCamOver, 10)
                Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
            EndIf

            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellExit)
            Wait(8)
            Call(ResetAllActorSounds, ACTOR_SELF)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Wait(4)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_BeginPanic1)
            Wait(1)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_BeginPanic2)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar1, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(2.0))
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Panic)
            Wait(6)
            Sub(LVar1, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Panic)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, FALSE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Idle)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetPartYaw, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(YieldTurn)
            Goto(Lbl_End)
        EndCaseGroup
    EndSwitch
    // Not lucky

    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(24.0))
    Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, WHIRLWIND_DECOR_IDX)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, SUPPRESS_EVENT_ALL, DMG_STATUS_KEY(STATUS_FLAG_SHRINK, 2, 100), DMG_CHARGED_SHELL_TOSS, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            // Hit partner
            Call(SetTargetActor, ACTOR_SELF, ACTOR_PARTNER)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(12.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 22, FALSE, TRUE, FALSE)
            Wait(2)
            Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, SUPPRESS_EVENT_ALL, 0, DMG_CHARGED_SHELL_TOSS_PARTNER, BS_FLAGS1_TRIGGER_EVENTS)
            Switch(LVar0)
                CaseOrEq(HIT_RESULT_HIT)
                CaseOrEq(HIT_RESULT_NO_DAMAGE)
                    // return home
                    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
                    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
                    Add(LVar0, 40)
                    Set(LVar1, 0)
                    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
                    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                    Call(JumpToGoal, ACTOR_SELF, 10, FALSE, TRUE, FALSE)
                    Add(LVar0, 30)
                    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                    Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
                    Add(LVar0, 20)
                    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                    Call(JumpToGoal, ACTOR_SELF, 6, FALSE, TRUE, FALSE)
                    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellExit)
                    Wait(8)
                    Call(YieldTurn)
                    SetConst(LVar0, PRT_MAIN)
                    SetConst(LVar1, ANIM_KoopaTroopa_Run)
                    ExecWait(EVS_Enemy_ReturnHome)
                    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Idle)
            EndSwitch
        EndCaseGroup
    EndSwitch

    Label(Lbl_End)
    Call(SetActorVar, ACTOR_SELF, AVAR_IsInShell, FALSE)
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(DefaultAnims)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(SetDefenseTable, ACTOR_SELF, PRT_MAIN, Ref(N(NormalDefense)))
    Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -4, 32)
    Call(SetPartEventFlags, ACTOR_SELF, PRT_MAIN, ACTOR_EVENT_FLAG_FLIPABLE)

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End

    #undef Lbl_End
};

#define BobombId LVarE

EvtScript N(BobombCleanup) = {
    Call(StopLoopingSoundAtActor, ACTOR_SELF, 0)
    Call(EnableActorPaletteEffects, ACTOR_SELF, PRT_MAIN, FALSE)
    Return
    End
};

EvtScript N(BobombExplode) = {
    ExecWait(N(BobombCleanup))
    Call(StartRumble, BTL_RUMBLE_PLAYER_MAX)
    Thread
        Call(ShakeCam, CAM_BATTLE, 0, 2, Float(0.75))
        Call(ShakeCam, CAM_BATTLE, 0, 5, Float(3.0))
        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(4.5))
        Call(ShakeCam, CAM_BATTLE, 0, 5, Float(3.0))
    EndThread
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar2, 2)
    PlayEffect(EFFECT_SMOKE_RING, 0, LVar0, LVar1, LVar2, 0)
    Add(LVar1, 20)
    Add(LVar2, 2)
    PlayEffect(EFFECT_EXPLOSION, 0, LVar0, LVar1, LVar2, 0)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOMB_BLAST)
    Return
    End
};

EvtScript N(BobombThrow) = {
    Call(SetOwnerID, BobombId)

    Call(SetActorFlags, BobombId, ACTOR_FLAG_NO_ATTACK)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, BobombId)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)

    Call(PlayLoopingSoundAtActor, BobombId, 0, SOUND_LOOP_BOBOMB_FUSE)
    Call(SetAnimation, BobombId, PRT_MAIN, ANIM_Bobomb_RunLit)
    Call(EnableActorPaletteEffects, BobombId, PRT_MAIN, TRUE)
    Call(SetActorPaletteSwapParams, BobombId, PRT_MAIN, SPR_PAL_Bobomb, SPR_PAL_Bobomb_Burst, 0, 10, 0, 10, 0, 0)
    Call(SetActorPaletteEffect, BobombId, PRT_MAIN, ACTOR_PAL_ADJUST_BLEND_PALETTES_VARYING_INTERVALS)

    Call(SetTargetActor, BobombId, ACTOR_PLAYER)
    Call(SetGoalToTarget, BobombId)
    Call(SetActorSpeed, BobombId, Float(12.0))
    Call(SetActorJumpGravity, BobombId, Float(1.2))

    Call(EnemyTestTarget, BobombId, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(GetGoalPos, BobombId, LVar0, LVar1, LVar2)
            Add(LVar0, 30)
            Call(SetGoalPos, BobombId, LVar0, LVar1, LVar2)
            Call(JumpToGoal, BobombId, 22, FALSE, TRUE, FALSE)
            ExecWait(N(BobombExplode))
            Call(SetAnimation, BobombId, PRT_MAIN, ANIM_Bobomb_BurnStill)
            Wait(2)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, BobombId, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(10)
            Call(SetActorRotationOffset, BobombId, 0, 0, 0)
            Call(SetActorRotation, BobombId, 0, 0, 0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, -1)
            Set(LVar2, EXEC_DEATH_NO_SPINNING)
            ExecWait(EVS_Enemy_Death)
            Return
        EndCaseGroup
    EndSwitch

    Call(GetGoalPos, BobombId, LVar0, LVar1, LVar2)
    Call(SetGoalPos, BobombId, LVar0, LVar1, LVar2)
    Call(JumpToGoal, BobombId, 22, FALSE, TRUE, FALSE)
    ExecWait(N(BobombExplode))
    Call(SetAnimation, BobombId, PRT_MAIN, ANIM_Bobomb_BurnStill)
    Call(EnemyDamageTarget, BobombId, LVar0, DAMAGE_TYPE_BLAST | DAMAGE_TYPE_NO_CONTACT, 0, 0, 5, BS_FLAGS1_TRIGGER_EVENTS)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Wait(15)
    SetConst(LVar0, PRT_MAIN)
    SetConst(LVar1, ANIM_Bobomb_BurnStill)
    Set(LVar2, EXEC_DEATH_NO_SPINNING)
    ExecWait(EVS_Enemy_Death)
    Return
    End
};

EvtScript N(EVS_ChargedShellShotIntoBobomb) = {
    #define Lbl_End 0x10
    Call(SetTargetActor, ACTOR_SELF, BobombId)

    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Wait(10)
    ChildThread
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 4)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
        Wait(3)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
        Wait(2)
        PlayEffect(EFFECT_SMOKE_IMPACT, 1, LVar0, LVar1, LVar2, 32, 4, 0, 10, 0)
    EndChildThread
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SHELL_SPIN)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellSpin)
    Wait(20)
    Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, WHIRLWIND_DECOR_IDX, ACTOR_DECORATION_WHIRLWIND)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_DIZZY_SHELL)
    Wait(40)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE) // BTL_CAM_YADJ_NONE -> focus cam on y pos of enemy

    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SHELL_TOSS)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_WALK, SOUND_NONE, SOUND_NONE)
    //Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    // Not lucky


    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(24.0))
    Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, WHIRLWIND_DECOR_IDX)
    Wait(2)

    Thread
        // return home
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 40)
        Set(LVar1, 0)
        Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 10, FALSE, TRUE, FALSE)
        Add(LVar0, 30)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
        Add(LVar0, 20)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 6, FALSE, TRUE, FALSE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_ShellExit)
        Wait(8)
        SetConst(LVar0, PRT_MAIN)
        SetConst(LVar1, ANIM_KoopaTroopa_Run)
        ExecWait(EVS_Enemy_ReturnHome)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KoopaTroopa_Idle)
    EndThread

    // Launch bobomb
    ExecGetTID(N(BobombThrow), LVarF)
    Label(0x1e)
    IsThreadRunning(LVarF, LVarD)
    IfEq(LVarD, TRUE)
        Wait(1)
        Goto(0x1e)
    EndIf

    Label(Lbl_End)
    Call(SetActorVar, ACTOR_SELF, AVAR_IsInShell, FALSE)
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(DefaultAnims)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(SetDefenseTable, ACTOR_SELF, PRT_MAIN, Ref(N(NormalDefense)))
    Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -4, 32)
    Call(SetPartEventFlags, ACTOR_SELF, PRT_MAIN, ACTOR_EVENT_FLAG_FLIPABLE)

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End

    #undef Lbl_End
};

// todo: move somewhere nice
API_CALLABLE(N(FindEnemyWithId)) {
    Bytecode* args = script->ptrReadPos;
    BattleStatus* battleStatus = &gBattleStatus;
    s32 targetId = evt_get_variable(script, *args++);
    s32 retActorId = *args++;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(battleStatus->enemyActors); i++) {
        Actor* targetActor = battleStatus->enemyActors[i];
        if (targetActor == NULL)
            continue;
        if (targetActor->actorType != targetId)
            continue;

        evt_set_variable(script, retActorId, ACTOR_ENEMY0 + i);
        return ApiStatus_DONE2;
    }

    evt_set_variable(script, retActorId, FALSE);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(FindBestBobomb)) {
    Bytecode* args = script->ptrReadPos;
    BattleStatus* battleStatus = &gBattleStatus;
    s32 retActorId = *args++;
    s32 i;
    s32 highestTurnCount = -1;
    s32 bestI = -1;

    for (i = 0; i < ARRAY_COUNT(battleStatus->enemyActors); i++) {
        Actor* targetActor = battleStatus->enemyActors[i];
        if (targetActor == NULL)
            continue;
        if (targetActor->actorType != ACTOR_TYPE_BOB_OMB)
            continue;

        // Don't launch into already ignited bobombs, thats a waste
        if (targetActor->state.varTable[8/*AVAR_Ignited*/] == 1)
            continue;


        s32 turnCount = targetActor->state.varTable[0/*AVAR_TurnsUntilIgnition*/];
        if (turnCount >= highestTurnCount) {
            highestTurnCount = turnCount;
            bestI = i;
        }
    }

    if (bestI != -1) {
        evt_set_variable(script, retActorId, ACTOR_ENEMY0 + bestI);
        return ApiStatus_DONE2;
    }

    evt_set_variable(script, retActorId, FALSE);
    return ApiStatus_DONE2;
}

EvtScript N(EVS_TakeTurn) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)

    Call(GetActorVar, ACTOR_SELF, AVAR_IsFlipped, LVar0)
    IfEq(LVar0, TRUE)
        ExecWait(N(EVS_GetUp))
        ExecWait(N(EVS_ShellShot))
        Return
    EndIf

    Call(GetActorVar, ACTOR_SELF, AVAR_IsInShell, LVar0)
    IfEq(LVar0, TRUE)
        Call(N(FindBestBobomb), LVarE)
        IfEq(LVarE, FALSE)
            ExecWait(N(EVS_ChargedShellShot))
            Return
        EndIf
        // We have a bobomb on LVarE
        ExecWait(N(EVS_ChargedShellShotIntoBobomb))
        Return
    EndIf

    // Not flipped at the start of turn - shell shot or item, then go into shell
    STANDARD_ITEM_USE_AI_GOTO(5)
    ExecWait(N(EVS_ShellShot))
    Label(5)
    ExecWait(N(EVS_GoIntoShell))

    Return
    End
};
