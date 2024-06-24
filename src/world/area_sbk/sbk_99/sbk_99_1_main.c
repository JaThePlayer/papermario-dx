#include "sbk_99.h"
#include "world/common/complete/portable_item_network_boo.inc.c"

extern EvtScript N(EVS_Main);
extern EvtScript N(EVS_MakeEntities);

EntryList N(Entrances) = {
    [sbk_99_ENTRY_0]    { -484.0,  100.0,    5.0,   90.0 },
    [sbk_99_ENTRY_1]    {  346.0,    0.0, -342.0,  220.0 },
};

MapSettings N(settings) = {
    .main = &N(EVS_Main),
    .entryList = &N(Entrances),
    .entryCount = ENTRY_COUNT(N(Entrances)),
    .background = &gBackgroundImage,
    .tattle = { MSG_MapTattle_sbk_99 },
};

EvtScript N(EVS_ExitWalk_iwa_04_1) = EVT_EXIT_WALK(60, sbk_99_ENTRY_0, "iwa_04", iwa_04_ENTRY_1);
EvtScript N(EVS_ExitWalk_sbk_30_0) = EVT_EXIT_WALK(60, sbk_99_ENTRY_1, "sbk_30", sbk_30_ENTRY_0);

EvtScript N(EVS_BindExitTriggers) = {
    BindTrigger(Ref(N(EVS_ExitWalk_iwa_04_1)), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(N(EVS_ExitWalk_sbk_30_0)), TRIGGER_FLOOR_ABOVE, COLLIDER_deiline, 1, 0)
    Return
    End
};

EvtScript N(EVS_EnterMap) = {
    Call(GetLoadType, LVar1)
    IfEq(LVar1, LOAD_FROM_FILE_SELECT)
        Exec(EnterSavePoint)
        Exec(N(EVS_BindExitTriggers))
        Return
    EndIf
    Set(LVar0, Ref(N(EVS_BindExitTriggers)))
    Exec(EnterWalk)
    Return
    End
};

NpcGroupList N(NpcGroup) = {
    NPC_GROUP(N(NpcData_PINBoo)),
    {}
};

EvtScript N(EVS_Main) = {
    Set(GB_WorldLocation, LOCATION_DRY_DRY_DESERT)
    Call(SetSpriteShading, SHADING_NONE)
    Call(MakeNpcs, FALSE, Ref(N(NpcGroup)))
    ExecWait(N(EVS_SetupPortableItemNetworkBoo))

    EVT_SETUP_CAMERA_NO_LEAD(0, 0, 0)
    Set(GF_MAP_DryDryDesert, TRUE)
    ExecWait(N(EVS_MakeEntities))
    Call(SetMusicTrack, 0, SONG_MT_RUGGED, 0, 8)
    Exec(N(EVS_EnterMap))
    Wait(1)
    Return
    End
};
