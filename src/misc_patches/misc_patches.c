#include "common.h"
#include "common_structs.h"
#include "custom_status.h"
#include "enemy_items/api.h"
#include "move_enum.h"
#include "variables.h"
#include "hud_element.h"
#include "misc_patches/status_rework_helpers.h"
#include "battle/battle_menu.h"
#include "misc_patches/scrollable_desc_draw.h"

#include "misc_events.c"
#include "actor_interfaces.c"
#include "item_effects.c"

// returns how much fp costs are changed by
s32 getFpCostChange(Actor* player) {
    s32 fpCost = 0;

    fpCost -= player_team_is_ability_active(player, ABILITY_FLOWER_SAVER);
    fpCost -= player_team_is_ability_active(player, ABILITY_FLOWER_FANATIC) * 2;
    fpCost += custom_status_get_potency(player, FP_COST_STATUS);

    return fpCost;
}

s32 getDamageChangeFromStatus(Actor* actor) {
    s32 change = 0;

    change += custom_status_get_potency(actor, ATK_UP_TEMP_STATUS);
    change -= custom_status_get_potency(actor, ATK_DOWN_TEMP_STATUS);
    change += custom_status_get_potency(actor, CHARGE_STATUS);

    return change;
}

void markActorAsNotAttackedThisTurn(Actor* actor) {
    actor->attackedThisTurn = false;
}

API_CALLABLE(MarkActorAsNotAttackedThisTurn) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    markActorAsNotAttackedThisTurn(actor);
    return ApiStatus_DONE2;
}

void clearChargesFrom(Actor* actor) {
    custom_status_clear(actor, CHARGE_STATUS);
}

API_CALLABLE(GetPartnerKoDuration) {
    evt_set_variable(script, *script->ptrReadPos, gBattleStatus.partnerActor->koDuration);
    return ApiStatus_DONE2;
}

// actor, chanceToTargetPlayer(in percent), out isPlayer
API_CALLABLE(TargetPlayerOrPartner) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    s32 chanceToTargetPlayer = evt_get_variable(script, *args++);

    actor->targetPartID = 1;
    if (gBattleStatus.partnerActor->koDuration > 0 || (rand_int(100) < chanceToTargetPlayer)) {
        actor->targetActorID = ACTOR_PLAYER;
        evt_set_variable(script, *args++, true);
    } else {
        actor->targetActorID = ACTOR_PARTNER;
        evt_set_variable(script, *args++, false);
    }

    return ApiStatus_DONE2;
}

s32 get_focus_cap() {
    s32 cap = 1;
    cap += is_ability_active(ABILITY_DEEP_FOCUS);
    return cap;
}

// Used by Trial Boo, needs to be somewhere global
void* gCurrentTrial = nullptr;
s32 gCurrentTrialFormationId = 0;

void _onActorCtor(Actor* actor) {
    enemy_items_zero_initialize(actor);
    custom_status_zero_initialize(actor);
    actor->attackedThisTurn = false;
    actor->overridenLevel = -1;
    actor->onEnemyDamagedScript = nullptr;
}

API_CALLABLE(OverrideActorLevel) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor;

    s32 actorID = evt_get_variable(script, *args++);
    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }
    actor = get_actor(actorID);

    s32 level = evt_get_variable(script, *args++);

    actor->overridenLevel = level;

    return ApiStatus_DONE2;
}

// Challenge Runs

b32 is_midas_stone() {
    return find_item(ITEM_MIDAS_STONE) != -1;
}

b32 is_ascetic_note() {
    return find_item(ITEM_ASCETIC_NOTE) != -1;
}

b32 is_lone_idol() {
    return find_item(ITEM_LONE_IDOL) != -1;
}

b32 is_orb_of_lug() {
    return find_item(ITEM_ORB_OF_LUG) != -1;
}

s32 get_bp_cost_of_move(s32 moveId) {
    s32 bp = gMoveTable[moveId].costBP;
    if (is_orb_of_lug()) {
        if (bp > 1)
            bp = 1;
        else if (bp < -1)
            bp = -1;
    }
    return bp;
}

