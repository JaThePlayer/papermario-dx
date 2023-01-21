#include "kpa_61.h"

#include "world/common/enemy/complete/Koopatrol_Wander.inc.c"
#include "world/common/enemy/complete/FlyingMagikoopa.inc.c"

StaticNpc N(NpcData_Koopatrol_01) = {
    .id = NPC_Koopatrol_01,
    .settings = &N(NpcSettings_Koopatrol_Wander),
    .pos = { 300.0f, -160.0f, 140.0f },
    .yaw = 270,
    .flags = ENEMY_FLAG_400 | ENEMY_FLAG_800,
    .drops = KOOPATROL_DROPS,
    .territory = {
        .wander = {
            .isFlying = FALSE,
            .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED,
            .wanderShape = SHAPE_CYLINDER,
            .centerPos  = { 300, -160, 140 },
            .wanderSize = { 50 },
            .detectShape = SHAPE_CYLINDER,
            .detectPos  = { 300, -160, 140 },
            .detectSize = { 200 },
        }
    },
    .animations = KOOPATROL_ANIMS,
};

StaticNpc N(NpcData_Koopatrol_02) = {
    .id = NPC_Koopatrol_02,
    .settings = &N(NpcSettings_Koopatrol_Wander),
    .pos = { 850.0f, -160.0f, 390.0f },
    .yaw = 270,
    .flags = ENEMY_FLAG_400 | ENEMY_FLAG_800,
    .drops = KOOPATROL_DROPS,
    .territory = {
        .wander = {
            .isFlying = FALSE,
            .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED,
            .wanderShape = SHAPE_CYLINDER,
            .centerPos  = { 850, -160, 390 },
            .wanderSize = { 50 },
            .detectShape = SHAPE_CYLINDER,
            .detectPos  = { 850, -160, 390 },
            .detectSize = { 200 },
        }
    },
    .animations = KOOPATROL_ANIMS,
};

StaticNpc N(NpcData_FlyingMagikoopa_01)[] = {
    {
        .id = NPC_FlyingMagikoopa_01,
        .settings = &N(NpcSettings_FlyingMagikoopa),
        .pos = { 500.0f, 250.0f, -50.0f },
        .yaw = 90,
        .flags = ENEMY_FLAG_800,
        .drops = FLYING_MAGIKOOPA_DROPS,
        .territory = {
            .wander = {
                .isFlying = TRUE,
                .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED,
                .wanderShape = SHAPE_RECT,
                .centerPos  = { 500, 250, -50 },
                .wanderSize = { 120, 25 },
                .detectShape = SHAPE_CYLINDER,
                .detectPos  = { 500, 250, -50 },
                .detectSize = { 200 },
            }
        },
        .animations = FLYING_MAGIKOOPA_ANIMS,
        .extraAnimations = N(ExtraAnims_FlyingMagikoopa),
        .aiDetectFlags = AI_DETECT_SIGHT | AI_DETECT_SENSITIVE_MOTION,
    },
    FLYING_MAGIKOOPA_SPELL_HITBOX(NPC_FlyingMagikoopa_01_Spell),
};

StaticNpc N(NpcData_FlyingMagikoopa_02)[] = {
    {
        .id = NPC_FlyingMagikoopa_02,
        .settings = &N(NpcSettings_FlyingMagikoopa),
        .pos = { 200.0f, 250.0f, -50.0f },
        .yaw = 90,
        .flags = ENEMY_FLAG_800,
        .drops = FLYING_MAGIKOOPA_DROPS,
        .territory = {
            .wander = {
                .isFlying = TRUE,
                .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED,
                .wanderShape = SHAPE_RECT,
                .centerPos  = { 200, 250, -50 },
                .wanderSize = { 120, 25 },
                .detectShape = SHAPE_CYLINDER,
                .detectPos  = { 200, 250, -50 },
                .detectSize = { 200 },
            }
        },
        .animations = FLYING_MAGIKOOPA_ANIMS,
        .extraAnimations = N(ExtraAnims_FlyingMagikoopa),
        .aiDetectFlags = AI_DETECT_SIGHT | AI_DETECT_SENSITIVE_MOTION,
    },
    FLYING_MAGIKOOPA_SPELL_HITBOX(NPC_FlyingMagikoopa_02_Spell),
};

NpcGroupList N(DefaultNPCs) = {
    NPC_GROUP(N(NpcData_Koopatrol_01), BTL_KPA_FORMATION_23, BTL_KPA_STAGE_08),
    NPC_GROUP(N(NpcData_Koopatrol_02), BTL_KPA_FORMATION_21, BTL_KPA_STAGE_08),
    NPC_GROUP(N(NpcData_FlyingMagikoopa_01), BTL_KPA_FORMATION_35, BTL_KPA_STAGE_08),
    NPC_GROUP(N(NpcData_FlyingMagikoopa_02), BTL_KPA_FORMATION_33, BTL_KPA_STAGE_08),
    {}
};