#include "area.h"

#include "battle/common/actor/wave_battle_controller.inc.c"
#include "battle/common/actor/goomba.inc.c"

Vec3i A(BlasterPos1) = { 50, 0, -20 };
Vec3i A(BlasterPos2) = { 80, 0, 0 };
Vec3i A(BlasterPos3) = { 110, 0, 20 };

extern ActorBlueprint A(koopa_troopa);
extern ActorBlueprint A(paratroopa);
extern ActorBlueprint A(bob_omb);
extern ActorBlueprint A(bill_blaster);
extern ActorBlueprint A(bullet_bill);
extern ActorBlueprint A(fire_bros);

extern Stage A(trd_00);
extern Stage A(trd_01);
extern Stage A(trd_02);
extern Stage A(trd_02b);
extern Stage A(trd_02c);
extern Stage A(trd_02d);
extern Stage A(trd_03);
extern Stage A(trd_04);
extern Stage A(trd_05);
extern Stage A(trd_05b);
extern Stage A(trd_05c);
extern Stage A(trd_05d);
extern Stage A(trd_05e);
extern Stage A(trd_05f);

/*
Koopa, Paratroopa,
Bobomb (needs var1 - ignition timer)
Fire Bros
Bullet Bill
Bill Blaster (miniboss)
*/

Formation A(Formation_03_Wave2) = { // trd_01 - KEY
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_A,  10, 0),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B,  9,  4),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_C, 8),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_D, 7),
};

Formation A(Formation_03) = { // trd_01 - KEY
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 9, 0),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_C, 8, 1),
    WAVE_BATTLE(A(Formation_03_Wave2), 2),
};


Formation A(Formation_0F) = { // trd_02
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_A, 10, 0), // troll
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 9, 3),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_C, 8, 3),
    ACTOR_BY_IDX(A(fire_bros), BTL_POS_GROUND_D, 7),
};

Formation A(Formation_10) = { // trd_02
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 10, 2),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_C, 9, 2),
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_D, 7),
};

Formation A(Formation_07) = { // trd_02
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 8, 3),
    ACTOR_BY_IDX(A(fire_bros), BTL_POS_GROUND_C, 7),
};


Formation A(Formation_09) = { // trd_03
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_B, 9),
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_C, 8),
};

Formation A(Formation_02) = { // trd_03
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(fire_bros), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_C, 8),
};

Formation A(Formation_04) = { // trd_03
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_C, 8),
};

s16 A(Formation_05_Wave2_HammerBroItems)[] = {
    ITEM_MUSHROOM, ITEM_POWER_PLUS_A,
};

Formation A(Formation_05_Wave2) = {
    ACTOR_BY_IDX_ITEMS(A(fire_bros), BTL_POS_GROUND_B, 10, A(Formation_05_Wave2_HammerBroItems)),
};

Formation A(Formation_05) = { // trd_04 - switch
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 9, 3),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_C, 8, 3),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_D, 7),
    WAVE_BATTLE(A(Formation_05_Wave2), 2),
};

s16 A(Formation_08_BackBobombItems)[] = {
    ITEM_GOOMNUT, ITEM_MUSHROOM,
};

Formation A(Formation_08) = { // trd_07 (ambush)
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_A, 10),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 9, 0),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_C, 8, 1),
    ACTOR_BY_IDX_ARG_ITEMS(A(bob_omb), BTL_POS_GROUND_D, 7, 2, A(Formation_08_BackBobombItems)),
};

Formation A(Formation_0A) = { // trd_07 (ambush)
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_A, 10),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_B, 9, 2, ITEM_GOOMNUT),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_C, 8, 1, ITEM_SPICY_SOUP),
};

Formation A(Formation_16) = { // trd_07 (ambush)
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_B, 9, 3, ITEM_POWER_PLUS_A),
    ACTOR_BY_IDX(A(fire_bros), BTL_POS_GROUND_C, 8),
};

s16 A(Formation_06_Wave3_GoombaItems)[] = {
    ITEM_HP_PLUS_A, ITEM_HP_PLUS_A, ITEM_POWER_PLUS_A
};

Formation A(Formation_06_Wave3) = {
    ACTOR_BY_IDX_ITEMS(A(goomba), BTL_POS_GROUND_B, 10, A(Formation_06_Wave3_GoombaItems)),
};

