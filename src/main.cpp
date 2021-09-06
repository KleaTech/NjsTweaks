#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "bs-utils/shared/utils.hpp"
#include "custom-types/shared/register.hpp"
#include "modloader/shared/modloader.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"
#include "GlobalNamespace/StandardLevelDetailViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "NjsTweaksCommon.hpp"
#include "NjsTweaksViewController.hpp"
#include "NjsTweaksUI.hpp"
#include "NjsTweaksUtils.hpp"

#include "GlobalNamespace/LevelSelectionNavigationController.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/LevelCollectionNavigationController.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace NjsTweaks;
using namespace bs_utils;
using namespace QuestUI;
using namespace NjsTweaks::Common;
using namespace NjsTweaks::UI;
using namespace NjsTweaks::Utils;
using namespace TMPro;
using namespace HMUI;

static StandardLevelDetailViewController* standardLevelDetailViewController = nullptr;
static LevelSelectionNavigationController* levelSelectionNavigationController = nullptr;

MAKE_HOOK_OFFSETLESS(BeatmapObjectSpawnMovementData_Init, void, BeatmapObjectSpawnMovementData* self,
    int noteLinesCount,
    float startNoteJumpMovementSpeed,
    float startBpm,
    float noteJumpStartBeatOffset,
    float jumpOffsetY,
    Vector3 rightVec,
    Vector3 forwardVec
) {
    auto newNjs = njsSetting;
    auto newOffset = offsetSetting;
    //When the NJS is invalid (<= 0) startNoteJumpMovementSpeed will already be a fallback value.
    //(I'm actually not quite sure about the behavior of 0, but this approach should be safe and future proof.)
    //If the original is 0, we accept the fallback value. If the player don't want the fallback they can set whatever value they like.
    //Crucially though if the NJS would be negative, we will replace the fallback value with this original negative value.
    //Therefor we can enjoy the rare gems of negative NJS maps.
    if (!floatEquals(newNjs, 0) && (newNjs != startNoteJumpMovementSpeed || newOffset != noteJumpStartBeatOffset)) {
        getLogger().info("Starting song at %.2f NJS instead of %.2f and %.2 Offset instead of %.2.", newNjs, startNoteJumpMovementSpeed, newOffset, noteJumpStartBeatOffset);
        startNoteJumpMovementSpeed = newNjs;
        noteJumpStartBeatOffset = newOffset;
        Submission::disable(modInfo); //This is not necessary, we already disabled it beforehand. This is just to be sure.
    }
    BeatmapObjectSpawnMovementData_Init(self, noteLinesCount, startNoteJumpMovementSpeed, startBpm, noteJumpStartBeatOffset, jumpOffsetY, rightVec, forwardVec);
}

MAKE_HOOK_OFFSETLESS(StandardLevelDetailView_RefreshContent, void, StandardLevelDetailView* self) {
    StandardLevelDetailView_RefreshContent(self);
    onMapChange(self -> get_selectedDifficultyBeatmap());
}

MAKE_HOOK_OFFSETLESS(StandardLevelDetailViewController_DidActivate, void, StandardLevelDetailViewController* self, 
    bool firstActivation, 
    bool addedToHierarchy, 
    bool screenSystemEnabling
) {
    standardLevelDetailViewController = self;
    if (firstActivation) {
        createNjsTweaksBarControl(self -> get_transform());
    }
    StandardLevelDetailViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

extern "C" void setup(ModInfo& info) {
    try {
        info.id = "NjsTweaks";
        info.version = "0.0.2";
        NjsTweaks::Common::initialize(info);
    } catch (...) {
        (new Logger(info)) -> critical("NjsTweaks encountered an error during setup().");
    }
}

extern "C" void load() {
    il2cpp_functions::Init();
    QuestUI::Init();
    getLogger().debug("Installing NjsTweaks hooks...");
    
    custom_types::Register::RegisterType<NjsTweaksViewController>();
    QuestUI::Register::RegisterModSettingsViewController<NjsTweaksViewController*>(modInfo);

    INSTALL_HOOK_OFFSETLESS(getLogger(), BeatmapObjectSpawnMovementData_Init, il2cpp_utils::FindMethodUnsafe("", "BeatmapObjectSpawnMovementData", "Init", 7));
    INSTALL_HOOK_OFFSETLESS(getLogger(), StandardLevelDetailViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(getLogger(), StandardLevelDetailView_RefreshContent, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailView", "RefreshContent", 0));

    getLogger().debug("Installed all NjsTweaks hooks successful.");
}