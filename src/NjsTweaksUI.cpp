#include "NjsTweaksUI.hpp"
#include "NjsTweaksUtils.hpp"
#include "NjsTweaksCommon.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/Color.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/HoverHint.hpp"
#include "bs-utils/shared/utils.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace GlobalNamespace;
using namespace TMPro;
using namespace NjsTweaks::Common;
using namespace NjsTweaks::Utils;
using namespace HMUI;
using namespace bs_utils;

//This file uses a seemingly overcomplicated aproach to create the toolbar. I tried a lot of ways to create the necessary UI controls in a better way, but I failed.
//I could not modifie the IncrementSetting provided by BeatSaberUI and I could not use BeatSaberUI's code to create a similar control myself, because it either 
//produced errors I could not fix or looked very wrong. I could not add the controls to a HorizontalLayoutGroup, because the size, position and scaling were always very off.
//So I decided to add all of the controls one by one, scaling, sizing and positioning each one. This is a painstaking process, but the results are nice.
namespace NjsTweaks { namespace UI {
    //Setting 0.5 NJS by default. If something goes wrong, this should be very noticable to the player.
    float njsSetting = 0.5;
    float offsetSetting = 0.5;

    static float originalNjs = 0.5;
    static float originalOffset = 0.5;

    static const float MIN_NJS = 6.0;
    static const float MAX_NJS = 30.0;
    static const float NJS_INCREMENT = 0.5;
    static const float MIN_OFFSET = -2.0;
    static const float MAX_OFFSET = 2.0;
    static const float OFFSET_INCREMENT = 0.1;

    static const float HOFF = 0.0; //Horizontal offset for the whole toolbar, can be useful in the future.
    static float VOFF; //Vertical offset, useful if another mod will reside here in the future.
    static const float ZOFF = 1.0; //Offset in the Z axis to make sure the toolbar wont "cover" existing buttons. Does not seem to be necessary, but I'll leave it in for now. TODO check
    
    static Transform* transform; //Parent transform, here for easy access.
    
    static TextMeshProUGUI* njsText; //The text component that contains the NJS value.
    static TextMeshProUGUI* offsetText; //The text component that contains the Offset value.
    static TextMeshProUGUI* submissionText; //The whole submission text including the label and the value.
    static HoverHint* submissionHint; //The hint that appears over the submission text.
    static Button* resetButton; //The reset score button
    static std::unordered_set<Button*> buttons; //Contains all the real toolbar buttons to be able to enable/disable them.
    static bool previouslyEnabled = false; //Holds if the buttons were enabled previously.

    //Creates a button at the given position
    static Button* createButton(std::string textValue, float posx, float posy, std::function<void()> onClick) {
        static auto const BUTTON_SCALE = Vector3(0.5, 0.4, 0.5);
        static auto const BUTTON_SIZE = Vector2(8.0, 8.0);
        auto button = BeatSaberUI::CreateUIButton(transform, textValue, Vector2(posx + HOFF, posy + VOFF), BUTTON_SIZE, onClick);
        button -> GetComponentInChildren<TextMeshProUGUI*>() -> set_alignment(TextAlignmentOptions::Left);
        button -> get_transform() -> set_localScale(BUTTON_SCALE);
        auto buttonTransform = button -> get_transform();
        auto localPosition = buttonTransform -> get_localPosition();
        localPosition.z += ZOFF;
        buttonTransform -> set_localPosition(localPosition);
        buttons.insert(button);
        return button;
    }

    //Creates an invisible button that's useful for adding hover hint to text (when hint on text is disabled)
    static Button* createInvisibleButton(float posx, float posy, float length) {
        auto button = BeatSaberUI::CreateUIButton(transform, "", Vector2(posx + HOFF, posy + VOFF), Vector2(length, 5.0), [](){});
        button -> set_interactable(false);
        buttons.insert(button);
        return button;
    }

    //Creates a text component at the given position. This disables hover hint on the text, 
    //because otherwise it covers a lot if invisible space and makes buttons unclickable underneath. So use CreateInvisibleButton to add hint.
    static TextMeshProUGUI* createText(std::string textValue, float posx, float posy) {
        static auto const TEXT_SCALE = Vector3(0.9, 0.6, 0.6);
        static auto const FONT_STYLE = FontStyles::Bold | FontStyles::Italic;
        auto text = BeatSaberUI::CreateText(transform, textValue, Vector2(posx + HOFF, posy + VOFF));
        text -> get_transform() -> set_localScale(TEXT_SCALE);
        text -> set_fontStyle(FONT_STYLE);
        text -> set_alpha(0.5f);
        auto textTransform = text -> get_transform();
        auto localPosition = textTransform -> get_localPosition();
        localPosition.z += ZOFF;
        textTransform -> set_localPosition(localPosition);
        text -> set_raycastTarget(false);
        return text;
    }

