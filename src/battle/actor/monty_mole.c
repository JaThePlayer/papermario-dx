#include "PR/ultratypes.h"
#include "battle/battle.h"
#include "battle/interfaces/ICanBeUnflipped.h"
#include "common_structs.h"
#include "enums.h"
#include "macros.h"
#include "misc_patches/actor_interfaces.h"
#include "misc_patches/battle_api_ext.h"
#include "script_api/battle.h"
#include "script_api/macros.h"
#include "sprite/npc/MontyMole.h"

#define NAMESPACE A(monty_mole)

#include "common/TeamAttacks.inc.c"

extern s32 N(DefaultAnims)[];
extern s32 N(RockAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_ROCK        = 3,
    PRT_ROCK_2      = 4,
};

enum N(ActorVars) {
    AVAR_IN_Hole_Small  = 0,
};

enum N(ActorParams) {
    DMG_FRIENDLY_TOSS  = 1,
    DMG_TOSS           = 1,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              90,
    STATUS_KEY_POISON,             80,
    STATUS_KEY_FROZEN,            100,
    STATUS_KEY_DIZZY,              90,
    STATUS_KEY_UNUSED,              0,
    STATUS_KEY_STATIC,             10,
    STATUS_KEY_PARALYZE,          100,
    STATUS_KEY_SHRINK,             90,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,         -1,
    STATUS_TURN_MOD_UNUSED,         0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       1,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

ActorPartBlueprint N(ActorParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -2, 16 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -7 },
    },
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_PRIMARY_TARGET | ACTOR_PART_FLAG_SKIP_MOVEMENT_ALLOC,
        .index = PRT_TARGET,
        .posOffset = { 0, 0, -15 },
        .targetOffset = { -2, 12 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -7 },
    },
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_ROCK,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = N(RockAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_ROCK_2,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = N(RockAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
};

export ActorBlueprint blueprint = {
    .flags = 0,
    .type = ACTOR_TYPE_MONTY_MOLE,
    .level = ACTOR_LEVEL_MONTY_MOLE,
    .maxHP = 3,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 80,
    .airLiftChance = 70,
    .hurricaneChance = 50,
    .spookChance = 85,
    .upAndAwayChance = 95,
    .spinSmashReq = 4,
    .powerBounceChance = 100,
    .coinReward = 1,
    .size = { 34, 18 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
    .spPool = CURRENT_SP_POOL,
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_MontyMole_Idle,
    STATUS_KEY_STONE,     ANIM_MontyMole_Still,
    STATUS_KEY_SLEEP,     ANIM_MontyMole_Sleep,
    STATUS_KEY_POISON,    ANIM_MontyMole_Idle,
    STATUS_KEY_STOP,      ANIM_MontyMole_Still,
    STATUS_KEY_STATIC,    ANIM_MontyMole_Idle,
    STATUS_KEY_PARALYZE,  ANIM_MontyMole_Still,
    STATUS_KEY_DIZZY,     ANIM_MontyMole_Sleep,
    STATUS_KEY_UNUSED,    ANIM_MontyMole_Sleep,
    STATUS_END,
};

s32 N(RockAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_MontyMole_Rock,
    STATUS_END,
};

s32 N(EmptyHoleAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_MontyMole_Hole,
    STATUS_END,
};

// unused
s32 N(LeapAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_MontyMole_HurtJump,
    STATUS_END,
};

ActorPartBlueprint N(HoleParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_DAMAGE_IMMUNE | ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = N(EmptyHoleAnims),
        .defenseTable = nullptr,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
};

EvtScript N(EVS_Hole_Init) = {
    Call(GetActorVar, ACTOR_SELF, AVAR_IN_Hole_Small, LVar0)
    IfEq(LVar0, true)
        Call(SetPartScale, ACTOR_SELF, PRT_MAIN, Float(0.4), Float(0.4), Float(0.4))
    EndIf
    Return
    End
};

ActorBlueprint N(hole) = {
    .flags = ACTOR_FLAG_NO_SHADOW | ACTOR_FLAG_NO_HEALTH_BAR | ACTOR_FLAG_NO_ATTACK | ACTOR_FLAG_NO_DMG_APPLY,
    .type = ACTOR_TYPE_MONTY_HOLE,
    .level = ACTOR_LEVEL_MONTY_HOLE,
    .maxHP = 5,
    .partCount = ARRAY_COUNT(N(HoleParts)),
    .partsData = N(HoleParts),
    .initScript = &N(EVS_Hole_Init),
    .statusTable = nullptr,
    .escapeChance = 80,
    .airLiftChance = 0,
    .hurricaneChance = 80,
    .spookChance = 80,
    .upAndAwayChance = 90,
    .spinSmashReq = 4,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 32, 18 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
    .spPool = CURRENT_SP_POOL,
};

Vec3i N(SummonPos) = { NPC_DISPOSE_LOCATION };

Formation N(HoleFormation) = {
    ACTOR_BY_POS(N(hole), N(SummonPos), 0, false),
};

Formation N(SmallHoleFormation) = {
    ACTOR_BY_POS(N(hole), N(SummonPos), 0, true),
};

EvtScript N(EVS_CreateEmptyHole) = {
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, STATUS_FLAG_SHRINK)
        Call(SummonEnemy, Ref(N(SmallHoleFormation)), false)
    Else
        Call(SummonEnemy, Ref(N(HoleFormation)), false)
    EndIf
    Call(GetActorPos, ACTOR_SELF, LVar1, LVar2, LVar3)
    Sub(LVar3, 1)
    Call(SetActorPos, LVar0, LVar1, LVar2, LVar3)
    Return
    End
};

EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastElement, LVarE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            IfFlag(LVarE, DAMAGE_TYPE_SMASH)
                ChildThread
                    Wait(8)
                    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtMidair)
                EndChildThread
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_MontyMole_HurtJump)
                ExecWait(EVS_Enemy_Hit)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtDropDown)
                Wait(10)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_MontyMole_Hurt)
                ExecWait(EVS_Enemy_Hit)
            EndIf
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_MontyMole_BurnHurt)
            SetConst(LVar2, ANIM_MontyMole_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_MontyMole_BurnHurt)
            SetConst(LVar2, ANIM_MontyMole_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            ExecWait(N(EVS_CreateEmptyHole))
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_MontyMole_BurnHurtJump)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
            Call(GetLastElement, LVar0)
            IfFlag(LVar0, DAMAGE_TYPE_SMASH)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
                Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Burrow)
                Wait(80)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
                Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Emerge)
                Wait(20)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Sleep)
                Wait(20)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_MontyMole_Idle)
                ExecWait(EVS_Enemy_NoDamageHit)
            EndIf
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_MontyMole_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtJump)
            Wait(10)
            ExecWait(N(EVS_CreateEmptyHole))
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_MontyMole_Panic)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtJump)
            Wait(8)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtMidair)
            Wait(5)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtDropDown)
            Wait(8)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtJump)
            Wait(15)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtMidair)
            Wait(15)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_HurtDropDown)
            Wait(15)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_TARGET_ONLY | ACTOR_FLAG_NO_DMG_APPLY, true)
            Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(EmptyHoleAnims)))
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_MontyMole_Hurt)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            ExecWait(N(EVS_CreateEmptyHole))
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                Call(SetActorDispOffset, ACTOR_SELF, 0, -5, 0)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Stand)
            Else
                IfFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(SetActorDispOffset, ACTOR_SELF, 0, -9, 0)
                Else
                    Call(SetActorDispOffset, ACTOR_SELF, 0, -23, 0)
                EndIf
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Panic)
            EndIf
            Wait(1000)
            Return
        CaseEq(EVENT_AIR_LIFT_FAILED)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Burrow)
            Wait(40)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Emerge)
            Wait(20)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Sleep)
            Wait(20)
        CaseEq(EVENT_UP_AND_AWAY)
            ExecWait(N(EVS_CreateEmptyHole))
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

