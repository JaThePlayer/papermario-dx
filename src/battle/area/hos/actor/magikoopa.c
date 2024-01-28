#include "../area.h"
#include "sprite/npc/FlyingMagikoopa.h"
#include "sprite/npc/Magikoopa.h"

#define NAMESPACE A(magikoopa)

extern s32 N(GroundAnims)[];
extern s32 N(FlyingAnims)[];
extern s32 N(BroomAnims)[];

extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Flying_Init);
extern EvtScript N(EVS_HandleEvent);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_Flying_HandleEvent);

enum N(ActorPartIDs) {
    PRT_GROUND      = 1,
    PRT_FLYING      = 2,
    PRT_BROOM       = 3,
};

enum N(ActorVars) {
    AVAR_HitTypeFlags           = 0,
    AVAL_HitType_Combo          = 1,
    AVAR_IsClone                = 1,
    AVAR_ShouldKnockDown        = 2,
    AVAR_HealAllCount           = 3,
    AVAR_HealOneCount           = 4,
    AVAR_AttackBoostCount       = 5,
    AVAR_DefenseBoostCount      = 6,
    AVAR_ElectrifyCount         = 7,
    AVAR_TransparifyCount       = 8,
    AVAR_CloneActorID           = 9,
    AVAR_MadeCloneLastTurn      = 10,
    AVAR_LastMove               = 11,
    AVAR_TurnCounter            = 12,
    AVAL_LastMove_None          = -1,
    AVAL_LastMove_HealOne       = 0,
    AVAL_LastMove_HealAll       = 1,
    AVAL_LastMove_MagicBlast    = 2,
    AVAL_LastMove_MakeClone     = 3,
    AVAL_LastMove_BoostAttack   = 4,
    AVAL_LastMove_BoostDefense  = 5,
    AVAL_LastMove_Electrify     = 6,
    AVAL_LastMove_Transparify   = 7,
};

enum N(ActorParams) {
    MAX_HP              = 18,
    DMG_MAGIC_BLAST     = 2,
    HEAL_AMT_ONE        = 0,
    HEAL_AMT_ALL        = 0,
    ATTACK_BOOST_AMT    = 0,
    DEFENSE_BOOST_AMT   = 0,
    ELECTRIFY_TURNS     = 0,
    TRANSPARIFY_TURNS   = 0,
    MAX_HEAL_ONE        = 0,
    MAX_HEAL_ALL        = 0,
    MAX_ATTACK_BOOST    = 0,
    MAX_DEFENSE_BOOST   = 0,
    MAX_ELECTRIFY       = 0,
    MAX_TRANSPARIFY     = 0,
    HEAL_THRESHOLD_1    = 0,
    HEAL_THRESHOLD_2    = 0,
    HEAL_THRESHOLD_3    = 0,
    HEAL_CHANCE_1       = 0,
    HEAL_CHANCE_2       = 0,
    HEAL_CHANCE_3       = 0,
    HEAL_ONE_CHANCE     = 0,
    HEAL_ALL_CHANCE     = 0,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(FlyingDefense)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,             100,
    STATUS_KEY_POISON,             50,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,              50,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,             50,
    STATUS_KEY_PARALYZE,          100,
    STATUS_KEY_SHRINK,            100,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       0,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

s32 N(FlyingStatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,             100,
    STATUS_KEY_POISON,             50,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,             100,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,             50,
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
        .index = PRT_GROUND,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 2, 35 },
        .opacity = 255,
        .idleAnimations = N(GroundAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { -5, -12 },
    },
};

