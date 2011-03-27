//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

namespace borb {

    union VECTOR2
    {
	    double data[2];
	    struct { double x, y; };
        VECTOR2() { x = 0; y = 0; }
        VECTOR2(double x_, double y_) { x = x_; y = y_; }
    };

    bool HadUnhandledException();
    void UnhandledException(const std::exception& ex, const std::string& moduleName);

    // A wrapper around oapiWriteLog that fixes the const annoyance.
    inline void WriteLog(const std::string& message)
    {
        std::vector<char> copyMsg(message.begin(), message.end());
        copyMsg.push_back(0);
        oapiWriteLog(&copyMsg[0]); // could just const_cast, but who knows...
    }

    // Returns a handle to the vessel of the specified name, or NULL if the named vessel could not be found.
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

    inline double clamp(double value, double min, double max)
    {
        if (value < min)
            return min;
        else if (value > max)
            return max;
        else
            return value;
    }

    inline int clamp(int value, int min, int max)
    {
        if (value < min)
            return min;
        else if (value > max)
            return max;
        else
            return value;
    }

    inline double round(double value, double magnitude)
    {
        if (value > 0)
            return floor(value / magnitude + 0.5) * magnitude;
        else
            return -floor(-value / magnitude + 0.5) * magnitude;
    }

    struct AveragePoint
    {
        double MJD;
        double Value;
    };

    class RecentAverageTracker
    {
    public:
        void Update(double mjd, double value);
        double GetAverage() { return _total / (double) _count; }
        void SetWindow(double fromTMinusSeconds, double toTMinusSeconds);

    private:
        double _fromTMinusMJD, _toTMinusMJD;
        std::deque<AveragePoint> _points;
        double _total, _lastMJD;
        int _count;
    };

    template<typename TMfdClass>
    class MfdMode
    {
    public:
        MfdMode(int modeActivateOapiKey, const std::string& modeLabel)
        {
            _label = vector<char>(modeLabel.begin(), modeLabel.end());
            _label.push_back(0);

            MFDMODESPEC spec;
            spec.name = &_label[0];
            spec.key = modeActivateOapiKey;
            spec.msgproc = mfdMsgProc;

            _mfdModeId = oapiRegisterMFDMode(spec);
        }

        ~MfdMode()
        {
            oapiUnregisterMFDMode(_mfdModeId);
        }

    private:
        int _mfdModeId;
        std::vector<char> _label; // the API documentation doesn't mention whether the non-const char* is an error or not, so better safe...

        static int mfdMsgProc(UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
        {
            if (msg == OAPI_MSG_MFD_OPENED)
            {
                return (int) new TMfdClass(LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam);
            }
            return 0;
        }
    };

}
