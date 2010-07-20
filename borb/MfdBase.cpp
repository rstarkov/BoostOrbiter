//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "MfdBase.h"

namespace borb {

    using namespace std;

#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
    bool MfdBase::Update(oapi::Sketchpad* skp)
    {
        shared_ptr<borb::SketchpadHelper> skh = make_shared<borb::SketchpadHelper>(skp, GetWidth(), GetHeight());
        Update(skh.get());
    }
#else
    void MfdBase::Update(HDC hDC)
    {
        shared_ptr<borb::Sketchpad> skp = GetSketchpad(hDC);
        shared_ptr<borb::SketchpadHelper> skh = make_shared<borb::SketchpadHelper>(skp.get(), GetWidth(), GetHeight());
        Update(skh.get());
    }
#endif

}
