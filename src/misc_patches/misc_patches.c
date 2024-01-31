#include "common.h"
#include "custom_status.h"

// returns how much fp costs are changed by
s32 getFpCostChange(Actor* player) {
    s32 fpCost = 0;

    fpCost -= player_team_is_ability_active(player, ABILITY_FLOWER_SAVER);
    fpCost -= player_team_is_ability_active(player, ABILITY_FLOWER_FANATIC) * 2;
    fpCost += custom_status_get_potency(player, FP_COST_STATUS);

    return fpCost;
}
