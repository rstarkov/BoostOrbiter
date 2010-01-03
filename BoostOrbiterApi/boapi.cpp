#include <PrecompiledBoostOrbiter.h>

namespace BoostOrbiter
{
    namespace BoostOrbiterApi
    {
        OBJHANDLE boapiGetVesselByName(const std::string& name)
        {
            char cstr[256];
            size_t length = name.copy(cstr, sizeof(cstr) - 1);
            cstr[length] = 0;
            return oapiGetVesselByName(cstr);
        }
    }
}