#include "status_rework_helpers.h"

void status_rework_on_dispatch_event_actor(Actor* actor, s32 event) {
    if (actor->debuff == STATUS_KEY_SLEEP
    && gBattleStatus.lastAttackDamage > 0
    && !(gBattleStatus.curAttackStatus & STATUS_FLAG_SLEEP)) {
        switch (event)
        {
        case EVENT_HIT:
        case EVENT_BURN_HIT:
        case EVENT_POWER_BOUNCE_HIT:
        case EVENT_SHOCK_HIT:
        case EVENT_SPIN_SMASH_HIT:
        case EVENT_SPIN_SMASH_LAUNCH_HIT:
            actor->debuffDuration = 0;
            actor->debuff = 0;
            actor->disableEffect->data.disableX->koDuration = 0;
            actor->koStatus = 0;
            actor->koDuration = 0;
            remove_status_debuff(actor->hudElementDataIndex);
            break;
        default:
            break;
        }
    }
}
