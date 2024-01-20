#ifndef _MISC_PACTHES_SP_POOLS_H_
#define _MISC_PACTHES_SP_POOLS_H_

#include "common.h"

/*
prologue                                 - 1
ch1 - pleasent path, koopa bros fortress - 2
ch2 - mt rugged, desert, temple          - 3
ch3 - forever forest, gulch, tubba place - 3
ch4 - toybox - big enough to warrant 2u8?- 2 (merged)
ch5 - jungle, volcano                    - 2
ch6 - flower fields                      - 2 (merged) (or maybe cloud climb???)
ch7 - shiver path, path to palace, palace- 3
ch8 - star road, castle                  - 2
*/
#define SP_POOL_NONE 0
// Prologue
#define SP_POOL_PROLOGUE 1
// CH1
#define SP_POOL_PLEASANT_PATH 2
#define SP_POOL_KOOPA_BROS_FORT 3
// CH2
#define SP_POOL_MT_RUGGED 4
#define SP_POOL_DESERT 5
#define SP_POOL_DRY_DRY_RUINS 6
// CH3
#define SP_POOL_FOREST 7
#define SP_POOL_GULCH 8
#define SP_POOL_TUBBA 9
// CH4
#define SP_POOL_TOYBOX 10
#define SP_POOL_TOYBOX_P2 11
// CH5
#define SP_POOL_JUNGLE 12
#define SP_POOL_VOLCANO 13
// CH6
#define SP_POOL_FLOWER_FIELDS 14
#define SP_POOL_FLOWER_FIELDS_P2 15
// CH7
#define SP_POOL_SHIVER_PATH 16
#define SP_POOL_PATH_TO_PALACE 17
#define SP_POOL_CRYSTAL_PALACE 18
// CH8
#define SP_POOL_STAR_ROAD 19
#define SP_POOL_BOWSER_CASTLE 20
#define SP_POOL_BOWSER_CASTLE_P2 21
#define SP_POOL_PEACH_CASTLE 22
// OPTIONAL
#define SP_POOL_SEWERS 23

// should be equal to the highest SP_POOL_ index
#define SP_POOL_COUNT 23

extern u8 sp_pool_caps[SP_POOL_COUNT];

// Whether the given pool id actually corresponds to a pool
u8 sp_pool_is_pooled(u8 id);

// returns how much sp has been used from the given pool.
u8 sp_pool_used(u8 id);

// returns how much sp is left in a given pool.
u8 sp_pool_remaining(u8 id);

// Uses up at most 'amt' sp from the given pool. Returns how much sp was actually be spent.
u8 sp_pool_use(u8 id, u8 amt);

#endif
