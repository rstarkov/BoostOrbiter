//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include "ScenarioTree.h"

namespace borb {

    // Provides a base class for creating an Orbiter module. This class should be used in conjunction with
    // "Module.cpp.h", which instantiates this class and invokes methods as and when appropriate.
    class ModuleBase : private boost::noncopyable
    {
    public:
        // Called when the simulation begins. At this stage global settings have definitely been loaded. No guarantees
        // as to whether scenario settings have been loaded yet.
        virtual void SimulationStart() { }
        // Called when the simulation ends. At this stage global settings may still be modified; the callback to save
        // them is guaranteed to happen afterwards. Scenario settings have already been saved at this stage.
        virtual void SimulationEnd() { }

        // Called to let the module save any global settings into a module-specific file under /Config/Modules. Note
        // that this is also called at simulation start, to ensure that the file exists even in case of CTD. If left empty,
        // the file will be deleted if it already exists.
        virtual void SaveToGlobal(borb::ScenarioNode* node) { }
        // Called to let the module load any global settings from a module-specific file. The "node" parameter is always
        // a valid instance, but may be empty (e.g. first ever use of this module). Called more than once!
        virtual void LoadFromGlobal(borb::ScenarioNode* node) { }

        // Called to let the module save per-scenario settings into the scenario file. The "node" parameter is guaranteed
        // to be a valid empty instance, and should be filled by this method. If left empty, nothing will be saved.
        virtual void SaveToScenario(borb::ScenarioNode* node) { }
        // Called to let the module load per-scenario settings from the scenario file. The "node" parameter is guaranteed
        // to be a valid instance, but may be completely empty (e.g. first ever use of this module).
        virtual void LoadFromScenario(borb::ScenarioNode* node) { }

        // Called before every time step. Keep this as fast as possible.
        virtual void PreStep(double simt, double simdt, double mjd) { }
        // Called after every time step. Keep this as fast as possible.
        virtual void PostStep(double simt, double simdt, double mjd) { }

        // Called whenever a vessel is deleted. This is the module's last chance to access this vessel.
        virtual void DeleteVessel(VESSEL* vessel) { }
    };

}