// New function to setup cost reductions for the menu, which does not duplicate logic
void setup_move_cost_reductions(Actor* playerActor, s32 i)
{
    s32 change = getFpCostChange(playerActor);
    s32 color;

    switch (change) {
        case 0:
            color = 0;
            break;
        case -1:
            color = 1;
            break;
        default:
            color = change <= -2 ? 2 : 0;
        break;
    }

    MovesOptionDiscounts[i] = -change;
    MovesOptionDiscountColors[i] = color;
}

s32 fixed_star_power_index(s32 moveId) {
    if (moveId == MOVE_THREAT_FOCUS)
        moveId = MOVE_FOCUS;
    return STAR_POWER_INDEX(moveId);
}

void _onFrame() {
    draw_item_gc();

    if (is_midas_stone()) {
        // Intentionally put this before ascetic note.
        gPlayerData.coins = 999;
    }

    if (is_ascetic_note()) {
        gPlayerData.coins = 0;
        gPlayerData.starPieces = 0;
    }

    if (is_orb_of_lug()) {
        gPlayerData.maxBP = 24;
        gPlayerData.starPoints = 0;
    }

    if (is_lone_idol() && gBattleStatus.partnerActor != nullptr
        && gPlayerData.curPartner != PARTNER_GOOMPA
        && gPlayerData.curPartner != PARTNER_GOOMBARIA
        && gPlayerData.curPartner != PARTNER_TWINK) {
        gBattleStatus.flags2 |= BS_FLAGS2_PARTNER_TURN_USED;
    }
}

API_CALLABLE(BindOnEnemyDamaged) {
    Bytecode* args = script->ptrReadPos;
    s32 actorID = evt_get_variable(script, *args++);

    if (actorID == ACTOR_SELF) {
        actorID = script->owner1.actorID;
    }

    get_actor(actorID)->onEnemyDamagedScript = (EvtScript*) evt_get_variable(script, *args++);
    return ApiStatus_DONE2;
}

void _on_dispatch_event_actor(Actor* actor, s32 event) {
    status_rework_on_dispatch_event_actor(actor, event);

    switch (event)
    {
        case EVENT_HIT:
        case EVENT_DEATH:
        case EVENT_BURN_HIT:
        case EVENT_BURN_DEATH:
        case EVENT_POWER_BOUNCE_HIT:
        case EVENT_POWER_BOUNCE_DEATH:
        case EVENT_SHOCK_HIT:
        case EVENT_SHOCK_DEATH:
        case EVENT_SPIN_SMASH_HIT:
        case EVENT_SPIN_SMASH_DEATH:
        case EVENT_SPIN_SMASH_LAUNCH_HIT:
        case EVENT_SPIN_SMASH_LAUNCH_DEATH:
        case EVENT_HIT_COMBO:
        case EVENT_FLIP_TRIGGER:
        case EVENT_FALL_TRIGGER:
            if (gBattleStatus.lastAttackDamage > 0
                && actor != gBattleStatus.playerActor
                && actor != gBattleStatus.partnerActor)
                for (s32 i = 0; i < ARRAY_COUNT(gBattleStatus.enemyActors); i++)
                {
                    Actor* actor = gBattleStatus.enemyActors[i];
                    if (actor != nullptr && actor->onEnemyDamagedScript != nullptr) {
                        Evt* script = start_script(actor->onEnemyDamagedScript, EVT_PRIORITY_A, EVT_FLAG_RUN_IMMEDIATELY);
                        script->owner1.actorID = actor->actorID;
                    }
                }
            break;
    }
}

s32 player_count_badges_with_move_id(s32 moveId) {
    s32 sum = 0;
    for (s32 idx = 0; idx < ARRAY_COUNT(gPlayerData.equippedBadges); idx++) {
        s32 badgeMoveID = gItemTable[gPlayerData.equippedBadges[idx]].moveID;
        if (badgeMoveID == moveId) {
            sum += 1;
        }
    }
    return sum;
}

