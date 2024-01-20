// Contains macro for fast creation of enemy formations

#define ENEMY(name, homeIndex, prio) { &A(name), .home = { .index = homeIndex }, .priority = prio }
#define ENEMY_ITEM(name, homeIndex, prio, itemId) { &A(name), .home = { .index = homeIndex }, .priority = prio, .item = itemId }

// declares the first ground enemy in the battle (prio 10)
#define ENEMY0(name) ENEMY(name, BTL_POS_GROUND_A, 10)
#define ENEMY1(name) ENEMY(name, BTL_POS_GROUND_B, 9)
#define ENEMY2(name) ENEMY(name, BTL_POS_GROUND_C, 8)
#define ENEMY3(name) ENEMY(name, BTL_POS_GROUND_D, 7)

#define ENEMY_AIR0(name) ENEMY(name, BTL_POS_AIR_A, 10)
#define ENEMY_AIR1(name) ENEMY(name, BTL_POS_AIR_B, 9)
#define ENEMY_AIR2(name) ENEMY(name, BTL_POS_AIR_C, 8)
#define ENEMY_AIR3(name) ENEMY(name, BTL_POS_AIR_D, 7)

#define ENEMY0_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_GROUND_A, 10, itemId)
#define ENEMY1_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_GROUND_B, 9, itemId)
#define ENEMY2_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_GROUND_C, 8, itemId)
#define ENEMY3_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_GROUND_D, 7, itemId)

#define ENEMY_AIR0_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_AIR_A, 10, itemId)
#define ENEMY_AIR1_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_AIR_B, 9, itemId)
#define ENEMY_AIR2_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_AIR_C, 8, itemId)
#define ENEMY_AIR3_ITEM(name, itemId) ENEMY_ITEM(name, BTL_POS_AIR_D, 7, itemId)
