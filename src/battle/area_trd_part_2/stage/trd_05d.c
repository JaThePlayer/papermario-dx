#include "common.h"
#include "battle/battle.h"

#define NAMESPACE b_area_trd_part_2_trd_05d

EvtSource N(80234540) = {
    EVT_SET(LW(10), LW(0))
    EVT_LABEL(0)
    EVT_CALL(RotateGroup, LW(10), 2, 0, 0, 1)
    EVT_WAIT_FRAMES(5)
    EVT_CALL(RotateGroup, LW(10), 1, 0, 0, 1)
    EVT_WAIT_FRAMES(5)
    EVT_CALL(RotateGroup, LW(10), -2, 0, 0, 1)
    EVT_WAIT_FRAMES(5)
    EVT_CALL(RotateGroup, LW(10), 1, 0, 0, 1)
    EVT_WAIT_FRAMES(5)
    EVT_CALL(RandInt, 30, LW(0))
    EVT_ADD(LW(0), 30)
    EVT_WAIT_FRAMES(LW(0))
    EVT_GOTO(0)
    EVT_RETURN
    EVT_END
};

EvtSource N(80234658) = {
    EVT_SET(LW(10), LW(0))
    EVT_LOOP(0)
        EVT_CALL(TranslateModel, LW(10), 1, 0, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), 1, 0, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 1, EVT_FLOAT(-0.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 1, -1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), EVT_FLOAT(1.5), -1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 2, -1, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), 2, -1, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), 2, -1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), EVT_FLOAT(2.5), EVT_FLOAT(-1.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 3, -2, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), 3, -2, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 3, EVT_FLOAT(-2.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 3, -3, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), 3, -3, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), 3, -3, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 3, EVT_FLOAT(-2.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 3, -2, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), EVT_FLOAT(2.5), -2, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 2, -2, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 2, EVT_FLOAT(-1.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 2, -1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), EVT_FLOAT(1.5), EVT_FLOAT(-0.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 1, 0, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 1, EVT_FLOAT(0.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 1, 1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), EVT_FLOAT(0.5), 1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 0, 1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), EVT_FLOAT(-0.5), 1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), -1, 1, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), -1, 1, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), -1, EVT_FLOAT(0.5), 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), -1, 0, 0)
        EVT_WAIT_FRAMES(2)
        EVT_CALL(TranslateModel, LW(10), -1, 0, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), EVT_FLOAT(-0.5), 0, 0)
        EVT_WAIT_FRAMES(1)
        EVT_CALL(TranslateModel, LW(10), 0, 0, 0)
        EVT_WAIT_FRAMES(2)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtSource N(beforeBattle_80234C04) = {
    EVT_CALL(SetSpriteShading, -1)
    EVT_CALL(SetCamBGColor, 1, 0, 0, 0)
    EVT_CALL(EnableModel, 10, 0)
    EVT_CALL(EnableModel, 15, 0)
    EVT_CALL(EnableModel, 16, 0)
    EVT_CALL(EnableModel, 17, 0)
    EVT_CALL(EnableModel, 25, 0)
    EVT_CALL(EnableModel, 14, 0)
    EVT_CALL(EnableModel, 27, 0)
    EVT_THREAD
        EVT_SET(LW(0), 6)
        EVT_EXEC(N(80234658))
        EVT_WAIT_FRAMES(5)
        EVT_SET(LW(0), 4)
        EVT_EXEC(N(80234658))
        EVT_WAIT_FRAMES(5)
        EVT_SET(LW(0), 2)
        EVT_EXEC(N(80234658))
    EVT_END_THREAD
    EVT_RETURN
    EVT_END
};

EvtSource N(afterBattle_80234D48) = {
    EVT_RETURN
    EVT_END
};

Stage NAMESPACE = {
    .texture = "trd_tex",
    .shape = "trd_bt05_shape",
    .hit = "trd_bt05_hit",
    .preBattle = N(beforeBattle_80234C04),
    .postBattle = N(afterBattle_80234D48),
};