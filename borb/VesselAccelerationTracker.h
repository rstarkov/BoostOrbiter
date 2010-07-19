//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

namespace borb {

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

}
