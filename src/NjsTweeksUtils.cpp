#include "NjsTweaksUtils.hpp"
#include "NjsTweaksCommon.hpp"
using namespace NjsTweaks::Common;

namespace NjsTweaks { namespace Utils {
    Il2CppString* FloatKeyValueToString(std::string key, float value) {
        try {
            std::stringstream stream;
            stream << key;
            stream << ": ";
            stream << std::fixed << std::setprecision(1) << value;
            std::string_view str = stream.str();
            return il2cpp_utils::createcsstr(str);
        } catch (...) {
            getLogger().error("Error in FloatKeyValueToString");
        }
        return il2cpp_utils::createcsstr("Error");
    }
}}