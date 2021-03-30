
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "bs-utils/shared/utils.hpp"
#include "custom-types/shared/register.hpp"
#include "modloader/shared/modloader.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/LevelParamsPanel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/BeatmapDifficultySegmentedControlController.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSegmentedControlController.hpp"
#include "GlobalNamespace/BeatmapCharacteristicCollectionSO.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "NjsTweaksCommon.hpp"
#include "NjsTweaksViewController.hpp"
#include "NjsTweaksUtils.hpp"
using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace NjsTweaks;
using namespace bs_utils;
using namespace QuestUI;
using namespace NjsTweaks::Common;
using namespace NjsTweaks::Utils;

static std::string exception = "ex";
static TMPro::TextMeshProUGUI* njsDisplay = nullptr;
static TMPro::TextMeshProUGUI* offsetDisplay = nullptr;
static StandardLevelDetailViewController* standardLevelDetailViewController = nullptr;
static IDifficultyBeatmap* difficulty = nullptr;

MAKE_HOOK_OFFSETLESS(BeatmapObjectSpawnMovementData_Init, void, BeatmapObjectSpawnMovementData* self,
    int noteLinesCount,
    float startNoteJumpMovementSpeed,
    float startBpm,
    float noteJumpStartBeatOffset,
    float jumpOffsetY,
    Vector3 rightVec,
    Vector3 forwardVec
) {
    try {
        if (startNoteJumpMovementSpeed == 10.0f && myConfig.enabled) {
            getLogger().info("Found a song with NJS 10 and NjsTweaks is enabled.");
            if (myConfig.onlyOnExpert && difficulty -> get_difficulty().value != BeatmapDifficulty::Expert) {
                getLogger().info("OnlyOnExpert is enabled, but the difficulty is not Expert.");
            } else {
                Submission::disable(modInfo);
                float targetNjs = myConfig.autoIncreaseTargetNjs;
                getLogger().info("Auto-Increasing NJS to %.2f", targetNjs);
                startNoteJumpMovementSpeed = targetNjs;
            }
        }
    } catch (...) {
        getLogger().error("Error in BeatmapObjectSpawnMovementData_Init");
    }    
    BeatmapObjectSpawnMovementData_Init(self, noteLinesCount, startNoteJumpMovementSpeed, startBpm, noteJumpStartBeatOffset, jumpOffsetY, rightVec, forwardVec);
}

MAKE_HOOK_OFFSETLESS(StandardLevelDetailView_HandleBeatmapDifficultySegmentedControlControllerDidSelectDifficulty, void, StandardLevelDetailView* self, 
    BeatmapDifficultySegmentedControlController* controller, 
    BeatmapDifficulty difficulty
) {
    StandardLevelDetailView_HandleBeatmapDifficultySegmentedControlControllerDidSelectDifficulty(self, controller, difficulty);
    try {
        if (standardLevelDetailViewController == nullptr) {
            getLogger().error("Nullptr error: standardLevelDetailViewController");
            throw exception;
        }
        standardLevelDetailViewController -> get_selectedDifficultyBeatmap();
    } catch (...) {
        getLogger().error("Error in StandardLevelDetailView_HandleBeatmapDifficultySegmentedControlControllerDidSelectDifficulty");
    }
}

MAKE_HOOK_OFFSETLESS(StandardLevelDetailView_HandleBeatmapCharacteristicSegmentedControlControllerDidSelectBeatmapCharacteristic, void, StandardLevelDetailView* self, 
    BeatmapCharacteristicSegmentedControlController* controller, 
    BeatmapCharacteristicSO* beatmapCharacteristic
) {
    StandardLevelDetailView_HandleBeatmapCharacteristicSegmentedControlControllerDidSelectBeatmapCharacteristic(self, controller, beatmapCharacteristic);
    try {
        if (standardLevelDetailViewController == nullptr) {
            getLogger().error("Nullptr error: standardLevelDetailViewController");
            throw exception;
        }
        standardLevelDetailViewController -> get_selectedDifficultyBeatmap();
    } catch (...) {
        getLogger().error("Error in StandardLevelDetailView_HandleBeatmapCharacteristicSegmentedControlControllerDidSelectBeatmapCharacteristic");
    }
}

