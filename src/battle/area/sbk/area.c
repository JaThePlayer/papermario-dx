#include "area.h"

#include "battle/common/actor/dry_bubble.inc.c"

#include "actor/bandit_leader.c"

extern ActorBlueprint A(pokey);
extern ActorBlueprint A(bandit);

extern Stage A(sbk_02);

Formation A(Formation_00) = { // plenty
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_C, 9),
};

Formation A(Formation_01) = { // plenty
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_C, 9),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_D, 8),
};

Formation A(Formation_02) = { // plenty
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_C, 8),
};

Formation A(Formation_03) = { // plenty
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_AIR_B, 8),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_C, 7),
};

Formation A(Formation_04) = { // few
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 9),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_AIR_C, 8),
};

Formation A(Formation_05) = { // few
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 8),
};

Formation A(Formation_06) = { // 26, 46
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 8),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_D, 7),
};

Formation A(Formation_07) = { // 15
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX_ITEM(A(bandit), BTL_POS_GROUND_B, 9, ITEM_SPICY_SOUP),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_C, 8),
};

Formation A(Formation_08) = { // 22
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 8),
    ACTOR_BY_IDX_ITEM(A(pokey), BTL_POS_GROUND_D, 7, ITEM_MUSHROOM),
};

Formation A(Formation_09) = { // 66
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_C, 9),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_D, 8),
};

Formation A(Formation_0A) = { // plenty
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 9),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_D, 8),
};

Formation A(Formation_0B) = { // plenty
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 8),
};

Formation A(Formation_0C) = { // plenty
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(dry_bubble), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 8),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_D, 7),
};

Formation A(Formation_0D) = { // 11, 46
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX_ITEM(A(pokey), BTL_POS_GROUND_C, 9, ITEM_NUTTY_CAKE),
};

Formation A(Formation_0E) = { // 4x
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_C, 8),
};

Formation A(Formation_0F) = { // 05 (tons of pokeys room), 66
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_C, 8),
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_D, 7),
};

Formation A(Formation_10) = { // 05 (tons of pokeys room), 66
    ACTOR_BY_IDX(A(pokey), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX_ITEM(A(pokey), BTL_POS_GROUND_B, 9, ITEM_FRIED_EGG),
    ACTOR_BY_IDX(A(bandit), BTL_POS_GROUND_C, 8),
};

Formation A(Formation_11) = { // MINIBOSS
    ACTOR_BY_IDX(A(bandit_leader), BTL_POS_GROUND_C, 10),
};

BattleList A(Formations) = {
    BATTLE(A(Formation_00), A(sbk_02), "サンボ"),
    BATTLE(A(Formation_01), A(sbk_02), "サンボx2"),
    BATTLE(A(Formation_02), A(sbk_02), "サンボx3"),
    BATTLE(A(Formation_03), A(sbk_02), "きいろサンボx4"),
    BATTLE(A(Formation_04), A(sbk_02), "サンボ,ボロドー"),
    BATTLE(A(Formation_05), A(sbk_02), "サンボx2,ボロドー"),
    BATTLE(A(Formation_06), A(sbk_02), "サンボx2,ボロドーx2"),
    BATTLE(A(Formation_07), A(sbk_02), "サンボ,ボロドー,サンボ"),
    BATTLE(A(Formation_08), A(sbk_02), "サンボx2,ボロドー,サンボ"),
    BATTLE(A(Formation_09), A(sbk_02), "ボロドー"),
    BATTLE(A(Formation_0A), A(sbk_02), "ボロドーx2"),
    BATTLE(A(Formation_0B), A(sbk_02), "ボロドーx3"),
    BATTLE(A(Formation_0C), A(sbk_02), "ボロドーx4"),
    BATTLE(A(Formation_0D), A(sbk_02), "ボロドー,サンボ"),
    BATTLE(A(Formation_0E), A(sbk_02), "ボロドーx2,サンボ"),
    BATTLE(A(Formation_0F), A(sbk_02), "ボロドーx2,サンボx2"),
    BATTLE(A(Formation_10), A(sbk_02), "ボロドー,サンボ,ボロドー"),
    BATTLE(A(Formation_11), A(sbk_02), ""),
    {},
};

StageList A(Stages) = {
    STAGE("sbk_01", A(sbk_02)),
    {},
};
