#pragma once

#include "macros.h"

typedef s32 AnimationBank[];

/// Call(SetBankAnimation, ACTOR_SELF, PRT_MAIN, Ref(bank), slotId)
API_CALLABLE(SetBankAnimation);

/// Call(SetBankAnimation, ACTOR_SELF, PRT_MAIN, Ref(bank), slotId)
#define SetTypedBankAnimation(actorId, partId, bankRef, bankType, bankSlotField) \
    Call(SetBankAnimation, actorId, partId, bankRef, __builtin_offsetof(bankType, bankSlotField) / 4)
