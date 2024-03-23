#include "battle/battle.h"
#include "script_api/battle.h"
#include "misc_patches/custom_status.h"
#include "sprite/npc/FireBros.h"

#define NAMESPACE A(fire_bros)

extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

extern EvtScript A(bob_omb_EVS_Ignite);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
    PRT_FIREBALL    = 2,
};

enum N(ActorVars) {
    AVAR_Unused     = 8, // possibly a topple state that was never implemented
};

enum N(ActorParams) {
    DMG_FIRE_THROW    = 1, // + fire
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_FireBros_Anim02,
    STATUS_KEY_STONE,     ANIM_FireBros_Anim00,
    STATUS_KEY_SLEEP,     ANIM_FireBros_Anim11,
    STATUS_KEY_POISON,    ANIM_FireBros_Anim02,
    STATUS_KEY_STOP,      ANIM_FireBros_Anim00,
    STATUS_KEY_STATIC,    ANIM_FireBros_Anim02,
    STATUS_KEY_PARALYZE,  ANIM_FireBros_Anim00,
    STATUS_KEY_DIZZY,     ANIM_FireBros_Anim12,
    STATUS_KEY_FEAR,      ANIM_FireBros_Anim12,
    STATUS_END,
};

s32 N(FireballAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_FireBros_Fireball,
    STATUS_END,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              75,
    STATUS_KEY_POISON,            100,
    STATUS_KEY_FROZEN,            100,
    STATUS_KEY_DIZZY,              75,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,            100,
    STATUS_KEY_PARALYZE,          100,
    STATUS_KEY_SHRINK,             70,
    STATUS_KEY_STOP,               80,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         1,
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
        .targetOffset = { -3, 35 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { -3, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_FIREBALL,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = N(FireballAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    }
};

ActorBlueprint NAMESPACE = {
    .flags = 0,
    .type = ACTOR_TYPE_FIRE_BROS,
    .level = ACTOR_LEVEL_FIRE_BROS,
    .maxHP = 5,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 90,
    .airLiftChance = 90,
    .hurricaneChance = 60,
    .spookChance = 80,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 80,
    .coinReward = 1,
    .size = { 28, 38 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 35 },
    .spPool = CURRENT_SP_POOL,
};

EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    Call(SetActorVar, ACTOR_SELF, AVAR_Unused, 0)
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Label(0)
        Call(GetStatusFlags, ACTOR_SELF, LVar0)
        Switch(LVar0)
            CaseFlag(STATUS_FLAG_SLEEP)
                Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -5, 15)
                Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 0)
            CaseDefault
                Call(GetActorVar, ACTOR_SELF, AVAR_Unused, LVar0)
                Switch(LVar0)
                    CaseEq(0)
                        Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -3, 35)
                        Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, -3, -10)
                    CaseEq(1)
                        Call(SetTargetOffset, ACTOR_SELF, PRT_MAIN, -5, 15)
                        Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_MAIN, 0, 0)
                EndSwitch
        EndSwitch
        Wait(1)
        Goto(0)
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
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0F)
            SetConst(LVar2, ANIM_FireBros_Anim10)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0F)
            SetConst(LVar2, ANIM_FireBros_Anim10)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim10)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim15)
            ExecWait(EVS_Enemy_NoDamageHit)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_FireBros_Anim17)
                Wait(10)
            EndIf
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim02)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim07)
            SetConst(LVar2, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim07)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                IfFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(SetActorDispOffset, ACTOR_SELF, -2, 1, 0)
                Else
                    Call(SetActorDispOffset, ACTOR_SELF, -5, 3, 0)
                EndIf
            EndIf
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim0E)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_FireBros_Anim02)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(fireThrowAtMario) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Call(MoveBattleCamOver, 1)
    EndIf
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        ChildThread
            Set(LVar0, 0)
            Label(0)
            Add(LVar0, 20)
            Mod(LVar0, 360)
            Call(SetPartRotation, ACTOR_SELF, PRT_FIREBALL, 0, 0, LVar0)
            Wait(1)
            Goto(0)
        EndChildThread
        Call(SetPartPos, ACTOR_SELF, PRT_FIREBALL, 0, 50, 0)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, FALSE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_FireBros_Anim0C)
        Goto(100)
    EndIf
    ChildThread
        Set(LVar0, 0)
        Label(0)
        Add(LVar0, 20)
        Mod(LVar0, 360)
        Call(SetPartRotation, ACTOR_SELF, PRT_FIREBALL, 0, 0, LVar0)
        Wait(1)
        Goto(0)
    EndChildThread
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Sub(LVar0, 23)
    Add(LVar1, 23)
    Sub(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_FireBros_Anim0C)
    Wait(8)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, FALSE)
    Label(100)
    Thread
        Wait(15)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_FireBros_Anim02)
    EndThread
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, STATUS_FLAG_SHRINK)
        Call(SetPartScale, ACTOR_SELF, PRT_FIREBALL, Float(0.4), Float(0.4), Float(0.4))
    Else
        Call(SetPartScale, ACTOR_SELF, PRT_FIREBALL, Float(1.0), Float(1.0), Float(1.0))
    EndIf
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetPartSounds, ACTOR_SELF, PRT_FIREBALL, ACTOR_SOUND_JUMP, SOUND_NONE, SOUND_NONE)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_FIREBALL, SOUND_LAVA_PIRANHA_SPIT_FIRE)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 20)
            Set(LVar1, 10)
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_FIREBALL, Float(1.3))
            Call(JumpPartTo, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2, 15, TRUE)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVarA, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Sub(LVar0, 40)
            Call(JumpPartTo, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2, 15, TRUE)
            Sub(LVar0, 30)
            Call(JumpPartTo, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2, 10, TRUE)
            Sub(LVar0, 20)
            Call(JumpPartTo, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2, 5, TRUE)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, TRUE)
            Wait(15)
            Call(YieldTurn)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            Wait(20)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetPartSounds, ACTOR_SELF, PRT_FIREBALL, ACTOR_SOUND_JUMP, SOUND_NONE, SOUND_NONE)
    Call(PlaySoundAtPart, ACTOR_SELF, PRT_FIREBALL, SOUND_LAVA_PIRANHA_SPIT_FIRE) // SOUND_HAMMER_BROS_THROW
    Call(SetGoalToTarget, ACTOR_SELF)

    Call(SetPartJumpGravity, ACTOR_SELF, PRT_FIREBALL, Float(2.5))
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpPartTo, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2, 10, TRUE)
    Wait(2)

    // set burn status
    Call(SetNextAttackCustomStatus, BURN_STATUS, 2, 1, 100) // 2 turn t1 burn
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_FIRE, 0, 0, DMG_FIRE_THROW, BS_FLAGS1_TRIGGER_EVENTS)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, TRUE)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Wait(20)
            Call(YieldTurn)
        EndCaseGroup
    EndSwitch

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

