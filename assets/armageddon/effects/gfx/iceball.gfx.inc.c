#include "common.h"

Gfx D_iceball_gfx[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCycleType(G_CYC_1CYCLE), // 1
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),

    gsDPSetCombineLERP(
        TEXEL0, 0, SHADE, 0,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, 0, SHADE, 0,
        TEXEL0, 0, PRIMITIVE, 0),

   /*
    gsDPSetCombineLERP(
        TEXEL0, PRIMITIVE, SHADE, 0,
        TEXEL0, 0, PRIMITIVE, 0,
        TEXEL0, PRIMITIVE, SHADE, 0,
        TEXEL0, 0, PRIMITIVE, 0
    ),*/

    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal16(0, D_iceball_pal),
    gsDPLoadTextureTile_4b(D_iceball, G_IM_FMT_CI, 16, 0, 0, 0, 15, 15, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsSPClearGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};
