//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

#include "SketchpadHelper.h"

namespace borb {

    class MfdBase
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        : public MFD2
#else
        : public MFD
#endif
    {
    public:
        MfdBase(DWORD width, DWORD height, VESSEL *vessel)
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
            : MFD2(width, height, vessel), _vessel(vessel)
#else
            : MFD(width, height, vessel), _vessel(vessel)
#endif
        {
        }

        virtual void Update(SketchpadHelper* skh) = 0;

#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        bool Update(oapi::Sketchpad* skp);
#else
        void Update(HDC hDC);
        int GetWidth() { return W; }
        int GetHeight() { return H; }
#endif

    protected:
        VESSEL* _vessel; // owned by Orbiter - rather more readable than "pv"
    };
}
