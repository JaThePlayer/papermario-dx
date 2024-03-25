#include "misc_patches/sp_pools.h"
#include "common.h"

u8 sp_pool_caps[SP_POOL_COUNT] = {
    [SP_POOL_PROLOGUE] = 60,
    [SP_POOL_PLEASANT_PATH] = 125,
    [SP_POOL_KOOPA_BROS_FORT] = 90,
};

// stores how much sp was taken from each pool in the current battle
u8 sp_pool_used_up_this_battle[SP_POOL_COUNT] = { };

// converts a pool id into an index into sp_pool_caps (since 0 is used for SP_POOL_NONE, and we don't want to waste that byte)
#define INDEX_POOL(id) (id - 1)

// Whether the given pool id actually corresponds to a pool
u8 sp_pool_is_pooled(u8 id) {
    return id > SP_POOL_NONE;
}

// returns how much sp has been used from the given pool.
u8 sp_pool_used(u8 id) {
    if (id == SP_POOL_NONE) {
        return 0;
    }

    return gPlayerData.spAreaPools[INDEX_POOL(id)];
}

/// Returns the cap of the given pool
u8 sp_pool_cap(u8 id) {
    if (id == SP_POOL_NONE) {
        return 0;
    }

    return sp_pool_caps[id];
}

// returns how much sp is left in a given pool
u8 sp_pool_remaining(u8 id) {
    s8 remaining;
    if (id == SP_POOL_NONE) {
        return 0;
    }

    remaining = sp_pool_cap(id) - sp_pool_used(id);
    if (remaining < 0)
        remaining = 0;

    return remaining;
}

// Uses up at most 'amt' sp from the given pool. Returns how much sp was actually spent.
u8 sp_pool_use(u8 id, u8 amt) {
    if (id == SP_POOL_NONE) {
        return amt;
    }

    u8 remaining = sp_pool_remaining(id);

    if (amt > remaining) {
        amt = remaining;
    }

    gPlayerData.spAreaPools[INDEX_POOL(id)] += amt;
    sp_pool_used_up_this_battle[INDEX_POOL(id)] += amt;

    return amt;
}

void sp_pool_return_this_battle() {
    for (s32 i = 0; i < SP_POOL_COUNT; i++) {
        u8 used = sp_pool_used_up_this_battle[i];

        gPlayerData.spAreaPools[i] -= used;
        sp_pool_used_up_this_battle[i] = 0;
    }
}

void sp_pool_end_of_battle() {
    for (s32 i = 0; i < SP_POOL_COUNT; i++) {
        sp_pool_used_up_this_battle[i] = 0;
    }
}
