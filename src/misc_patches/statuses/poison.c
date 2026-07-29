#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE poison_status

#define poison_status_DECREMENT_LATE false

extern b32 dispatch_damage_event_player(s32 damageAmount, s32 event, b32 noHitSound);
extern EvtScript EVS_PlaySleepHitFX;

extern void set_actor_pal_adjustment(Actor* actor, s32 palAdjust);

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

extern ActorPart* get_actor_part_for_def(Actor* actor);

static s32 lookup_poison_defense(s32* defenseTable) {
    while (defenseTable[DICTIONARY_KEY] != ELEMENT_END) {
        if (defenseTable[DICTIONARY_KEY] == ELEMENT_POISON) {
            return defenseTable[DICTIONARY_VALUE];
        }
        defenseTable += DICTIONARY_SIZE;
    }

    return 0;
}

void N(on_decrement)(Actor* target) {
    s32 dmg = custom_status_get_potency(target, POISON_STATUS);
    dmg += badge_count_by_move_id_in_both_teams(MOVE_SLOW_GO);
    target->state.goalPos = target->curPos;

    s32 def = lookup_poison_defense(get_actor_part_for_def(target)->defenseTable);
    if (def < 0) {
        // Heal from poison instead.
        s32 hpBoost = dmg;
        s32 x1 = target->curPos.x + target->headOffset.x;
        s32 y1, y2;
        if (target->flags & ACTOR_FLAG_UPSIDE_DOWN) {
            y1 = target->curPos.y + target->headOffset.y - target->size.y / 2;
        } else if (!(target->flags & ACTOR_FLAG_HALF_HEIGHT)) {
            y1 = target->curPos.y + target->headOffset.y + target->size.y / 2;
        } else {
            y1 = target->curPos.y + target->headOffset.y + target->size.y;
        }
        s32 z1 = target->curPos.z + target->headOffset.z + 10.0f;

        s32 x2 = target->curPos.x + target->headOffset.x + target->size.x / 2;
        if (target->flags & ACTOR_FLAG_UPSIDE_DOWN) {
            y2 = target->curPos.y + target->headOffset.y - target->size.y;
        } else if (!(target->flags & ACTOR_FLAG_HALF_HEIGHT)) {
            y2 = target->curPos.y + target->headOffset.y + target->size.y;
        } else {
            y2 = target->curPos.y + target->headOffset.y + target->size.y * 2;
        }
        s32 z2 = target->curPos.z + target->headOffset.z + 10.0f;


        fx_recover(0, x2, y2, z2, hpBoost);
        show_start_recovery_shimmer(x1, y1, z1, hpBoost);
        target->curHP += hpBoost;
        if (target->maxHP < target->curHP) {
            target->curHP = target->maxHP;
        }
        return;
    }

    dmg -= def;
    if (dmg < 0)
        dmg = 0;

    if (target == gBattleStatus.playerActor) {
        dispatch_damage_event_player(dmg, EVENT_HIT, false);
    } else {
        dispatch_damage_event_actor_1(target, dmg, EVENT_HIT);
    }
}

#undef NAMESPACE