ActorPartBlueprint N(FlyingParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_GROUND,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 2, 35 },
        .opacity = 255,
        .idleAnimations = N(GroundAnims),
        .defenseTable = N(FlyingDefense),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { -5, -12 },
    },
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_FLYING,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -10, 35 },
        .opacity = 255,
        .idleAnimations = N(FlyingAnims),
        .defenseTable = N(FlyingDefense),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -8 },
    },
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_BROOM,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = N(BroomAnims),
        .defenseTable = N(FlyingDefense),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = 0,
    .type = ACTOR_TYPE_FLYING_MAGIKOOPA_BOSS,
    .level = ACTOR_LEVEL_MAGIKOOPA_BOSS,
    .maxHP = MAX_HP,
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
    .powerBounceChance = 100,
    .coinReward = 0,
    .size = { 36, 40 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint N(flying) = {
    .flags = ACTOR_FLAG_FLYING,
    .type = ACTOR_TYPE_FLYING_MAGIKOOPA_BOSS,
    .level = ACTOR_LEVEL_FLYING_MAGIKOOPA_BOSS,
    .maxHP = MAX_HP,
    .partCount = ARRAY_COUNT(N(FlyingParts)),
    .partsData = N(FlyingParts),
    .initScript = &N(EVS_Flying_Init),
    .statusTable = N(FlyingStatusTable),
    .escapeChance = 0,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 4,
    .powerBounceChance = 100,
    .coinReward = 0,
    .size = { 48, 40 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

s32 N(GroundAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_Magikoopa_Anim01,
    STATUS_KEY_STONE,     ANIM_Magikoopa_Anim00,
    STATUS_KEY_SLEEP,     ANIM_Magikoopa_Anim08,
    STATUS_KEY_POISON,    ANIM_Magikoopa_Anim01,
    STATUS_KEY_STOP,      ANIM_Magikoopa_Anim00,
    STATUS_KEY_STATIC,    ANIM_Magikoopa_Anim01,
    STATUS_KEY_PARALYZE,  ANIM_Magikoopa_Anim00,
    STATUS_KEY_DIZZY,     ANIM_Magikoopa_Anim07,
    STATUS_KEY_FEAR,      ANIM_Magikoopa_Anim07,
    STATUS_END,
};

s32 N(FlyingAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_FlyingMagikoopa_Anim01,
    STATUS_KEY_STONE,     ANIM_FlyingMagikoopa_Anim00,
    STATUS_KEY_SLEEP,     ANIM_FlyingMagikoopa_Anim08,
    STATUS_KEY_POISON,    ANIM_FlyingMagikoopa_Anim01,
    STATUS_KEY_STOP,      ANIM_FlyingMagikoopa_Anim00,
    STATUS_KEY_STATIC,    ANIM_FlyingMagikoopa_Anim01,
    STATUS_KEY_PARALYZE,  ANIM_FlyingMagikoopa_Anim00,
    STATUS_KEY_DIZZY,     ANIM_FlyingMagikoopa_Anim07,
    STATUS_KEY_FEAR,      ANIM_FlyingMagikoopa_Anim07,
    STATUS_END,
};

s32 N(BroomAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_FlyingMagikoopa_Anim0A,
    STATUS_END,
};

#include "common/ShrinkActor.inc.c"

EvtScript N(EVS_Idle) = {
    Return
    End
};

#include "common/GetSelectedMoveID.inc.c"

EvtScript N(EVS_KnockDownCheck) = {
    Call(GetBattleFlags, LVar0)
    Call(SetActorVar, ACTOR_SELF, AVAR_ShouldKnockDown, FALSE)
    Return
    End
};

#include "common/battle/SetAbsoluteStatusOffsets.inc.c"
#include "common/StartRumbleWithParams.inc.c"

EvtScript N(EVS_KnockDown) = {
    Return
    End
};

EvtScript N(EVS_Flee) = {
    Return
    End
};

EvtScript N(EVS_HandlePhase) = {
    Call(GetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
    BitwiseAndConst(LVar0, ~AVAL_HitType_Combo)
    Call(SetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
    Return
    End
};

EvtScript N(EVS_Init) = {
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    Call(BindHandlePhase, ACTOR_SELF, Ref(N(EVS_HandlePhase)))
    Call(GetBattleVar, BTL_VAR_Magikoopa_LastIndexBoosted, LVar0)
    IfEq(LVar0, 0)
        Call(SetBattleVar, BTL_VAR_Magikoopa_LastIndexBoosted, -1)
    EndIf
    Call(SetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_IsClone, FALSE)
    Call(SetActorVar, ACTOR_SELF, AVAR_ShouldKnockDown, FALSE)
    Call(SetActorVar, ACTOR_SELF, AVAR_HealAllCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_HealOneCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_AttackBoostCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_DefenseBoostCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_ElectrifyCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_TransparifyCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_CloneActorID, -1)
    Call(SetActorVar, ACTOR_SELF, AVAR_MadeCloneLastTurn, FALSE)
    Call(SetActorVar, ACTOR_SELF, AVAR_LastMove, AVAL_LastMove_None)
    Call(SetActorVar, ACTOR_SELF, AVAR_TurnCounter, 0)
    Return
    End
};

EvtScript N(EVS_Flying_Init) = {
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_Flying_HandleEvent)))
    Call(GetBattleVar, BTL_VAR_Magikoopa_LastIndexBoosted, LVar0)
    IfEq(LVar0, 0)
        Call(SetBattleVar, BTL_VAR_Magikoopa_LastIndexBoosted, -1)
    EndIf
    Call(SetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_IsClone, FALSE)
    Call(SetActorVar, ACTOR_SELF, AVAR_ShouldKnockDown, FALSE)
    Call(SetActorVar, ACTOR_SELF, AVAR_HealAllCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_HealOneCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_AttackBoostCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_DefenseBoostCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_ElectrifyCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_TransparifyCount, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_CloneActorID, -1)
    Call(SetActorVar, ACTOR_SELF, AVAR_MadeCloneLastTurn, FALSE)
    Call(SetActorVar, ACTOR_SELF, AVAR_LastMove, AVAL_LastMove_None)
    Return
    End
};

EvtScript N(EVS_RemoveClone) = {
    Call(GetActorVar, ACTOR_SELF, AVAR_IsClone, LVar0)
    IfEq(LVar0, 1)
        Return
    EndIf
    Call(GetActorVar, ACTOR_SELF, AVAR_CloneActorID, LVar0)
    Call(ActorExists, LVar0, LVar1)
    IfEq(LVar1, 0)
        Return
    EndIf
    Call(SetActorVar, ACTOR_SELF, AVAR_MadeCloneLastTurn, FALSE)
    Call(GetLastElement, LVarA)
    IfFlag(LVarA, DAMAGE_TYPE_MULTIPLE_POPUPS)
        Label(0)
        Call(GetBattleFlags, LVarA)
        IfFlag(LVarA, BS_FLAGS1_EXECUTING_MOVE)
            Wait(1)
            Goto(0)
        EndIf
    EndIf
    Call(GetActorVar, ACTOR_SELF, AVAR_CloneActorID, LVar0)
    Call(SetActorVar, ACTOR_SELF, AVAR_CloneActorID, -1)
    Call(GetActorFlags, ACTOR_SELF, LVarA)
    IfFlag(LVarA, ACTOR_FLAG_FLYING)
        Call(SetPartEventBits, ACTOR_SELF, PRT_FLYING, ACTOR_EVENT_FLAG_ATTACK_CHARGED, FALSE)
    Else
        Call(SetPartEventBits, ACTOR_SELF, PRT_GROUND, ACTOR_EVENT_FLAG_ATTACK_CHARGED, FALSE)
    EndIf
    Call(GetActorPos, LVar0, LVar1, LVar2, LVar3)
    Call(RemoveActor, LVar0)
    Thread
        Add(LVar3, 2)
        Loop(2)
            PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar1, LVar2, LVar3, 0, 0, 0, 0, 0)
            Wait(2)
        EndLoop
    EndThread
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetPartAlpha, ACTOR_SELF, PRT_GROUND, 255)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            Call(GetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
            BitwiseOrConst(LVar0, AVAL_HitType_Combo)
            Call(SetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
        CaseOrEq(EVENT_HIT)
        CaseOrEq(EVENT_UP_AND_AWAY)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_RemoveClone))
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim05)
            SetConst(LVar2, ANIM_Magikoopa_Anim06)
            ExecWait(EVS_Enemy_BurnHit)
            ExecWait(N(EVS_RemoveClone))
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim05)
            SetConst(LVar2, ANIM_Magikoopa_Anim06)
            ExecWait(EVS_Enemy_BurnHit)
            ExecWait(N(EVS_RemoveClone))
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim06)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_SpinSmashHit)
            ExecWait(N(EVS_RemoveClone))
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_SpinSmashHit)
            ExecWait(N(EVS_RemoveClone))
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_ZERO_DAMAGE)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim01)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseEq(EVENT_IMMUNE)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim01)
            ExecWait(EVS_Enemy_NoDamageHit)
            Call(GetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
            IfFlag(LVar0, AVAL_HitType_Combo)
                ExecWait(N(EVS_RemoveClone))
                Return
            EndIf
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNe(LVar0, 0)
                ExecWait(N(EVS_RemoveClone))
            EndIf
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_RemoveClone))
            Wait(10)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim01)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            ExecWait(N(EVS_RemoveClone))
            Call(SetAnimation, ACTOR_SELF, PRT_GROUND, ANIM_Magikoopa_Anim04)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.5))
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
            Wait(15)
            Call(SetAnimation, ACTOR_SELF, PRT_GROUND, ANIM_Magikoopa_Anim02)
            ExecWait(N(EVS_Flee))
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim02)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            ExecWait(N(EVS_RemoveClone))
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                IfFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(SetPartDispOffset, ACTOR_SELF, PRT_GROUND, 0, 2, 0)
                Else
                    Call(SetPartDispOffset, ACTOR_SELF, PRT_GROUND, -4, 5, 0)
                EndIf
            EndIf
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim01)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseEq(EVENT_STAR_BEAM)
            SetConst(LVar0, PRT_GROUND)
            SetConst(LVar1, ANIM_Magikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_RemoveClone))
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_Flying_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetPartAlpha, ACTOR_SELF, PRT_FLYING, 255)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            Call(GetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
            BitwiseOrConst(LVar0, AVAL_HitType_Combo)
            Call(SetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
            ExecWait(N(EVS_KnockDownCheck))
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
        CaseOrEq(EVENT_HIT)
        CaseOrEq(EVENT_FALL_TRIGGER)
            ExecWait(N(EVS_KnockDownCheck))
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_RemoveClone))
            ExecWait(N(EVS_KnockDown))
        EndCaseGroup
        CaseEq(EVENT_UP_AND_AWAY)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_RemoveClone))
        CaseOrEq(EVENT_BURN_HIT)
        CaseOrEq(EVENT_15)
            ExecWait(N(EVS_KnockDownCheck))
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim05)
            SetConst(LVar2, ANIM_FlyingMagikoopa_Anim06)
            ExecWait(EVS_Enemy_BurnHit)
            ExecWait(N(EVS_RemoveClone))
            ExecWait(N(EVS_KnockDown))
        EndCaseGroup
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim05)
            SetConst(LVar2, ANIM_FlyingMagikoopa_Anim06)
            ExecWait(EVS_Enemy_BurnHit)
            ExecWait(N(EVS_RemoveClone))
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim06)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_SpinSmashHit)
            ExecWait(N(EVS_RemoveClone))
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_SpinSmashHit)
            ExecWait(N(EVS_RemoveClone))
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_ZERO_DAMAGE)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim01)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseEq(EVENT_IMMUNE)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim01)
            ExecWait(EVS_Enemy_NoDamageHit)
            Call(GetActorVar, ACTOR_SELF, AVAR_HitTypeFlags, LVar0)
            IfFlag(LVar0, AVAL_HitType_Combo)
                ExecWait(N(EVS_RemoveClone))
                Return
            EndIf
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNe(LVar0, 0)
                ExecWait(N(EVS_RemoveClone))
            EndIf
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_RemoveClone))
            Wait(10)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_BEGIN_FIRST_STRIKE)
            Call(SetActorPos, ACTOR_SELF, 20, 0, 0)
            Call(HPBarToCurrent, ACTOR_SELF)
        CaseEq(EVENT_END_FIRST_STRIKE)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim01)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(4.0))
            Call(FlyToGoal, ACTOR_SELF, 0, 1, EASING_LINEAR)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim01)
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim01)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            ExecWait(N(EVS_RemoveClone))
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim04)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.5))
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
            Wait(15)
            Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim02)
            ExecWait(N(EVS_Flee))
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim02)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            ExecWait(N(EVS_RemoveClone))
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                IfFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(SetPartDispOffset, ACTOR_SELF, PRT_FLYING, -3, 0, 0)
                Else
                    Call(SetPartDispOffset, ACTOR_SELF, PRT_FLYING, -15, 0, 0)
                EndIf
            EndIf
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim01)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseEq(EVENT_STAR_BEAM)
            SetConst(LVar0, PRT_FLYING)
            SetConst(LVar1, ANIM_FlyingMagikoopa_Anim04)
            ExecWait(EVS_Enemy_Hit)
            ExecWait(N(EVS_RemoveClone))
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

