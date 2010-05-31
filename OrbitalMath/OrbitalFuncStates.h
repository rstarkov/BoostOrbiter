#pragma once
#include <OrbitalMath/OrbitalMath.h>

namespace OrbitalMath { namespace OrbitalFunc {

    void OrbitalStateConv_Nat2Rect(const OrbitalState_Nat &src, OrbitalState_Rect *dest)
    {
        // Sources: demosvoe.pdf (State Vectors and Orbital Elements/Numerit), kep2cart_2002.doc, orbiter pdf and a whole bunch of googling

        double r = src.SemiLatusRectum / (1 + src.Eccentricity * cos(src.TrueAnomaly));
        double WV = src.ArgPeriapsis + src.TrueAnomaly; // ω + ν
        double sinWV = sin(WV);
        double cosWV = cos(WV);
        double sinLAN = sin(src.LonAscendingNode);  // Ω
        double cosLAN = cos(src.LonAscendingNode);
        double sinInclination = sin(src.Inclination);
        double cosInclination = cos(src.Inclination);
        double sinWVsinLAN = sinWV * sinLAN;
        double sinWVcosLAN = sinWV * cosLAN;
        double cosWVsinLAN = cosWV * sinLAN;
        double cosWVcosLAN = cosWV * cosLAN;

        dest->Pos.X = r * (cosWVcosLAN - sinWVsinLAN * cosInclination);
        dest->Pos.Y = r * (cosWVsinLAN + sinWVcosLAN * cosInclination);
        dest->Pos.Z = r * (sinWV * sinInclination);

        double scale = src.SpecRelAngMomentum * src.Eccentricity * sin(src.TrueAnomaly) / (r * src.SemiLatusRectum);

        dest->Vel.X = dest->Pos.X * scale - src.SpecRelAngMomentum / r * (sinWVcosLAN + cosWVsinLAN * cosInclination);
        dest->Vel.Y = dest->Pos.Y * scale - src.SpecRelAngMomentum / r * (sinWVsinLAN - cosWVcosLAN * cosInclination);
        dest->Vel.Z = dest->Pos.Z * scale + src.SpecRelAngMomentum / r * (cosWV * sinInclination);
    }

    void OrbitalStateConv_Nat2Compat(const OrbitalState_Nat &src, OrbitalState_Compat *dest)
    {
        dest->Eccentricity = src.Eccentricity;
        dest->Inclination = src.Inclination;
        dest->LonAscendingNode = src.LonAscendingNode;
        dest->TrueAnomaly = src.TrueAnomaly;

        double EccentricAnomaly = EccentricAnomaly2(src.Eccentricity, src.TrueAnomaly);
        double MeanAnomaly = MeanAnomaly2(src.Eccentricity, EccentricAnomaly);

        dest->SemiMajorAxis = SemiMajorAxis3(src.Eccentricity, src.SemiLatusRectum);
        dest->LonPeriapsis = LonPeriapsis1(src.ArgPeriapsis, src.LonAscendingNode);
        dest->MeanLonAtEpoch = MeanLonAtEpoch0(MeanAnomaly, dest->LonPeriapsis);
        dest->StdGravParam = StdGravParam2(src.SemiLatusRectum, src.SpecRelAngMomentum);
    }

    void OrbitalStateConv_Compat2Nat(const OrbitalState_Compat &src, OrbitalState_Nat *dest)
    {
        dest->Eccentricity = src.Eccentricity;
        dest->Inclination = src.Inclination;
        dest->LonAscendingNode = src.LonAscendingNode;
        dest->TrueAnomaly = src.TrueAnomaly;

        dest->SemiLatusRectum = SemiLatusRectum3(src.Eccentricity, src.SemiMajorAxis);
        dest->ArgPeriapsis = ArgPeriapsis1(src.LonPeriapsis, src.LonAscendingNode);
        dest->SpecRelAngMomentum = SpecRelAngMomentum2(dest->SemiLatusRectum, src.StdGravParam);
    }

    inline void OrbitalStateConv_Compat2Rect(const OrbitalState_Compat &src, OrbitalState_Rect *dest)
    {
        OrbitalState_Nat nat;
        OrbitalStateConv_Compat2Nat(src, &nat);
        OrbitalStateConv_Nat2Rect(nat, dest);
    }

}}
