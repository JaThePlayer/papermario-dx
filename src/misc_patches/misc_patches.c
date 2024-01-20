#include "common.h"

f32 __deltaTime = 1.0f;

void _updateDeltaTime() {
    if (
        (gGameStatusPtr->curButtons[0] & BUTTON_L) &&
        (gGameStatusPtr->curButtons[0] & BUTTON_R) &&
        (gGameStatusPtr->pressedButtons[0] & BUTTON_B)
        ) {
        __deltaTime = __deltaTime > 1.0f ? 1.0f : 4.0f;
    }
}
