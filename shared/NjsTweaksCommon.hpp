#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "bs-utils/shared/utils.hpp"

namespace NjsTweaks { namespace Common {
    struct config_t {
        bool enabled = true;
        bool logToFile = false;
        float barControlVerticalOffset = 0.0f;
    };

    const std::string exception = "ex"; //Since empty throw statement is not a good idea, this dummy exception can be used with throw statements, eg. catch (...) { throw exception; }

    extern config_t myConfig; //Holds the mod settings after initialize in memory. Call saveConfig to persist.
    extern ModInfo modInfo; //Holds the current ModInfo.
    
    //Initializes mod settings, logging and potentially other common stuff.
    void initialize(ModInfo modInfo);
    //Returns the logger after it was configured. The logger instance changes dinamically based on the logToFile mod setting.
    Logger& getLogger();
    //Writes the current state of configuration to file. Without calling this, the config changes won't persist.
    void saveConfig();
    //If score submission is enabled, this returns an empty string, otherwise returns the first diabling mod. NjsTweaks will always be returned if it's amongst the disabling mods.
    std::string getSubmissionDisablingMod();
}}