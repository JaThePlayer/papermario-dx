#pragma once

#include "misc_patches/actor_interfaces.h"

typedef struct GroundedGloombaAnims {
    s32 idle;
    s32 midair;
    s32 dizzy;
    s32 sleep;
    s32 hurt;
    s32 tense;
    s32 run;
} GroundedGloombaAnims;

typedef struct IGroundedGloomba {
    GroundedGloombaAnims anims;
    s32 headbonkDamage;
    s32 friendlyHeadbonkDamage;
    s32 buffedHeadbonkDamage;
} IGroundedGloomba;
