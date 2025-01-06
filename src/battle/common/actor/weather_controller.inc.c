#include "common.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "misc_patches/custom_status.h"
#include "weather_controller.h"

#define NAMESPACE A(weather_controller)

extern s32 N(DefaultAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorVars) {
    N(AVAR_Type) = 0, // Should be set by formations!
    N(AVAR_DelayedInit) = 1,  // bool. Should be set by formations! Whether the weather init should be delayed until manually triggered.
    N(AVAR_TurnCount) = 8, // Which turn this is.
    N(AVAR_ControllerType) = 14,
    N(ControllerType_Weather) = 123,
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
    .flags = ACTOR_FLAG_INVISIBLE | ACTOR_FLAG_NO_SHADOW | ACTOR_FLAG_NO_HEALTH_BAR | ACTOR_FLAG_NO_DMG_APPLY,
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

extern s32 BattleMessages[];

extern EvtScript A(EVS_WeatherInitImpl);

API_CALLABLE(A(EnableWeatherByType)) {
    Bytecode* args = script->ptrReadPos;

    s32 id = evt_get_variable(script, *args++);
    BattleStatus* battleStatus = &gBattleStatus;
    s32 i;

    if (!isInitialCall) {
        if (script->userData == NULL)
            return ApiStatus_DONE2;
        if (does_script_exist_by_ref((Evt*)script->userData))
            return ApiStatus_BLOCK;
        script->userData = NULL;
        return ApiStatus_DONE2;
    }

    for (i = 0; i < ARRAY_COUNT(battleStatus->enemyActors); i++) {
        Actor* targetActor = battleStatus->enemyActors[i];
        if (targetActor == NULL)
            continue;
        if (targetActor->actorType != ACTOR_TYPE_CONTROLLER)
            continue;
        if (targetActor->state.varTable[N(AVAR_ControllerType)] != N(ControllerType_Weather))
            continue;
        if (targetActor->state.varTable[N(AVAR_Type)] != id)
            continue;

        if (targetActor->state.varTable[N(AVAR_DelayedInit)]) {
            targetActor->state.varTable[N(AVAR_DelayedInit)] = FALSE;
            Evt* newScript = start_script(&A(EVS_WeatherInitImpl), EVT_PRIORITY_A, 0);
            newScript->owner1.actorID = targetActor->actorID;

            script->userData = newScript;
        }
    }

    return ApiStatus_BLOCK;
}

// args: msgId
API_CALLABLE(N(ShowMessage)) {
    Bytecode* args = script->ptrReadPos;

    s32 id = evt_get_variable(script, *args++);
    BattleMessages[BTL_MSG_CUSTOM] = id;
    btl_show_battle_message(BTL_MSG_CUSTOM, 160);

    return ApiStatus_DONE2;
}

// args: r, g, b, alpha, minDist, maxDist
API_CALLABLE(N(SetFogAlpha)) {
    Bytecode* args = script->ptrReadPos;

    f32 r = evt_get_variable(script, *args++);
    f32 g = evt_get_variable(script, *args++);
    f32 b = evt_get_variable(script, *args++);
    f32 alpha = evt_get_variable(script, *args++);
    s32 minDist = evt_get_variable(script, *args++);
    s32 maxDist = evt_get_variable(script, *args++);

    alpha = alpha > 255 ? 255 : alpha;
    f32 mult = alpha / 255.0;
    set_world_fog_dist(minDist, maxDist);
    set_world_fog_color(r * mult, g * mult, b * mult, alpha);
    enable_world_fog();

    return ApiStatus_DONE2;
}

EvtScript A(EVS_WeatherInitImpl) = {
    GET_ACTOR_VAR(N(AVAR_Type), LVar1)
    Switch(LVar1)
        CaseEq(WEATHER_TutanPoisonFog)
            Set(LVarA, 5)
            Loop(50)
                Call(N(SetFogAlpha), 29, 173, 73, LVarA, 900, 1100)
                Add(LVarA, 5)
                Wait(1)
            EndLoop

            Call(InflictCustomStatus, ACTOR_PLAYER, POISON_STATUS, 9, 1, 100)
            Call(N(ShowMessage), MSG_Menus_Tutan_PoisonFog)
            Call(WaitForMessageBoxDone)
            Call(N(ShowMessage), MSG_Menus_Tutan_PoisonFog_2)
            Call(WaitForMessageBoxDone)
    EndSwitch

    Return
    End
};

EvtScript N(EVS_HandlePhase) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)

    // Start of battle popup
    Call(GetBattlePhase, LVar0)
    Switch(LVar0)
        CaseEq(PHASE_PLAYER_BEGIN)
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
    Call(SetActorVar, ACTOR_SELF, N(AVAR_TurnCount), 0)
    Call(SetActorVar, ACTOR_SELF, N(AVAR_ControllerType), N(ControllerType_Weather))

    GET_ACTOR_VAR(N(AVAR_DelayedInit), LVar1)
    IfFalse(LVar1)
        ExecWait(A(EVS_WeatherInitImpl))
    EndIf

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

EvtScript N(EVS_TakeTurn) = {
    GET_ACTOR_VAR(N(AVAR_DelayedInit), LVar1)
    IfTrue(LVar1)
        Return
    EndIf

    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)

    INCREMENT_ACTOR_VAR(N(AVAR_TurnCount))
    GET_ACTOR_VAR(N(AVAR_TurnCount), LVar0)
    GET_ACTOR_VAR(N(AVAR_Type), LVar1)

    Switch(LVar1)
        CaseEq(WEATHER_TutanPoisonFog)
            IfNe(LVar0, 1)
                Set(LVarA, LVar0)
                Call(InflictCustomStatus, ACTOR_PLAYER, POISON_STATUS, 9, LVarA, 100)
                Call(N(ShowMessage), MSG_Menus_Tutan_PoisonFog_Stronger)
                Call(WaitForMessageBoxDone)
            EndIf
    EndSwitch

    Label(0)
        Wait(1)
        Call(IsMessageBoxDisplayed, LVar0)
        IfEq(LVar0, TRUE)
            Goto(0)
        EndIf

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};