#define BOLT_COUNT 3
#define whichHitsPartner LVarF
#define boltId LVarE
#define hasClone LFlag0
#define isFlying LFlag1
#define cloneId LVarA
#define thisBoltTarget LVarD
#define damagingBoltId LVarC

#define GET_BOLT_TARGET(forBoltId, outVar) \
        IfEq(forBoltId, whichHitsPartner) \
            SetConst(outVar, ACTOR_PARTNER) \
        Else \
            SetConst(outVar, ACTOR_PLAYER) \
        EndIf

#define GET_BOLT_DMG(forBoltId, outVar) \
        IfEq(forBoltId, whichHitsPartner) \
            SetConst(outVar, 1) \
        Else \
            SetConst(outVar, DMG_MAGIC_BLAST) \
        EndIf

EvtScript N(EVS_Attack_MagicBlast_One) = {
    IfEq(hasClone, TRUE)
        Call(PlaySoundAtActor, cloneId, SOUND_SPELL_CAST2)
    EndIf
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST2)

    IfEq(hasClone, TRUE)
        Call(SetAnimation, cloneId, 1, ANIM_FlyingMagikoopa_Anim03)
    EndIf
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim03)

    Wait(5)

    GET_BOLT_TARGET(boltId, thisBoltTarget)

    Call(GetActorPos, thisBoltTarget, LVar4, LVar5, LVar6)

    Set(LVar0, LVar4)
    Add(LVar0, 20)
    Set(LVar1, LVar5)
    Add(LVar1, 300)

    // ShrinkActor = ShootMagicBolt
    Call(N(ShrinkActor), LVar0, LVar1, LVar6, LVar4, LVar5, LVar6, 30)
    Wait(30)

    GET_BOLT_TARGET(boltId, thisBoltTarget)
    GET_BOLT_DMG(boltId, LVarB)
    Call(SetTargetActor, ACTOR_SELF, thisBoltTarget)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarB, DAMAGE_TYPE_MAGIC | DAMAGE_TYPE_NO_CONTACT, 0, 0, LVarB, BS_FLAGS1_TRIGGER_EVENTS)
    Return
    End
};

