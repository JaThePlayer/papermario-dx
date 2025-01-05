#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/Fuzzy.h"
#include "sprite/player.h"
#include "effects.h"

#define NAMESPACE A(flower_fuzzy)

extern s32 N(DefaultAnims)[];
extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_HandleEvent);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
};

enum N(ActorParams) {
    DMG_LEECH           = 2,
    DMG_LIGHTNING       = 3,
    DMG_LIGHTNING_COUNT = 2,

    AVAR_Fp             = 1,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,             100,
    STATUS_KEY_POISON,            100,
    STATUS_KEY_FROZEN,            100,
    STATUS_KEY_DIZZY,              75,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,             70,
    STATUS_KEY_PARALYZE,           75,
    STATUS_KEY_SHRINK,             75,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          -1,
    STATUS_TURN_MOD_POISON,         -1,
    STATUS_TURN_MOD_FROZEN,         -1,
    STATUS_TURN_MOD_DIZZY,          -1,
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
        .targetOffset = { 0, 20 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = 0,
    .type = ACTOR_TYPE_FLOWER_FUZZY,
    .level = ACTOR_LEVEL_FLOWER_FUZZY,
    .maxHP = 20,
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
    .coinReward = 1,
    .size = { 33, 28 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
    .spPool = SP_POOL_NONE,
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_Fuzzy_Flower_Idle,
    STATUS_KEY_STONE,     ANIM_Fuzzy_Flower_Still,
    STATUS_KEY_SLEEP,     ANIM_Fuzzy_Flower_Sleep,
    STATUS_KEY_POISON,    ANIM_Fuzzy_Flower_Idle,
    STATUS_KEY_STOP,      ANIM_Fuzzy_Flower_Still,
    STATUS_KEY_STATIC,    ANIM_Fuzzy_Flower_Idle,
    STATUS_KEY_PARALYZE,  ANIM_Fuzzy_Flower_Still,
    STATUS_KEY_DIZZY,     ANIM_Fuzzy_Flower_Stunned,
    STATUS_KEY_FEAR,      ANIM_Fuzzy_Flower_Stunned,
    STATUS_END,
};

EvtScript N(EVS_Init) = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    SET_ACTOR_VAR(AVAR_Fp, 0)
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
    Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            Call(GetLastElement, LVar0)
            IfFlag(LVar0, DAMAGE_TYPE_SHOCK)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Fuzzy_Flower_HurtShock)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Fuzzy_Flower_Hurt)
            EndIf
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Burn)
            SetConst(LVar2, ANIM_Fuzzy_Flower_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Burn)
            SetConst(LVar2, ANIM_Fuzzy_Flower_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_BurnStill)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_HurtShock)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_HurtShock)
            ExecWait(EVS_Enemy_Knockback)
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Run)
            ExecWait(EVS_Enemy_HopHome)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.6))
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_HurtShock)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            Call(GetLastElement, LVar0)
            IfFlag(LVar0, DAMAGE_TYPE_SHOCK)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Fuzzy_Flower_HurtShock)
                ExecWait(EVS_Enemy_Hit)
                Wait(10)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Fuzzy_Flower_HurtShock)
                ExecWait(EVS_Enemy_Death)
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Fuzzy_Flower_Hurt)
                ExecWait(EVS_Enemy_Hit)
                Wait(10)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_Fuzzy_Flower_Hurt)
                ExecWait(EVS_Enemy_Death)
            EndIf
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Run)
            SetConst(LVar2, ANIM_Fuzzy_Flower_Anim09)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Run)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Fuzzy_Flower_Run)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

#include "common/SpawnEnemyDrainFX.inc.c"

API_CALLABLE(N(SpawnFlowerRecoveryFX)) {
    Bytecode* args = script->ptrReadPos;
    s32 x = evt_get_variable(script, *args++);
    s32 y = evt_get_variable(script, *args++);
    s32 z = evt_get_variable(script, *args++);
    s32 amt = evt_get_variable(script, *args++);

    fx_recover(1, x, y, z, amt);

    return ApiStatus_DONE2;
}

#define Recover_FP(tempX, tempY, tempZ, amt) \
    Call(GetActorPos, ACTOR_SELF, tempX, tempY, tempZ) \
    Add(tempX, 20) \
    Add(tempY, 30) \
    PlayEffect(EFFECT_RECOVER, 1, tempX, tempY, tempZ, amt, 0) \
    GET_ACTOR_VAR(AVAR_Fp, tempX) \
    Add(tempX, amt) \
    SET_ACTOR_VAR(AVAR_Fp, tempX)