#define LFlag1_UsedExtraAction LFlag1

EvtScript N(TryUnflipCleft) = {
    #define LVarF_Enemy LVarF
    #define LVarD_NextEnemy LVarD
    #define LVarC_BestScore LVarC
    #define LVar6_NextScore LVar6
    #define LABEL_END_SCORE_CALC 2
    #define LVarB_FoundOnEnemyHitEvent LVarB
    #define LVarA_Damage LVarA
    Set(LVarB_FoundOnEnemyHitEvent, 0)
    FIND_BEST_ENEMY(LVarF_Enemy, LVarC_BestScore, LVarD_NextEnemy, LVar6_NextScore, 1, LABEL_END_SCORE_CALC, TARGET_FLAG_PRIMARY_ONLY,
        DoesActorImplement(LVarD_NextEnemy, ICanBeUnflipped, LVarE)
        IfFalse(LVarE)
            Goto(LABEL_END_SCORE_CALC)
        EndIf

        ExecWaitInterface(LVarD_NextEnemy, ICanBeUnflipped, canBeUnflippedScript)
        IfEq(LVarE, 0)
            Goto(LABEL_END_SCORE_CALC)
        EndIf

        // score = maxHp - hp + 1 (prioritize almost dead enemies, we want to resque them!)
        Call(GetActorMaxHP, LVarD_NextEnemy, LVar6_NextScore)
        Call(GetActorHP, LVarD_NextEnemy, LVarE)
        Sub(LVar6_NextScore, LVarE)
        Add(LVar6_NextScore, 1)
    )

    IfEq(LVarF_Enemy, 0)
        Return
    EndIf

    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
    Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Burrow)
    Wait(20)

    Call(GetActorPos, LVarF_Enemy, LVar0, LVar1, LVar2)
    Add(LVar2, 10)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorSpeed, ACTOR_SELF, Float(18.0))
    Call(RunToGoal, ACTOR_SELF, 0, false)

    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
    Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Emerge)
    Wait(2)

    Thread
        ExecWaitInterface(LVarF_Enemy, ICanBeUnflipped, getUpImmediatelyScript)
    EndThread
    Wait(10)

    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
    Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Burrow)
    Wait(20)

    Call(SetGoalToHome, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(18.0))
    Call(RunToGoal, ACTOR_SELF, 0, false)

    Return
    End
};

