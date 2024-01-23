#include "pause_common.h"
#include "message_ids.h"
//#include "battle/actor_types.inc.c"
static s32 bActorNames[] = {
    [ACTOR_TYPE_RED_GOOMBA]                 MSG_EnemyName_RedGoomba,
    [ACTOR_TYPE_RED_PARAGOOMBA]             MSG_EnemyName_RedParagoomba,
    [ACTOR_TYPE_GLOOMBA]                    MSG_EnemyName_Gloomba,
    [ACTOR_TYPE_PARAGLOOMBA]                MSG_EnemyName_Paragloomba,
    [ACTOR_TYPE_SPIKED_GLOOMBA]             MSG_EnemyName_SpikedGloomba,
    [ACTOR_TYPE_DARK_KOOPA]                 MSG_EnemyName_DarkKoopa,
    [ACTOR_TYPE_DARK_PARATROOPA]            MSG_EnemyName_DarkParatroopa,
    [ACTOR_TYPE_GOOMBA]                     MSG_EnemyName_Goomba,
    [ACTOR_TYPE_PARAGOOMBA]                 MSG_EnemyName_Paragoomba,
    [ACTOR_TYPE_SPIKED_GOOMBA]              MSG_EnemyName_SpikedGoomba,
    [ACTOR_TYPE_FUZZY]                      MSG_EnemyName_Fuzzy,
    [ACTOR_TYPE_KOOPA_TROOPA]               MSG_EnemyName_KoopaTroopa,
    [ACTOR_TYPE_PARATROOPA]                 MSG_EnemyName_Paratroopa,
    [ACTOR_TYPE_BOB_OMB]                    MSG_EnemyName_BobOmb,
    [ACTOR_TYPE_BOB_OMB_DUP]                MSG_EnemyName_BobOmb,
    [ACTOR_TYPE_BULLET_BILL]                MSG_EnemyName_BulletBill,
    [ACTOR_TYPE_BILL_BLASTER]               MSG_EnemyName_BillBlaster,
    [ACTOR_TYPE_CLEFT]                      MSG_EnemyName_Cleft,
    [ACTOR_TYPE_MONTY_MOLE]                 MSG_EnemyName_MontyMole,
    [ACTOR_TYPE_BANDIT]                     MSG_EnemyName_Bandit,
    [ACTOR_TYPE_POKEY]                      MSG_EnemyName_Pokey,
    [ACTOR_TYPE_POKEY_MUMMY]                MSG_EnemyName_PokeyMummy,
    [ACTOR_TYPE_SWOOPER]                    MSG_EnemyName_Swooper,
    [ACTOR_TYPE_BUZZY_BEETLE]               MSG_EnemyName_BuzzyBeetle,
    [ACTOR_TYPE_STONE_CHOMP]                MSG_EnemyName_StoneChomp,
    [ACTOR_TYPE_PIRANHA_PLANT]              MSG_EnemyName_PiranhaPlant,
    [ACTOR_TYPE_FOREST_FUZZY]               MSG_EnemyName_ForestFuzzy,
    [ACTOR_TYPE_HYPER_GOOMBA]               MSG_EnemyName_HyperGoomba,
    [ACTOR_TYPE_HYPER_PARAGOOMBA]           MSG_EnemyName_HyperParagoomba,
    [ACTOR_TYPE_HYPER_CLEFT]                MSG_EnemyName_HyperCleft,
    [ACTOR_TYPE_CLUBBA]                     MSG_EnemyName_Clubba,
    [ACTOR_TYPE_SHY_GUY]                    MSG_EnemyName_ShyGuy,
    [ACTOR_TYPE_GROOVE_GUY]                 MSG_EnemyName_GrooveGuy,
    [ACTOR_TYPE_SKY_GUY]                    MSG_EnemyName_SkyGuy,
    [ACTOR_TYPE_MEDI_GUY]                   MSG_EnemyName_MediGuy,
    [ACTOR_TYPE_PYRO_GUY]                   MSG_EnemyName_PyroGuy,
    [ACTOR_TYPE_SPY_GUY]                    MSG_EnemyName_SpyGuy,
    [ACTOR_TYPE_FUZZIPEDE]                  MSG_EnemyName_Fuzzipede,
    [ACTOR_TYPE_HURT_PLANT]                 MSG_EnemyName_HurtPlant,
    [ACTOR_TYPE_M_BUSH]                     MSG_EnemyName_MBush,
    [ACTOR_TYPE_AQUA_FUZZY]                 MSG_EnemyName_AquaFuzzy,
    [ACTOR_TYPE_JUNGLE_FUZZY]               MSG_EnemyName_JungleFuzzy,
    [ACTOR_TYPE_SPEAR_GUY]                  MSG_EnemyName_SpearGuy,
    [ACTOR_TYPE_LAVA_BUBBLE]                MSG_EnemyName_LavaBubble,
    [ACTOR_TYPE_SPIKE_TOP]                  MSG_EnemyName_SpikeTop,
    [ACTOR_TYPE_PUTRID_PIRANHA]             MSG_EnemyName_PutridPiranha,
    [ACTOR_TYPE_LAKITU]                     MSG_EnemyName_Lakitu,
    [ACTOR_TYPE_SPINY]                      MSG_EnemyName_Spiny,
    [ACTOR_TYPE_MONTY_MOLE_BOSS]            MSG_EnemyName_MontyMoleBoss,
    [ACTOR_TYPE_BZZAP]                      MSG_EnemyName_Bzzap,
    [ACTOR_TYPE_CRAZEE_DAYZEE]              MSG_EnemyName_CrazeeDayzee,
    [ACTOR_TYPE_AMAZY_DAYZEE]               MSG_EnemyName_AmazyDayzee,
    [ACTOR_TYPE_RUFF_PUFF]                  MSG_EnemyName_RuffPuff,
    [ACTOR_TYPE_SPIKE]                      MSG_EnemyName_Spike,
    [ACTOR_TYPE_GULPIT]                     MSG_EnemyName_Gulpit,
    [ACTOR_TYPE_GULPIT_ROCKS]               MSG_EnemyName_GulpitRocks,
    [ACTOR_TYPE_WHITE_CLUBBA]               MSG_EnemyName_WhiteClubba,
    [ACTOR_TYPE_FROST_PIRANHA]              MSG_EnemyName_FrostPiranha,
    [ACTOR_TYPE_SWOOPULA]                   MSG_EnemyName_Swoopula,
    [ACTOR_TYPE_DUPLIGHOST]                 MSG_EnemyName_Duplighost,
    [ACTOR_TYPE_GHOST_GOOMBARIO]            MSG_EnemyName_GhostGoombario,
    [ACTOR_TYPE_GHOST_KOOPER]               MSG_EnemyName_GhostKooper,
    [ACTOR_TYPE_GHOST_BOMBETTE]             MSG_EnemyName_GhostBombette,
    [ACTOR_TYPE_GHOST_PARAKARRY]            MSG_EnemyName_GhostParakarry,
    [ACTOR_TYPE_GHOST_BOW]                  MSG_EnemyName_GhostBow,
    [ACTOR_TYPE_GHOST_WATT]                 MSG_EnemyName_GhostWatt,
    [ACTOR_TYPE_GHOST_SUSHIE]               MSG_EnemyName_GhostSushie,
    [ACTOR_TYPE_GHOST_LAKILESTER]           MSG_EnemyName_GhostLakilester,
    [ACTOR_TYPE_ALBINO_DINO]                MSG_EnemyName_AlbinoDino,
    [ACTOR_TYPE_EMBER]                      MSG_EnemyName_Ember,
    [ACTOR_TYPE_BONY_BEETLE]                MSG_EnemyName_BonyBeetle,
    [ACTOR_TYPE_DRY_BONES]                  MSG_EnemyName_DryBones,
    [ACTOR_TYPE_DRY_BONES2]                 MSG_EnemyName_DryBones,
    [ACTOR_TYPE_BOMBSHELL_BLASTER]          MSG_EnemyName_BombshellBlaster,
    [ACTOR_TYPE_BOMBSHELL_BILL]             MSG_EnemyName_BombshellBill,
    [ACTOR_TYPE_HAMMER_BROS]                MSG_EnemyName_HammerBros,
    [ACTOR_TYPE_KOOPATROL]                  MSG_EnemyName_Koopatrol,
    [ACTOR_TYPE_MAGIKOOPA]                  MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_FLYING_MAGIKOOPA]           MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_MAGICLONE]                  MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_FLYING_MAGICLONE]           MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_RED_MAGIKOOPA]              MSG_EnemyName_RedMagikoopa,
    [ACTOR_TYPE_FLYING_RED_MAGIKOOPA]       MSG_EnemyName_RedMagikoopa,
    [ACTOR_TYPE_GREEN_MAGIKOOPA]            MSG_EnemyName_GreenMagikoopa,
    [ACTOR_TYPE_FLYING_GREEN_MAGIKOOPA]     MSG_EnemyName_GreenMagikoopa,
    [ACTOR_TYPE_YELLOW_MAGIKOOPA]           MSG_EnemyName_YellowMagikoopa,
    [ACTOR_TYPE_FLYING_YELLOW_MAGIKOOPA]    MSG_EnemyName_YellowMagikoopa,
    [ACTOR_TYPE_GRAY_MAGIKOOPA]             MSG_EnemyName_GrayMagikoopa,
    [ACTOR_TYPE_FLYING_GRAY_MAGIKOOPA]      MSG_EnemyName_GrayMagikoopa,
    [ACTOR_TYPE_WHITE_MAGIKOOPA]            MSG_EnemyName_WhiteMagikoopa,
    [ACTOR_TYPE_FLYING_WHITE_MAGIKOOPA]     MSG_EnemyName_WhiteMagikoopa,
    [ACTOR_TYPE_UNUSED_5B]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_5C]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_5D]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_5E]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_5F]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_60]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_61]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_62]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_63]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_64]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_65]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_66]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_67]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_68]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_69]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_6A]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_MONTY_HOLE]                 MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_6C]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_6D]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_PLAYER]                     MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_GOOMBARIO]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_KOOPER]                     MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_BOMBETTE]                   MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_PARAKARRY]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_BOW]                        MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_WATT]                       MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_SUSHIE]                     MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_LAKILESTER]                 MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_TWINK]                      MSG_Menus_Party_Twink,
    [ACTOR_TYPE_UNUSED_78]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_UNUSED_79]                  MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_THE_MASTER_1]               MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_THE_MASTER_2]               MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_THE_MASTER_3]               MSG_EnemyName_TheMaster,
    [ACTOR_TYPE_CHAN]                       MSG_EnemyName_Chan,
    [ACTOR_TYPE_LEE]                        MSG_EnemyName_Lee,
    [ACTOR_TYPE_LEE_GOOMBARIO]              MSG_Menus_Party_Goombario,
    [ACTOR_TYPE_LEE_KOOPER]                 MSG_Menus_Party_Kooper,
    [ACTOR_TYPE_LEE_BOMBETTE]               MSG_Menus_Party_Bombette,
    [ACTOR_TYPE_LEE_PARAKARRY]              MSG_Menus_Party_Parakarry,
    [ACTOR_TYPE_LEE_BOW]                    MSG_Menus_Party_Bow,
    [ACTOR_TYPE_LEE_WATT]                   MSG_Menus_Party_Watt,
    [ACTOR_TYPE_LEE_SUSHIE]                 MSG_Menus_Party_Sushie,
    [ACTOR_TYPE_LEE_LAKILESTER]             MSG_Menus_Party_Lakilester,
    [ACTOR_TYPE_KAMMY_KOOPA]                MSG_EnemyName_KammyKoopa,
    [ACTOR_TYPE_JR_TROOPA_1]                MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_JR_TROOPA_2]                MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_JR_TROOPA_3]                MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_JR_TROOPA_4]                MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_JR_TROOPA_5]                MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_JR_TROOPA_6]                MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_JR_TROOPA_DUP1]             MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_JR_TROOPA_DUP2]             MSG_EnemyName_JrTroopa,
    [ACTOR_TYPE_BLUE_GOOMBA_BOSS]           MSG_EnemyName_BlueGoombaBoss,
    [ACTOR_TYPE_RED_GOOMBA_BOSS]            MSG_EnemyName_RedGoombaBoss,
    [ACTOR_TYPE_GOOMBA_KING]                MSG_EnemyName_GoombaKing,
    [ACTOR_TYPE_GOOMNUT_TREE]               MSG_EnemyName_GoomnutTree,
    [ACTOR_TYPE_GOOMBARIO_TUTOR1]           MSG_Menus_Party_Goombario,
    [ACTOR_TYPE_MAGIKOOPA_BOSS]             MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_FLYING_MAGIKOOPA_BOSS]      MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_MAGIKOOPA_DUP1]             MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_MAGIKOOPA_DUP2]             MSG_EnemyName_Magikoopa,
    [ACTOR_TYPE_FAKE_BOWSER]                MSG_EnemyName_FakeBowser,
    [ACTOR_TYPE_KOOPA_BROS]                 MSG_EnemyName_KoopaBros,
    [ACTOR_TYPE_GREEN_NINJAKOOPA]           MSG_EnemyName_GreenNinjakoopa,
    [ACTOR_TYPE_RED_NINJAKOOPA]             MSG_EnemyName_RedNinjakoopa,
    [ACTOR_TYPE_BLACK_NINJAKOOPA]           MSG_EnemyName_BlueNinjakoopa,
    [ACTOR_TYPE_YELLOW_NINJAKOOPA]          MSG_EnemyName_YellowNinjakoopa,
    [ACTOR_TYPE_ELDSTAR]                    MSG_Menus_Party_Goombario,
    [ACTOR_TYPE_BUZZAR]                     MSG_EnemyName_Buzzar,
    [ACTOR_TYPE_TUTANKOOPA]                 MSG_EnemyName_Tutankoopa,
    [ACTOR_TYPE_CHOMP]                      MSG_EnemyName_Chomp,
    [ACTOR_TYPE_TUBBA_BLUBBA_INVINCIBLE]    MSG_EnemyName_TubbaBlubba,
    [ACTOR_TYPE_TUBBA_BLUBBA]               MSG_EnemyName_TubbaBlubba,
    [ACTOR_TYPE_TUBBA_HEART]                MSG_EnemyName_TubbaHeart,
    [ACTOR_TYPE_STILT_GUY]                  MSG_EnemyName_StiltGuy,
    [ACTOR_TYPE_SHY_STACK]                  MSG_EnemyName_ShyStack,
    [ACTOR_TYPE_SHY_SQUAD]                  MSG_EnemyName_ShySquad,
    [ACTOR_TYPE_GENERAL_GUY]                MSG_EnemyName_GeneralGuyDup,
    [ACTOR_TYPE_TOY_TANK]                   MSG_EnemyName_GeneralGuy,
    [ACTOR_TYPE_LIGHT_BULB]                 MSG_EnemyName_Bulb,
    [ACTOR_TYPE_SIGNAL_GUY]                 MSG_EnemyName_ShyGuy,
    [ACTOR_TYPE_SHY_SQUAD_REDUX]            MSG_EnemyName_ShySquadRedux,
    [ACTOR_TYPE_SHY_SQUAD_STUB]             MSG_EnemyName_ShyGuy,
    [ACTOR_TYPE_ANTI_GUY_OMO]               MSG_EnemyName_AntiGuy,
    [ACTOR_TYPE_ANTI_GUY_KPA]               MSG_EnemyName_AntiGuy,
    [ACTOR_TYPE_BIG_LANTERN_GHOST]          MSG_EnemyName_BigLanternGhost,
    [ACTOR_TYPE_GOOMBA_KING_DUP]            MSG_EnemyName_GoombaKing,
    [ACTOR_TYPE_LAVA_PIRANHA_PHASE_1]       MSG_EnemyName_LavaPiranha,
    [ACTOR_TYPE_LAVA_PIRANHA_PHASE_2]       MSG_EnemyName_LavaPiranha,
    [ACTOR_TYPE_LAVA_BUD_PHASE_1]           MSG_EnemyName_LavaBud,
    [ACTOR_TYPE_LAVA_BUD_PHASE_2]           MSG_EnemyName_LavaBud,
    [ACTOR_TYPE_PETIT_PIRANHA]              MSG_EnemyName_PetitPiranha,
    [ACTOR_TYPE_PETIT_PIRANHA_BOMB]         MSG_EnemyName_LavaPiranha,
    [ACTOR_TYPE_KENT_C_KOOPA]               MSG_EnemyName_KentCKoopa,
    [ACTOR_TYPE_HUFF_N_PUFF]                MSG_EnemyName_HuffNPuff,
    [ACTOR_TYPE_TUFF_PUFF]                  MSG_EnemyName_TuffPuff,
    [ACTOR_TYPE_MONSTAR]                    MSG_EnemyName_Monstar,
    [ACTOR_TYPE_CRYSTAL_KING]               MSG_EnemyName_CrystalKing,
    [ACTOR_TYPE_CRYSTAL_CLONE]              MSG_EnemyName_CrystalKing,
    [ACTOR_TYPE_CRYSTAL_BIT]                MSG_EnemyName_CrystalBit,
    [ACTOR_TYPE_INTRO_BOWSER]               MSG_EnemyName_Bowser,
    [ACTOR_TYPE_HALLWAY_BOWSER]             MSG_EnemyName_Bowser,
    [ACTOR_TYPE_HALLWAY_BOWSER_DUP]         MSG_EnemyName_Bowser,
    [ACTOR_TYPE_FINAL_BOWSER_1]             MSG_EnemyName_Bowser,
    [ACTOR_TYPE_FINAL_BOWSER_1_DUP]         MSG_EnemyName_Bowser,
    [ACTOR_TYPE_FINAL_BOWSER_2]             MSG_EnemyName_Bowser,
    [ACTOR_TYPE_FINAL_BOWSER_2_DUP]         MSG_EnemyName_Bowser,
    [ACTOR_TYPE_BLOOPER]                    MSG_EnemyName_Blooper,
    [ACTOR_TYPE_ELECTRO_BLOOPER1]           MSG_EnemyName_ElectroBlooper,
    [ACTOR_TYPE_ELECTRO_BLOOPER2]           MSG_EnemyName_ElectroBlooper,
    [ACTOR_TYPE_SUPER_BLOOPER1]             MSG_EnemyName_SuperBlooper,
    [ACTOR_TYPE_SUPER_BLOOPER2]             MSG_EnemyName_SuperBlooper,
    [ACTOR_TYPE_BLOOPER_BABY]               MSG_EnemyName_BlooperBaby,
    [ACTOR_TYPE_LAKILESTER_DUP]             MSG_EnemyName_GhostLakilester,
    [ACTOR_TYPE_SLOT_MACHINE_START]         MSG_EnemyName_SlotMachineStart,
    [ACTOR_TYPE_SLOT_MACHINE_STOP]          MSG_EnemyName_SlotMachineStop,
    [ACTOR_TYPE_WHACKA]                     MSG_EnemyName_Whacka,
    [ACTOR_TYPE_SLOT_MACHINE_START_DUP1]    MSG_EnemyName_SlotMachineStart,
    [ACTOR_TYPE_SLOT_MACHINE_START_DUP2]    MSG_EnemyName_SlotMachineStart,
    [ACTOR_TYPE_SLOT_MACHINE_START_DUP3]    MSG_EnemyName_SlotMachineStart,
};

