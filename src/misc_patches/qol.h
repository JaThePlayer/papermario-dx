// Many battle-specific macros for qol

#define SET_ACTOR_VAR(id, val) Call(SetActorVar, ACTOR_SELF, id, val)

#define GET_ACTOR_VAR(id, outVar) Call(GetActorVar, ACTOR_SELF, id, outVar)

#define INCREMENT_ACTOR_VAR(id) Call(AddActorVar, ACTOR_SELF, id, 1)
