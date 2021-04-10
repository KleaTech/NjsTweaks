#include "NjsTweaksUtils.hpp"
#include "NjsTweaksCommon.hpp"
using namespace NjsTweaks::Common;

namespace NjsTweaks { namespace Utils {
    Il2CppString* floatToString(float value) {
        try {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(1) << value;
            return il2cpp_utils::createcsstr(stream.str());
        } catch (...) {
            getLogger().error("Error in FloatToString");
        }
        return il2cpp_utils::createcsstr("Error");
    }
}}