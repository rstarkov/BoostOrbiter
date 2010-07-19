//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "VesselAccelerationTracker.h"

namespace borb {

    using namespace std;

    void VesselAccelerationTracker::PreStep(VESSEL *vessel, double simdt)
    {
        MATRIX3 vesselRotLocal2Global;
        vessel->GetRotationMatrix(vesselRotLocal2Global);

        // Main acceleration calculation
        VECTOR3 vel, gravForce;
        vessel->GetWeightVector(gravForce);
        GravAccel = mul(vesselRotLocal2Global, gravForce / vessel->GetMass());
        vessel->GetGlobalVel(vel);
        TotalAccel = (vel - _lastGlobalVel) / _simdtLast; // true acceleration in the inertial reference frame
        PerceivedAccel = tmul(vesselRotLocal2Global, TotalAccel - GravAccel); // perceived acceleration onboard the ship
        _simdtLast = simdt;
        _lastGlobalVel = vel;

        // Auxillary values
        LatDeflectionFromVertical = atan2(-PerceivedAccel.x, PerceivedAccel.y);

        if (_first)
        {
            _first = false;
            LatDeflectionFromVertical = 0;
            TotalAccel = GravAccel = PerceivedAccel = _V(0, 0, 0);
        }
    }

}
