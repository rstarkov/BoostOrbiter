//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>
#include <OrbitalMath/OrbitalMath.h>

namespace OrbitalMath { namespace OrbitalFunc {

    // These are functions that are unlikely to be useful in all the possible forms. Functions that make up
    // a complete set are placed in OrbitalFuncCore.h

    inline double StdGravParam0(double Mass)
    {
        return OrbitalMath::G * Mass;
    }

    inline double DistanceAtApoapsis1(double Eccentricity, double SemiLatusRectum)
    {
        return SemiLatusRectum / (1 - Eccentricity);
    }

    inline double Distance1(double Eccentricity, double SemiLatusRectum, double TrueAnomaly)
    {
        // From the polar form of a conic section
        return SemiLatusRectum / (1 + Eccentricity * cos(TrueAnomaly));
    }

    inline double Distance2(double Eccentricity, double SemiMajorAxis, double EccentricAnomaly)
    {
        return SemiMajorAxis * (1 - Eccentricity * cos(EccentricAnomaly));
    }

    inline double TimeOfPeriapsisPassage1(double Period, double MeanAnomaly)
    {
        return Period * MeanAnomaly / (2*PI);
    }

    // Computes the [period] of the orbit.
    inline double Period1(double SemiMajorAxis, double StdGravParam)
    {
        SemiMajorAxis = abs(SemiMajorAxis);
        return 2*PI * SemiMajorAxis * sqrt(SemiMajorAxis / StdGravParam);
    }

    // Computes the orbital speed at [periapsis]
    inline double SpeedAtPeriapsis1(double Eccentricity, double SemiLatusRectum, double StdGravParam)
    {
        return (1 + Eccentricity) * sqrt(StdGravParam / SemiLatusRectum);
    }

    // Computes the orbital speed at [apoapsis]
    inline double SpeedAtApoapsis1(double Eccentricity, double SemiLatusRectum, double StdGravParam)
    {
        return (1 - Eccentricity) * sqrt(StdGravParam / SemiLatusRectum);
    }

    inline double EscapeSpeed1(double StdGravParam, double Distance)
    {
        return sqrt(2 * StdGravParam / Distance);
    }

    inline double GravAccel1(double StdGravParam, double Distance)
    {
        return StdGravParam / (Distance * Distance);
    }

}}
