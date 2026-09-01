#pragma once

#include "common_structs.h"

/// Allows the actor to get unflipped by external sources, like Monty Moles.
typedef struct ICanBeUnflipped {
    /// Script to execute to check whether this Actor can be unflipped.
    /// Returns bool on LVarE.
    EvtScript* canBeUnflippedScript;
    EvtScript* getUpImmediatelyScript;
} ICanBeUnflipped;
