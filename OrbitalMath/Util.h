//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>
#include <OrbitalMath/OrbitalMath.h>

namespace OrbitalMath {

    // Computes the inverse hyperbolic sine of the specified argument. 
    double asinh(double x)
    {
        // Source: http://www.tiac.net/~sw/2007/02/asinh_perspective/index.html
        if (x >= 0)
            return log(sqrt(x*x + 1) + x);
        else
            return -log(sqrt(x*x + 1) - x);
    }

}
