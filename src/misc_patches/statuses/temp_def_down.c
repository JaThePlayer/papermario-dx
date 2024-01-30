#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE temp_def_down

#define temp_def_down_DECREMENT_LATE TRUE

extern EvtScript EVS_PlaySleepHitFX;

void N(on_apply)(Actor* actor, Vec3f position, u8 potency) {
    EffectInstance* debuffEffect = fx_debuff(2, position.x, position.y, position.z);
    Vec3f arrowPos = get_expected_arrow_pos(actor);
    fx_stat_change(ARROW_TYPE_DEF_DOWN, potency, arrowPos.x, arrowPos.y, arrowPos.z, 1.0f, 60);

    debuffEffect->data.debuff->primCol.r = 135;
    debuffEffect->data.debuff->primCol.g = 206;
    debuffEffect->data.debuff->primCol.b = 250;

    debuffEffect->data.debuff->envCol.r = 135 + 12;
    debuffEffect->data.debuff->envCol.g = 206 + 12;
    debuffEffect->data.debuff->envCol.b = 250 + 5;

    sfx_play_sound_at_position(SOUND_INFLICT_CHILL_OUT, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);
}

void N(create_icon)(Actor* target) {
    create_custom_status_icon(target->hudElementDataIndex, DEF_DOWN_ICON_ID);
}

void N(remove_icon)(s32 id) {
    remove_custom_status_icon(id, DEF_DOWN_ICON_ID);
}

void N(on_decrement)(Actor* target) {
}

#undef NAMESPACE
