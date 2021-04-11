#include <sstream>
#include <iomanip>
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

namespace NjsTweaks { namespace Utils {
    //Converts a float to a string with 1 decimal place.
    Il2CppString* floatToString(float value);
    //Checks if two float numbers are equal to 1 decimal place.
    bool floatEquals(float f1, float f2);
    //Checks if the forst float is lower than the second to 1 decimal place.
    bool floatLowerThan(float f1, float f2);
}}