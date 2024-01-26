#include "area.h"

extern ActorBlueprint A(goomba);
extern ActorBlueprint A(paragoomba);
extern ActorBlueprint A(spiked_goomba);

extern Stage A(kmr_02);
extern Stage A(kmr_03);
extern Stage A(kmr_04);
extern Stage A(kmr_05);
extern Stage A(kmr_06);

Formation A(Formation_00) = { // kmr_05
    ENEMY0(goomba),
    ENEMY1_ITEM(goomba, ITEM_MUSHROOM)
};

Formation A(Formation_01) = { // kmr_09
    ENEMY0(goomba),
    ENEMY1(goomba),
    ENEMY_AIR2_ITEM(paragoomba, ITEM_MUSHROOM),
};

Formation A(Formation_02) = { // kmr_03
    ENEMY0(goomba),
    ENEMY1(spiked_goomba),
    ENEMY2(goomba),
};

Formation A(Formation_03) = { // kmr_05
    ENEMY0(goomba),
    ENEMY_AIR1(paragoomba),
    ENEMY2(goomba),
};

Formation A(Formation_04) = { // kmr_09
    ENEMY0(goomba),
    ENEMY1(goomba),
    ENEMY2_ITEM(goomba, ITEM_GOOMNUT),
    ENEMY3(spiked_goomba),
};

Formation A(Formation_05) = { // kmr_12
    ENEMY0(goomba),
    ENEMY1(spiked_goomba),
    ENEMY_AIR2(paragoomba),
};

Formation A(Formation_06) = { // placeholder?
    ENEMY0(goomba),
    ENEMY_AIR1(paragoomba),
    ENEMY2(goomba),
    ENEMY_AIR3(paragoomba),
};

Formation A(Formation_07) = { // unused
    ENEMY_AIR0(paragoomba),
    ENEMY_AIR1(paragoomba),
};

Formation A(Formation_08) = { // kmr_05
    ENEMY_AIR0(paragoomba),
    ENEMY_AIR1(paragoomba),
    ENEMY_AIR2(paragoomba),
};

Formation A(Formation_09) = { // kmr_06
    ENEMY_AIR1(paragoomba),
    ENEMY_AIR2(paragoomba),
    ENEMY3_ITEM(spiked_goomba, ITEM_LIFE_SHROOM),
};

Formation A(Formation_0A) = { // kmr_05
    ENEMY0_ITEM(spiked_goomba, ITEM_LIFE_SHROOM),
};

Formation A(Formation_0B) = { // kmr_06
    ENEMY0(spiked_goomba),
    ENEMY1(spiked_goomba),
    ENEMY_AIR2_ITEM(paragoomba, ITEM_THUNDER_BOLT),
};

BattleList A(Formations) = {
    BATTLE(A(Formation_00), A(kmr_04), "クリボー"),
    BATTLE(A(Formation_01), A(kmr_04), "クリボーx２"),
    BATTLE(A(Formation_02), A(kmr_04), "クリボーx３"),
    BATTLE(A(Formation_03), A(kmr_04), "クリボー、パタクリボー"),
    BATTLE(A(Formation_04), A(kmr_04), "クリボーx４"),
    BATTLE(A(Formation_05), A(kmr_04), "クリボー,トゲクリボー"),
    BATTLE(A(Formation_06), A(kmr_04), "クリボー,パタクリボー,クリボー,パタクリボー"),
    BATTLE(A(Formation_07), A(kmr_04), "パタクリボー"),
    BATTLE(A(Formation_08), A(kmr_04), "パタクリボーx２"),
    BATTLE(A(Formation_09), A(kmr_04), "パタクリボーx３"),
    BATTLE(A(Formation_0A), A(kmr_04), "トゲクリボー"),
    BATTLE(A(Formation_0B), A(kmr_04), "トゲクリボー、クリボー"),
    {},
};

StageList A(Stages) = {
    STAGE("kmr_02", A(kmr_02)),
    STAGE("kmr_03", A(kmr_03)),
    STAGE("kmr_04", A(kmr_04)),
    STAGE("kmr_05", A(kmr_05)),
    STAGE("kmr_06", A(kmr_06)),
    {},
};
