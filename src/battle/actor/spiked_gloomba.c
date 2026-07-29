#include "battle/battle.h"
#include "misc_patches/actor_interfaces.h"
#include "battle/interfaces/IGloomba.h"
#include "script_api/battle.h"
#include "sprite/npc/SpikedGoomba.h"
#include "effects.h"

#define NAMESPACE A(spiked_gloomba)

#include "battle/interfaces/IGloomba.inc.c"

extern s32 N(DefaultAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_POISON,  -1, // heal
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              70,
    STATUS_KEY_POISON,             80,
    STATUS_KEY_FROZEN,            100,
    STATUS_KEY_DIZZY,              80,
    STATUS_KEY_UNUSED,      80,
    STATUS_KEY_STATIC,             80,
    STATUS_KEY_PARALYZE,           90,
    STATUS_KEY_SHRINK,             80,
    STATUS_KEY_STOP,               90,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_UNUSED,         0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       0,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

ActorPartBlueprint N(ActorParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 24 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAG_SPIKY_TOP,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -12 },
    },
};

export ActorBlueprint blueprint = {
    .flags = 0,
    .type = ACTOR_TYPE_SPIKED_GLOOMBA,
    .level = ACTOR_LEVEL_SPIKED_GLOOMBA,
    .maxHP = 7,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 65,
    .airLiftChance = 90,
    .hurricaneChance = 85,
    .spookChance = 75,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 1,
    .size = { 24, 28 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
    .spPool = CURRENT_SP_POOL,
};

IMPLEMENT(IGroundedGloomba,
    .anims = {
        .idle = ANIM_SpikedGoomba_Dark_Idle,
        .midair = ANIM_SpikedGoomba_Dark_Midair,
        .dizzy = ANIM_SpikedGoomba_Dark_Dizzy,
        .sleep = ANIM_SpikedGoomba_Dark_Sleep,
        .hurt = ANIM_SpikedGoomba_Dark_Hurt,
        .tense = ANIM_SpikedGoomba_Dark_Still,
        .run = ANIM_SpikedGoomba_Dark_Run,
    },
    .friendlyHeadbonkDamage = 1,
    .headbonkDamage = 3,
    .buffedHeadbonkDamage = 5,
);

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_SpikedGoomba_Dark_Idle,
    STATUS_KEY_STONE,     ANIM_SpikedGoomba_Dark_Still,
    STATUS_KEY_SLEEP,     ANIM_SpikedGoomba_Dark_Sleep,
    STATUS_KEY_POISON,    ANIM_SpikedGoomba_Dark_Idle,
    STATUS_KEY_STOP,      ANIM_SpikedGoomba_Dark_Still,
    STATUS_KEY_STATIC,    ANIM_SpikedGoomba_Dark_Idle,
    STATUS_KEY_PARALYZE,  ANIM_SpikedGoomba_Dark_Still,
    STATUS_KEY_DIZZY,     ANIM_SpikedGoomba_Dark_Dizzy,
    STATUS_KEY_UNUSED,    ANIM_SpikedGoomba_Dark_Dizzy,
    STATUS_END,
};

s32 N(ShuffleAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_SpikedGoomba_Dark_Run,
    STATUS_KEY_STONE,     ANIM_SpikedGoomba_Dark_Still,
    STATUS_KEY_SLEEP,     ANIM_SpikedGoomba_Dark_Sleep,
    STATUS_KEY_POISON,    ANIM_SpikedGoomba_Dark_Idle,
    STATUS_KEY_STOP,      ANIM_SpikedGoomba_Dark_Still,
    STATUS_KEY_STATIC,    ANIM_SpikedGoomba_Dark_Run,
    STATUS_KEY_PARALYZE,  ANIM_SpikedGoomba_Dark_Still,
    STATUS_KEY_DIZZY,     ANIM_SpikedGoomba_Dark_Dizzy,
    STATUS_KEY_UNUSED,    ANIM_SpikedGoomba_Dark_Dizzy,
    STATUS_END,
};

EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Label(10)
        Call(RandInt, 80, LVar0)
        Add(LVar0, 80)
        Loop(LVar0)
            Label(0)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(0)
                EndIf
            Wait(1)
        EndLoop
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 5)
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(ShuffleAnims)))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleRunToGoal, ACTOR_SELF, 0)
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(DefaultAnims)))
        Loop(20)
            Label(1)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(1)
                EndIf
            Wait(1)
        EndLoop
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Sub(LVar0, 5)
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(ShuffleAnims)))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleRunToGoal, ACTOR_SELF, 0)
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(DefaultAnims)))
        Loop(80)
            Label(2)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(2)
                EndIf
            Wait(1)
        EndLoop
        Goto(10)
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Hurt)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_BurnEyesClosed)
            SetConst(LVar2, ANIM_SpikedGoomba_Dark_BurnEyesOpen)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_BurnEyesClosed)
            SetConst(LVar2, ANIM_SpikedGoomba_Dark_BurnEyesOpen)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_BurnEyesOpen)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Electrocute)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Hurt)
            ExecWait(EVS_Enemy_Knockback)
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SpikedGoomba_Dark_Dizzy)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, false)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            Wait(5)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SpikedGoomba_Dark_Idle)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.6))
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Electrocute)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_STAR_BEAM)
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_END_FIRST_STRIKE)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SpikedGoomba_Dark_Run)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(4.0))
            Call(RunToGoal, ACTOR_SELF, 0, false)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Run)
            SetConst(LVar2, ANIM_SpikedGoomba_Dark_Hurt)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Run)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SpikedGoomba_Dark_Hurt)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseEq(EVENT_SPIKE_TAUNT)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SpikedGoomba_Dark_Laugh)
            EndIf
            Wait(10)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(GetStatusFlags, ACTOR_SELF, LVar3)
            IfFlag(LVar3, STATUS_FLAG_SHRINK)
                Add(LVar1, 10)
            Else
                Add(LVar1, 26)
            EndIf
            PlayEffect(EFFECT_LENS_FLARE, 0, LVar0, LVar1, LVar2, 20, 0)
            Wait(20)
        CaseDefault
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SpikedGoomba_Dark_Idle)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};
