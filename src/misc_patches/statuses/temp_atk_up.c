#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE temp_atk_up

#define temp_atk_up_DECREMENT_LATE TRUE

extern EvtScript EVS_PlaySleepHitFX;

void N(on_apply)(Actor* target, Vec3f position) {
    EffectInstance* debuffEffect = fx_debuff(2, position.x, position.y, position.z);

    debuffEffect->data.debuff->primCol.r = 255;
    debuffEffect->data.debuff->primCol.g = 165;
    debuffEffect->data.debuff->primCol.b = 0;

    debuffEffect->data.debuff->envCol.r = 255;
    debuffEffect->data.debuff->envCol.g = 165 + 12;
    debuffEffect->data.debuff->envCol.b = 0 + 12;

    sfx_play_sound_at_position(SOUND_INFLICT_CHILL_OUT, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);
}

void N(create_icon)(Actor* target) {
    create_custom_status_icon(target->hudElementDataIndex, ATK_UP_ICON_ID);
}

void N(remove_icon)(s32 id) {
    remove_custom_status_icon(id, ATK_UP_ICON_ID);
}

#undef NAMESPACE
