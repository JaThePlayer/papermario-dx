#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE poison_status

#define poison_status_DECREMENT_LATE FALSE

extern EvtScript EVS_PlaySleepHitFX;

void N(on_apply)(Actor* actor, Vec3f position, u8 potency) {
    set_actor_pal_adjustment(actor, ACTOR_PAL_ADJUST_POISON);

    sfx_play_sound_at_position(SOUND_INFLICT_STATUS, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);

    EffectInstance* debuffEffect = fx_debuff(2, position.x, position.y, position.z);
    debuffEffect->data.debuff->primCol.r = 60;
    debuffEffect->data.debuff->primCol.g = 160;
    debuffEffect->data.debuff->primCol.b = 0;
    debuffEffect->data.debuff->envCol.r = 90;
    debuffEffect->data.debuff->envCol.g = 240;
    debuffEffect->data.debuff->envCol.b = 0;
}

void N(create_icon)(Actor* target) {
    create_custom_status_icon(target, POISON_ICON_ID);
}

void N(remove_icon)(s32 id) {
    remove_custom_status_icon(id, POISON_ICON_ID);
}

void N(on_decrement)(Actor* target) {
    s32 dmg = custom_status_get_potency(target, POISON_STATUS);
    dmg += badge_count_by_move_id_in_both_teams(MOVE_SLOW_GO);
    target->state.goalPos = target->curPos;

    if (target == gBattleStatus.playerActor) {
        dispatch_damage_event_player(dmg, EVENT_HIT, FALSE);
    } else {
        dispatch_damage_event_actor_1(target, dmg, EVENT_HIT);
    }
}

#undef NAMESPACE
