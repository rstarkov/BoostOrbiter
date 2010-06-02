//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "boapi.h"

namespace borb
{
    namespace boapi
    {
        OBJHANDLE GetVesselByName(const std::string& name)
        {
            char cstr[256];
            size_t length = name.copy(cstr, sizeof(cstr) - 1);
            cstr[length] = 0;
            return oapiGetVesselByName(cstr);
        }

        VECTOR3 LocalToHorizon(const VECTOR3 &vect, double lon, double lat)
        {
            //1. rotate by negative longitude (around y)
            //2. rotate by negative lattitude (around z)
            //3. rotate by +90deg lattitude (around z)
            return mul(matrix_rot_rz(-lat + PI/2), mul(matrix_rot_ly(-lon), vect));
        }

        VECTOR3 HorizonToLocal(const VECTOR3 &vect, double lon, double lat)
        {
            return tmul(matrix_rot_ly(-lon), tmul(matrix_rot_rz(-lat + PI/2), vect));
        }

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
}