s32 get_focus_move_id() {
    if (player_count_badges_with_move_id(MOVE_THREAT_FOCUS))
        return MOVE_THREAT_FOCUS;
    if (player_count_badges_with_move_id(MOVE_PAIN_FOCUS))
        return MOVE_PAIN_FOCUS;

    return MOVE_FOCUS;
}

s8 getMaxStarEnergy() {
    s8 max = gPlayerData.maxStarPower;

    max += player_count_badges_with_move_id(MOVE_SP_PLUS);

    return max;
}

extern HudScriptList SPIncrementHudScripts;
extern HudScriptList SPStarHudScripts;
extern HudScript HES_StatusSPEmptyIncrement;
extern HudScript HES_StatusStarEmpty;
extern s32 StatusBarSPIncrementOffsets[];
extern HudElementList* gHudElements;
extern HudScript HES_StatusStarRainbow;
extern HudScript HES_StatusStarWhite;
extern HudScript HES_StatusSPIncrementRainbow;
extern HudScript HES_StatusSPIncrementWhite;

static u32 frameCounter = 0;

// hue is in [0-360], s and v are in [0-1]
Color_RGB8 hsv_to_rgb(float hue, float s, float v) {
    int i;
    float f, p, q, t;
    float r, g, b;

    if (s == 0) {
        r = g = b = v;
    } else {
        hue /= 60.0f;
        i = (int)hue;
        f = hue - i;
        p = v * (1.0f - s);
        q = v * (1.0f - s * f);
        t = v * (1.0f - s * (1.0f - f));
        switch (i) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            default:
                r = v;
                g = p;
                b = q;
                break;
        }
    }

    Color_RGB8 color;
    color.r = (u8)(r * 255.0f);
    color.g = (u8)(g * 255.0f);
    color.b = (u8)(b * 255.0f);
    return color;
}

void render_se_bar(s32 id, s32 x, s32 y, s32 startSegment, s32 limit) {
    // 7 in vanilla
    #define MAX_SE_ICONS 8
    s32 i = startSegment;
    s32 maxStarPower = getMaxStarEnergy();
    s32 maxSegments = maxStarPower * (SP_PER_BAR / SP_PER_SEG);

    for (; i < maxSegments; i++) {
        b32 isActive = i < limit;
        s32 currentSegment = i % (SP_PER_BAR / SP_PER_SEG);
        s32 currentSpUnit = i / (SP_PER_BAR / SP_PER_SEG);
        s32 currentSpUnitToRender = currentSpUnit;
        b32 isRainbow = currentSpUnit >= 7;
        b32 isStar = currentSegment == (SP_PER_BAR / SP_PER_SEG) - 1;

        if (!isActive && currentSpUnitToRender > 7)
            break;

        HudScriptPtr script;
        if (!isActive) {
            script = isStar ? HES_StatusStarEmpty : HES_StatusSPEmptyIncrement;
        } else if (isRainbow) {
            script = isStar ? HES_StatusStarRainbow : HES_StatusSPIncrementRainbow;
        } else if (currentSpUnitToRender + 7 <= maxStarPower - 1) {
            script = isStar ? HES_StatusStarWhite : HES_StatusSPIncrementWhite;
        } else {
            script = isStar ? SPStarHudScripts[MIN(currentSpUnitToRender, MAX_SE_ICONS - 1)] : SPIncrementHudScripts[MIN(currentSpUnitToRender, MAX_SE_ICONS - 1)];
        }
        hud_element_set_script(id, script);

        if (isStar) {
            hud_element_set_render_pos(id, x + 12 + ((currentSpUnit % 7) * 20), y);
        } else {
            hud_element_set_render_pos(id, x + ((currentSpUnit % 7) * 20) + StatusBarSPIncrementOffsets[currentSegment], y - 2);
        }

        if (isRainbow) {
            Color_RGB8 color = hsv_to_rgb(((i * 2) + frameCounter) % 360, 0.65, 1);
            hud_element_set_tint(id, color.r, color.g, color.b);
        } else {
            hud_element_set_tint(id, 255, 255, 255);
        }

        hud_element_draw_next(id);
    }

    frameCounter++;
}