Formation A(Formation_06_Wave2) = {
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_A, 10, 2, ITEM_P_DOWN_D_UP),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_B, 9, 2, ITEM_P_DOWN_D_UP),
    ACTOR_BY_IDX_ARG(A(koopa_troopa), BTL_POS_GROUND_C, 8, TRUE),
    ACTOR_BY_IDX_ARG(A(koopa_troopa), BTL_POS_GROUND_D, 7, TRUE),
    NEXT_WAVE(A(Formation_06_Wave3)),
};

Formation A(Formation_06) = { // trd_06 - jailers
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 9, 0),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_C, 8, 1),
    WAVE_BATTLE(A(Formation_06_Wave2), 3)
};

Formation A(Formation_0D) = { // trd_04 - upper
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_A, 10),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_B, 9, 2, ITEM_THUNDER_BOLT),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_C, 8, 1),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_D, 8),
};

Formation A(Formation_00) = { // trd_01 (upper)
    ACTOR_BY_IDX_ITEM(A(koopa_troopa), BTL_POS_GROUND_B, 10, ITEM_VOLT_SHROOM),
    ACTOR_BY_IDX_ITEM(A(koopa_troopa), BTL_POS_GROUND_C, 9, ITEM_VOLT_SHROOM),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_D, 8, 4, ITEM_VOLT_SHROOM),
};

Formation A(Formation_01) = { // trd_01 (upper)
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX_ITEM(A(fire_bros), BTL_POS_GROUND_C, 9, ITEM_P_DOWN_D_UP),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_D, 8, 2, ITEM_MUSHROOM),
};

Formation A(Formation_11) = { // trd_01 (upper)
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_A, 10, 1, ITEM_HP_PLUS_A),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_B, 9, 1, ITEM_SLEEPY_SHEEP),
    ACTOR_BY_IDX_ARG_ITEM(A(bob_omb), BTL_POS_GROUND_C, 8, 1, ITEM_SLEEPY_SHEEP),
};

Formation A(Formation_17) = { // trd_09
    ACTOR_BY_POS(A(bill_blaster), A(BlasterPos1), 10),
    ACTOR_BY_POS(A(bill_blaster), A(BlasterPos2), 9),
    ACTOR_BY_POS(A(bill_blaster), A(BlasterPos3), 8),
};

Formation A(Formation_0B) = {
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_A, 10),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_C, 8),
};

Formation A(Formation_0C) = {
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_A, 10),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_C, 8),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_D, 7),
};

Formation A(Formation_1A) = { // trd_09
    ACTOR_BY_IDX(A(bullet_bill), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(bullet_bill), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(bullet_bill), BTL_POS_GROUND_C, 8),
};


Formation A(Formation_0E) = {
    ACTOR_BY_IDX(A(paratroopa), BTL_POS_AIR_A, 10),
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_C, 9),
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_D, 8),
};

Formation A(Formation_13) = {
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_C, 9),
};

Formation A(Formation_14) = {
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX(A(koopa_troopa), BTL_POS_GROUND_C, 8),
};

// BOO TRIAL - CHAPTER 1

Formation A(Formation_19) = { // Boo Trial - ch1 - part 4
    ACTOR_BY_IDX(A(bullet_bill), BTL_POS_AIR_A, 10),
    ACTOR_BY_IDX(A(bullet_bill), BTL_POS_AIR_B, 9),
    ACTOR_BY_IDX(A(bullet_bill), BTL_POS_AIR_C, 8),
    ACTOR_BY_IDX(A(bullet_bill), BTL_POS_AIR_D, 7),
};

s16 A(Formation_18_GoombaItems)[] = {
    ITEM_HP_PLUS_A, ITEM_HP_PLUS_A, ITEM_FIRE_FLOWER
};

s16 A(Formation_18_FireBrosItems)[] = {
    ITEM_HP_PLUS_A, ITEM_EMBER_EMBLEM
};

Formation A(Formation_18) = { // Boo Trial - ch1 - part 5
    ACTOR_BY_IDX(A(bob_omb), BTL_POS_GROUND_A, 10),
    ACTOR_BY_IDX(A(fire_bros), BTL_POS_GROUND_B, 9),
    ACTOR_BY_IDX_ITEMS(A(fire_bros), BTL_POS_GROUND_C, 8, A(Formation_18_FireBrosItems)),
    ACTOR_BY_IDX_ITEMS(A(goomba), BTL_POS_GROUND_D, 7, A(Formation_18_GoombaItems)),
};

