#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"
#include "NjsTweaksCommon.hpp"
#include "NjsTweaksViewController.hpp"
using namespace HMUI;
using namespace UnityEngine;
using namespace NjsTweaks;
using namespace NjsTweaks::Common;
using namespace QuestUI;
using namespace TMPro;

DEFINE_TYPE(NjsTweaks, NjsTweaksViewController);

namespace NjsTweaks {

    void NjsTweaksViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        try {
            if (firstActivation) {
                getLogger().debug("Creating NjsTweaks settings view...");
                get_gameObject() -> AddComponent<Touchable*>();
                auto container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

                auto enabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "Enable", myConfig.enabled, [](bool newValue) {
                    myConfig.enabled = newValue;
                });
                BeatSaberUI::AddHoverHint(enabledToggleObject -> get_gameObject(), "When disabled all song will have their original NJS.");

                auto logToFileEnabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "Log to file", myConfig.logToFile, [](bool newValue) {
                    myConfig.logToFile = newValue;
                });
                BeatSaberUI::AddHoverHint(logToFileEnabledToggleObject -> get_gameObject(), "If you encounter errors, please enable this and send me the NjsTweaks log from Android/data/com.beatgames.beatsaber/files/logs/.");

                auto barControlOffsetObject = BeatSaberUI::CreateIncrementSetting(container -> get_transform(), "Toolbar vertical offset", 1, 0.5, myConfig.barControlVerticalOffset, -50.0f, 50.0f, [](float newValue) {
                    myConfig.barControlVerticalOffset = newValue;
                });
                BeatSaberUI::AddHoverHint(barControlOffsetObject -> get_gameObject(), "Restart BeatSaber after setting this. If in the future another mod would add some content to the same place on the UI where NjsTweaks resides, you can relocate the NjsTweaks toolbar.");

                getLogger().debug("Created NjsTweaks settings view.");
            }
        } catch (...) {
            getLogger().error("Error in NjsTweaksViewController::DidActivate");
        }
    }

    void NjsTweaksViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
        saveConfig();
    }
}
