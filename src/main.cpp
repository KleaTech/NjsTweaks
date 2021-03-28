#include "main.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"
#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"
#include "NjsTweaksViewController.hpp"
#include "bs-utils/shared/utils.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace NjsTweaks;
using namespace rapidjson;
using namespace bs_utils;

static ModInfo modInfo;

MAKE_HOOK_OFFSETLESS(BeatmapObjectSpawnMovementData_Init, void,
    BeatmapObjectSpawnMovementData* self,
    int noteLinesCount,
    float startNoteJumpMovementSpeed,
    float startBpm,
    float noteJumpStartBeatOffset,
    float jumpOffsetY,
    Vector3 rightVec,
    Vector3 forwardVec
) {
    if (startNoteJumpMovementSpeed == 10.0f && getConfig().config[ConfigEnabled].GetBool()) {
        Submission::disable(modInfo);
        float targetNjs = getConfig().config[ConfigTargetNjs].GetFloat();
        getLogger().info("Found a song with NJS 10, increasing it to %.2f", targetNjs);
        startNoteJumpMovementSpeed = targetNjs;
    }
    BeatmapObjectSpawnMovementData_Init(self, noteLinesCount, startNoteJumpMovementSpeed, startBpm, noteJumpStartBeatOffset, jumpOffsetY, rightVec, forwardVec);
}

MAKE_HOOK_OFFSETLESS(GameplaySetupViewController_DidActivate, void, GlobalNamespace::GameplaySetupViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    GameplaySetupViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (!Submission::getEnabled()) {
        Submission::enable(modInfo);
    }
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

extern "C" void setup(ModInfo& info) {
    info.id = "NjsTweaks";
    info.version = "0.0.1";
    modInfo = info;
	
    getConfig().Load();
    getLogger().info("Completed setup!");

    Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    bool configModified = false;
    if (!getConfig().config.HasMember(ConfigEnabled)) {
      getConfig().config.AddMember(StringRef(ConfigEnabled), Value().SetBool(true), allocator);
      configModified = true;
    }
    if (!getConfig().config.HasMember(ConfigTargetNjs)) {
      getConfig().config.AddMember(StringRef(ConfigTargetNjs), Value().SetFloat(14), allocator);
      configModified = true;
    }
    if (configModified) {
      getConfig().Write();
    }
}

extern "C" void load() {
    il2cpp_functions::Init();
    QuestUI::Init();
    auto& logger = getLogger();
    logger.info("Installing NjsTweaks hooks...");
    
    custom_types::Register::RegisterType<NjsTweaksViewController>();
    QuestUI::Register::RegisterModSettingsViewController<NjsTweaksViewController*>(modInfo);

    INSTALL_HOOK_OFFSETLESS(logger, BeatmapObjectSpawnMovementData_Init, il2cpp_utils::FindMethodUnsafe("", "BeatmapObjectSpawnMovementData", "Init", 7));
    INSTALL_HOOK_OFFSETLESS(logger, GameplaySetupViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "GameplaySetupViewController", "DidActivate", 3));

    logger.info("Installed all NjsTweaks hooks...");
}