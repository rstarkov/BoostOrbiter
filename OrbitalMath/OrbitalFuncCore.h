//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <OrbitalMath/OrbitalMath.h>

namespace OrbitalMath { namespace OrbitalFunc {

    inline double Eccentricity1(double SemiLatusRectum, double DistanceAtPeriapsis)
    {
        return SemiLatusRectum / DistanceAtPeriapsis - 1;
    }

    inline double SemiLatusRectum1(double Eccentricity, double DistanceAtPeriapsis)
    {
        return DistanceAtPeriapsis * (1 + Eccentricity);
    }

    inline double DistanceAtPeriapsis1(double SemiLatusRectum, double Eccentricity)
    {
        return SemiLatusRectum / (1 + Eccentricity);
    }

    //====================================================//

    inline double SemiLatusRectum2(double StdGravParam, double SpecRelAngMomentum)
    {
        return SpecRelAngMomentum * SpecRelAngMomentum / StdGravParam;
    }

    inline double StdGravParam2(double SemiLatusRectum, double SpecRelAngMomentum)
    {
        return SpecRelAngMomentum * SpecRelAngMomentum / SemiLatusRectum;
    }

    inline double SpecRelAngMomentum2(double SemiLatusRectum, double StdGravParam)
    {
        return sqrt(SemiLatusRectum * StdGravParam);
    }

    //====================================================//

    inline double Eccentricity3(double SemiMajorAxis, double SemiLatusRectum)
    {
        return sqrt(1 - SemiLatusRectum / SemiMajorAxis);
    }

    inline double SemiMajorAxis3(double Eccentricity, double SemiLatusRectum)
    {
        return SemiLatusRectum / (1 - Eccentricity * Eccentricity);
    }

    inline double SemiLatusRectum3(double Eccentricity, double SemiMajorAxis)
    {
        return SemiMajorAxis * (1 - Eccentricity * Eccentricity);
    }

    //====================================================//

    inline double SemiMajorAxis1(double StdGravParam, double SpecOrbitalEnergy)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return -StdGravParam / (2 * SpecOrbitalEnergy);
    }

    inline double StdGravParam1(double SemiMajorAxis, double SpecOrbitalEnergy)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return -2 * SpecOrbitalEnergy * SemiMajorAxis;
    }

    inline double SpecOrbitalEnergy1(double SemiMajorAxis, double StdGravParam)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return -StdGravParam / (2 * SemiMajorAxis);
    }

    //====================================================//

    inline double Eccentricity2(double SemiMajorAxis, double DistanceAtPeriapsis)
    {
        return 1 - DistanceAtPeriapsis / SemiMajorAxis;
    }

    inline double SemiMajorAxis2(double Eccentricity, double DistanceAtPeriapsis)
    {
        return DistanceAtPeriapsis / (1 - Eccentricity);
    }

    inline double DistanceAtPeriapsis2(double Eccentricity, double SemiMajorAxis)
    {
        return (1 - Eccentricity) * SemiMajorAxis;
    }

    //====================================================//

    inline double ArgPeriapsis1(double LonPeriapsis, double LonAscendingNode)
    {
        return LonPeriapsis - LonAscendingNode;
    }

    inline double LonPeriapsis1(double ArgPeriapsis, double LonAscendingNode)
    {
        return LonAscendingNode + ArgPeriapsis;
    }

    inline double LonAscendingNode1(double ArgPeriapsis, double LonPeriapsis)
    {
        return LonPeriapsis - ArgPeriapsis;
    }

    //====================================================//

    inline double MeanAnomaly3(double MeanLonAtEpoch, double LonPeriapsis)
    {
        return MeanLonAtEpoch - LonPeriapsis;
    }

    inline double LonPeriapsis3(double MeanAnomaly, double MeanLonAtEpoch)
    {
        return MeanLonAtEpoch - MeanAnomaly;
    }

    inline double MeanLonAtEpoch3(double MeanAnomaly, double LonPeriapsis)
    {
        return MeanAnomaly + LonPeriapsis;
    }

    //====================================================//
    //====================================================//

    inline double StdGravParam3(double SpecOrbitalEnergy, double Speed, double Distance)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return Distance * (Speed * Speed / 2  -  SpecOrbitalEnergy);
    }

    inline double SpecOrbitalEnergy3(double StdGravParam, double Speed, double Distance)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return Speed * Speed / 2  -  StdGravParam / Distance;
    }

    inline double Speed3(double StdGravParam, double SpecOrbitalEnergy, double Distance)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return sqrt(2 * (SpecOrbitalEnergy + StdGravParam / Distance));
    }

    inline double Distance3(double StdGravParam, double SpecOrbitalEnergy, double Speed)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return StdGravParam / (Speed * Speed / 2 - SpecOrbitalEnergy);
    }

    //====================================================//

    inline double Eccentricity4(double StdGravParam, double SpecOrbitalEnergy, double SpecRelAngMomentum)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return sqrt(1 + (2 * SpecOrbitalEnergy * SpecRelAngMomentum * SpecRelAngMomentum) / (StdGravParam * StdGravParam));
        // sqrt ok because always positive
    }

    inline double StdGravParam4(double Eccentricity, double SpecOrbitalEnergy, double SpecRelAngMomentum)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return sqrt(SpecOrbitalEnergy * (-2 * SpecRelAngMomentum * SpecRelAngMomentum) / (1 - Eccentricity*Eccentricity));
        // sqrt ok because always positive
    }

    inline double SpecOrbitalEnergy4(double Eccentricity, double StdGravParam, double SpecRelAngMomentum)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return StdGravParam * StdGravParam * (1 - Eccentricity*Eccentricity) / (-2 * SpecRelAngMomentum * SpecRelAngMomentum);
    }

    inline double SpecRelAngMomentum4(double Eccentricity, double StdGravParam, double SpecOrbitalEnergy)
    {
        // http://en.wikipedia.org/wiki/Specific_orbital_energy
        return sqrt(StdGravParam * StdGravParam * (1 - Eccentricity*Eccentricity) / (-2 * SpecOrbitalEnergy));
        // hyperbolic: orbital energy positive => denominator negative. 1 - e^2 negative so numerator negative too. => always positive
        // elliptic: orbital energy negative => denominator positive. 1 - e^2 positive so numerator positive too. => always positive
    }

    //====================================================//

    inline double SemiMajorAxis5(double StdGravParam, double Distance, double Speed)
    {
        return Distance * StdGravParam / (2 * StdGravParam - Distance * Speed * Speed);
    }
    
    inline double StdGravParam5(double SemiMajorAxis, double Speed, double Distance)
    {
        return Speed * Speed * Distance * SemiMajorAxis / (2 * SemiMajorAxis - Distance);
    }

    inline double Speed5(double SemiMajorAxis, double StdGravParam, double Distance)
    {
        return sqrt(StdGravParam * (2 / Distance - 1 / SemiMajorAxis));
    }

    inline double Distance5(double SemiMajorAxis, double StdGravParam, double Speed)
    {
        return 2 * StdGravParam * SemiMajorAxis / (Speed * Speed * SemiMajorAxis + StdGravParam);
    }

}}