EvtScript N(EVS_NormalAttack) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, FALSE)
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 50)
        Set(LVar1, 0)
        Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Goto(100)
    EndIf
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Run)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 50)
    Set(LVar1, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
    ExecWait(EVS_Enemy_HopToPos)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Idle)
    Label(100)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 10, 0)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Leech)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 9, 0)
    Wait(1)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 8, 0)
    Wait(5)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_IGNORE_DEFENSE | DAMAGE_TYPE_NO_CONTACT, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 5)
            Set(LVar1, 0)
            Sub(LVar2, 5)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(3.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Jump)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 11, FALSE, TRUE, FALSE)
            Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Anim09)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 20)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(3.0))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 6, FALSE, TRUE, FALSE)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 15)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(3.0))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Wait(4)
            Wait(5)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(5)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Run)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            ExecWait(EVS_Enemy_HopToPos)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, TRUE)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, -3, -12, -15)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(3.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Jump)
    Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
    /*
    IfEq(LFlag0, TRUE)
        Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_IGNORE_DEFENSE | DAMAGE_TYPE_NO_CONTACT, 0, 0, 0, BS_FLAGS1_TRIGGER_EVENTS)
        Return
    EndIf*/
    Wait(10)
    Call(UseIdleAnimation, ACTOR_PLAYER, FALSE)
    Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_MarioB1_Leeching)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_LEECH)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Bite)
    Wait(21)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Still)
    Call(SetActorRotation, ACTOR_SELF, 0, 0, 330)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_LEECH)
    Call(SetActorDispOffset, ACTOR_SELF, -3, -2, 0)
    Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
    Wait(1)
    Call(SetActorScale, ACTOR_SELF, Float(0.9), Float(1.2), Float(1.0))
    Wait(1)
    Call(SetActorScale, ACTOR_SELF, Float(0.8), Float(1.3), Float(1.0))
    Wait(1)
    Call(SetActorScale, ACTOR_SELF, Float(0.7), Float(1.4), Float(1.0))
    Wait(1)
    Call(SetActorScale, ACTOR_SELF, Float(0.7), Float(1.5), Float(1.0))
    Wait(2)
    Call(SetActorScale, ACTOR_SELF, Float(0.6), Float(1.6), Float(1.0))
    Wait(10)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_IGNORE_DEFENSE | DAMAGE_TYPE_NO_CONTACT, 0, 0, DMG_LEECH, BS_FLAGS1_TRIGGER_EVENTS)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Idle)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(GetLastDamage, ACTOR_PLAYER, LVar3)
            IfNe(LVar3, 0)
                //Call(PlaySoundAtActor, ACTOR_SELF, SOUND_RECOVER_HEART)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_FLOWER_BOUNCE)
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Add(LVar1, 10)
                //Call(N(SpawnFlowerRecoveryFX), LVar0, LVar1, LVar2, LVar3)
                //Call(N(SpawnDrainHealthStartFX), LVar0, LVar1, LVar2, LVar3)
                Thread
                    Wait(15)
                    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_STAR_BOUNCE_A)
                    //Call(N(SpawnDrainHealthContinueFX), LVar0, LVar1, LVar2, LVar3)
                EndThread
                Add(LVar0, 20)
                Add(LVar1, 20)
                /*
                PlayEffect(EFFECT_RECOVER, 1, LVar0, LVar1, LVar2, LVar3, 0)
                // Add fp
                GET_ACTOR_VAR(AVAR_Fp, LVar0)
                Add(LVar0, LVar3)
                SET_ACTOR_VAR(AVAR_Fp, LVar0)*/

                Add(LVar3, 1) // +1 fp recovery on top of dmg dealt
                Recover_FP(LVar0, LVar1, LVar2, LVar3)
            EndIf
            Thread
                Call(SetActorScale, ACTOR_SELF, Float(1.6), Float(0.6), Float(1.0))
                Call(SetActorRotationOffset, ACTOR_SELF, 0, 10, 0)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 330)
                Wait(1)
                Call(SetActorScale, ACTOR_SELF, Float(0.7), Float(1.5), Float(1.0))
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 300)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 270)
                Wait(1)
                Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(0.7), Float(1.0))
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 240)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 210)
                Wait(1)
                Call(SetActorScale, ACTOR_SELF, Float(0.7), Float(1.4), Float(1.0))
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 180)
                Wait(2)
                Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(0.7), Float(1.0))
                Wait(2)
                Call(SetActorScale, ACTOR_SELF, Float(1.2), Float(0.5), Float(1.0))
            EndThread
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 40)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, FALSE, TRUE, FALSE)
            Call(SetActorScale, ACTOR_SELF, Float(1.4), Float(0.2), Float(1.0))
            Wait(1)
            Thread
                Wait(1)
                Call(SetActorScale, ACTOR_SELF, Float(1.2), Float(0.5), Float(1.0))
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 150)
                Wait(1)
                Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 120)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 90)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 60)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 30)
                Wait(1)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
                Call(SetActorRotationOffset, ACTOR_SELF, 0, 0, 0)
            EndThread
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
            Wait(5)
            Call(YieldTurn)
            Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Run)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorSpeed, ACTOR_SELF, Float(8.0))
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            ExecWait(EVS_Enemy_HopToPos)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Fuzzy_Flower_Idle)
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

