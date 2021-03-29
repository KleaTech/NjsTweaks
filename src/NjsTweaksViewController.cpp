#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"
#include "NjsTweaksCommon.hpp"
#include "NjsTweaksViewController.hpp"
using namespace HMUI;
using namespace UnityEngine;
using namespace NjsTweaks;
using namespace NjsTweaks::Common;
using namespace QuestUI;

DEFINE_CLASS(NjsTweaksViewController);

void NjsTweaksViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        get_gameObject() -> AddComponent<Touchable*>();
        auto container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        
        auto enabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "Enable", myConfig.enabled, [](bool newValue) {
            myConfig.enabled = newValue;
        });
        BeatSaberUI::AddHoverHint(enabledToggleObject -> get_gameObject(), "When disabled all song will have their original NJS.");

        auto autoIncrease10NjsEnabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "Auto-increase 10 NJS", myConfig.autoIncrease10Njs, [](bool newValue) {
            myConfig.autoIncrease10Njs = newValue;
        });
        BeatSaberUI::AddHoverHint(autoIncrease10NjsEnabledToggleObject -> get_gameObject(), "When enabled all 10 NJS songs will play with a higher NJS by default.");
        
        auto autoIncreaseTargetNjsObject = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Auto-increase NJS target", 1, 0.5, myConfig.autoIncreaseTargetNjs, 10.0f, 25.0f, [](float newValue) {
            myConfig.autoIncreaseTargetNjs = newValue;
        });
        BeatSaberUI::AddHoverHint(autoIncreaseTargetNjsObject -> get_gameObject(), "NJS to use instead of 10 when Auto-increase 10 NJS is enabled.");
    }
}

void NjsTweaksViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    SaveConfig();
}