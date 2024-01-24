#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE temp_def_down

#define temp_def_down_DECREMENT_LATE TRUE

extern EvtScript EVS_PlaySleepHitFX;

void N(on_apply)(Actor* target, Vec3f position) {
    EffectInstance* debuffEffect = fx_debuff(2, position.x, position.y, position.z);

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

#undef NAMESPACE