EvtScript N(EVS_Attack_MagicBlast) = {
    Call(SetActorVar, ACTOR_SELF, AVAR_LastMove, AVAL_LastMove_MagicBlast)
    Set(hasClone, FALSE)
    Set(isFlying, FALSE)
    Call(GetActorVar, ACTOR_SELF, AVAR_MadeCloneLastTurn, LVar0)
    IfEq(LVar0, 1)
        Set(hasClone, TRUE)
        Call(GetActorVar, ACTOR_SELF, AVAR_CloneActorID, cloneId)
    EndIf
    Call(GetActorFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, ACTOR_FLAG_FLYING)
        Set(isFlying, TRUE)
    EndIf
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    IfEq(hasClone, TRUE)
        Call(UseIdleAnimation, cloneId, FALSE)
    EndIf
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Call(MoveBattleCamOver, 1)
    Else
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Call(MoveBattleCamOver, 10)
    EndIf
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST1)

    // gather magic effect
    IfEq(hasClone, TRUE)
        Call(SetAnimation, cloneId, 1, ANIM_FlyingMagikoopa_Anim02)
        Call(GetActorPos, cloneId, LVar0, LVar1, LVar2)
        Sub(LVar0, 30)
        Add(LVar1, 36)
        PlayEffect(EFFECT_GATHER_MAGIC, 0, LVar0, LVar1, LVar2, Float(0.5), 30, 0)
    EndIf
    Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim02)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetStatusFlags, ACTOR_SELF, LVar3)
    IfFlag(LVar3, STATUS_FLAG_SHRINK)
        Sub(LVar0, 12)
        Add(LVar1, 14)
    Else
        Sub(LVar0, 30)
        Add(LVar1, 36)
    EndIf
    PlayEffect(EFFECT_GATHER_MAGIC, 0, LVar0, LVar1, LVar2, Float(0.5), 30, 0)

    Wait(30)

    Call(EnemyTestTarget, ACTOR_SELF, LVar9, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar9)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            IfEq(hasClone, TRUE)
                Call(PlaySoundAtActor, cloneId, SOUND_SPELL_CAST2)
            EndIf
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST2)
            IfEq(isFlying, FALSE)
                IfEq(hasClone, TRUE)
                    Call(SetAnimation, cloneId, 1, ANIM_Magikoopa_Anim03)
                EndIf
                Call(SetAnimation, ACTOR_SELF, PRT_GROUND, ANIM_Magikoopa_Anim03)
            Else
                IfEq(hasClone, TRUE)
                    Call(SetAnimation, cloneId, 1, ANIM_FlyingMagikoopa_Anim03)
                EndIf
                Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim03)
            EndIf
            Wait(5)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
            Sub(LVar4, 50)
            Set(LVar5, 0)
            IfEq(isFlying, FALSE)
                IfEq(hasClone, TRUE)
                    Call(GetActorPos, cloneId, LVar0, LVar1, LVar2)
                    Sub(LVar0, 20)
                    Add(LVar1, 20)
                    Call(N(ShrinkActor), LVar0, LVar1, LVar6, LVar4, LVar5, LVar6, 30)
                EndIf
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(GetStatusFlags, ACTOR_SELF, LVar3)
                IfFlag(LVar3, STATUS_FLAG_SHRINK)
                    Sub(LVar0, 8)
                    Add(LVar1, 8)
                Else
                    Sub(LVar0, 20)
                    Add(LVar1, 20)
                EndIf
                Call(N(ShrinkActor), LVar0, LVar1, LVar6, LVar4, LVar5, LVar6, 30)
            Else
                IfEq(hasClone, TRUE)
                    Call(GetActorPos, cloneId, LVar0, LVar1, LVar2)
                    Sub(LVar0, 20)
                    Add(LVar1, 20)
                    Call(N(ShrinkActor), LVar0, LVar1, LVar6, LVar4, LVar5, LVar6, 30)
                EndIf
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(GetStatusFlags, ACTOR_SELF, LVar3)
                IfFlag(LVar3, STATUS_FLAG_SHRINK)
                    Sub(LVar0, 8)
                    Add(LVar1, 8)
                Else
                    Sub(LVar0, 20)
                    Add(LVar1, 20)
                EndIf
                Call(N(ShrinkActor), LVar0, LVar1, LVar6, LVar4, LVar5, LVar6, 30)
            EndIf
            Wait(50)
            IfEq(LVar9, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(15)
            Call(YieldTurn)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            IfEq(hasClone, TRUE)
                Call(UseIdleAnimation, cloneId, TRUE)
            EndIf
            Return
        EndCaseGroup
    EndSwitch

    Call(RandInt, BOLT_COUNT - 1, whichHitsPartner)

    // Fire magic bolts
    Set(boltId, 0)
    Exec(N(EVS_Attack_MagicBlast_One))
    Wait(20)

    Set(boltId, 1)
    Exec(N(EVS_Attack_MagicBlast_One))
    Wait(20)

    Set(boltId, 2)
    Exec(N(EVS_Attack_MagicBlast_One))
    Wait(30)

    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    IfEq(hasClone, TRUE)
        Call(UseIdleAnimation, cloneId, TRUE)
    EndIf
    Return
    End

};
#undef BOLT_COUNT
#undef whichHitsPartner
#undef hasClone
#undef isFlying
#undef cloneId
#undef thisBoltTarget

