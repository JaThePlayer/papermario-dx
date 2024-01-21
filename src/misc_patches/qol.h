// Many battle-specific macros for qol

#define SET_ACTOR_VAR(id, val) EVT_CALL(SetActorVar, ACTOR_SELF, id, val)

#define GET_ACTOR_VAR(id, outVar) EVT_CALL(GetActorVar, ACTOR_SELF, id, outVar)

#define INCREMENT_ACTOR_VAR(id) EVT_CALL(AddActorVar, ACTOR_SELF, id, 1)
