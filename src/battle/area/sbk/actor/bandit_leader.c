#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/Bandit.h"
#include "misc_patches/custom_status.h"
#include "misc_patches/misc_patches.h"

#define NAMESPACE A(bandit_leader)

extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_Init_Coin);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

#define PRT_MAIN 1


enum N(ActorVars) {
    AVAR_Summon1 = 1,
    AVAR_Summon2 = 2,
};

enum N(ActorParams) {
    DMG_LD_TACKLE          = 3,
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_Bandit_Leader_Idle,
    STATUS_KEY_STONE,     ANIM_Bandit_Leader_Still,
    STATUS_KEY_SLEEP,     ANIM_Bandit_Leader_Sleep,
    STATUS_KEY_POISON,    ANIM_Bandit_Leader_Idle,
    STATUS_KEY_STOP,      ANIM_Bandit_Leader_Still,
    STATUS_KEY_STATIC,    ANIM_Bandit_Leader_Idle,
    STATUS_KEY_PARALYZE,  ANIM_Bandit_Leader_Still,
    STATUS_KEY_DIZZY,     ANIM_Bandit_Leader_Dizzy,
    STATUS_KEY_FEAR,      ANIM_Bandit_Leader_Dizzy,
    STATUS_END,
};

s32 N(HoldingAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_Bandit_Leader_IdleHolding,
    STATUS_KEY_STONE,     ANIM_Bandit_Leader_StillHolding,
    STATUS_KEY_SLEEP,     ANIM_Bandit_Leader_Sleep,
    STATUS_KEY_POISON,    ANIM_Bandit_Leader_IdleHolding,
    STATUS_KEY_STOP,      ANIM_Bandit_Leader_StillHolding,
    STATUS_KEY_STATIC,    ANIM_Bandit_Leader_IdleHolding,
    STATUS_KEY_PARALYZE,  ANIM_Bandit_Leader_StillHolding,
    STATUS_KEY_DIZZY,     ANIM_Bandit_Leader_Dizzy,
    STATUS_KEY_FEAR,      ANIM_Bandit_Leader_Dizzy,
    STATUS_END,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              40,
    STATUS_KEY_POISON,            100,
    STATUS_KEY_FROZEN,             40,
    STATUS_KEY_DIZZY,              40,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,             30,
    STATUS_KEY_PARALYZE,           40,
    STATUS_KEY_SHRINK,             75,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,        -1,
    STATUS_TURN_MOD_DIZZY,         -2,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,        -1,
    STATUS_TURN_MOD_PARALYZE,      -1,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,          -1,
    STATUS_END,
};

ActorPartBlueprint N(ActorParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -2, 28 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -12 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = 0,
    .type = ACTOR_TYPE_BANDIT_LEADER,
    .level = ACTOR_LEVEL_BANDIT_LEADER,
    .maxHP = 30,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 0,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 0,
    .powerBounceChance = 80,
    .coinReward = 0,
    .size = { 26, 32 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 25 },
    .statusTextOffset = { 10, 25 },
    .spPool = SP_POOL_NONE,
};


EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))

    SET_ACTOR_VAR(AVAR_Summon1, ACTOR_ENEMY19)
    SET_ACTOR_VAR(AVAR_Summon2, ACTOR_ENEMY19)
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_Hit)
        CaseOrEq(EVENT_HIT)
        CaseOrEq(EVENT_SPIN_SMASH_LAUNCH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_BurnHurt)
            SetConst(LVar2, ANIM_Bandit_Leader_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_BurnHurt)
            SetConst(LVar2, ANIM_Bandit_Leader_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_BurnStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_Knockback)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Run)
            ExecWait(EVS_Enemy_ReturnHome)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.6))
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar1, ANIM_Bandit_Leader_Idle)
            SetConst(LVar0, PRT_MAIN)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseOrEq(EVENT_DEATH)
        CaseOrEq(EVENT_SPIN_SMASH_LAUNCH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        EndCaseGroup
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Run)
            SetConst(LVar2, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Bandit_Leader_Hurt)
            SetConst(LVar2, STATUS_FLAG_DIZZY)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseDefault
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Idle)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

Vec3i N(SummonPos) = { NPC_DISPOSE_LOCATION };
extern ActorBlueprint A(bandit);

Formation N(SummonFormation) = {
    ACTOR_BY_POS(A(bandit), N(SummonPos), 100),
};

