#include "misc_patches/misc_events.h"
#include "battle/states/states.h"
#include "common_structs.h"
#include "effects.h"
#include "variables.h"
#include "misc_patches/misc_patches.h"

void _on_battle_start() {
    BattleStatus* battleStatus = &gBattleStatus;

    battleStatus->turnCounter = 0;
    battleStatus->focusUses = 0;
    battleStatus->extraBurnDamage = 0;
}

void _on_turn_start() {
    BattleStatus* battleStatus = &gBattleStatus;

    battleStatus->turnCounter++;
}

void _on_handle_passive_se_regen() {
    PlayerData* playerData = &gPlayerData;

    // New in armageddon: Passive SP regen 3 times per battle.
    s32 starCharms = player_count_badges_with_move_id(MOVE_PRETTY_LUCKY);
    s32 starTuners = player_count_badges_with_move_id(MOVE_LUCKY_DAY);
    if (starTuners > 0 || gBattleStatus.turnCounter <= (starCharms + 3)) {
        playerData->starPower += SP_PER_SEG * (starCharms + (starTuners * 2) + 1);
        if (playerData->starPower > getMaxStarEnergy() * SP_PER_BAR) {
            playerData->starPower = getMaxStarEnergy() * SP_PER_BAR;
        }
    }
}

static void update_actor_ko(Actor* actor) {
    s32 oldKoDuration = actor->koDuration;
    actor->koDuration = actor->debuffDuration;
    if (actor->koDuration > 0) {
        actor->koStatus = STATUS_KEY_KO;
        actor->disableEffect->data.disableX->koDuration = actor->koDuration;
    } else if (oldKoDuration != actor->koDuration) {
        actor->koStatus = 0;
        actor->disableEffect->data.disableX->koDuration = 0;
    }
}

void _on_dispatch_phase_enemy_end(Actor* actor) {
    // New: handle dizzy and shrink later
    if (actor != NULL && actor->debuffDuration >= 0 && (actor->debuff == STATUS_KEY_DIZZY || actor->debuff == STATUS_KEY_SHRINK)) {
        actor->debuffDuration--;
        if (actor->debuffDuration <= 0) {
            actor->debuff = 0;
            remove_status_debuff(actor->hudElementDataIndex);
            dispatch_event_actor(actor, EVENT_RECOVER_STATUS);
            BattleStatusUpdateDelay = 20;

            update_actor_ko(actor);
        }
    }
}