    //Handle NJS and Offset value change
    static void onValueChange() {
        getLogger().debug("Handling NJS/Offset value change: %.2f / %.2f", njsSetting, offsetSetting);
        njsText -> set_text(floatToString(njsSetting));
        offsetText -> set_text(floatToString(offsetSetting));
        if (!floatEquals(njsSetting, originalNjs) || !floatEquals(offsetSetting, originalOffset)) {
            resetButton -> get_gameObject() -> SetActive(true);
            Submission::disable(modInfo);
            submissionText -> set_text(il2cpp_utils::createcsstr("<color=#FF0000><s>Score</s>"));
            submissionHint -> set_text(il2cpp_utils::createcsstr("Score submission is disabled by NjsTweaks. Click reset to enable it."));
        } else {
            resetButton -> get_gameObject() -> SetActive(false);
            Submission::enable(modInfo);
            auto disablingMod = getSubmissionDisablingMod();
            getLogger().debug("Got submission state.");
            if (disablingMod != "") {
                submissionText -> set_text(il2cpp_utils::createcsstr("Score<color=#FFFF00>?"));
                submissionHint -> set_text(il2cpp_utils::createcsstr("Score submission is disabled by another mod: " + disablingMod));
            } else {
                submissionText -> set_text(il2cpp_utils::createcsstr("<color=#00FF00>Score"));
                submissionHint -> set_text(il2cpp_utils::createcsstr("Score submission is enabled."));
            }
        }
    }

    //Handle mod setting changes. Won't handle vertical offset change, because that would require destorying and recreating every single control. Restarting the game is easier.
    static void onConfigChange() {
        getLogger().debug("NjsTweaks toolbar is handling config change.");
        if (myConfig.enabled) {
            for (auto button : buttons) {
                button -> get_gameObject() -> SetActive(true);
            }
            submissionText -> get_gameObject() -> SetActive(true);
            previouslyEnabled = true;
        } else {
            for (auto button : buttons) {
                button -> get_gameObject() -> SetActive(false);
            }
            submissionText -> get_gameObject() -> SetActive(false);
            previouslyEnabled = false;
        }
    }

    void createNjsTweaksBarControl(Transform* parent) {
        getLogger().info("Creating NjsTweaks toolbar...");
        try {
            VOFF = myConfig.barControlVerticalOffset;
            transform = parent;
            
            //NJS setting
            createText("NJS:", -3.0, -1.0);
            createButton("<", -22.0, 0.5, [](){
               njsSetting -= NJS_INCREMENT;
               if (floatLowerThan(njsSetting, MIN_NJS)) {
                   njsSetting = MIN_NJS;
               }
               onValueChange();
            });
            njsText = createText("ERR", 7.0, -1.0);
            createButton(">", -12.5, 0.5, [](){
                njsSetting += NJS_INCREMENT;
                if (floatLowerThan(MAX_NJS, njsSetting)) {
                    njsSetting = MAX_NJS;
                }
                onValueChange();
            });

            //Offset setting
            createText("Offset:", 20.0, -1.0);
            createButton("<", 4.0, 0.5, [](){
                offsetSetting -= OFFSET_INCREMENT;
                if (floatLowerThan(offsetSetting, MIN_OFFSET)) {
                    offsetSetting = MIN_OFFSET;
                }
                onValueChange();
            });
            offsetText = createText("ERR", 33.0, -1.0);
            createButton(">", 13.5, 0.5, [](){
                offsetSetting += OFFSET_INCREMENT;
                if (floatLowerThan(MAX_OFFSET, offsetSetting)) {
                    offsetSetting = MAX_OFFSET;
                }
                onValueChange();
            });

            //Submission status display
            submissionText = createText("Score: ERROR", 46.0, -1.0);
            auto submissionHintDummy = createInvisibleButton(23.0, 0.5, 11.0);
            submissionHint = BeatSaberUI::AddHoverHint(submissionHintDummy -> get_gameObject(), "Score submission is enabled");

            //Reset settings button
            resetButton = createButton("<color=#FF0000>X", 32.0, 0.5, [](){
                njsSetting = originalNjs;
                offsetSetting = originalOffset;
                onValueChange();
            });
            resetButton -> GetComponentsInChildren<Image*>() -> values[1] -> set_color(Color::get_red());
            BeatSaberUI::AddHoverHint(resetButton -> get_gameObject(), "Reset the NJS and Offset to default");

            onConfigChange();

            getLogger().info("NjsTweaks toolbar is created.");
        } catch (...) {
            getLogger().error("Error in CreateNjsTweaksBarControl");
        }
    }

    void onMapChange(IDifficultyBeatmap* mapDifficulty) {
        if (previouslyEnabled != myConfig.enabled) {
            onConfigChange();
        }
        getLogger().debug("Handling map/difficulty/characteristic change.");
        originalNjs = mapDifficulty -> get_noteJumpMovementSpeed();
        originalOffset = mapDifficulty -> get_noteJumpStartBeatOffset();
        njsSetting = originalNjs;
        offsetSetting = originalOffset;
        IPreviewBeatmapLevel* previewLevel = reinterpret_cast<IPreviewBeatmapLevel*>(mapDifficulty -> get_level());
        auto levelId = previewLevel -> get_levelID();
        getLogger().debug("%s", to_utf8(csstrtostr(levelId)).c_str());
        onValueChange();
    }
}}