EvtScript N(EVS_TryFindLocation) = {
    GET_ACTOR_VAR(AVAR_Summon1, LVar0)
    Call(ActorExists, LVar0, LVar0)
    IfFalse(LVar0)
        Set(LVar9, AVAR_Summon1)
        Return
    EndIf

    GET_ACTOR_VAR(AVAR_Summon2, LVar0)
    Call(ActorExists, LVar0, LVar0)
    IfFalse(LVar0)
        Set(LVar9, AVAR_Summon2)
        Return
    EndIf

    Set(LVar9, -1)
    Return
    End
};

#define BUFF_ENEMY(avar) \
    GET_ACTOR_VAR(avar, LVar0) \
    Call(ActorExists, LVar0, LVar1) \
    IfTrue(LVar1) \
        Call(GetCustomStatusPotency, LVar0, ATK_UP_TEMP_STATUS, LVar1) \
        Add(LVar1, 1) \
        Call(InflictCustomStatus, LVar0, ATK_UP_TEMP_STATUS, 2, LVar1, 100) \
    EndIf

EvtScript N(EVS_Summon) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GENERAL_WHISTLE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Relief)

    Wait(8)
    // Buff existing bandits
    BUFF_ENEMY(AVAR_Summon1)
    BUFF_ENEMY(AVAR_Summon2)

    ExecWait(N(EVS_TryFindLocation))
    Switch(LVar9)
        CaseEq(-1)
            Return
        CaseEq(AVAR_Summon1)
            EVT_VEC3I_SET(LVarA, 5, 0, -20)
        CaseEq(AVAR_Summon2)
            EVT_VEC3I_SET(LVarA, 45, 0, -5)
    EndSwitch

    Call(SummonEnemy, Ref(N(SummonFormation)), FALSE)
    Set(LVarF, LVar0)
    Call(OverrideActorLevel, LVarF, 0)
    Set(LVarD, ANIM_Bandit_Run)
    Set(LVarE, ANIM_Bandit_Idle)

    Call(SetActorVar, ACTOR_SELF, LVar9, LVarF)
    Call(SetAnimation, LVarF, PRT_MAIN, LVarD)
    Call(SetActorSpeed, LVarF, Float(6))

    Call(SetActorPos, LVarF, 200, 0, 50)
    Call(SetGoalPos, LVarF, 60, 0, 50)
    Call(RunToGoal, LVarF, 0, 0)
    Call(SetGoalPos, LVarF, LVarA, LVarB, LVarC)
    Call(RunToGoal, LVarF, 0, 0)

    Call(SetAnimation, LVarF, PRT_MAIN, LVarE)
    Call(GetActorPos, LVarF, LVar0, LVar1, LVar2)
    Call(ForceHomePos, LVarF, LVar0, LVar1, LVar2)
    Call(HPBarToHome, LVarF)

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)

    Return
    End
};

EvtScript N(EVS_Tackle) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)

    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
    Call(GetBattlePhase, LVar0)

    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Run)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 50, 0, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
    Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Idle)
    Wait(6)

    Call(SetActorSpeed, ACTOR_SELF, Float(2.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Walk)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 35, 0, 0)
    Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BANDIT_STEAL)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_WALK, SOUND_NONE, SOUND_NONE)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Tackle)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, FALSE)
            Call(ResetAllActorSounds, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.5))
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
            Sub(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
            Wait(5)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(5)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Run)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, FALSE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Idle)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Tackle)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
    Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, DMG_LD_TACKLE, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            // dont steal coins if no damage, blocked
            Call(GetLastDamage, ACTOR_PLAYER, LVar0)
            IfLe(LVar0, 0)
                Set(LVar0, 1)
                Goto(10)
            EndIf
            Call(GetBattleFlags, LVar0)
            IfFlag(LVar0, BS_FLAGS1_ATK_BLOCKED)
                Set(LVar0, 1)
                Goto(10)
            EndIf
            // cant steal coins if shrunk
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfFlag(LVar0, STATUS_FLAG_SHRINK)
                Set(LVar0, 0)
                Goto(10)
            EndIf
             // cant steal coins if player is stone
            Call(GetStatusFlags, ACTOR_PLAYER, LVar0)
            IfFlag(LVar0, STATUS_FLAG_STONE)
                Set(LVar0, 0)
                Goto(10)
            EndIf

            // steal here

            Label(10)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 30)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, FALSE, TRUE, FALSE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Idle)
            Wait(20)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Bandit_Leader_Run)

            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(RunToGoal, ACTOR_SELF, 0, FALSE)
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_TakeTurn) = {
    ExecWait(N(EVS_Summon))

    ExecWait(N(EVS_Tackle))

    Return
    End
};

#undef PRT_MAIN