EvtScript N(EVS_GetAvailableColumn) = {
    Call(CreateHomeTargetList, TARGET_FLAG_2 | TARGET_FLAG_PRIMARY_ONLY)
    Call(InitTargetIterator)
    Label(0)
        Call(GetOwnerTarget, LVar0, LVar1)
        Call(GetIndexFromHome, LVar0, LVar1)
        Mod(LVar1, 4)
        Switch(LVar1)
            CaseEq(0)
                Set(LFlagB, TRUE)
            CaseEq(1)
                Set(LFlagC, TRUE)
            CaseEq(2)
                Set(LFlagD, TRUE)
            CaseEq(3)
                Set(LFlagE, TRUE)
        EndSwitch
        Call(ChooseNextTarget, ITER_NEXT, LVar0)
        IfNe(LVar0, ITER_NO_MORE)
            Goto(0)
        EndIf
    Set(LVar0, -1)
    Call(GetIndexFromHome, ACTOR_SELF, LVar1)
    Mod(LVar1, 4)
    Switch(LVar1)
        CaseEq(0)
            IfEq(LFlagC, FALSE)
                Set(LVar0, 1)
            EndIf
        CaseEq(1)
            IfEq(LFlagB, FALSE)
                Set(LVar0, 0)
            EndIf
            IfEq(LFlagD, FALSE)
                Set(LVar0, 2)
            EndIf
        CaseEq(2)
            IfEq(LFlagC, FALSE)
                Set(LVar0, 1)
            EndIf
            IfEq(LFlagE, FALSE)
                Set(LVar0, 3)
            EndIf
        CaseEq(3)
            IfEq(LFlagD, FALSE)
                Set(LVar0, 2)
            EndIf
    EndSwitch
    IfNe(LVar0, -1)
        Call(GetActorFlags, ACTOR_SELF, LVar1)
        IfFlag(LVar1, ACTOR_FLAG_FLYING)
            Add(LVar0, 4)
        EndIf
    EndIf
    Return
    End
};

