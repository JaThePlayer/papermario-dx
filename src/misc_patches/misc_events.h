#ifndef _MISC_EVENTS_H_
#define _MISC_EVENTS_H_

#include "common_structs.h"

/// Called when a battle starts
void _on_battle_start();

/// Called whenever a turn starts
void _on_turn_start();

/// Called whenever passive SE regen happens.
void _on_handle_passive_se_regen();

void _on_dispatch_phase_enemy_end(Actor* actor);

#endif
