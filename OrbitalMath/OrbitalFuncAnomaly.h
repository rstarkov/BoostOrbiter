//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>
#include <OrbitalMath/OrbitalMath.h>

namespace OrbitalMath { namespace OrbitalFunc {

    // Main source: http://www.astro.uu.nl/~strous/AA/en/reken/kepler.html (warning: hyperbolic eccentric anomaly equation has a wrong sign)

    // Computes the [mean anomaly] of a body in an orbit.
    inline double MeanAnomaly1(double Period, double TimeOfPeriapsisPassage)
    {
        return 2*PI * TimeOfPeriapsisPassage / Period;
    }

    // Computes the [mean anomaly] of a body in orbit.
    inline double MeanAnomaly2(double Eccentricity, double EccentricAnomaly)
    {
        return EccentricAnomaly - Eccentricity * (Eccentricity < 1 ? sin(EccentricAnomaly) : sinh(EccentricAnomaly));
    }

    // Computes the [eccentric anomaly] of a body in an elliptic orbit. Eccentricity must be less than 1.
    double EccentricAnomalyElliptic1(double Eccentricity, double MeanAnomaly)
    {
        if (Eccentricity >= 1)
            return std::numeric_limits<double>::quiet_NaN();
        // Solve: MeanAnomaly = EccentricAnomaly - Eccentricity * sin(EccentricAnomaly)
        // Function: x - Eccentricity * sin(x) - MeanAnomaly   (for Eccentricity < 1, the function is monotonically increasing)
        // Derivative: 1 - Eccentricity * cos(x)
        double x1 = MeanAnomaly; // a natural starting point
        double x2;
        double epsilon = 1e-12;

        x2 = x1 - (x1 - Eccentricity * sin(x1) - MeanAnomaly)  /  (1 - Eccentricity * cos(x1)); // very unlikely to succeed on the first two iterations
        x1 = x2 - (x2 - Eccentricity * sin(x2) - MeanAnomaly)  /  (1 - Eccentricity * cos(x2));
        x2 = x1 - (x1 - Eccentricity * sin(x1) - MeanAnomaly)  /  (1 - Eccentricity * cos(x1)); if (abs(x1 - x2) < epsilon) return x2;
        x1 = x2 - (x2 - Eccentricity * sin(x2) - MeanAnomaly)  /  (1 - Eccentricity * cos(x2)); if (abs(x1 - x2) < epsilon) return x1;
        x2 = x1 - (x1 - Eccentricity * sin(x1) - MeanAnomaly)  /  (1 - Eccentricity * cos(x1)); if (abs(x1 - x2) < epsilon) return x2;
        x1 = x2 - (x2 - Eccentricity * sin(x2) - MeanAnomaly)  /  (1 - Eccentricity * cos(x2)); if (abs(x1 - x2) < epsilon) return x1;
        x2 = x1 - (x1 - Eccentricity * sin(x1) - MeanAnomaly)  /  (1 - Eccentricity * cos(x1)); if (abs(x1 - x2) < epsilon) return x2;
        x1 = x2 - (x2 - Eccentricity * sin(x2) - MeanAnomaly)  /  (1 - Eccentricity * cos(x2)); if (abs(x1 - x2) < epsilon) return x1;

        double _min = MeanAnomaly - 2*PI;
        double _max = MeanAnomaly + 2*PI;
        while (true) // ~1% of all inputs get into this loop
        {
            x2 = x1 - (x1 - Eccentricity * sin(x1) - MeanAnomaly)  /  (1 - Eccentricity * cos(x1));

            // Do a bisection step if the Newton-Raphson step failed
            if (x2 < _min || x2 > _max)
            {
                double _cen = _min + (_max - _min) / 2;
                double fcen = _cen - Eccentricity * sin(_cen) - MeanAnomaly;
                // The next statement assumes that the function is monotonically increasing
                if (fcen > 0)
                    _max = _cen;
                else
                    _min = _cen;
                x2 = _min + (_max - _min) / 2;
            }

            if (abs(x1 - x2) < epsilon)
                return x2;
            x1 = x2;
        }
    }

    // Computes the [eccentric anomaly] of a body in a hyperbolic orbit. Eccentricity must be 1 or greater.
    double EccentricAnomalyHyperbolic1(double Eccentricity, double MeanAnomaly)
    {
        if (Eccentricity < 1)
            return std::numeric_limits<double>::quiet_NaN();
        // Solve: EccentricAnomaly = MeanAnomaly + Eccentricity * sinh(EccentricAnomaly)
        // Function: x - Eccentricity * sin(x) - MeanAnomaly   (for Eccentricity < 1, the function is monotonically increasing)
        // Derivative: 1 - Eccentricity * cos(x)
        double x1 = MeanAnomaly; // a natural starting point
        double x2;
        double epsilon = 1e-12;

        x2 = asinh((x1 + MeanAnomaly) / Eccentricity); // very unlikely to succeed on the first few iterations
        x1 = asinh((x2 + MeanAnomaly) / Eccentricity);
        x2 = asinh((x1 + MeanAnomaly) / Eccentricity);
        x1 = asinh((x2 + MeanAnomaly) / Eccentricity);
        x2 = asinh((x1 + MeanAnomaly) / Eccentricity);
        x1 = asinh((x2 + MeanAnomaly) / Eccentricity); if (abs(x1 - x2) < epsilon) return x1;
        x2 = asinh((x1 + MeanAnomaly) / Eccentricity); if (abs(x1 - x2) < epsilon) return x2;
        x1 = asinh((x2 + MeanAnomaly) / Eccentricity); if (abs(x1 - x2) < epsilon) return x1;
        x2 = asinh((x1 + MeanAnomaly) / Eccentricity); if (abs(x1 - x2) < epsilon) return x2;
        x1 = asinh((x2 + MeanAnomaly) / Eccentricity); if (abs(x1 - x2) < epsilon) return x1;
        x2 = asinh((x1 + MeanAnomaly) / Eccentricity); if (abs(x1 - x2) < epsilon) return x2;
        x1 = asinh((x2 + MeanAnomaly) / Eccentricity); if (abs(x1 - x2) < epsilon) return x1;

        while (true)
        {
            x2 = asinh((x1 + MeanAnomaly) / Eccentricity);

            if (abs(x1 - x2) < epsilon)
                return x2;
            x1 = x2;
        }
    }

    // Computes the [eccentric anomaly] of a body in orbit.
    inline double EccentricAnomaly1(double Eccentricity, double MeanAnomaly)
    {
        if (Eccentricity < 1)
            return EccentricAnomalyElliptic1(Eccentricity, MeanAnomaly);
        else
            return EccentricAnomalyHyperbolic1(Eccentricity, MeanAnomaly);
    }

    // Computes the [eccentric anomaly] of a body in orbit.
    inline double EccentricAnomaly2(double Eccentricity, double TrueAnomaly)
    {
        return 2 * atan(sqrt((1 - Eccentricity) / (1 + Eccentricity)) * tan(TrueAnomaly / 2));
    }

    // Computes the [true anomaly] of a body in orbit.
    inline double TrueAnomaly1(double Eccentricity, double EccentricAnomaly)
    {
        // For elliptic orbits: http://www.space-plasma.qmw.ac.uk/heliocoords/systems2art/node15.html
        // For hyperbolic orbits: same thing with a cosh
        double cosine = Eccentricity < 1 ? cos(EccentricAnomaly) : cosh(EccentricAnomaly);
        return acos( (Eccentricity - cosine) / (Eccentricity * cosine - 1) );
    }

}}