void pause_tattle_log_draw_contents(MenuPanel* menu, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening);
void pause_tattle_log_init(MenuPanel* panel);
void pause_tattle_log_handle_input(MenuPanel* panel);
void pause_tattle_log_update(MenuPanel* panel);
void pause_tattle_log_cleanup(MenuPanel* panel);

typedef struct LogEntry {
    s16 id;
} LogEntry;

#define ENTRY(i) { .id = i }

s32 curEntry = 0;

LogEntry entries[] = {
    ENTRY(ACTOR_TYPE_GOOMBA),
    ENTRY(ACTOR_TYPE_SPIKED_GOOMBA),
    ENTRY(ACTOR_TYPE_PARAGOOMBA),
};

MenuWindowBP gPauseTattleLogWindowBPs[] = {
    {
        /*
        .windowID = WINDOW_ID_PAUSE_MAP,
        .unk_01 = 0,
        .pos = { .x = 3, .y = 16 },
        .width = 289,
        .height = 154,
        .priority = WINDOW_PRIORITY_1,
        .fpDrawContents = &pause_tattle_log_draw_contents,
        .tab = NULL,
        .parentID = WINDOW_ID_PAUSE_MAIN,
        .fpUpdate = { WINDOW_UPDATE_HIDE },
        .extraFlags = 0,
        .style = { .customStyle = &gPauseWS_19 }*/
        .windowID = WINDOW_ID_PAUSE_MAP,
        .unk_01 = 0,
        .pos = { .x = 3, .y = 16 },
        .width = 289,
        .height = 154,
        .priority = WINDOW_PRIORITY_1,
        .fpDrawContents = &pause_tattle_log_draw_contents,
        .tab = NULL,
        .parentID = WINDOW_ID_PAUSE_MAIN,
        .fpUpdate = { WINDOW_UPDATE_HIDE },
        .extraFlags = 0,
        .style = { .customStyle = &gPauseWS_26 }
    }
};

