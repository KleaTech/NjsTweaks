#include "UnityEngine/Transform.hpp"

namespace NjsTweaks { namespace UI {
    extern float njsSetting;        //Contains the NJS that the user set using this mod
    extern float offsetSetting;     //Contains the Offset that the user set using this mod

    //Creates the toolbar where the user sees the NJS and Offset values, and where they can change them is the mod is enabled. This toolbar should be added to StandardLevelDetailViewController.
    void createNjsTweaksBarControl(UnityEngine::Transform* parent);
    //This method must be called when the map, it's difficulty or it's characteristic has changed.
    void onMapChange(float originalNjs, float originalOffset);
}}