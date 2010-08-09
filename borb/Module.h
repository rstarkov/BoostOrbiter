//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include "ScenarioTree.h"

namespace borb {

    class ModuleBase : private boost::noncopyable
    {
    public:
        virtual void SimulationStart() { }
        virtual void SimulationEnd() { }

        virtual void SaveTo(borb::ScenarioNode* node) { }
        virtual void LoadFrom(borb::ScenarioNode* node) { }

        virtual void PreStep(double simt, double simdt, double mjd) { }
        virtual void PostStep(double simt, double simdt, double mjd) { }
    };

}