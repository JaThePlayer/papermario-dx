#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/KoopaTroopa.h"
#include "sprite/npc/ParaTroopa.h"

#define NAMESPACE A(paratroopa)

extern s32 N(FlyingAnims)[];
extern s32 N(WingAnims)[];
extern EvtScript N(EVS_Flying_Init);
extern EvtScript N(EVS_Flying_Idle);
extern EvtScript N(EVS_Flying_TakeTurn);
extern EvtScript N(EVS_Flying_HandleEvent);
extern EvtScript N(EVS_KnockDown);

extern EvtScript A(koopa_troopa_EVS_Init);
extern EvtScript A(koopa_troopa_EVS_Idle);
extern EvtScript A(koopa_troopa_EVS_TakeTurn);
extern EvtScript A(koopa_troopa_EVS_HandleEvent);
extern EvtScript A(koopa_troopa_EVS_TakeTurn);
extern EvtScript A(koopa_troopa_EVS_HandleEvent);
extern s32 A(koopa_troopa_NormalDefense)[];
extern s32 A(koopa_troopa_StatusTable);
extern s32 A(koopa_troopa_DefaultAnims)[];

#include "common/StartRumbleWithParams.inc.c"
#include "common/battle/SetAbsoluteStatusOffsets.inc.c"

enum N(ActorPartIDs) {
    PRT_DOWNED          = 1,
    PRT_FLYING          = 2,
    PRT_DUMMY_WINGS     = 3,
};

enum N(ActorVars) {
    AVAR_AerialFlip     = 0, // TRUE if current hit should trigger a flip event when KnockDown is done
    AVAR_IsFlipped      = 8,
    AVAR_FlippedTurns   = 9,
};

enum N(ActorParams) {
    DMG_AIR_TACKLE      = 2,
    DMG_SHELL_TOSS      = 1,
};

s32 N(FlyingDefense)[] = {
    ELEMENT_NORMAL,   1,
    ELEMENT_END,
};

s32 N(FlippedDefense)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(FlyingStatusTable)[] = {
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
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_DOWNED,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -2, 36 },
        .opacity = 255,
        .idleAnimations = A(koopa_troopa_DefaultAnims),
        .defenseTable = A(koopa_troopa_NormalDefense),
        .eventFlags = ACTOR_EVENT_FLAG_FLIPABLE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -7 },
    },
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_FLYING,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -4, 36 },
        .opacity = 255,
        .idleAnimations = N(FlyingAnims),
        .defenseTable = N(FlyingDefense),
        .eventFlags = ACTOR_EVENT_FLAG_GROUNDABLE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 2, -14 },
    },
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_DUMMY_WINGS,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 32 },
        .opacity = 255,
        .idleAnimations = N(WingAnims),
        .defenseTable = A(koopa_troopa_NormalDefense),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = ACTOR_FLAG_FLYING,
    .type = ACTOR_TYPE_PARATROOPA,
    .level = ACTOR_LEVEL_PARATROOPA,
    .maxHP = 4,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Flying_Init),
    .statusTable = N(FlyingStatusTable),
    .escapeChance = 60,
    .airLiftChance = 95,
    .hurricaneChance = 100,
    .spookChance = 90,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 1,
    .size = { 50, 38 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 29 },
    .statusTextOffset = { 8, 30 },
    .spPool = CURRENT_SP_POOL,
};

s32 N(FlyingAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_ParaTroopa_Idle,
    STATUS_KEY_STONE,     ANIM_ParaTroopa_Still,
    STATUS_KEY_SLEEP,     ANIM_ParaTroopa_Sleep,
    STATUS_KEY_POISON,    ANIM_ParaTroopa_Still,
    STATUS_KEY_STOP,      ANIM_ParaTroopa_Still,
    STATUS_KEY_STATIC,    ANIM_ParaTroopa_Idle,
    STATUS_KEY_PARALYZE,  ANIM_ParaTroopa_Still,
    STATUS_KEY_DIZZY,     ANIM_ParaTroopa_Stunned,
    STATUS_KEY_FEAR,      ANIM_ParaTroopa_Stunned,
    STATUS_END,
};

s32 N(FlyingShuffleAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_ParaTroopa_Idle,
    STATUS_KEY_STONE,     ANIM_ParaTroopa_Still,
    STATUS_KEY_SLEEP,     ANIM_ParaTroopa_Sleep,
    STATUS_KEY_POISON,    ANIM_ParaTroopa_Still,
    STATUS_KEY_STOP,      ANIM_ParaTroopa_Still,
    STATUS_KEY_STATIC,    ANIM_ParaTroopa_Idle,
    STATUS_KEY_PARALYZE,  ANIM_ParaTroopa_Still,
    STATUS_KEY_DIZZY,     ANIM_ParaTroopa_Stunned,
    STATUS_KEY_FEAR,      ANIM_ParaTroopa_Stunned,
    STATUS_END,
};

