#pragma once

#include <math.h>

#include "Consts.h"
#include "Util.h"
#include "Types.h"
#include "OrbitalFuncCore.h"
#include "OrbitalFuncAux.h"
#include "OrbitalFuncAnomaly.h"
#include "OrbitalFuncStates.h"

#if 0

TODO:
- MeanLonAtEpoch / TimeOfPeriapsisPassage - name?

====================================================

Ordering of parameters:

- Eccentricity             (0..Inf, 1 is special in that many functions break; some also break at 0)
- SemiLatusRectum   (always positive; defined for any kind of orbit)
- SemiMajorAxis       (positive for elliptic, negative for hyperbolic, undefined for parabolic)

- StdGravParam                    (always positive)
- SpecOrbitalEnergy             (negative for elliptic, positive for hyperbolic, zero for parabolic)
- SpecRelAngMomentum    (always positive)

- Period         (always positive; defined for hyperbolic orbits as the positive period of an elliptic orbit with the same semi-major axis. Undefined for parabolic orbits)
- Speed         (always positive)
- Distance     (always positive)

- MeanAnomaly
- EccentricAnomaly
- TrueAnomaly
- ArgPeriapsis
- LonPeriapsis
- LonAscendingNode       (undefined for equatorial orbits)
- MeanLonAtEpoch
- TimeOfPeriapsisPassage    (time since the last passage of the periapsis; may be negative)

====================================================

SemiMajorAxis            1,2,3,5
StdGravParam             0,1,2,3,4,5
Speed                          3,5
Distance                       3,5
Eccentricity                   1,2,3,4
SpecOrbitalEnergy       1,3,4
SpecRelAngMomentum      2,4
DistanceAtPeriapsis     1,2
SemiLatusRectum         1,2,3
LonAscendingNode      1
LonPeriapsis                  1,3
ArgPeriapsis                  1
MeanLonAtEpoch         3
MeanAnomaly              1,2,3
EccentricAnomaly        1,2
TrueAnomaly                1

#endif