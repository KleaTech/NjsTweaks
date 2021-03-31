#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "bs-utils/shared/utils.hpp"

namespace NjsTweaks { namespace Common {
    struct config_t {
        bool enabled = true;
        bool autoIncrease10Njs = false;
        bool onlyOnExpert = false;
        float autoIncreaseTargetNjs = 14.0f;
        bool logToFile = false;
    };

    enum SubmissionState { SubmissionEnabled, SubmissionDisabled, SubmissionDisabledByOthers };

    extern config_t myConfig;
    extern ModInfo modInfo;
    
    void Initialize(ModInfo modInfo);
    Logger& getLogger();
    void SaveConfig();
    SubmissionState GetSubmissionState();
}}