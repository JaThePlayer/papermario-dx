#ifndef _H_CUSTOM_STATUS_DEFS
#define _H_CUSTOM_STATUS_DEFS

#include "types.h"

#define CUSTOM_STATUS_AMT 16

typedef struct StatusInfo {
    u8 turns;
    s8 potency;
} StatusInfo;

#endif
