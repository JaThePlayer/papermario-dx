#include "area.h"

extern Stage A(iwa_01);
extern Stage A(iwa_01b);
extern Stage A(iwa_02);

extern ActorBlueprint A(buzzar);

Vec3i A(BossPos) = { 90, 70, 0 };

Formation A(Formation_07) = { // iwa_00
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 10),
    OVL_ACTOR_BY_IDX("rokitu", BTL_POS_AIR_C, 9),
};

Formation A(Formation_06) = { // iwa_00, first
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 10),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_C, 9),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_D, 8),
};

Formation A(Formation_04) = { // iwa_01, slide endpoint ambush, UNAVOIDABLE
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 8),
    OVL_ACTOR_BY_IDX("red_paragoomba", BTL_POS_AIR_C, 7),
};

Formation A(Formation_0E) = { // iwa_01, thrower
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 8),
};

Formation A(Formation_0F) = { // iwa_01, thrower
    OVL_ACTOR_BY_IDX_ITEM("monty_mole", BTL_POS_GROUND_B, 10, ITEM_POWER_PLUS_A),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_C, 9),
};

Formation A(Formation_10) = { // iwa_01, thrower
    OVL_ACTOR_BY_IDX_ITEM("monty_mole", BTL_POS_GROUND_B, 10, ITEM_P_DOWN_D_UP),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 9),
    OVL_ACTOR_BY_IDX("rokitu", BTL_POS_AIR_C, 8),
};

Formation A(Formation_11) = { // iwa_01, wall ambush
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("red_paragoomba", BTL_POS_AIR_B, 9),
    OVL_ACTOR_BY_IDX("rokitu", BTL_POS_AIR_C, 8),
};

Formation A(Formation_12) = { // iwa_01, quake ambush
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 8),
    OVL_ACTOR_BY_IDX("rokitu", BTL_POS_AIR_C, 7),
    OVL_ACTOR_BY_IDX_ITEM("monty_mole", BTL_POS_GROUND_D, 6, ITEM_POWER_PLUS_A),
    OVL_ACTOR_BY_IDX("red_paragoomba", BTL_POS_AIR_D, 5),
};

s16 A(Formation_05_Items)[] = { ITEM_HP_PLUS_A, ITEM_POWER_PLUS_A };

Formation A(Formation_05) = { // iwa_02
    OVL_ACTOR_BY_IDX_ITEMS("monty_mole", BTL_POS_GROUND_B, 10, A(Formation_05_Items)),
};

Formation A(Formation_00) = { // iwa_02, first
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 8),
};

Formation A(Formation_01) = { // iwa_02
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX_ITEM("monty_mole", BTL_POS_GROUND_C, 8, ITEM_FRIED_SHROOM),
};

Formation A(Formation_03) = { // iwa_02
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 8),
    OVL_ACTOR_BY_IDX("rokitu", BTL_POS_AIR_C, 7),
};


Formation A(Formation_02) = { // iwa_03
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX_ITEM("red_paragoomba", BTL_POS_AIR_B, 8, ITEM_HP_PLUS_A),
};

Formation A(Formation_08) = { // iwa_03
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 8),
    OVL_ACTOR_BY_IDX("red_paragoomba", BTL_POS_AIR_C, 7),
};

Formation A(Formation_09) = { // iwa_03
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX_ITEM("cleft", BTL_POS_GROUND_B, 9, ITEM_THUNDER_BOLT),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_C, 8),
    OVL_ACTOR_BY_IDX("red_goomba", BTL_POS_GROUND_D, 7),
};

Formation A(Formation_0B) = { // iwa_03
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_C, 8),
    OVL_ACTOR_BY_IDX("red_paragoomba", BTL_POS_AIR_C, 7),
};

Formation A(Formation_0A) = { // iwa_04
    OVL_ACTOR_BY_IDX_ITEM("cleft", BTL_POS_GROUND_A, 10, ITEM_HP_PLUS_A),
    OVL_ACTOR_BY_IDX("rokitu", BTL_POS_AIR_B, 9),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 8),
};

Formation A(Formation_0C) = {
    ACTOR_BY_POS(A(buzzar), A(BossPos), 10),
};

Formation A(Formation_0D) = { // unused
    OVL_ACTOR_BY_IDX("cleft", BTL_POS_GROUND_A, 10),
    OVL_ACTOR_BY_IDX("monty_mole", BTL_POS_GROUND_B, 9),
    OVL_ACTOR_BY_IDX("whacka", BTL_POS_GROUND_C, 8),
};

API_CALLABLE(A(make_unescapable)) {
    gBattleStatus.flags2 &= ~BS_FLAGS2_CAN_FLEE;
    gCurrentEncounter.forbidFleeing = true;

    return ApiStatus_DONE2;
}

EvtScript A(slide_ambush_script) = {
    Set(GF_IWA_01_SlideAmbush, true)
    Call(A(make_unescapable))
    Return
    End
};

BattleList A(Formations) = {
    BATTLE(A(Formation_00), A(iwa_01), "シンエモン"),
    BATTLE(A(Formation_01), A(iwa_01), "シンエモンx２"),
    BATTLE(A(Formation_02), A(iwa_01), "シンエモンx３"),
    BATTLE(A(Formation_03), A(iwa_01), "シンエモン,チョロプー"),
    BATTLE_WITH_SCRIPT(A(Formation_04), A(iwa_01), A(slide_ambush_script), "シンエモン,チョロプーx２"),
    BATTLE(A(Formation_05), A(iwa_01), "チョロプー"),
    BATTLE(A(Formation_06), A(iwa_01), "チョロプーx２"),
    BATTLE(A(Formation_07), A(iwa_01), "チョロプーx３"),
    BATTLE(A(Formation_08), A(iwa_01), "チョロプーx４"),
    BATTLE(A(Formation_09), A(iwa_01), "チョロプー,シンエモン"),
    BATTLE(A(Formation_0A), A(iwa_01), "チョロプーx２,シンエモン"),
    BATTLE(A(Formation_0B), A(iwa_01), "チョロプー,シンエモン,チョロプー"),
    BATTLE(A(Formation_0C), A(iwa_02), "ゲーハー"),
    BATTLE(A(Formation_0D), A(iwa_02), "コブロン"),
    // new
    BATTLE(A(Formation_0E), A(iwa_01), ""),
    BATTLE(A(Formation_0F), A(iwa_01), ""),
    BATTLE(A(Formation_10), A(iwa_01), ""),
    BATTLE(A(Formation_11), A(iwa_01), ""),
    BATTLE(A(Formation_12), A(iwa_01), ""),
    {},
};

StageList A(Stages) = {
    STAGE("iwa_01", A(iwa_01)),
    STAGE("iwa_01b", A(iwa_01b)),
    STAGE("iwa_02", A(iwa_02)),
    {},
};