EvtScript N(EVS_Clone_Explode) = {
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

EvtScript N(EVS_Clone_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            Return
        CaseOrEq(EVENT_HIT)
        CaseOrEq(EVENT_BURN_HIT)
        CaseOrEq(EVENT_BURN_DEATH)
        CaseOrEq(EVENT_SPIN_SMASH_HIT)
        CaseOrEq(EVENT_SPIN_SMASH_DEATH)
        CaseOrEq(EVENT_EXPLODE_TRIGGER)
            ExecWait(N(EVS_Clone_Explode))
            Return
        EndCaseGroup
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            ExecWait(N(EVS_Clone_Explode))
            Return
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            ExecWait(N(EVS_Clone_Explode))
            Return
        CaseOrEq(EVENT_SHOCK_HIT)
        CaseOrEq(EVENT_SHOCK_DEATH)
            ExecWait(N(EVS_Clone_Explode))
            Return
        EndCaseGroup
        CaseEq(EVENT_RECOVER_STATUS)
            Return
        CaseEq(EVENT_SCARE_AWAY)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            Return
        CaseEq(EVENT_BLOW_AWAY)
            Return
        CaseEq(EVENT_UP_AND_AWAY)
            Return
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_Clone_Init) = {
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_Clone_HandleEvent)))
    Call(SetActorVar, ACTOR_SELF, AVAR_IsClone, TRUE)
    Return
    End
};

