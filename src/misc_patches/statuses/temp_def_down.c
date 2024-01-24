#include "misc_patches/custom_status.h"
#include "misc_patches/custom_status_icons.h"

#define NAMESPACE temp_def_down

extern EvtScript EVS_PlaySleepHitFX;

void N(on_apply)(Actor* target, Vec3f position) {
    Evt* evt = start_script(&EVS_PlaySleepHitFX, EVT_PRIORITY_A, 0);
    evt->varTable[0] = position.x;
    evt->varTable[1] = position.y;
    evt->varTable[2] = position.z;
    sfx_play_sound_at_position(SOUND_INFLICT_SLEEP, SOUND_SPACE_DEFAULT, position.x, position.y, position.z);
}

void N(create_icon)(Actor* target) {
    create_custom_status_icon(target->hudElementDataIndex, DEF_DOWN_ICON_ID);
}

void N(remove_icon)(s32 id) {
    remove_custom_status_icon(id, DEF_DOWN_ICON_ID);
}

#undef NAMESPACE
