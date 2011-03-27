//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

// WHAT THIS FILE GIVES YOU:
//
// - your module methods are invoked regardless of which Orbiter API version you're compiling against
//
// - smarter callbacks: the loading/saving callbacks have significant amounts of logic to make your life simpler,
//    in particular making global settings (not specific to a scenario) easy.
//
// - any C++/SEH exceptions that escape your callbacks will be captured, displayed along with your module's name
//    (so that the user knows who to blame for the almost-crash), and then stops invoking any further callbacks, effectively
//    shutting down your module. Of course this still doesn't completely prevent your module from CTDing Orbiter...
//
// - your module is properly deallocated before shutting down, allowing the use of msvc runtime leak tracking.
//
//
// HOW TO USE THIS FILE:
//
// - add a "Module.cpp" and a "Module.h" to your module project
//
// - define your borb::ModuleBase descendant in Module.h, e.g.:
//        class ReFuelModule : public borb::ModuleBase { ... }
//
// - insert the following lines at the beginning of your Module.cpp:
//        #define BORB_MODULE_VARIABLE Module
//        #define BORB_MODULE_CLASS ReFuelModule
//        #define BORB_MODULE_NAME "ReFuelMFD"
//        #include "borb/Module.cpp.h"
//        std::shared_ptr<ReFuelModule> Module;
//
//        (where "Module" is the name of the shared_ptr variable storing your module instance, "ReFuelModule" is
//         the name of the subclass you defined, "ReFuelMFD" is the string displayed in Orbiter UI and also used as
//         the filename for the module's global settings)
//
// - implement callbacks by adding overrides for the methods you're interested in. See documentation in 
//    borb/Module.h which explains each available override.

#include "borb.h"

#if !defined(BORB_MODULE_VARIABLE)
# error Module variable (BORB_MODULE_VARIABLE) is undefined. Define it before including this file, and ensure it's of type "std::shared_ptr<BORB_MODULE_CLASS>".
#endif
#if !defined(BORB_MODULE_CLASS)
# error Module class (BORB_MODULE_CLASS) is undefined. Define it before including this file.
#endif
#if !defined(BORB_MODULE_NAME)
# error User-friendly module name (BORB_MODULE_NAME) is undefined. Define it before including this file.
#endif


// The following is never compiled into the final module, but helps make changes to this file with Intellisense operational.
#if !defined(BORB_MODULE_CLASS) || !defined(BORB_MODULE_CLASS) || !defined(BORB_MODULE_VARIABLE)
    class dummyModuleClass : public borb::ModuleBase
    {
    };
    std::shared_ptr<dummyModuleClass> dummyModule;
#  define BORB_MODULE_VARIABLE dummyModule
#  define BORB_MODULE_CLASS dummyModuleClass
#  define BORB_MODULE_NAME "dummy"
#endif


using namespace std;

void StartLeakMonitor();
void LogLeaks();

std::string getGlobalSettingsFilename() { return std::string("Config/Modules/") + BORB_MODULE_NAME + ".cfg"; }
void loadGlobalSettings();
void saveGlobalSettings();

void loadGlobalSettings()
{
    borb::ScenarioNode root;
    FILEHANDLE file = oapiOpenFile(getGlobalSettingsFilename().c_str(), FILE_IN);
    if (file != NULL)
	{
        root.LoadFrom(file);
        oapiCloseFile(file, FILE_IN);
    }
    BORB_MODULE_VARIABLE->LoadFromGlobal(&root);
}

void saveGlobalSettings()
{
    std::string filename = getGlobalSettingsFilename();
    borb::ScenarioNode root;
    BORB_MODULE_VARIABLE->SaveToGlobal(&root);
    if (root.IsEmpty())
    {
        // Ensure the file is deleted if the module doesn't use settings
        DeleteFile(filename.c_str());
    }
    else
    {
        namespace fs = boost::filesystem;
        try { fs::create_directories(fs::path(filename).parent_path()); }
        catch (...) { borb::WriteLog(std::string("Not saving global settings for module ") + BORB_MODULE_NAME + " - couldn't create path for file " + filename); }
        FILEHANDLE file = oapiOpenFile(filename.c_str(), FILE_OUT);
        if (file != NULL)
        {
            root.SaveTo(file);
            oapiCloseFile(file, FILE_OUT);
        }
    }
}

DLLCLBK void InitModule(HINSTANCE hDLL)
{
    try
    {
        StartLeakMonitor();
        BORB_MODULE_VARIABLE = make_shared<BORB_MODULE_CLASS>();
        // Load & save global settings once whenever Launchpad starts or the module is enabled, to ensure the
        // file exists and is up-to-date. This allows the user to create/edit this file without starting full simulation.
        // (of course the file still won't be created if the module doesn't use any global settings)
        loadGlobalSettings();
        saveGlobalSettings();
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        BORB_MODULE_VARIABLE.reset();
        LogLeaks();
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}


DLLCLBK void opcOpenRenderViewport(HWND renderWnd, DWORD width, DWORD height, bool fullscreen)
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        loadGlobalSettings(); // load on simulation start, so that any changes the user made to the file between sessions are effected
        BORB_MODULE_VARIABLE->SimulationStart();
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}

DLLCLBK void opcCloseRenderViewport()
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        BORB_MODULE_VARIABLE->SimulationEnd();
        saveGlobalSettings();
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}


DLLCLBK void opcLoadState(FILEHANDLE scn)
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        borb::ScenarioNode root;
        root.LoadFrom(scn);
        BORB_MODULE_VARIABLE->LoadFromScenario(&root);
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}

DLLCLBK void opcSaveState(FILEHANDLE scn)
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        borb::ScenarioNode root;
        BORB_MODULE_VARIABLE->SaveToScenario(&root);
        if (!root.IsEmpty())
            root.SaveTo(scn);
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}


DLLCLBK void opcPreStep(double simt, double simdt, double mjd)
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        BORB_MODULE_VARIABLE->PreStep(simt, simdt, mjd);
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}

DLLCLBK void opcPostStep(double simt, double simdt, double mjd)
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        BORB_MODULE_VARIABLE->PostStep(simt, simdt, mjd);
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}

DLLCLBK void opcDeleteVessel(OBJHANDLE hVessel)
{
    if (borb::HadUnhandledException())
        return;
    try
    {
        BORB_MODULE_VARIABLE->DeleteVessel(oapiGetVesselInterface(hVessel));
    }
    catch (exception& ex)
    {
        borb::UnhandledException(ex, BORB_MODULE_NAME);
    }
}



#ifdef _DEBUG

static _CrtMemState state;

void StartLeakMonitor()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
    _CrtMemCheckpoint(&state);
}

void LogLeaks()
{
    OutputDebugString("--- Start Leak Dump ---\n"); // The CRT dumps all leaks too early, so make the true dump visually clearer in the trace
    _CrtMemDumpAllObjectsSince(&state);
}

#else

void StartLeakMonitor()
{
}

void LogLeaks()
{
}

#endif
