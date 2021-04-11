#include <math.h>
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

    //This implementation is probably correct, but I'm not 100% sure. Use with caution.
    bool floatEquals(float f1, float f2) {
        if (fabsf(f1 - f2) < 0.05) {
            return true;
        }
        return false;
    }

    //This implementation is probably correct, but I'm not 100% sure. Use with caution.
    bool floatLowerThan(float f1, float f2) {
        if (fabsf(f2 - f1) < 0.05) {
            return true;
        }
        return false;
    }
}}