// in: LVar9 - part
EvtScript N(ThrowRock) = {
    // Try to throw rocks at enemies to proc on-enemy-hit events.
    #define LVarF_Enemy LVarF
    #define LVarD_NextEnemy LVarD
    #define LVarC_BestScore LVarC
    #define LVar6_NextScore LVar6
    #define LABEL_END_SCORE_CALC 2
    #define LVarB_FoundOnEnemyHitEvent LVarB
    #define LVarA_Damage LVarA
    Set(LVarB_FoundOnEnemyHitEvent, 0)
    FIND_BEST_ENEMY(LVarF_Enemy, LVarC_BestScore, LVarD_NextEnemy, LVar6_NextScore, 1, LABEL_END_SCORE_CALC, TARGET_FLAG_PRIMARY_ONLY,
        DoesActorImplement(LVarD_NextEnemy, IHasBeneficialOnEnemyDamagedEvent, LVarE)
        IfTrue(LVarE)
            Add(LVarB_FoundOnEnemyHitEvent, 1)
        EndIf

        // Skip enemies with defense that makes the attack do nothing.
        Set(LVar1, DMG_FRIENDLY_TOSS)
        Call(GetActorDefense, LVarD_NextEnemy, ELEMENT_NORMAL, LVar0)
        IfGe(LVar0, LVar1)
            Goto(LABEL_END_SCORE_CALC)
        EndIf
        Sub(LVar1, LVar0)

        // score = hp (deprioritize almost dead enemies, we don't want to kill them!)
        Call(GetActorHP, LVarD_NextEnemy, LVarE)
        IfLe(LVarE, LVar1)
            Goto(LABEL_END_SCORE_CALC)
        EndIf
        Set(LVar6_NextScore, LVarE)

        // Prioritize enemies with defense that can tank the hit.
        Mul(LVar0, 10)
        Add(LVar6_NextScore, LVarE)
    )

    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Set(LVarA_Damage, DMG_TOSS)
    IfGt(LVarB_FoundOnEnemyHitEvent, 0)
        IfNe(LVarF_Enemy, 0)
            Call(SetTargetActor, ACTOR_SELF, LVarF_Enemy)
            Set(LVarA_Damage, DMG_FRIENDLY_TOSS)

            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(GetActorPos, LVarF_Enemy, LVar3, LVar4, LVar5)
            IfLe(LVar0, LVar3)
                Call(SetActorYaw, ACTOR_SELF, 180)
            EndIf
        EndIf
    EndIf
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, 20)
    Call(SetPartPos, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2)
    Wait(1)
    Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, STATUS_FLAG_SHRINK)
        Call(SetPartScale, ACTOR_SELF, LVar9, Float(0.4), Float(0.4), Float(0.4))
    Else
        Call(SetPartScale, ACTOR_SELF, LVar9, Float(1.0), Float(1.0), Float(1.0))
    EndIf
    Call(PlaySoundAtPart, ACTOR_SELF, LVar9, SOUND_MOLE_THROW)
    Call(SetPartSounds, ACTOR_SELF, LVar9, ACTOR_SOUND_FLY, SOUND_NONE, SOUND_NONE)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, LVarA_Damage, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 100)
            Set(LVar1, -5)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVar9, Float(12.0))
            Call(SetPartJumpGravity, ACTOR_SELF, LVar9, Float(0.1))
            Call(SetAnimation, ACTOR_SELF, LVar9, ANIM_MontyMole_Rock)
            Call(FlyPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 0, 30, EASING_LINEAR)
            Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, true)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, LVar9, Float(12.0))
    Call(SetPartJumpGravity, ACTOR_SELF, LVar9, Float(0.1))
    Call(SetAnimation, ACTOR_SELF, LVar9, ANIM_MontyMole_Rock)
    Call(FlyPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 0, 20, EASING_LINEAR)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, 0, LVarA_Damage, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 55)
            Set(LVar1, 0)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVar9, Float(6.0))
            Call(SetPartJumpGravity, ACTOR_SELF, LVar9, Float(0.1))
            Call(FlyPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 0, 25, EASING_LINEAR)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVar9, Float(4.0))
            Sub(LVar0, 20)
            Call(FlyPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 0, 21, EASING_LINEAR)
            Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, true)
        EndCaseGroup
    EndSwitch
    Return
    End

    #undef LVarF_Enemy
    #undef LVarD_NextEnemy
    #undef LVarC_BestScore
    #undef LVar6_NextScore
    #undef LABEL_END_SCORE_CALC
    #undef LVarB_FoundOnEnemyHitEvent
};

