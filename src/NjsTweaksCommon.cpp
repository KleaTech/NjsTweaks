#include "NjsTweaksCommon.hpp"
using namespace bs_utils;

static Configuration* configUtil;
static Logger* logger;

namespace NjsTweaks { namespace Common {
    config_t myConfig;
    ModInfo modInfo;

    static void createLogger(bool logToFile) {
        if (logger != nullptr) {
            logger -> flush();
            logger -> close();
        }
        logger = new Logger(modInfo, LoggerOptions(false, logToFile));
        getLogger().debug("Logger created with save to file setting %s", logToFile ? "on." : "off.");
    }

    static void loadConfig() {
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

            if (configUtil -> config.HasMember("NjsTweaksConfig_LogToFile")) {
                myConfig.logToFile = configUtil -> config["NjsTweaksConfig_LogToFile"].GetBool();
            } else {
                configUtil -> config.AddMember("NjsTweaksConfig_LogToFile", myConfig.logToFile, allocator);
                configModified = true;
            }

            if (configUtil -> config.HasMember("NjsTweaksConfig_BarVerticalOffset")) {
                myConfig.barControlVerticalOffset = configUtil -> config["NjsTweaksConfig_BarVerticalOffset"].GetFloat();
            } else {
                configUtil -> config.AddMember("NjsTweaksConfig_BarVerticalOffset", myConfig.barControlVerticalOffset, allocator);
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

    void saveConfig() {
        try {
            getLogger().debug("Saving config...");
            if (configUtil -> config["NjsTweaksConfig_LogToFile"].GetBool() != myConfig.logToFile) {
                getLogger().info("Log to file was changed, now it is %s", myConfig.logToFile ? "enabled." : "disabled.");
                createLogger(myConfig.logToFile);
            }
            configUtil -> config["NjsTweaksConfig_LogToFile"].SetBool(myConfig.logToFile);
            configUtil -> config["NjsTweaksConfig_Enabled"].SetBool(myConfig.enabled);
            configUtil -> config["NjsTweaksConfig_BarVerticalOffset"].SetFloat(myConfig.barControlVerticalOffset);
            configUtil -> Write();
            getLogger().debug("Saving config successful.");
        } catch (...) {
            getLogger().error("Error in SaveConfig");
        }
    }

    void initialize(ModInfo modInfo_in) {
        modInfo = modInfo_in;
        createLogger(false);
        loadConfig();
        if (myConfig.logToFile) {
            getLogger().debug("Immediatly replacing default logger to file logger in Initialize.");
            createLogger(true);
        }
        getLogger().debug("NjsTweaks initializtion successful.");
    }

    Logger& getLogger() {
        return *logger;
    }

    std::string getSubmissionDisablingMod() {
        getLogger().debug("Getting submission state.");
        try {
            if (Submission::getEnabled()) {
                return "";
            }
            for (auto mod : Submission::getDisablingMods()) {
                if (mod.id == modInfo.id) {
                    return mod.id;
                }
            }
        } catch (...) {
            getLogger().error("Error in GetSubmissionState");
        }
        return Submission::getDisablingMods().begin() -> id;
    }
}}