s32 N(WingAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_ParaTroopa_WingsStill,
    STATUS_KEY_STONE,     ANIM_ParaTroopa_Still,
    STATUS_KEY_SLEEP,     ANIM_ParaTroopa_Sleep,
    STATUS_KEY_POISON,    ANIM_ParaTroopa_Still,
    STATUS_KEY_STOP,      ANIM_ParaTroopa_Still,
    STATUS_KEY_STATIC,    ANIM_ParaTroopa_WingsStill,
    STATUS_KEY_PARALYZE,  ANIM_ParaTroopa_Still,
    STATUS_KEY_DIZZY,     ANIM_ParaTroopa_Stunned,
    STATUS_KEY_FEAR,      ANIM_ParaTroopa_Stunned,
    STATUS_END,
};

EvtScript N(EVS_Flying_HandlePhase) = {
    Call(SetActorVar, ACTOR_SELF, AVAR_AerialFlip, FALSE)
    Return
    End
};

EvtScript N(EVS_Flying_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_Flying_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Flying_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_Flying_HandleEvent)))
    Call(BindHandlePhase, ACTOR_SELF, Ref(N(EVS_Flying_HandlePhase)))
    Call(SetActorVar, ACTOR_SELF, AVAR_AerialFlip, FALSE)
    Return
    End
};

EvtScript N(EVS_Flying_UpdateTargetOffsets) = {
    IfFlag(LVar1, STATUS_FLAG_DIZZY | STATUS_FLAG_SLEEP)
        Call(SetTargetOffset, ACTOR_SELF, PRT_FLYING, 0, 25)
        Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_FLYING, 3, -6)
        Call(N(SetAbsoluteStatusOffsets), -19, 25, 4, 28)
    Else
        Call(SetTargetOffset, ACTOR_SELF, PRT_FLYING, -2, 36)
        Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_FLYING, 0, -7)
        Call(N(SetAbsoluteStatusOffsets), -10, 29, 8, 30)
    EndIf
    Return
    End
};

EvtScript N(EVS_Flying_Idle) = {
    Label(10)
        Call(RandInt, 80, LVar0)
        Add(LVar0, 80)
        Loop(LVar0)
            Label(0)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                ExecWait(N(EVS_Flying_UpdateTargetOffsets))
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(0)
                EndIf
            Wait(1)
        EndLoop
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 5)
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(0.6))
        Call(SetIdleAnimations, ACTOR_SELF, PRT_FLYING, Ref(N(FlyingShuffleAnims)))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, -5, EASING_LINEAR)
        Call(SetIdleAnimations, ACTOR_SELF, PRT_DOWNED, Ref(N(FlyingAnims)))
        Loop(20)
            Label(1)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                ExecWait(N(EVS_Flying_UpdateTargetOffsets))
                IfFlag(LVar1, STATUS_FLAGS_IMMOBILIZED)
                    Wait(1)
                    Goto(1)
                EndIf
            Wait(1)
        EndLoop
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Sub(LVar0, 5)
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(0.6))
        Call(SetIdleAnimations, ACTOR_SELF, PRT_FLYING, Ref(N(FlyingShuffleAnims)))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, -5, EASING_LINEAR)
        Call(SetIdleAnimations, ACTOR_SELF, PRT_FLYING, Ref(N(FlyingAnims)))
        Loop(40)
            Label(2)
                Call(GetStatusFlags, ACTOR_SELF, LVar1)
                ExecWait(N(EVS_Flying_UpdateTargetOffsets))
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

EvtScript N(EVS_FlyHome) = {
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Run)
    Call(SetGoalToHome, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
    Call(FlyToGoal, ACTOR_SELF, 0, 1, EASING_SIN_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Idle)
    Return
    End
};

