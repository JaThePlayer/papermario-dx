#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE burn_status

#define burn_status_DECREMENT_LATE FALSE

extern EvtScript EVS_PlaySleepHitFX;

void N(on_apply)(Actor* actor, Vec3f position, u8 potency) {
    fx_ring_blast(0, position.x, position.y, position.z + 5.0f, 1.0f, 24);
    sfx_play_sound_at_position(SOUND_HIT_FIRE, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);
}

void N(create_icon)(Actor* target) {
    create_custom_status_icon(target, BURN_ICON_ID);
}

void N(remove_icon)(s32 id) {
    remove_custom_status_icon(id, BURN_ICON_ID);
}

void N(on_decrement)(Actor* target) {
    StatusInfo* info = custom_status_get_info(target, BURN_STATUS);
    s32 dmg = info->potency;

    if (badge_count_by_move_id_in_opposing_team(target, MOVE_EMBER_EMBLEM) > 0) {
        dmg *= (info->turns + 1) / 2;
    }

    target->state.goalPos = target->curPos;

    if (target == gBattleStatus.playerActor) {
        dispatch_damage_event_player(dmg, EVENT_BURN_HIT, FALSE);
    } else {
        dispatch_damage_event_actor_1(target, dmg, EVENT_BURN_HIT);
    }
}

#undef NAMESPACE
