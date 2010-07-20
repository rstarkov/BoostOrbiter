//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "Misc.h"

namespace borb {

    using namespace std;

    static const char* unhandled_exception_message;
    static bool had_unhandled_exception = false;
    static string unhandled_exception_modulename;

    bool HadUnhandledException()
    {
        return had_unhandled_exception;
    }

    BOOL CALLBACK UnhandledException_EnumThreadWndProc(HWND hwnd, LPARAM lparam)
    {
        if (had_unhandled_exception)
            return TRUE;
        had_unhandled_exception = true;
        MessageBox(hwnd, (string("Module ") + unhandled_exception_modulename + " has thrown an exception.\n\nMessage:\n" + unhandled_exception_message + "\n\nNo further messages will be shown about exceptions in ReFuelMFD.").c_str(), "ReFuelMFD", MB_ICONERROR | MB_TOPMOST);
        return TRUE;
    }

    void UnhandledException(const exception& ex, const string& moduleName)
    {
        unhandled_exception_modulename = moduleName;
        unhandled_exception_message = ex.what();
        EnumThreadWindows(GetCurrentThreadId(), UnhandledException_EnumThreadWndProc, 0);
        UnhandledException_EnumThreadWndProc(NULL, 0);
    }

    OBJHANDLE GetVesselByName(const string& name)
    {
        char cstr[256];
        size_t length = name.copy(cstr, sizeof(cstr) - 1);
        cstr[length] = 0;
        return oapiGetVesselByName(cstr);
    }

    VECTOR3 LocalToHorizon(const VECTOR3& vect, double lon, double lat)
    {
        //1. rotate by negative longitude (around y)
        //2. rotate by negative lattitude (around z)
        //3. rotate by +90deg lattitude (around z)
        return mul(matrix_rot_rz(-lat + PI/2), mul(matrix_rot_ly(-lon), vect));
    }

    VECTOR3 HorizonToLocal(const VECTOR3& vect, double lon, double lat)
    {
        return tmul(matrix_rot_ly(-lon), tmul(matrix_rot_rz(-lat + PI/2), vect));
    }

    void RecentAverageTracker::Update(double mjd, double value)
    {
        if (_points.size() > 0 && mjd <= _points.back().MJD)
            throw exception("RecentAverageTracker: time must be strictly monotonically increasing.");
        AveragePoint pt;
        pt.MJD = mjd;
        pt.Value = value;
        _points.push_back(pt);

        double fr = mjd + _fromTMinusMJD, to = mjd + _toTMinusMJD;
        while (_points.size() > 0 && _points.front().MJD < fr)
        {
            pt = _points.front();
            _points.pop_front();
            if (pt.MJD <= _lastMJD)
            {
                _total -= pt.Value;
                _count--;
            }
        }

        for (deque<AveragePoint>::iterator i = _points.begin(), end = _points.end(); i < end; ++i)
        {
            if (i->MJD > _lastMJD && i->MJD <= to)
            {
                _total += i->Value;
                _count++;
                _lastMJD = i->MJD;
            }
            else if (i->MJD > to)
                break;
        }
    }

    void RecentAverageTracker::SetWindow(double fromTMinusSeconds, double toTMinusSeconds)
    {
        _fromTMinusMJD = -abs(fromTMinusSeconds) / 86400;
        _toTMinusMJD = -abs(toTMinusSeconds) / 86400;
    }

}
