#include <string>
#include "il2cpp-api-types.h"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"

using namespace GlobalNamespace;

namespace NjsTweaks { namespace Persistence {
    struct entity_t {
        bool isAvailable;
        float njs;
        float offset;
    };
    std::string mapHash(Il2CppString* levelId, BeatmapDifficulty difficulty, BeatmapCharacteristicSO* characteristic);
    entity_t load(std::string mapHash);
    void save(entity_t entity);
}}