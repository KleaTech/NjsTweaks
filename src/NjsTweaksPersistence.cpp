#include "NjsTweaksPersistance.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "NjsTweaksCommon.hpp"

using namespace NjsTweaks::Common;

namespace NjsTweaks { namespace Persistence {
    std::string mapHash(Il2CppString* levelId, BeatmapDifficulty difficulty, BeatmapCharacteristicSO* characteristic) {
        std::string levelIdStr = to_utf8(csstrtostr(levelId));
        std::string difficultyStr = std::to_string(difficulty.value);
        std::string characteristicStr = std::to_string(characteristic -> sortingOrder);
        getLogger().debug("Map hash: id %s diff %s char %s", levelIdStr.c_str(), difficultyStr.c_str(), characteristicStr.c_str());
        return levelIdStr + difficultyStr + characteristicStr;
    }
    
    entity_t load(std::string mapHash);
    void save(entity_t entity);
}}