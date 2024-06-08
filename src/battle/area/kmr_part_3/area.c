#include "area.h"

#include "battle/common/actor/wave_battle_controller.inc.c"

extern ActorBlueprint A(jr_troopa);
extern ActorBlueprint A(egg_jr_troopa);
extern ActorBlueprint A(para_jr_troopa);
extern ActorBlueprint A(spiked_para_jr_troopa);
extern ActorBlueprint A(mage_jr_troopa);
extern ActorBlueprint A(final_jr_troopa);

// Egg Jr Troopa Summons
extern ActorBlueprint A(fuzzy);
extern ActorBlueprint A(koopa_troopa);
extern ActorBlueprint A(egg_jr_troopa_final_wave_notifier);

extern Stage A(kmr_02);
extern Stage A(kmr_03);
extern Stage A(kmr_04);
extern Stage A(kmr_05b);
extern Stage A(kmr_06);
extern Stage A(nok_01);
extern Stage A(mim_01);
extern Stage A(sam_01);
extern Stage A(mac_01);
extern Stage A(kpa_13);

Formation A(Formation_00) = {
    ACTOR_BY_IDX(A(jr_troopa), BTL_POS_GROUND_C, 0),
};

Formation A(Formation_01) = {
    ACTOR_BY_IDX(A(jr_troopa), BTL_POS_GROUND_C, 10),
};

Formation A(Formation_02) = {
    ACTOR_BY_IDX(A(jr_troopa), BTL_POS_GROUND_C, 0),
};

Formation A(Formation_03_Wave4) = {
    ACTOR_BY_IDX(A(egg_jr_troopa_final_wave_notifier), BTL_POS_GROUND_A, 10),
};

Formation A(Formation_03_Wave3) = {
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(fuzzy), BTL_POS_GROUND_B, 9),

    NEXT_WAVE(A(Formation_03_Wave4)),
};

Formation A(Formation_03_Wave2) = {
    ACTOR_BY_IDX(A(fuzzy), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(fuzzy), BTL_POS_GROUND_B, 9),

    NEXT_WAVE(A(Formation_03_Wave3)),
};

Formation A(Formation_03) = {
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_B, 9),

    ACTOR_BY_IDX(A(egg_jr_troopa), BTL_POS_GROUND_D, 11),
    WAVE_BATTLE(A(Formation_03_Wave2), 4),
};

Formation A(Formation_04) = {
    ACTOR_BY_IDX(A(para_jr_troopa), BTL_POS_GROUND_C, 0),
};

Formation A(Formation_05) = {
    ACTOR_BY_IDX(A(spiked_para_jr_troopa), BTL_POS_GROUND_B, 0),
};

Formation A(Formation_06) = {
    ACTOR_BY_IDX(A(mage_jr_troopa), BTL_POS_GROUND_C, 0),
};

Formation A(Formation_07) = {
    ACTOR_BY_IDX(A(final_jr_troopa), BTL_POS_GROUND_C, 0),
};

BattleList A(Formations) = {
    BATTLE(A(Formation_00), A(kmr_03), "トゲクリボー（レクチャー）"),
    BATTLE(A(Formation_01), A(kmr_03), "パタクリボー（レクチャー）"),
    BATTLE(A(Formation_02), A(kmr_05b), "コワッパ１"),
    BATTLE(A(Formation_03), A(nok_01), "コワッパ２"),
    BATTLE(A(Formation_04), A(mim_01), "コワッパ４"),
    BATTLE(A(Formation_05), A(mac_01), "コワッパ５"),
    BATTLE(A(Formation_06), A(sam_01), "コワッパ６"),
    BATTLE(A(Formation_07), A(kpa_13), "コワッパ７"),
    {},
};

StageList A(Stages) = {
    STAGE("kmr_02", A(kmr_02)),
    STAGE("kmr_03", A(kmr_03)),
    STAGE("kmr_04", A(kmr_04)),
    STAGE("kmr_05", A(kmr_05b)),
    STAGE("kmr_06", A(kmr_06)),
    {},
};
