#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

namespace NjsTweaks {
    const char* const ConfigEnabled = "NjsTweaks_Config_Enabled";
    const char* const ConfigTargetNjs = "NjsTweaks_Config_TargetNjs";
}

DECLARE_CLASS_CODEGEN(NjsTweaks, NjsTweaksViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);

    REGISTER_FUNCTION(NjsTweaksViewController,
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
    )
)