#include "NjsTweaksCommon.hpp"
using namespace bs_utils;

static Configuration* configUtil;
static Logger* _logger;

namespace NjsTweaks { namespace Common {
    config_t myConfig;
    ModInfo modInfo;

    static void CreateLogger(bool logToFile) {
        if (_logger != nullptr) {
            _logger -> flush();
            _logger -> close();
        }
        _logger = new Logger(modInfo, LoggerOptions(false, logToFile));
        getLogger().debug("Logger created with save to file setting %s", logToFile ? "on." : "off.");
    }

    static void LoadConfig() {
        try {
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

            if (configUtil -> config.HasMember("NjsTweaksConfig_OnlyOnExpert")) {
                myConfig.onlyOnExpert = configUtil -> config["NjsTweaksConfig_OnlyOnExpert"].GetBool();
            } else {
                configUtil -> config.AddMember("NjsTweaksConfig_OnlyOnExpert", myConfig.onlyOnExpert, allocator);
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
        } catch (...) {
            getLogger().error("Error in LoadConfig");
        }
    };

    void SaveConfig() {
        try {
            getLogger().debug("Saving config...");
            if (configUtil -> config["NjsTweaksConfig_LogToFile"].GetBool() != myConfig.logToFile) {
                getLogger().info("Log to file was changed, now it is %s", myConfig.logToFile ? "enabled." : "disabled.");
                CreateLogger(myConfig.logToFile);
            }
            configUtil -> config["NjsTweaksConfig_LogToFile"].SetBool(myConfig.logToFile);
            configUtil -> config["NjsTweaksConfig_Enabled"].SetBool(myConfig.enabled);
            configUtil -> config["NjsTweaksConfig_AutoIncrease10Njs"].SetBool(myConfig.autoIncrease10Njs);
            configUtil -> config["NjsTweaksConfig_OnlyOnExpert"].SetBool(myConfig.onlyOnExpert);
            configUtil -> config["NjsTweaksConfig_AutoIncreaseTarget"].SetFloat(myConfig.autoIncreaseTargetNjs);
            configUtil -> Write();
            getLogger().debug("Saving config successful.");
        } catch (...) {
            getLogger().error("Error in SaveConfig");
        }
    }

    void Initialize(ModInfo modInfo_in) {
        modInfo = modInfo_in;
        CreateLogger(false);
        LoadConfig();
        if (myConfig.logToFile) {
            getLogger().debug("Immediatly replacing default logger to file logger in Initialize.");
            CreateLogger(true);
        }
    }

    Logger& getLogger() {
        return *_logger;
    }

    SubmissionState GetSubmissionState() {
        try {
            if (Submission::getEnabled()) {
                return SubmissionEnabled;
            }
            for (auto mod : Submission::getDisablingMods()) {
                if (mod.id == modInfo.id) {
                    return SubmissionDisabled;
                }
            }
        } catch (...) {
            getLogger().error("Error in GetSubmissionState");
        }
        return SubmissionDisabledByOthers;
    }
}}