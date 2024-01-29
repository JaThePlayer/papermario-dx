#ifndef _H_STATUS_REWORK_HELPERS
#define _H_STATUS_REWORK_HELPERS

#include "common.h"
#include "effects.h"

// should be called whenever an event gets dispatched on an actor
void status_rework_on_dispatch_event_actor(Actor* actor, s32 event);

#endif
