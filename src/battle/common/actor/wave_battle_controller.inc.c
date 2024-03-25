// ACTOR_FLAG_NO_DMG_APPLY
#include "common.h"
#include "battle/battle.h"
#include "script_api/battle.h"

#define NAMESPACE A(wave_battle_controller)

extern s32 N(DefaultAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorVars) {
    AVAR_Formation = 0, // Should be set by formations!
    AVAR_FormationCount = 1, // Should be set by formations, should be equal to ARRAY_COUNT(AVAR_Formation)
    AVAR_WaveCount = 2, // Should be set by formations. If != 0, displays a popup when created
    AVAR_Index = 8, // Which wave this is.
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              0,
    STATUS_KEY_POISON,            0,
    STATUS_KEY_FROZEN,            0,
    STATUS_KEY_DIZZY,              0,
    STATUS_KEY_FEAR,              0,
    STATUS_KEY_STATIC,            0,
    STATUS_KEY_PARALYZE,           0,
    STATUS_KEY_SHRINK,             0,
    STATUS_KEY_STOP,               0,
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
        .flags = ACTOR_PART_FLAG_NO_TARGET,
        .index = 1,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 20 },
        .opacity = 255,
        .idleAnimations = NULL,
        .defenseTable = N(DefenseTable),
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
};

#define WAVE_BATTLE_CONTROLLER_TYPE ACTOR_TYPE_CONTROLLER

ActorBlueprint NAMESPACE = {
    .flags = ACTOR_FLAG_INVISIBLE | ACTOR_FLAG_NO_SHADOW | ACTOR_FLAG_NO_HEALTH_BAR,// | ACTOR_FLAG_NO_DMG_APPLY,
    .type = WAVE_BATTLE_CONTROLLER_TYPE,
    .level = 0,
    .maxHP = 99,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 70,
    .airLiftChance = 90,
    .hurricaneChance = 85,
    .spookChance = 80,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 1,
    .size = { 24, 24 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
    .spPool = SP_POOL_NONE,
};

EvtScript N(EVS_HandlePhase) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)

    // Start of battle popup
    Call(GetBattlePhase, LVar0)
    Switch(LVar0)
        CaseEq(PHASE_PLAYER_BEGIN)
            Call(GetActorVar, ACTOR_SELF, AVAR_WaveCount, LVar0)
            IfNe(LVar0, 0)
                Call(ShowVariableMessageBox, BTL_MSG_WAVE_BATTLE, 60, LVar0)
                Call(SetActorVar, ACTOR_SELF, AVAR_WaveCount, 0)
                Label(0)
                Wait(1)
                Call(IsMessageBoxDisplayed, LVar0)
                IfEq(LVar0, TRUE)
                    Goto(0)
                EndIf
            EndIf
    EndSwitch

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    Call(BindHandlePhase, ACTOR_SELF, Ref(N(EVS_HandlePhase)))
    Call(SetActorVar, ACTOR_SELF, AVAR_Index, 1)
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Label(10)
    Wait(999)
    Goto(10)
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

API_CALLABLE(N(AllEnemiesDefeated)) {
    Bytecode* args = script->ptrReadPos;

    evt_set_variable(script, *args++, btl_are_all_enemies_defeated());

    return ApiStatus_DONE2;
}

EvtScript N(EVS_OnSummonEnemy) = {
    Call(SummonEnemy, LVarA, FALSE)
    Call(GetCurrentActorType, LVar0, LVar1)
    IfEq(LVar1, WAVE_BATTLE_CONTROLLER_TYPE)
        Call(GetActorVar, ACTOR_SELF, AVAR_Index, LVar1)
        Add(LVar1, 1)
        Call(SetActorVar, LVar0, AVAR_Index, LVar1)
    EndIf
    Return
    End
};

API_CALLABLE(N(SummonAllEnemies)) {
    Bytecode* args = script->ptrReadPos;

    FormationRow* formation = (FormationRow*)evt_get_variable(script, *args++);
    s32 count = evt_get_variable(script, *args++);
    s32 waveIndex = evt_get_variable(script, *args++);

    for (s32 i = 0; i < count; i++) {
        f32 x, y, z;


        if (!(formation->actor->flags & ACTOR_FLAG_INVISIBLE)) {
            // Copypasta from create_actor :/
            if (formation->home.index >= EVT_LIMIT) {
                x = StandardActorHomePositions[formation->home.index].x;
                y = StandardActorHomePositions[formation->home.index].y;
                z = StandardActorHomePositions[formation->home.index].z;
            } else {
                x = formation->home.vec->x;
                y = formation->home.vec->y;
                z = formation->home.vec->z;
            }

            fx_big_smoke_puff(x, y, z);
        }

        Evt* inner = start_script(&N(EVS_OnSummonEnemy), EVT_PRIORITY_A, EVT_FLAG_RUN_IMMEDIATELY);
        inner->owner1.actor = script->owner1.actor;
        inner->varTablePtr[0xA] = formation;

        formation++;
    }

    btl_show_variable_battle_message(BTL_MSG_NEXT_WAVE, 40, waveIndex + 1);

    return ApiStatus_DONE2;
}

EvtScript N(EVS_TakeTurn) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)

    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_DMG_APPLY, TRUE)
    Call(N(AllEnemiesDefeated), LVar0)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_DMG_APPLY, FALSE)
    IfTrue(LVar0)
        Call(GetActorVar, ACTOR_SELF, AVAR_Formation, LVar0)
        Call(GetActorVar, ACTOR_SELF, AVAR_FormationCount, LVar1)
        Call(GetActorVar, ACTOR_SELF, AVAR_Index, LVar2)

        Call(N(SummonAllEnemies), LVar0, LVar1, LVar2)
        Label(0)
        Wait(1)
        Call(IsMessageBoxDisplayed, LVar0)
        IfEq(LVar0, TRUE)
            Goto(0)
        EndIf

        Call(RemoveActor, ACTOR_SELF)
        Return
    EndIf

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};