EvtScript N(EVS_Flying_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            Call(SetActorVar, ACTOR_SELF, AVAR_AerialFlip, TRUE)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_Hit)
        CaseEq(EVENT_HIT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                Call(GetEncounterTrigger, LVar0)
                IfEq(LVar0, ENCOUNTER_TRIGGER_HAMMER)
                    ExecWait(N(EVS_KnockDown))
                EndIf
            EndIf
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_BurnHurt)
            SetConst(LVar2, ANIM_ParaTroopa_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_BurnHurt)
            SetConst(LVar2, ANIM_ParaTroopa_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_BurnStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_FALL_TRIGGER)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_KnockDown))
        CaseEq(EVENT_15)
            SetConst(LVar0, PRT_DOWNED)
            SetConst(LVar1, ANIM_ParaTroopa_BurnHurt)
            SetConst(LVar1, ANIM_ParaTroopa_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            ExecWait(N(EVS_KnockDown))
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_DOWNED)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_DOWNED)
            SetConst(LVar1, ANIM_ParaTroopa_HurtStill)
            ExecWait(EVS_Enemy_Knockback)
            ExecWait(N(EVS_FlyHome))
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_ZERO_DAMAGE)
            Call(SetActorVar, ACTOR_SELF, AVAR_AerialFlip, TRUE)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_ShellEnter)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_ShellEnter)
            ExecWait(EVS_Enemy_NoDamageHit)
            Call(GetBattlePhase, LVar0)
            IfEq(LVar0, PHASE_FIRST_STRIKE)
                Call(GetEncounterTrigger, LVar0)
                IfEq(LVar0, ENCOUNTER_TRIGGER_HAMMER)
                    ExecWait(N(EVS_KnockDown))
                EndIf
            EndIf
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_BEGIN_FIRST_STRIKE)
            Call(SetActorPos, ACTOR_SELF, 20, 0, 0)
            Call(HPBarToCurrent, ACTOR_SELF)
        CaseEq(EVENT_END_FIRST_STRIKE)
            ExecWait(N(EVS_FlyHome))
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, FALSE)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Run)
            SetConst(LVar2, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Run)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_ParaTroopa_Hurt)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_Flying_TakeTurn) = {
    STANDARD_ITEM_USE_AI()

    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(UseBattleCamPreset, BTL_CAM_PRESET_05)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Run)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 40, 30, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(7.0))
    Call(FlyToGoal, ACTOR_SELF, 0, -16, EASING_LINEAR)
    Call(UseBattleCamPresetImmediately, BTL_CAM_PRESET_01)
    Wait(5)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_PREDIVE)
            Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_FLY, SOUND_NONE, SOUND_NONE)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Windup)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(AddGoalPos, ACTOR_SELF, 6, 6, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(2.0))
            Call(FlyToGoal, ACTOR_SELF, 0, 0, EASING_LINEAR)
            Wait(6)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_DIVE)
            Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_FLY, SOUND_NONE, SOUND_NONE)
            Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
            Sub(LVar0, 25)
            Set(LVar1, 0)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorSpeed, ACTOR_SELF, Float(10.0))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_ShellDive)
            Call(FlyToGoal, ACTOR_SELF, 0, 0, EASING_LINEAR)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(4.0))
            Thread
                Call(SetActorRotationOffset, ACTOR_SELF, 0, 10, 0)
                Set(LVar0, 0)
                Loop(8)
                    Add(LVar0, 90)
                    IfGe(LVar0, 360)
                        Sub(LVar0, 360)
                    EndIf
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Wait(1)
                EndLoop
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
                Call(SetActorRotationOffset, ACTOR_SELF, 0, -10, 0)
            EndThread
            Call(JumpToGoal, ACTOR_SELF, 6, FALSE, TRUE, FALSE)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(4.0))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Hurt)
            Call(JumpToGoal, ACTOR_SELF, 6, FALSE, TRUE, FALSE)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(4.0))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Hurt)
            Call(JumpToGoal, ACTOR_SELF, 6, FALSE, TRUE, FALSE)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar1, 30)
            Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Wait(20)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(ResetAllActorSounds, ACTOR_SELF)
            Call(AddActorDecoration, ACTOR_SELF, PRT_FLYING, 0, ACTOR_DECORATION_SWEAT)
            Call(SetActorYaw, ACTOR_SELF, 180)
            ExecWait(N(EVS_FlyHome))
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_FLYING, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
    EndSwitch
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_PREDIVE)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_FLY, SOUND_NONE, SOUND_NONE)
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Windup)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(AddGoalPos, ACTOR_SELF, 6, 6, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(2.0))
    Call(FlyToGoal, ACTOR_SELF, 0, 0, EASING_LINEAR)
    Wait(6)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_DIVE)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_FLY, SOUND_NONE, SOUND_NONE)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(10.0))
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_ShellDive)
    Call(FlyToGoal, ACTOR_SELF, 0, 0, EASING_LINEAR)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, SUPPRESS_EVENT_ALL, 0, DMG_AIR_TACKLE, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(AddGoalPos, ACTOR_SELF, 40, 30, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_ShellHit)
            Thread
                Call(SetActorRotationOffset, ACTOR_SELF, 0, 10, 0)
                Set(LVar0, 0)
                Loop(12)
                    Sub(LVar0, 90)
                    IfLt(LVar0, 0)
                        Add(LVar0, 360)
                    EndIf
                    Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                    Wait(1)
                EndLoop
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
                Call(SetActorRotationOffset, ACTOR_SELF, 0, -10, 0)
                Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_ParaTroopa_Idle)
            EndThread
            Call(FlyToGoal, ACTOR_SELF, 0, 30, EASING_LINEAR)
            Wait(10)
            Call(YieldTurn)
            Call(ResetAllActorSounds, ACTOR_SELF)
            ExecWait(N(EVS_FlyHome))
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_KnockDown) = {
    Call(SetPartDispOffset, ACTOR_SELF, PRT_FLYING, 0, 0, 0)
    Call(HideHealthBar, ACTOR_SELF)
    Call(SetPartFlags, ACTOR_SELF, PRT_FLYING, ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_SHADOW | ACTOR_PART_FLAG_NO_TARGET)
    Call(SetPartFlags, ACTOR_SELF, PRT_DUMMY_WINGS, ACTOR_PART_FLAG_NO_SHADOW | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION)
    Call(SetPartFlags, ACTOR_SELF, PRT_DOWNED, ACTOR_PART_FLAG_NO_SHADOW | ACTOR_PART_FLAG_PRIMARY_TARGET)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, 8)
    Sub(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_DUMMY_WINGS, LVar0, LVar1, LVar2)
    Call(SetAnimation, ACTOR_SELF, PRT_DOWNED, ANIM_KoopaTroopa_Hurt)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_FALL_QUICK, 0)
    Call(GetActorVar, ACTOR_SELF, AVAR_AerialFlip, LVarA)
    IfFalse(LVarA)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Set(LVar1, 0)
        Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
        Call(N(StartRumbleWithParams), 128, 7)
        Thread
            Call(ShakeCam, CAM_BATTLE, 0, 5, Float(0.3))
        EndThread
        Call(ResetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 10, FALSE, TRUE, FALSE)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
        Loop(10)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_DUMMY_WINGS, ACTOR_PART_FLAG_INVISIBLE, TRUE)
            Wait(1)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_DUMMY_WINGS, ACTOR_PART_FLAG_INVISIBLE, FALSE)
            Wait(1)
        EndLoop
        Call(SetPartFlagBits, ACTOR_SELF, PRT_DUMMY_WINGS, ACTOR_PART_FLAG_INVISIBLE, TRUE)
    Else
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Set(LVar1, 0)
        Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
        Thread
            Loop(10)
                Call(SetPartFlagBits, ACTOR_SELF, PRT_DUMMY_WINGS, ACTOR_PART_FLAG_INVISIBLE, TRUE)
                Wait(1)
                Call(SetPartFlagBits, ACTOR_SELF, PRT_DUMMY_WINGS, ACTOR_PART_FLAG_INVISIBLE, FALSE)
                Wait(1)
            EndLoop
            Call(SetPartFlagBits, ACTOR_SELF, PRT_DUMMY_WINGS, ACTOR_PART_FLAG_INVISIBLE, TRUE)
        EndThread
        Call(N(StartRumbleWithParams), 150, 7)
        Thread
            Call(ShakeCam, CAM_BATTLE, 0, 5, Float(0.3))
        EndThread
        Call(ResetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP)
    EndIf
    Call(GetIndexFromHome, ACTOR_SELF, LVar0)
    Mod(LVar0, 4)
    Call(SetGoalToIndex, ACTOR_SELF, LVar0)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetAnimation, ACTOR_SELF, PRT_DOWNED, ANIM_KoopaTroopa_Idle)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, FALSE)
    Call(SetStatusTable, ACTOR_SELF, Ref(A(koopa_troopa_StatusTable)))
    Call(BindHandlePhase, ACTOR_SELF, 0)
    ExecWait(A(koopa_troopa_EVS_Init))
    Call(SetActorType, ACTOR_SELF, ACTOR_TYPE_KOOPA_TROOPA)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_TYPE_CHANGED, TRUE)
    Call(HPBarToHome, ACTOR_SELF)
    Call(ResetAllActorSounds, ACTOR_SELF)
    IfTrue(LVarA)
        Call(DispatchEvent, ACTOR_SELF, EVENT_FLIP_TRIGGER)
    EndIf
    Return
    End
};