Formation A(Formation_12) = { // Boo Trial - ch1 - part 6
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_A, 10, 2),
    ACTOR_BY_IDX_ARG(A(bob_omb), BTL_POS_GROUND_B, 9, 2),
    ACTOR_BY_IDX_ARG(A(koopa_troopa), BTL_POS_GROUND_C, 8, TRUE),
    ACTOR_BY_IDX_ARG(A(koopa_troopa), BTL_POS_GROUND_D, 7, TRUE),
};

Formation A(Formation_15) = { // Boo Trial - ch1 - part 7
    ACTOR_BY_POS(A(bill_blaster), A(BlasterPos1), 10),
    ACTOR_BY_POS(A(bill_blaster), A(BlasterPos2), 9),
    ACTOR_BY_POS(A(bill_blaster), A(BlasterPos3), 8),
};

BattleList A(Formations) = {
    BATTLE(A(Formation_00), A(trd_01), "ノコノコ"),
    BATTLE(A(Formation_01), A(trd_01), "ノコノコx2"),
    BATTLE(A(Formation_02), A(trd_01), "ノコノコx3"),
    BATTLE(A(Formation_03), A(trd_01), "ノコノコ,ボムへい"),
    BATTLE(A(Formation_04), A(trd_01), "ノコノコ,ボムへいx2"),
    BATTLE(A(Formation_05), A(trd_01), "ノコノコ,ボムへいx3"),
    BATTLE(A(Formation_06), A(trd_01), "ノコノコx2,ボムへい"),
    BATTLE(A(Formation_07), A(trd_01), "ノコノコx2,ボムへいx2"),
    BATTLE(A(Formation_08), A(trd_01), "パタパタx２"),
    BATTLE(A(Formation_09), A(trd_01), "パタパタx３"),
    BATTLE(A(Formation_0A), A(trd_01), "パタパタ,ノコノコ"),
    BATTLE(A(Formation_0B), A(trd_01), "パタパタ,ノコノコ,パタパタ"),
    BATTLE(A(Formation_0C), A(trd_01), "パタパタ,ノコノコ,パタパタ,ノコノコ"),
    BATTLE(A(Formation_0D), A(trd_01), "パタパタ,ボムヘイx２"),
    BATTLE(A(Formation_0E), A(trd_01), "パタパタ,ボムヘイx３"),
    BATTLE(A(Formation_0F), A(trd_01), "ボムへい"),
    BATTLE(A(Formation_10), A(trd_01), "ボムへいx2"),
    BATTLE(A(Formation_11), A(trd_01), "ボムへいx3"),
    BATTLE(A(Formation_12), A(trd_01), "ボムへいx4"),
    BATTLE(A(Formation_13), A(trd_01), "ボムへい,ノコノコ"),
    BATTLE(A(Formation_14), A(trd_01), "ボムへいx2,ノコノコ"),
    BATTLE(A(Formation_15), A(trd_01), "ボムへい,ノコノコx2"),
    BATTLE(A(Formation_16), A(trd_01), "ノコノコ,ボムへいx2"),
    BATTLE(A(Formation_17), A(trd_01), "キラーたいほうx３"),
    BATTLE(A(Formation_18), A(trd_01), "キラーx２"),
    BATTLE(A(Formation_19), A(trd_01), "キラー"),
    BATTLE(A(Formation_1A), A(trd_01), "キラーx３"),
    {},
};

StageList A(Stages) = {
    STAGE("trd_00",  A(trd_00)),
    STAGE("trd_01",  A(trd_01)),
    STAGE("trd_02",  A(trd_02)),
    STAGE("trd_02b", A(trd_02b)),
    STAGE("trd_02c", A(trd_02c)),
    STAGE("trd_02d", A(trd_02d)),
    STAGE("trd_03",  A(trd_03)),
    STAGE("trd_04",  A(trd_04)),
    STAGE("trd_05",  A(trd_05)),
    STAGE("trd_05b", A(trd_05b)),
    STAGE("trd_05c", A(trd_05c)),
    STAGE("trd_05d", A(trd_05d)),
    STAGE("trd_05e", A(trd_05e)),
    STAGE("trd_05f", A(trd_05f)),
    {},
};
