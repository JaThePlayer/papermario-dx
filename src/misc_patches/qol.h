// Many battle-specific macros for qol

#define SET_ACTOR_VAR(id, val) Call(SetActorVar, ACTOR_SELF, id, val)

#define GET_ACTOR_VAR(id, outVar) Call(GetActorVar, ACTOR_SELF, id, outVar)

#define INCREMENT_ACTOR_VAR(id) Call(AddActorVar, ACTOR_SELF, id, 1)

#define PERMANENT_BOOST_ATK(actor, amt) \
    Thread \
        Wait(10) \
        Call(PlaySoundAtActor, actor, SOUND_MAGIKOOPA_POWER_UP) \
    EndThread \
    Thread \
        Call(FreezeBattleState, TRUE) \
        Call(BoostAttack, actor, amt) \
        Call(FreezeBattleState, FALSE) \
    EndThread \
    Call(WaitForBuffDone)

#define PERMANENT_BOOST_DEF(actor, amt) \
    Thread \
        Wait(10) \
        Call(PlaySoundAtActor, actor, SOUND_MAGIKOOPA_POWER_UP) \
    EndThread \
    Thread \
        Call(FreezeBattleState, TRUE) \
        Call(BoostDefense, actor, amt) \
        Call(FreezeBattleState, FALSE) \
    EndThread \
    Call(WaitForBuffDone)

// Executes instructions provided in the 'callback' argument for each enemy in the battle.
// 'label' is a label id to be used inside the macro, it should not be used anywhere else in the script.
// 'flags' are passed to CreateCurrentPosTargetList
// 'enemy' is an lvar that will be populated with an enemy id when 'callback' is executed
#define FOREACH_ENEMY(enemy, label, flags, callback) \
    Call(CreateCurrentPosTargetList, flags) \
    Call(InitTargetIterator) \
    Label(label) \
    Call(GetOwnerTarget, enemy, 0) \
    callback \
    Call(ChooseNextTarget, ITER_NEXT, enemy) \
    IfNe(enemy, ITER_NO_MORE) \
        Goto(label) \
    EndIf