EvtScript N(EVS_FlyingClone_Init) = {
    Call(SetPartEventBits, ACTOR_SELF, 1, ACTOR_EVENT_FLAG_EXPLODE_ON_CONTACT, TRUE)
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_Clone_HandleEvent)))
    Call(SetActorVar, ACTOR_SELF, AVAR_IsClone, TRUE)
    Return
    End
};

s32 N(CloneDefense)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(FlyingCloneDefense)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(CloneStatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,               0,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,               0,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,            0,
    STATUS_KEY_SHRINK,              0,
    STATUS_KEY_STOP,                0,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       0,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

s32 N(FlyingCloneStatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,               0,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,               0,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,            0,
    STATUS_KEY_SHRINK,              0,
    STATUS_KEY_STOP,                0,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       0,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

ActorPartBlueprint N(CloneParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_GROUND,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 2, 35 },
        .opacity = 255,
        .idleAnimations = N(GroundAnims),
        .defenseTable = N(CloneDefense),
        .eventFlags = ACTOR_EVENT_FLAG_EXPLODE_ON_CONTACT,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { -5, -12 },
    },
};

ActorPartBlueprint N(FlyingCloneParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET | ACTOR_PART_FLAG_DAMAGE_IMMUNE,
        .index = PRT_GROUND,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -10, 35 },
        .opacity = 255,
        .idleAnimations = N(FlyingAnims),
        .defenseTable = N(FlyingCloneDefense),
        .eventFlags = ACTOR_EVENT_FLAG_EXPLODE_ON_CONTACT,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -8 },
        .contactDamage = 6,
        .partnerContactDamage = 2,
    },
};

ActorBlueprint N(clone) = {
    .flags = ACTOR_FLAG_NO_ATTACK,
    .type = ACTOR_TYPE_FLYING_MAGIKOOPA_BOSS,
    .level = 0,
    .maxHP = MAX_HP,
    .partCount = ARRAY_COUNT(N(CloneParts)),
    .partsData = N(CloneParts),
    .initScript = &N(EVS_Clone_Init),
    .statusTable = N(CloneStatusTable),
    .escapeChance = 40,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 0,
    .powerBounceChance = 75,
    .coinReward = 0,
    .size = { 36, 40 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 32 },
};

ActorBlueprint N(clone_flying) = {
    .flags = ACTOR_FLAG_FLYING | ACTOR_FLAG_NO_ATTACK,
    .type = ACTOR_TYPE_FLYING_MAGIKOOPA_BOSS,
    .level = 0,
    .maxHP = MAX_HP,
    .partCount = ARRAY_COUNT(N(FlyingCloneParts)),
    .partsData = N(FlyingCloneParts),
    .initScript = &N(EVS_FlyingClone_Init),
    .statusTable = N(FlyingCloneStatusTable),
    .escapeChance = 40,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 0,
    .powerBounceChance = 75,
    .coinReward = 2,
    .size = { 48, 40 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -25, 20 },
    .statusTextOffset = { 1, 34 },
};

Vec3i N(SummonPos) = { NPC_DISPOSE_LOCATION };

Formation N(CloneFormation) = {
    ACTOR_BY_POS(N(clone), N(SummonPos), 0),
};

Formation N(FlyingCloneFormation) = {
    ACTOR_BY_POS(N(clone_flying), N(SummonPos), 0),
};