s32 N(MoleTypes)[] = {
    ACTOR_TYPE_MONTY_MOLE,
    ACTOR_TYPE_MONTY_MOLE_BOSS,
    0,
};

EvtScript N(EVS_TakeTurn) = {
    #define LABEL_END 0

    Set(LFlag1_UsedExtraAction, false)
    STANDARD_ITEM_USE_AI_CODE_IF_USED(
        Set(LFlag1_UsedExtraAction, true)
    )
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)

    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Call(MoveBattleCamOver, 1)
    EndIf

    ExecWait(N(TryUnflipCleft))

    ChildThread
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
        Wait(20)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
    EndChildThread
    IfNe(LVarF, 0)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
        Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
        // If we've flipped a cleft, that uses up one of our actions this turn.
        IfTrue(LFlag1_UsedExtraAction)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Emerge)
            Wait(20)
            Goto(LABEL_END)
        EndIf

        // We're currently burrowed, new animation to unborrow directly into throwing.
        Set(LFlag1_UsedExtraAction, true)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_EmergeThrow)
        Wait(25)
    Else
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_ThrowAttack)
        Wait(37 - 16)
    EndIf

    Set(LVar9, PRT_ROCK)
    Exec(N(ThrowRock))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Idle)
    Call(SetActorYaw, ACTOR_SELF, 0)
    Wait(2)

    // Check for team-attack
    //Call(N(NextEnemyIfTypeIsOneOf), Ref(N(MoleTypes)), LVarF)
    //IfNe(LVarF, nullptr)
    // TODO: breaks if the enemy we're team-attacking with just got hit by friendly fire... (The enemy skips their turn)
    //    Call(YieldTurn)
    //EndIf

    IfEq(LFlag1_UsedExtraAction, false)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_AnimSecondThrow)
        Wait(7)
        Set(LVar9, PRT_ROCK_2)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_MontyMole_Idle)
        ExecWait(N(ThrowRock))
        Call(SetActorYaw, ACTOR_SELF, 0)
    EndIf

    // Let the damage animation run a bit before ending turn, otherwise Mario's turn animation will get stuck and it looks weird.
    Wait(7)

    Label(LABEL_END)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
    #undef LABEL_END
};

#undef LFlag1_UsedExtraAction
