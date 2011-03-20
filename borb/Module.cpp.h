//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

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

DLLCLBK void InitModule(HINSTANCE hDLL)
{
    try
    {
        StartLeakMonitor();
        BORB_MODULE_VARIABLE = make_shared<BORB_MODULE_CLASS>();
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
        BORB_MODULE_VARIABLE->LoadFrom(&root);
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
        BORB_MODULE_VARIABLE->SaveTo(&root);
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
