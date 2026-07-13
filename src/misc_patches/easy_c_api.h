#pragma once

#include "evt.h"
#include "functions.h"
#include "macros.h"

// Internals

typedef struct {
    s32 currLine;
    s32 waitLeft;

} CtxBase;

CtxBase* api_start(Evt* script, b32 isInitialCall, s32 ctxSize) {
    CtxBase* ctx;
    if (isInitialCall) {
       ctx = (CtxBase*)malloc(ctxSize);
       mem_clear(ctx, ctxSize);
       script->functionTempPtr[0] = ctx;
    }
    else {
        ctx = (CtxBase*)script->functionTempPtr[0];
    }

    if (ctx->waitLeft > 0) {
        ctx->waitLeft--;
        return nullptr;
    }

    return ctx;
}

#define API_START(ctxStruct) \
    typedef struct { CtxBase base; \
        ctxStruct; \
    } __ctx; \
    __ctx* ctx = (__ctx*) api_start(script, isInitialCall, sizeof(__ctx)); \
    if (ctx == nullptr) \
        return 0; \
    Bytecode* args = script->ptrReadPos; \
    switch (ctx->base.currLine) { \
        case 0: \

#define API_END() \
    free(script->functionTempPtr[0]); \
    script->functionTempPtr[0] = nullptr; \
    return ApiStatus_DONE2; \
    } PANIC_MSG("Api func reached end!"); \

#define _API_YIELD(value, continuation) \
    ctx->base.currLine = continuation; \
    return value; \
    case continuation:

// PUBLIC API

/// Yields an ApiStatus from this function.
/// Execution will resume after the yield next frame, if permitted by the yielded ApiStatus.
#define API_YIELD(value) _API_YIELD(value, __LINE__)

/// Blocks execution for a frame.
/// Execution will resume after the block statement.
#define API_BLOCK() API_YIELD(ApiStatus_BLOCK)

/// Blocks execution for the given amount of frames.
/// Execution will resume after the block statement.
#define API_WAIT(frames) \
    ctx->base.waitLeft = (frames) - 1; \
    API_BLOCK();

/// Blocks execution until the given condition is met.
/// The condition will get re-checked each frame.
#define API_WAIT_UNTIL(condition) \
    while (!(condition)) { \
        API_BLOCK(); \
    }

/// Shorthand for evt_get_variable.
#define API_ARG() evt_get_variable(script, *args++)

/// Shorthand for *args++
#define API_OUT_ARG() *args++

/// Defines a new V2 API_CALLABLE function.
/// Needed to properly use any of the other macros.
#define API_CALLABLE_V2(name, ctx, code) \
    API_CALLABLE(name) { \
        API_START(ctx); \
        code \
        API_END(); \
    }

/*
Usage examples:

API_CALLABLE(AddCoinAwaitTallyV2) {
    API_START(
        s32 coins;
    );

    ctx->coins = API_ARG();
    show_coin_counter();
    script->varTable[0] = add_coins(ctx->coins);

    while (gStatusBar.displayCoins < gPlayerData.coins) {
        API_BLOCK();
    }

    hide_coin_counter();
    API_END();
}

API_CALLABLE_V2(AddCoinAwaitTallyV22,
    s32 coins; s32 hi,

    ctx->coins = API_ARG();
    show_coin_counter();
    script->varTable[0] = add_coins(ctx->coins);

    API_WAIT_UNTIL(gStatusBar.displayCoins >= gPlayerData.coins);

    hide_coin_counter_immediately();

    API_WAIT(30);

    show_coin_counter();
    API_WAIT(10);
    script->varTable[0] = add_coins(-ctx->coins);
    API_WAIT(30);
    hide_coin_counter_immediately();
)
*/
