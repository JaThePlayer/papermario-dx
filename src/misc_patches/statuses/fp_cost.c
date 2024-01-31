#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE fp_cost_status

#define fp_cost_status_DECREMENT_LATE TRUE

extern EvtScript EVS_PlaySleepHitFX;

void N(on_apply)(Actor* actor, Vec3f position, u8 potency) {
    EffectInstance* debuffEffect = fx_debuff(2, position.x, position.y, position.z);

    debuffEffect->data.debuff->primCol.r = 231;
    debuffEffect->data.debuff->primCol.g = 231;
    debuffEffect->data.debuff->primCol.b = 233;

    debuffEffect->data.debuff->envCol.r = 231;
    debuffEffect->data.debuff->envCol.g = 231;
    debuffEffect->data.debuff->envCol.b = 233;

    sfx_play_sound_at_position(SOUND_FLOWER_PICKUP, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);
}

void N(create_icon)(Actor* target) {
    create_custom_status_icon(target->hudElementDataIndex, FP_COST_DOWN_ICON_ID);
}

void N(remove_icon)(s32 id) {
    remove_custom_status_icon(id, FP_COST_DOWN_ICON_ID);
}

void N(on_decrement)(Actor* target) {

}

#undef NAMESPACE