#define BobombId LVarE

EvtScript N(bobombIgnite) = {
    Call(SetOwnerID, BobombId)
    ExecWait(A(bob_omb_EVS_Ignite))
    Return
    End
};

EvtScript N(fireThrowAtBomb) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, BobombId)
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Call(MoveBattleCamOver, 1)
    EndIf

    ChildThread
        Set(LVar0, 0)
        Label(0)
        Add(LVar0, 20)
        Mod(LVar0, 360)
        Call(SetPartRotation, ACTOR_SELF, PRT_FIREBALL, 0, 0, LVar0)
        Wait(1)
        Goto(0)
    EndChildThread

    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(SetPartPos, ACTOR_SELF, PRT_FIREBALL, 0, 50, 0)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, FALSE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_FireBros_Anim0C)
        Goto(100)
    EndIf

    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Sub(LVar0, 23)
    Add(LVar1, 23)
    Sub(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_FireBros_Anim0C)
    Wait(8)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, FALSE)
    Label(100)
    Thread
        Wait(15)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_FireBros_Anim02)
    EndThread
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, STATUS_FLAG_SHRINK)
        Call(SetPartScale, ACTOR_SELF, PRT_FIREBALL, Float(0.4), Float(0.4), Float(0.4))
    Else
        Call(SetPartScale, ACTOR_SELF, PRT_FIREBALL, Float(1.0), Float(1.0), Float(1.0))
    EndIf

    Call(SetPartSounds, ACTOR_SELF, PRT_FIREBALL, ACTOR_SOUND_JUMP, SOUND_NONE, SOUND_NONE)
    Call(PlaySoundAtPart, ACTOR_SELF, PRT_FIREBALL, SOUND_LAVA_PIRANHA_SPIT_FIRE) // SOUND_HAMMER_BROS_THROW
    Call(SetGoalToTarget, ACTOR_SELF)

    Call(SetPartJumpGravity, ACTOR_SELF, PRT_FIREBALL, Float(2.5))
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpPartTo, ACTOR_SELF, PRT_FIREBALL, LVar0, LVar1, LVar2, 10, TRUE)
    Wait(2)

    Call(SetPartFlagBits, ACTOR_SELF, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, TRUE)
    Exec(N(bobombIgnite))

    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

API_CALLABLE(N(FindBestBobomb)) {
    BattleStatus* battleStatus = &gBattleStatus;
    s32 i;
    s32 highestTurnCount = -1;
    s32 bestI = -1;

    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);
    s32 retActorId = *args++;

    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    Actor* untilActor = get_actor(actorID);

    for (i = 0; i < ARRAY_COUNT(battleStatus->enemyActors); i++) {
        Actor* targetActor = battleStatus->enemyActors[i];
        if (targetActor == untilActor)
            break;

        if (targetActor == NULL)
            continue;
        if (targetActor->actorType != ACTOR_TYPE_BOB_OMB)
            continue;

        // Don't fireball already ignited bobombs, thats a waste
        if (targetActor->state.varTable[8/*AVAR_Ignited*/] == TRUE)
            continue;

        s32 turnCount = targetActor->state.varTable[0/*AVAR_TurnsUntilIgnition*/];
        if (turnCount > highestTurnCount) {
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
    Call(N(FindBestBobomb), ACTOR_SELF, BobombId)
    IfNe(BobombId, FALSE)
        ExecWait(N(fireThrowAtBomb))
        Return
    EndIf

    ExecWait(N(fireThrowAtMario))
    Return
    End
};
