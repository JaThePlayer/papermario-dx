#include "../area.h"
#include "sprite/npc/GoombaBros.h"
#include "goomba_bros_common.h"

#define NAMESPACE A(red_goomba_1)

extern s32 N(DefaultAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);
extern EvtScript N(EVS_GoombaBros_Death);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
    PRT_ZERO        = 0,
};

enum N(ActorVars) {
    AVAR_Unused_0   = 0,
    AVAR_Unused_1   = 1,
    AVAR_TurnCount  = 2,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,             100,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,             100,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
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

ActorPartBlueprint N(ActorParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 24 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -3 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = 0,
    .type = ACTOR_TYPE_RED_GOOMBA_BOSS,
    .level = ACTOR_LEVEL_RED_GOOMBA_BOSS,
    .maxHP = 9,
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
    .size = { 32, 32 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 8, 25 },
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_GoombaBros_Red_Idle,
    STATUS_KEY_STONE,     ANIM_GoombaBros_Red_Still,
    STATUS_KEY_SLEEP,     ANIM_GoombaBros_Red_Sleep,
    STATUS_KEY_POISON,    ANIM_GoombaBros_Red_Idle,
    STATUS_KEY_STOP,      ANIM_GoombaBros_Red_Still,
    STATUS_KEY_STATIC,    ANIM_GoombaBros_Red_Idle,
    STATUS_KEY_PARALYZE,  ANIM_GoombaBros_Red_Still,
    STATUS_KEY_DIZZY,     ANIM_GoombaBros_Red_Dizzy,
    STATUS_KEY_FEAR,      ANIM_GoombaBros_Red_Dizzy,
    STATUS_END,
};

s32 N(IdleAnimations_8021B810)[] = {
    STATUS_KEY_NORMAL,    ANIM_GoombaBros_Red_Run,
    STATUS_KEY_STONE,     ANIM_GoombaBros_Red_Still,
    STATUS_KEY_SLEEP,     ANIM_GoombaBros_Red_Sleep,
    STATUS_KEY_POISON,    ANIM_GoombaBros_Red_Idle,
    STATUS_KEY_STOP,      ANIM_GoombaBros_Red_Still,
    STATUS_KEY_STATIC,    ANIM_GoombaBros_Red_Run,
    STATUS_KEY_PARALYZE,  ANIM_GoombaBros_Red_Still,
    STATUS_KEY_DIZZY,     ANIM_GoombaBros_Red_Dizzy,
    STATUS_KEY_FEAR,      ANIM_GoombaBros_Red_Dizzy,
    STATUS_END,
};

#include "red_goomba_shared.inc.c"

EvtScript N(EVS_Init_Inner) = {
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
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(IdleAnimations_8021B810)))
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
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(N(IdleAnimations_8021B810)))
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

EvtScript N(EVS_TakeTurn) = {
    INCREMENT_ACTOR_VAR(AVAR_TurnCount)
    GET_ACTOR_VAR(AVAR_TurnCount, LVar0)
    Mod(LVar0, 2)
    Switch(LVar0)
        CaseEq(0)
            Set(TargetPartnerVar, FALSE)
            ExecWait(N(Headbonk))
        CaseEq(1)
            Set(TargetPartnerVar, TRUE)
            ExecWait(N(Headbonk))
    EndSwitch

    Return
    End
};

EvtScript N(EVS_Init) = {
    ExecWait(N(EVS_Init_Inner))
    Call(SetActorVar, ACTOR_SELF, AVAR_Unused_0, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_Unused_1, 0)
    SET_ACTOR_VAR(AVAR_TurnCount, 0)
    Return
    End
};