MenuPanel gPausePanelTattleLog = {
    .initialized = FALSE,
    .col = 0,
    .row = 0,
    .selected = 0,
    .page = 0,
    .numCols = 0,
    .numRows = 0,
    .numPages = 0,
    .gridData = NULL,
    .fpInit = &pause_tattle_log_init,
    .fpHandleInput = &pause_tattle_log_handle_input,
    .fpUpdate = &pause_tattle_log_update,
    .fpCleanup = &pause_tattle_log_cleanup
};

void pause_tattle_log_handle_input(MenuPanel* panel) {
    if (gPausePressedButtons & BUTTON_B) {
        //if (gPauseItemsLevel == 0) {
            sfx_play_sound(SOUND_MENU_BACK);
            gPauseMenuCurrentTab = 0;
        //} else {
        //    sfx_play_sound(SOUND_MENU_BACK);
        //    gPauseItemsLevel = 0;
        //}
    }
}

void pause_tattle_log_init(MenuPanel* panel) {
    s32 i;

    /*
    gPauseItemsLevel = 0;
    gPauseItemsCurrentTab = 0;
    pause_items_load_items(FALSE);

    for (i = 0; i < ARRAY_COUNT(gPauseItemsIconIDs); i++) {
        gPauseItemsIconIDs[i] = hud_element_create(gPauseItemsElements[i]);
        hud_element_set_flags(gPauseItemsIconIDs[i], HUD_ELEMENT_FLAG_80);
    }
    */
    for (i = 0; i < ARRAY_COUNT(gPauseTattleLogWindowBPs); i++) {
        gPauseTattleLogWindowBPs[i].tab = panel;
    }

    setup_pause_menu_tab(gPauseTattleLogWindowBPs, ARRAY_COUNT(gPauseTattleLogWindowBPs));

    panel->initialized = TRUE;
}