EvtScript N(EVS_Move_MakeClone) = {
    Call(SetActorVar, ACTOR_SELF, AVAR_MadeCloneLastTurn, TRUE)
    Call(SetActorVar, ACTOR_SELF, AVAR_LastMove, AVAL_LastMove_MakeClone)
    Set(LFlag0, FALSE)
    Call(GetActorFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, ACTOR_FLAG_FLYING)
        Set(LFlag0, TRUE)
    EndIf
    IfEq(LFlag0, FALSE)
        Call(SummonEnemy, Ref(N(CloneFormation)), FALSE)
    Else
        Call(SummonEnemy, Ref(N(FlyingCloneFormation)), FALSE)
    EndIf
    Call(SetActorVar, ACTOR_SELF, AVAR_CloneActorID, LVar0)
    Set(LVarA, LVar0)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(EnableIdleScript, LVarA, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(UseIdleAnimation, LVarA, FALSE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_PRESET_14)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(MoveBattleCamOver, 25)
    Wait(25)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST3)
    IfEq(LFlag0, FALSE)
        Call(SetAnimation, ACTOR_SELF, PRT_GROUND, ANIM_Magikoopa_Anim02)
        Call(SetAnimation, LVarA, 1, ANIM_Magikoopa_Anim02)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 23)
    Else
        Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim02)
        Call(SetAnimation, LVarA, 1, ANIM_FlyingMagikoopa_Anim02)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 23)
    EndIf
    PlayEffect(EFFECT_ENERGY_IN_OUT, 1, LVar0, LVar1, LVar2, Float(1.0), 30, 0)
    Wait(10)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(EnableIdleScript, LVar9, IDLE_SCRIPT_ENABLE)
    Wait(30)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar2, -3)
    Call(SetActorPos, LVarA, LVar0, LVar1, LVar2)
    Set(LVar9, LVarA)
    Wait(15)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 30)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST4)
    Call(PlaySoundAtActor, LVar9, SOUND_SPELL_CAST4)
    Call(SetPartFlagBits, LVar9, 1, ACTOR_PART_FLAG_PRIMARY_TARGET, FALSE)
    Call(RandInt, 1000, LVar0)
    IfLt(LVar0, 500)
        Call(SetActorSpeed, ACTOR_SELF, Float(2.0))
        ExecWait(N(EVS_GetAvailableColumn))
        Call(SetGoalToIndex, ACTOR_SELF, LVar0)
        Call(FlyToGoal, ACTOR_SELF, 0, 0, EASING_LINEAR)
    Else
        Call(SetActorSpeed, LVar9, Float(2.0))
        ExecWait(N(EVS_GetAvailableColumn))
        Call(SetGoalToIndex, LVar9, LVar0)
        Call(FlyToGoal, LVar9, 0, 0, EASING_LINEAR)
    EndIf
    Call(SetPartFlagBits, LVar9, 1, ACTOR_PART_FLAG_PRIMARY_TARGET, TRUE)
    Call(GetActorHP, ACTOR_SELF, LVar0)
    Call(SetEnemyHP, LVar9, LVar0)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(ForceHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(HPBarToHome, ACTOR_SELF)
    Call(GetActorPos, LVar9, LVar0, LVar1, LVar2)
    Call(ForceHomePos, LVar9, LVar0, LVar1, LVar2)
    Call(HPBarToHome, LVar9)
    IfEq(LFlag0, FALSE)
        Call(SetPartEventBits, ACTOR_SELF, PRT_GROUND, ACTOR_EVENT_FLAG_ATTACK_CHARGED, TRUE)
    Else
        Call(SetPartEventBits, ACTOR_SELF, PRT_FLYING, ACTOR_EVENT_FLAG_ATTACK_CHARGED, TRUE)
    EndIf
    IfEq(LFlag0, FALSE)
        Call(SetAnimation, ACTOR_SELF, PRT_GROUND, ANIM_Magikoopa_Anim01)
        Call(SetAnimation, LVar9, 1, ANIM_Magikoopa_Anim01)
    Else
        Call(SetAnimation, ACTOR_SELF, PRT_FLYING, ANIM_FlyingMagikoopa_Anim01)
        Call(SetAnimation, LVar9, 1, ANIM_FlyingMagikoopa_Anim01)
    EndIf
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Call(UseIdleAnimation, LVar9, TRUE)
    Return
    End
};

#include "common/battle/CheckMagikoopaCastTarget.inc.c"

EvtScript N(EVS_TakeTurn) = {
    #define VarCloneExists LVar1

    //Call(GetActorVar, ACTOR_SELF, AVAR_CloneActorID, LVar0)
    //Call(ActorExists, LVar0, VarCloneExists)
    Call(GetActorVar, ACTOR_SELF, AVAR_MadeCloneLastTurn, VarCloneExists)

    Call(GetActorVar, ACTOR_SELF, AVAR_TurnCounter, LVar0)
    Mod(LVar0, 3)

    Set(LVar2, LVar0)
    Add(LVar2, 1)
    Call(SetActorVar, ACTOR_SELF, AVAR_TurnCounter, LVar2)

    Switch(LVar0)
        CaseEq(1) // turn 2
            IfEq(VarCloneExists, TRUE)
                ExecWait(N(EVS_Attack_MagicBlast))
            Else
                ExecWait(N(EVS_Move_MakeClone))
            EndIf
        CaseDefault
            ExecWait(N(EVS_Attack_MagicBlast))
    EndSwitch

    Return
    End
};
