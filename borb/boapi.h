//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

namespace borb { namespace boapi {

    OBJHANDLE GetVesselByName(const std::string& name);

    // Rotates the specified vector from planetary local frame to a "horizon" frame at the specified lon/lat.
    // The "y" axis of the transformed vector is the component of the original vector pointing away from the
    // planet at the specified lon/lat.
    //
    // Note: the "local" frame for a planet is effectively defined by Orbiter's EquToLocal function. The X axis
    // goes through the point with lon/lat 0/0. Longitude rotates from X towards Z, while lattitude rotates
    // from X towards Y.
    VECTOR3 LocalToHorizon(const VECTOR3& vect, double lon, double lat);

    // Rotates the specified vector from the "horizon" frame at the specified lon/lat to the planetary local frame.
    // See LocalToHorizon for more info.
    VECTOR3 HorizonToLocal(const VECTOR3& vect, double lon, double lat);

    // Returns a right-handed rotation matrix around the X axis (rotates Y->Z)
    inline MATRIX3 matrix_rot_rx(double angle)
    {
        double sinA = sin(angle), cosA = cos(angle);
	    MATRIX3 mat = {
            1, 0, 0,
            0, cosA, -sinA,
            0, sinA, cosA
        };
	    return mat;
    }

    // Returns a right-handed rotation matrix around the Y axis (rotates Z->X)
    inline MATRIX3 matrix_rot_ry(double angle)
    {
        double sinA = sin(angle), cosA = cos(angle);
	    MATRIX3 mat = {
            cosA, 0, sinA,
            0, 1, 0,
            -sinA, 0, cosA
        };
	    return mat;
    }

    // Returns a right-handed rotation matrix around the Z axis (rotates X->Y)
    inline MATRIX3 matrix_rot_rz(double angle)
    {
        double sinA = sin(angle), cosA = cos(angle);
	    MATRIX3 mat = {
            cosA, -sinA, 0,
            sinA, cosA, 0,
            0, 0, 1
        };
	    return mat;
    }

    // Returns a left-handed rotation matrix around the X axis (rotates Z->Y)
    inline MATRIX3 matrix_rot_lx(double angle)
    {
        double sinA = sin(angle), cosA = cos(angle);
	    MATRIX3 mat = {
            1, 0, 0,
            0, cosA, sinA,
            0, -sinA, cosA
        };
	    return mat;
    }

    // Returns a left-handed rotation matrix around the Y axis (rotates X->Z)
    inline MATRIX3 matrix_rot_ly(double angle)
    {
        double sinA = sin(angle), cosA = cos(angle);
	    MATRIX3 mat = {
            cosA, 0, -sinA,
            0, 1, 0,
            sinA, 0, cosA
        };
	    return mat;
    }

    // Returns a left-handed rotation matrix around the Z axis (rotates Y->X)
    inline MATRIX3 matrix_rot_lz(double angle)
    {
        double sinA = sin(angle), cosA = cos(angle);
	    MATRIX3 mat = {
            cosA, sinA, 0,
            -sinA, cosA, 0,
            0, 0, 1
        };
	    return mat;
    }

    // Calculates vessel's acceleration. This is a class because determining vessel acceleration requires some state to be kept track of.
    class VesselAccelerationTracker
    {
    public:
        VesselAccelerationTracker() { _first = true; }

        // This function should be called in every pre-step, and will update the various vectors and values that this class exposes.
        void PreStep(VESSEL *vessel, double simdt);

        // True change in velocity in a global inertial frame, in vessel's local coordinate system.
        // Doesn't really have a real-world meaning due to the equivalence principle.
        VECTOR3 TotalAccel;

        // Change in velocity due to the gravitational field, in vessel's local coordinate system.
        // Doesn't really have a real-world meaning due to the equivalence principle.
        VECTOR3 GravAccel;

        // Change in velocity due to forces other than gravity, in vessel's local coordinate system.
        // This is the acceleration that can be actually perceived and measured on board of a "sealed"
        // vessel with no external references to make use of.
        VECTOR3 PerceivedAccel;

        // The angle (radians) between the vessel's "down" and the "gravity" vector in the lateral/vertical plane. For example,
        // if the perceived gravity is pointing straight down towards the vessel's floor, this reads 0. A deflection to the right
        // from the pilot's perspective will result in a positive reading.
        double LatDeflectionFromVertical;

    private:
        double _simdtLast;
        VECTOR3 _lastGlobalVel;
        bool _first;
    };

} }
