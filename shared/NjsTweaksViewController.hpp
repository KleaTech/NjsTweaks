#include "config-utils/shared/config-utils.hpp"
#include "HMUI/ViewController.hpp"
#include "custom-types/shared/macros.hpp"

//Contains the mod settings UI
/*DECLARE_CONFIG(NjsTweaksConfig,
    CONFIG_VALUE(enabled, bool, "Enable", true);
    CONFIG_VALUE(logToFile, bool, "Log to file", false);
    CONFIG_VALUE(barControlVerticalOffset, float, "Toolbar vertical offset", 0.0f);
        CONFIG_INIT_FUNCTION(
            CONFIG_INIT_VALUE(enabled);
            CONFIG_INIT_VALUE(logToFile);
            CONFIG_INIT_VALUE(barControlVerticalOffset);
        )
) */

DECLARE_CLASS_CODEGEN(NjsTweaks, NjsTweaksViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
)