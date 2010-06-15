﻿//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>
#include <OrbitalMath/OrbitalMath.h>

namespace OrbitalMath
{

    struct Vector3
    {
        double X, Y, Z;
    };

    // A parametrization into rectangular coordinates
    struct OrbitalState_Rect
    {
        Vector3 Pos;
        Vector3 Vel;
    };

    struct OrbitalState_Nat
    {
        double SemiLatusRectum;
        double Eccentricity;
        double Inclination;
        double LonAscendingNode;
        double ArgPeriapsis;
        double SpecRelAngMomentum;
        double TrueAnomaly;
    };

    // An OrbiterAPI-compatible full state vector
    struct OrbitalState_Compat
    {
        double SemiMajorAxis;
        double Eccentricity;
        double Inclination;
        double LonAscendingNode;
        double LonPeriapsis;
        double MeanLonAtEpoch;
        double StdGravParam; //<- this can probably be calculated from the rest?
        double TrueAnomaly;
    };

}
