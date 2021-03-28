#include "NjsTweaksViewController.hpp"
#include "main.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"
using namespace HMUI;
using namespace UnityEngine;
using namespace NjsTweaks;
using namespace QuestUI;

DEFINE_CLASS(NjsTweaksViewController);

void NjsTweaksViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        get_gameObject() -> AddComponent<Touchable*>();
        auto container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        
        auto enabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "Enable", getConfig().config[ConfigEnabled].GetBool(), [](bool newValue) {
            getConfig().config[ConfigEnabled].SetBool(newValue);
        });
        BeatSaberUI::AddHoverHint(enabledToggleObject -> get_gameObject(), "When enabled all 10 NJS song will play at the set target NJS.");
        
        auto targetNjsSetterObject = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Target NJS", 1, 0.5, getConfig().config[ConfigTargetNjs].GetFloat(), 10.0f, 25.0f, [](float newValue) {
            getConfig().config[ConfigTargetNjs].SetFloat(newValue);
        });
        BeatSaberUI::AddHoverHint(targetNjsSetterObject -> get_gameObject(), "NJS to use instead of 10.");        
    }
}

void NjsTweaksViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    getConfig().Write();
}