void pause_tattle_log_update(MenuPanel* panel) {

}

void pause_tattle_log_cleanup(MenuPanel* panel) {
    /*
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gPauseItemsIconIDs); i++) {
        hud_element_free(gPauseItemsIconIDs[i]);
    }*/
}


s32 pause_log_get_pos_x(s32 itemIndex) {
    return itemIndex * 98;
}

s32 pause_log_get_pos_y(s32 itemIndex) {
    return itemIndex;//((page + 1) * 11) + (ITEM_MENU_PAGE(page)->listStart * 16) + ((itemIndex / ITEM_MENU_PAGE(page)->numCols) * 16);
}

void pause_tattle_log_draw_contents(MenuPanel* menu, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    //ASSERT_MSG(FALSE, "HELLO!?!?!@?!")
    s32 style = DRAW_MSG_STYLE_MENU;
    s32 palette = MSG_PAL_STANDARD;
    s32 i, startI;
    s32 x1 = baseX + 1;
    s32 y1 = baseY + 7;
    s32 x2 = baseX + width - 1;
    s32 y2 = baseY + height - 7;

    if (x1 <= 0) {
        x1 = 1;
    }
    if (y1 <= 0) {
        y1 = 1;
    }

    if (x2 <= 0 || y2 <= 0 || x1 >= SCREEN_WIDTH - 1 || y1 >= SCREEN_HEIGHT - 1) {
       // return;
    }

    if (x2 >= SCREEN_WIDTH - 1) {
        x2 = SCREEN_WIDTH - 1;
    }
    if (y2 >= SCREEN_HEIGHT - 1) {
        y2 = SCREEN_HEIGHT - 1;
    }

    s32 sp6C = baseX + 119;
    s32 sp70 = baseY + 17;


    //s32 currentItemColumn = gPauseItemsSelectedIndex / gPauseItemsPages[gPauseItemsCurrentPage].numCols;
    //s32 currentItemRow = gPauseItemsSelectedIndex % gPauseItemsPages[gPauseItemsCurrentPage].numCols;
    draw_box(DRAW_FLAG_NO_CLIP, &gPauseWS_18, baseX + 68, baseY, 0, width - 68, height, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    draw_box(0, &gPauseWS_27, baseX + 18, baseY + 14, 0, 250, 126, opacity, darkening, 1.0f, 1.0f, 0, 0, 0, 0, 0, 0, width, height, 0);
    //gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, x1, y1, x2, y2);

    startI = curEntry - 10;
    if (startI < 0)
        startI = 0;

    for (i = startI; i < startI + 10 && i < ARRAY_COUNT(entries); i++) {
        LogEntry* entry = &entries[i];
        s32 isSelected = FALSE;

        s32 posX = pause_log_get_pos_x(i);
        s32 posY = pause_log_get_pos_y(i);

        s32 itemOffsetX = 0;
        s32 itemOffsetY = 0;
        if (isSelected) {
            itemOffsetX = -1;
            itemOffsetY = -1;
        }


        draw_msg(bActorNames[entry->id], sp6C + pause_items_scroll_offset_x(posX) + itemOffsetX,
                sp70 + pause_items_scroll_offset_y(posY) + itemOffsetY, 255, palette, style);
/*
        if (i == 0) {
            style = DRAW_MSG_STYLE_MENU;
            palette = MSG_PAL_STANDARD;

            if (isSelected) {
                style = DRAW_MSG_STYLE_MENU | DRAW_MSG_STYLE_DROP_SHADOW;
            }
        }*/

        //if (!pause_items_is_visible(posY)) {
        //    continue;
        //}

/*
        if (i == 0) {

            if (isNone) {
                draw_msg(pause_get_menu_msg(PAUSE_MSG_BAGDE_DESC_NONE), sp6C + pause_items_scroll_offset_x(posX) + itemOffsetX,
                        sp70 + pause_items_scroll_offset_y(posY) + itemOffsetY, 255, palette, style);
            } else if (gItemTable[itemID].nameMsg) {
                if (gItemTable[itemID].nameMsg > 0) {
                    draw_msg(gItemTable[itemID].nameMsg, sp6C + pause_items_scroll_offset_x(posX) + itemOffsetX,
                            sp70 + pause_items_scroll_offset_y(posY) + itemOffsetY, 255, palette, style);
                }
            }

        }*/

        /*
        if (i == 1) {
            itemIcon = iconIDs[totalItemIndex];
            hud_element_clear_flags(itemIcon, HUD_ELEMENT_FLAG_DROP_SHADOW);
            hud_element_set_flags(itemIcon, HUD_ELEMENT_FLAG_FILTER_TEX);
            if (isNone) {
                itemIcon = gPauseItemsIconIDs[19];
            } else {
                if (isSelected) {
                    hud_element_set_flags(itemIcon, HUD_ELEMENT_FLAG_DROP_SHADOW);
                    gPauseCurrentDescIconScript = gItemHudScripts[gItemTable[itemID].hudElemID].enabled;
                }

                hud_element_set_script(itemIcon, gItemHudScripts[gItemTable[itemID].hudElemID].enabled);
                hud_element_set_scale(itemIcon, 0.670816f);
            }

            hud_element_set_render_pos(itemIcon, baseX + 105 + pause_items_scroll_offset_x(posX) + itemOffsetX,
                                        baseY + 23 + pause_items_scroll_offset_y(posY) + itemOffsetY);
            if (totalItemIndex == 0) {
                hud_element_draw_without_clipping(itemIcon);
            } else {
                hud_element_draw_next(itemIcon);
            }

            totalItemIndex++;
        }*/
    }
}
