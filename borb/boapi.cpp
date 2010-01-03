//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>

namespace borb
{
    namespace boapi
    {
        OBJHANDLE GetVesselByName(const std::string& name)
        {
            char cstr[256];
            size_t length = name.copy(cstr, sizeof(cstr) - 1);
            cstr[length] = 0;
            return oapiGetVesselByName(cstr);
        }
    }
}
