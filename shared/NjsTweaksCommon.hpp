#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"

namespace NjsTweaks { namespace Common {
    struct config_t {
        bool enabled = true;
        bool autoIncrease10Njs = false;
        bool onlyOnExpert = false;
        float autoIncreaseTargetNjs = 14.0f;
        bool logToFile = false;
    };

    extern config_t myConfig;
    extern ModInfo modInfo;
    Logger& getLogger();
    void SaveConfig();
    void Initialize(ModInfo modInfo);
}}