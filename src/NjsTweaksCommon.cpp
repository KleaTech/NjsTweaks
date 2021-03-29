#include "NjsTweaksCommon.hpp"

static Configuration* configUtil;
static Logger* _logger;

namespace NjsTweaks { namespace Common {
    config_t myConfig;
    ModInfo modInfo;

    static void CreateLogger(bool logToFile) {
        _logger = new Logger(modInfo, LoggerOptions(false, logToFile));
        if (logToFile) {
            getLogger().debug("Logger created with save to file setting on.");
        } else {
            getLogger().debug("Logger created with save to file setting off.");
        }
    }

    static void LoadConfig() {
        getLogger().debug("Loading config...");
        configUtil = new Configuration(modInfo);
        configUtil -> Load();
        auto& allocator = configUtil -> config.GetAllocator();
        bool configModified = false;

        if (configUtil -> config.HasMember("NjsTweaksConfig_Enabled")) {
            myConfig.enabled = configUtil -> config["NjsTweaksConfig_Enabled"].GetBool();
        } else {
            configUtil -> config.AddMember("NjsTweaksConfig_Enabled", myConfig.enabled, allocator);
            configModified = true;
        }

        if (configUtil -> config.HasMember("NjsTweaksConfig_AutoIncrease10Njs")) {
            myConfig.autoIncrease10Njs = configUtil -> config["NjsTweaksConfig_AutoIncrease10Njs"].GetBool();
        } else {
            configUtil -> config.AddMember("NjsTweaksConfig_AutoIncrease10Njs", myConfig.autoIncrease10Njs, allocator);
            configModified = true;
        }

        if (configUtil -> config.HasMember("NjsTweaksConfig_AutoIncreaseTarget")) {
            myConfig.autoIncreaseTargetNjs = configUtil -> config["NjsTweaksConfig_AutoIncreaseTarget"].GetFloat();
        } else {
            configUtil -> config.AddMember("NjsTweaksConfig_AutoIncreaseTarget", myConfig.autoIncreaseTargetNjs, allocator);
            configModified = true;
        }

        if (configUtil -> config.HasMember("NjsTweaksConfig_LogToFile")) {
            myConfig.logToFile = configUtil -> config["NjsTweaksConfig_LogToFile"].GetBool();
        } else {
            configUtil -> config.AddMember("NjsTweaksConfig_LogToFile", myConfig.logToFile, allocator);
            configModified = true;
        }

        if (configModified) {
            configUtil -> Write();
            getLogger().debug("Default values were written to the config.");
        }

        getLogger().debug("Loading config successful.");
    };

    void SaveConfig() {
        getLogger().debug("Saving config...");
        configUtil -> config["NjsTweaksConfig_Enabled"].SetBool(myConfig.enabled);
        configUtil -> config["NjsTweaksConfig_AutoIncrease10Njs"].SetBool(myConfig.autoIncrease10Njs);
        configUtil -> config["NjsTweaksConfig_AutoIncreaseTarget"].SetFloat(myConfig.autoIncreaseTargetNjs);
        configUtil -> Write();
        getLogger().debug("Saving config successful.");
    }

    void Initialize(ModInfo modInfo_in) {
        modInfo = modInfo_in;
        CreateLogger(false);
        LoadConfig();
    }

    Logger& getLogger() {
        return *_logger;
    }
}}