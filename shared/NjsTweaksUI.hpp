#include "UnityEngine/Transform.hpp"

namespace NjsTweaks { namespace UI {
    extern float njsSetting;
    extern float offsetSetting;

    void CreateNjsTweaksBarControl(UnityEngine::Transform* parent);
    void OnMapChange(float originalNjs, float originalOffset);
}}