API_CALLABLE(N(SpawnLightningFX)) {
    Actor* enemyTarget = get_actor(script->owner1.enemyID);
    Actor* actor = get_actor(enemyTarget->targetActorID);
    f32 posY, posX, posZ;
    s32 scaleX, scaleY;

    if (actor != NULL) {
        sfx_play_sound(SOUND_LIGHTNING_STRIKE);

        posX = actor->curPos.x;
        posY = actor->curPos.y + (actor->size.y / 10);
        posZ = actor->curPos.z;

        scaleX = (actor->size.x + (actor->size.x >> 2)) * actor->scalingFactor;
        scaleY = (actor->size.y - 2) * actor->scalingFactor;

        if (actor->flags & ACTOR_FLAG_HALF_HEIGHT) {
            posY -= actor->size.y / 2;
        }

        fx_lightning(rand_int(2) + 3, posX, posY, posZ, scaleX, scaleY);

        return ApiStatus_DONE2;
    }

    return ApiStatus_DONE2;
}

#include "common/FadeBackgroundDarken.inc.c"
#include "common/FadeBackgroundLighten.inc.c"

EvtScript N(ThunderBolt) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)

    //fx_stars_spread(iVar1, fVar2, fVar3, fVar4, iVar5, iVar6);
    //                0, part->pos.x + (xFlip ? 10.0f : -10.0f), part->pos.y, part->pos.z, 7, 20
    Call(PlaySound, SOUND_USE_ITEM)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    PlayEffect(EFFECT_STARS_SPREAD, 0, LVar0, LVar1, LVar2, 7, 20)

    Thread
        Wait(5)
        Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
        Call(MoveBattleCamOver, 20)
    EndThread

    Call(N(FadeBackgroundDarken))
    Call(PlaySound, SOUND_THUNDER_BOLT)
    Wait(10)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)

    Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_SHOCK | DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_MULTIPLE_POPUPS, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            IfEq(LVar0, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Goto(1)
    EndSwitch

    Loop(DMG_LIGHTNING_COUNT)
        Call(N(SpawnLightningFX))
        Wait(5)
        Call(StartRumble, BTL_RUMBLE_PLAYER_EXTREME)
        Call(ShakeCam, CAM_BATTLE, 0, 5, Float(1.0))
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_SHOCK | DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_MULTIPLE_POPUPS, 0, 0, DMG_LIGHTNING, BS_FLAGS1_TRIGGER_EVENTS)

        // Wait for the damage stars to be visible for a while
        Wait(20)
    EndLoop

    Label(1)
    Wait(5)
    Call(UseBattleCamPreset, BTL_CAM_VIEW_ENEMIES)
    Call(MoveBattleCamOver, 20)
    Wait(30)
    Call(N(FadeBackgroundLighten))
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_TakeTurn) = {
    GET_ACTOR_VAR(AVAR_Fp, LVar0)

    IfGe(LVar0, 5)
        // 5 FP is stored up, time for big attack
        Sub(LVar0, 5)
        SET_ACTOR_VAR(AVAR_Fp, LVar0)
        ExecWait(N(ThunderBolt))
        Return
    EndIf

    // Passive FP buildup
    Recover_FP(LVar0, LVar1, LVar2, 1)
    Wait(10)
    ExecWait(N(EVS_NormalAttack))

    Return
    End
};
