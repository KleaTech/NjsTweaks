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

DEFINE_CLASS(NjsTweaksViewController);

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

            auto autoIncreaseTextObject = BeatSaberUI::CreateText(container -> get_transform(), "Auto-increase NJS settings");
            autoIncreaseTextObject -> set_alignment(TextAlignmentOptions::BottomLeft);
            autoIncreaseTextObject -> set_fontSize(6.0f);
            autoIncreaseTextObject -> set_fontStyle(FontStyles::Italic | FontStyles::Bold);
            BeatSaberUI::AddHoverHint(autoIncreaseTextObject -> get_gameObject(), "Make very old maps more enjoyable.");

            auto autoIncrease10NjsEnabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "    Auto-increase 10 NJS", myConfig.autoIncrease10Njs, [](bool newValue) {
                myConfig.autoIncrease10Njs = newValue;
            });
            BeatSaberUI::AddHoverHint(autoIncrease10NjsEnabledToggleObject -> get_gameObject(), "When enabled all 10 NJS songs will play with a higher NJS by default.");

            auto onlyOnExpertEnabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "    Only on Expert", myConfig.onlyOnExpert, [](bool newValue) {
                myConfig.onlyOnExpert = newValue;
            });
            BeatSaberUI::AddHoverHint(onlyOnExpertEnabledToggleObject -> get_gameObject(), "When Auto-increase and this both are enabled only Expert 10 NJS songs will play with higher NJS.");
            
            auto autoIncreaseTargetNjsObject = BeatSaberUI::CreateIncrementSetting(container->get_transform(), "    Auto-increase NJS target", 1, 0.5, myConfig.autoIncreaseTargetNjs, 10.0f, 25.0f, [](float newValue) {
                myConfig.autoIncreaseTargetNjs = newValue;
            });
            BeatSaberUI::AddHoverHint(autoIncreaseTargetNjsObject -> get_gameObject(), "NJS to use instead of 10 when Auto-increase 10 NJS is enabled.");

            auto debugSettingsTextObject = BeatSaberUI::CreateText(container -> get_transform(), "Debug settings");
            debugSettingsTextObject -> set_alignment(TextAlignmentOptions::BottomLeft);
            debugSettingsTextObject -> set_fontSize(6.0f);
            debugSettingsTextObject -> set_fontStyle(FontStyles::Italic | FontStyles::Bold);
            BeatSaberUI::AddHoverHint(debugSettingsTextObject -> get_gameObject(), "Only touch this if you encounter errors");

            auto logToFileEnabledToggleObject = BeatSaberUI::CreateToggle(container -> get_transform(), "    Log to file", myConfig.logToFile, [](bool newValue) {
                myConfig.logToFile = newValue;
            });
            BeatSaberUI::AddHoverHint(logToFileEnabledToggleObject -> get_gameObject(), "If you encounter errors, please enable this and send me the NjsTweaks log from Android/data/com.beatgames.beatsaber/files/logs/.");

            auto otherSettingsTextObject = BeatSaberUI::CreateText(container -> get_transform(), "Other settings");
            otherSettingsTextObject -> set_alignment(TextAlignmentOptions::BottomLeft);
            otherSettingsTextObject -> set_fontSize(6.0f);
            otherSettingsTextObject -> set_fontStyle(FontStyles::Italic | FontStyles::Bold);
            BeatSaberUI::AddHoverHint(otherSettingsTextObject -> get_gameObject(), "Unsorted settings");

            auto barControlOffsetObject = BeatSaberUI::CreateIncrementSetting(container -> get_transform(), "    Toolbar vertical offset", 1, 0.5, myConfig.barControlVerticalOffset, -50.0f, 50.0f, [](float newValue) {
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
    SaveConfig();
}