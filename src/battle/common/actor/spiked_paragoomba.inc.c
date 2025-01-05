#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/SpikedParagoomba.h"

#define NAMESPACE A(spiked_paragoomba)

extern s32 N(DefaultAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorPartIDs) {
    PRT_FLYING          = 1, // body while flying
};

enum N(ActorParams) {
    DMG_AIR_KICK        = 1,
};


s32 N(FlyingDefense)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(FlyingStatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,             100,
    STATUS_KEY_POISON,             80,
    STATUS_KEY_FROZEN,            100,
    STATUS_KEY_DIZZY,             100,
    STATUS_KEY_FEAR,               80,
    STATUS_KEY_STATIC,             80,
    STATUS_KEY_PARALYZE,          100,
    STATUS_KEY_SHRINK,            100,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          1,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       0,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

ActorPartBlueprint N(ActorParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_FLYING,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 24 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(FlyingDefense),
        .eventFlags = ACTOR_EVENT_FLAG_SPIKY_TOP,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    }
};

ActorBlueprint NAMESPACE = {
    .flags = ACTOR_FLAG_FLYING,
    .type = ACTOR_TYPE_SPIKED_PARAGOOMBA,
    .level = ACTOR_LEVEL_SPIKED_PARAGOOMBA,
    .maxHP = 2,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(FlyingStatusTable),
    .escapeChance = 90,
    .airLiftChance = 100,
    .hurricaneChance = 100,
    .spookChance = 100,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 1,
    .size = { 36, 26 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
    .spPool = CURRENT_SP_POOL,
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_SpikedParagoomba_Idle,
    STATUS_KEY_STONE,     ANIM_SpikedParagoomba_Still,
    STATUS_KEY_SLEEP,     ANIM_SpikedParagoomba_Sleep,
    STATUS_KEY_POISON,    ANIM_SpikedParagoomba_Idle,
    STATUS_KEY_STOP,      ANIM_SpikedParagoomba_Still,
    STATUS_KEY_STATIC,    ANIM_SpikedParagoomba_Idle,
    STATUS_KEY_PARALYZE,  ANIM_SpikedParagoomba_Still,
    STATUS_KEY_DIZZY,     ANIM_SpikedParagoomba_Dizzy,
    STATUS_KEY_FEAR,      ANIM_SpikedParagoomba_Dizzy,
    STATUS_END,
};

s32 N(ShuffleAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_SpikedParagoomba_Run,
    STATUS_KEY_STONE,     ANIM_SpikedParagoomba_Still,
    STATUS_KEY_SLEEP,     ANIM_SpikedParagoomba_Sleep,
    STATUS_KEY_POISON,    ANIM_SpikedParagoomba_Idle,
    STATUS_KEY_STOP,      ANIM_SpikedParagoomba_Still,
    STATUS_KEY_STATIC,    ANIM_SpikedParagoomba_Run,
    STATUS_KEY_PARALYZE,  ANIM_SpikedParagoomba_Still,
    STATUS_KEY_DIZZY,     ANIM_SpikedParagoomba_Dizzy,
    STATUS_KEY_FEAR,      ANIM_SpikedParagoomba_Dizzy,
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
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(0.6))
        Call(SetIdleAnimations, ACTOR_SELF, PRT_FLYING, Ref(N(ShuffleAnims)))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, -5, EASING_LINEAR)
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
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(0.6))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, -5, EASING_LINEAR)
        Call(SetIdleAnimations, ACTOR_SELF, PRT_FLYING, Ref(N(DefaultAnims)))
        Loop(40)
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

EvtScript N(EVS_Paragoomba_GoHome) = {
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(SetGoalToHome, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(7.0))
    Call(FlyToGoal, ACTOR_SELF, 0, 1, EASING_SIN_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Idle)
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Hurt)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_BurnHurt)
            SetConst(LVar2, ANIM_SpikedParagoomba_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_BurnHurt)
            SetConst(LVar2, ANIM_SpikedParagoomba_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_BurnStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_FALL_TRIGGER)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Hurt)
            ExecWait(EVS_Enemy_Hit)
        CaseEq(EVENT_15)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_BurnHurt)
            SetConst(LVar2, ANIM_SpikedParagoomba_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Electrocute)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Hurt)
            ExecWait(EVS_Enemy_Knockback)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Dizzy)
            ExecWait(N(EVS_Paragoomba_GoHome))
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Electrocute)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_STAR_BEAM)
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_HurtStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_BEGIN_FIRST_STRIKE)
            Call(SetActorPos, ACTOR_SELF, 20, 0, 0)
            Call(HPBarToCurrent, ACTOR_SELF)
        CaseEq(EVENT_END_FIRST_STRIKE)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Run)
            ExecWait(N(EVS_Paragoomba_GoHome))
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, FALSE)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Run)
            SetConst(LVar2, ANIM_SpikedParagoomba_Hurt)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Run)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_SpikedParagoomba_Hurt)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_TakeTurn) = {
    STANDARD_ITEM_USE_AI()

    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Run)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 50, 0, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
    Call(FlyToGoal, ACTOR_SELF, 0, -4, EASING_LINEAR)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Wait(10)
            Call(UseBattleCamPreset, BTL_CAM_ENEMY_DIVE)
            Call(SetBattleCamDist, 300)
            Call(BattleCamTargetActor, ACTOR_SELF)
            Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_PREDIVE)
            Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(5.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Dive)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, FALSE, FALSE, FALSE)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_DIVE)
            Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_FLY, SOUND_NONE, SOUND_NONE)
            Call(SetGoalToTarget, ACTOR_SELF)
            Sub(LVar0, 80)
            Set(LVar1, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Dive)
            Call(FlyToGoal, ACTOR_SELF, 0, -10, EASING_QUADRATIC_OUT)
            Thread
                Loop(4)
                    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_FLY)
                    Wait(4)
                EndLoop
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Miss)
            Call(AddActorDecoration, ACTOR_SELF, PRT_FLYING, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimationRate, ACTOR_SELF, PRT_FLYING, Float(3.0))
            Wait(10)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(10)
            Call(SetAnimationRate, ACTOR_SELF, PRT_FLYING, Float(1.0))
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(ResetAllActorSounds, ACTOR_SELF)
            Call(YieldTurn)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Run)
            Call(SetActorYaw, ACTOR_SELF, 180)
            ExecWait(N(EVS_Paragoomba_GoHome))
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_FLYING, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
        CaseDefault
            Wait(10)
            Call(UseBattleCamPreset, BTL_CAM_ENEMY_DIVE)
            Call(SetBattleCamDist, 300)
            Call(BattleCamTargetActor, ACTOR_SELF)
            Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_PREDIVE)
            Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(5.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Dive)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, FALSE, FALSE, FALSE)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PARAGOOMBA_DIVE)
            Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_FLY, SOUND_NONE, SOUND_NONE)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Dive)
            Call(FlyToGoal, ACTOR_SELF, 0, -10, EASING_LINEAR)
    EndSwitch
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, DMG_AIR_KICK, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(AddGoalPos, ACTOR_SELF, 50, 10, 0)
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Run)
            Call(FlyToGoal, ACTOR_SELF, 0, -10, EASING_LINEAR)
            Call(ResetAllActorSounds, ACTOR_SELF)
            Wait(5)
            Call(YieldTurn)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_SpikedParagoomba_Run)
            ExecWait(N(EVS_Paragoomba_GoHome))
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};
