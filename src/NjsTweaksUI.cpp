#include "NjsTweaksUI.hpp"
#include "NjsTweaksUtils.hpp"
#include "NjsTweaksCommon.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/HoverHint.hpp"
#include "bs-utils/shared/utils.hpp"
using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace GlobalNamespace;
using namespace TMPro;
using namespace NjsTweaks::Common;
using namespace NjsTweaks::Utils;
using namespace HMUI;
using namespace bs_utils;

namespace NjsTweaks { namespace UI {
    //Setting 0.5 NJS by default. If something goes wrong, this should be very noticable to the player.
    float njsSetting = 0.5;
    float offsetSetting = 0.5;

    static float originalNjs = 0.5;
    static float originalOffset = 0.5;
    static const float HOFF = 0.0; //Horizontal offset for the whole toolbar, can be useful in the future
    static float VOFF; //Vertical offset
    static const float ZOFF = 1.0; //Offset in the Z axis to make sure the toolbar wont "cover" existing buttons
    static Transform* transform; //Parent transform, here for easy access
    static TextMeshProUGUI* njsText;
    static TextMeshProUGUI* offsetText;
    static TextMeshProUGUI* submissionText;
    static HoverHint* submissionHint;

    static Button* CreateButton(std::string textValue, float posx, float posy, std::function<void()> onClick) {
        static auto const BUTTON_SCALE = Vector3(0.5, 0.4, 0.5);
        static auto const BUTTON_SIZE = Vector2(8.0, 8.0);
        auto button = BeatSaberUI::CreateUIButton(transform, textValue, Vector2(posx + HOFF, posy + VOFF), BUTTON_SIZE, onClick);
        button -> GetComponentInChildren<TextMeshProUGUI*>() -> set_alignment(TextAlignmentOptions::Left);
        button -> get_transform() -> set_localScale(BUTTON_SCALE);
        auto buttonTransform = button -> get_transform();
        auto localPosition = buttonTransform -> get_localPosition();
        localPosition.z += ZOFF;
        buttonTransform -> set_localPosition(localPosition);
        return button;
    }

    static Button* CreateInvisibleButton(float posx, float posy, float length) {
        auto button = BeatSaberUI::CreateUIButton(transform, "", Vector2(posx + HOFF, posy + VOFF), Vector2(length, 5.0), [](){});
        button->set_interactable(false);
        //button -> set_image(nullptr);
        return button;
    }

    static TextMeshProUGUI* CreateText(std::string textValue, float posx, float posy) {
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

    static void OnValueChange() {
        njsText -> set_text(FloatToString(njsSetting));
        offsetText -> set_text(FloatToString(offsetSetting));
        //TODO: enable/disable score submission
    }

    void CreateNjsTweaksBarControl(Transform* parent) {
        try {
            VOFF = myConfig.barControlVerticalOffset;
            transform = parent;
            
            //NJS setting
            CreateText("NJS:", -3.0, -1.0);
            CreateButton("<", -22.0, 0.5, [](){});
            njsText = CreateText("ERR", 7.0, -1.0);
            CreateButton(">", -12.5, 0.5, [](){});

            //Offset setting
            CreateText("Offset:", 20.0, -1.0);
            CreateButton("<", 4.0, 0.5, [](){});
            offsetText = CreateText("ERR", 33.0, -1.0);
            CreateButton(">", 13.5, 0.5, [](){});

            //Submission status display
            //TODO change default ON to ERROR
            submissionText = CreateText("Score: ON", 46.0, -1.0);
            auto submissionHintDummy = CreateInvisibleButton(26.0, 0.5, 18.0);
            submissionHint = BeatSaberUI::AddHoverHint(submissionHintDummy -> get_gameObject(), "Score submission is enabled");
        } catch (...) {
            getLogger().error("Error in CreateNjsTweaksBarControl");
        }
    }

    void OnMapChange(float originalNjs_in, float originalOffset_in) {
        originalNjs = originalNjs_in;
        originalOffset = originalOffset_in;
        njsSetting = originalNjs;
        offsetSetting = originalOffset;
        OnValueChange();

        /*switch (GetSubmissionState()) {
            case SubmissionEnabled: 
                submissionText -> set_text(il2cpp_utils::createcsstr("Score: <color=#00FF0088>ON"));
                submissionHint -> set_text(il2cpp_utils::createcsstr("Score submission is enabled"));
                break;
            case SubmissionDisabled:
                submissionText -> set_text(il2cpp_utils::createcsstr("Score: <color=#FF000088>OFF"));
                submissionHint -> set_text(il2cpp_utils::createcsstr("Score submission is disabled by NjsTweaks. Click reset to enable score submission for this song."));
                break;
            case SubmissionDisabledByOthers:
                submissionText -> set_text(il2cpp_utils::createcsstr("Score: <color=#FFFF0088>OTHER MOD"));
                submissionHint -> set_text(il2cpp_utils::createcsstr("Score submission is disabled by another mod: " + Submission::getDisablingMods().begin() -> id));
                break;
            default:
                getLogger().error("GetSubmissionState returned an unexpected value.");
                throw exception;
        }*/
    }
}}