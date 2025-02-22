#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"
#include "effects.h"

#define NAMESPACE pain_focus_status

#define pain_focus_status_DECREMENT_LATE TRUE

void N(on_apply)(Actor* actor, Vec3f position, u8 potency) {
    EffectInstance* debuffEffect = fx_debuff(2, position.x, position.y, position.z);
    debuffEffect->data.debuff->primCol.r = 66;
    debuffEffect->data.debuff->primCol.g = 135;
    debuffEffect->data.debuff->primCol.b = 245;

    debuffEffect->data.debuff->envCol.r = 66;
    debuffEffect->data.debuff->envCol.g = 135;
    debuffEffect->data.debuff->envCol.b = 245;

    sfx_play_sound_at_position(SOUND_MAGIKOOPA_POWER_UP, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);
}

void N(create_icon)(Actor* target) {
    create_custom_status_icon(target, PAIN_FOCUS_ICON_ID);
}

void N(remove_icon)(s32 id) {
    remove_custom_status_icon(id, PAIN_FOCUS_ICON_ID);
}

void N(on_decrement)(Actor* target) {
    StatusInfo* info = custom_status_get_info(target, PAIN_FOCUS_STATUS);
    s32 dmg = info->potency - 1;
    s32 recovered = 0;

    while (dmg > 8) {
        recovered += SP_PER_SEG / 4;
        dmg--;
    }
    while (dmg > 4) {
        recovered += SP_PER_SEG / 2;
        dmg--;
    }
    while (dmg > 0) {
        recovered += SP_PER_SEG;
        dmg--;
    }

    add_star_power(recovered);
    info->potency = 1;
}

#undef NAMESPACE