MAKE_HOOK_OFFSETLESS(StandardLevelDetailViewController_get_selectedDifficultyBeatmap, IDifficultyBeatmap*, StandardLevelDetailViewController* self
) {
    difficulty = StandardLevelDetailViewController_get_selectedDifficultyBeatmap(self);
    try {
        float njs = difficulty -> get_noteJumpMovementSpeed();
        float offset = difficulty -> get_noteJumpStartBeatOffset();
        if (njsDisplay == nullptr || offsetDisplay == nullptr) {
            getLogger().error("Nullptr error: njsDisplay or offsetDisplay");
            throw exception;
        }
        njsDisplay -> SetText(FloatKeyValueToString("NJS", njs));
        offsetDisplay -> SetText(FloatKeyValueToString("Offset", offset));
    } catch (...) {
        getLogger().error("Error in StandardLevelDetailViewController_get_selectedDifficultyBeatmap");
    }    
    return difficulty;
}

MAKE_HOOK_OFFSETLESS(StandardLevelDetailViewController_DidActivate, void, StandardLevelDetailViewController* self, 
    bool firstActivation, 
    bool addedToHierarchy, 
    bool screenSystemEnabling
) {
    try {
        standardLevelDetailViewController = self;
        if (firstActivation) {
            BeatSaberUI::ClearCache();
            auto layout = BeatSaberUI::CreateHorizontalLayoutGroup(self -> get_transform());
            auto transform = layout -> get_gameObject() -> GetComponent<RectTransform*>();
            transform -> set_anchoredPosition(Vector2(2.0f, 0.0f));
            transform -> set_localScale(Vector3(0.9, 0.5, 0.6));
            njsDisplay = BeatSaberUI::CreateText(transform, "Error");
            njsDisplay -> set_fontStyle(TMPro::FontStyles::Bold | TMPro::FontStyles::Italic);
            njsDisplay -> set_alpha(0.5f);
            offsetDisplay = BeatSaberUI::CreateText(transform, "Error");
            offsetDisplay -> set_fontStyle(TMPro::FontStyles::Bold | TMPro::FontStyles::Italic);
            offsetDisplay -> set_alpha(0.5f);
        }
    } catch (...) {
        getLogger().error("Error in StandardLevelDetailViewController_DidActivate");
    }    
    StandardLevelDetailViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_HOOK_OFFSETLESS(GameplaySetupViewController_DidActivate, void, GameplaySetupViewController* self, 
    bool firstActivation, 
    bool addedToHierarchy, 
    bool screenSystemEnabling
) {
    GameplaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    try {
        if (!Submission::getEnabled()) {
            Submission::enable(modInfo);
        }
    } catch (...) {
        getLogger().error("Error in GameplaySetupViewController_DidActivate");
    }
}

extern "C" void setup(ModInfo& info) {
    try {
        info.id = "NjsTweaks";
        info.version = "0.0.1";
        NjsTweaks::Common::Initialize(info);
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
    INSTALL_HOOK_OFFSETLESS(getLogger(), GameplaySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "GameplaySetupViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(getLogger(), StandardLevelDetailViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(getLogger(), StandardLevelDetailView_HandleBeatmapDifficultySegmentedControlControllerDidSelectDifficulty, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailView", "HandleBeatmapDifficultySegmentedControlControllerDidSelectDifficulty", 2));
    INSTALL_HOOK_OFFSETLESS(getLogger(), StandardLevelDetailView_HandleBeatmapCharacteristicSegmentedControlControllerDidSelectBeatmapCharacteristic, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailView", "HandleBeatmapCharacteristicSegmentedControlControllerDidSelectBeatmapCharacteristic", 2));
    INSTALL_HOOK_OFFSETLESS(getLogger(), StandardLevelDetailViewController_get_selectedDifficultyBeatmap, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailViewController", "get_selectedDifficultyBeatmap", 0));

    getLogger().debug("Installed all NjsTweaks hooks successful.");
}