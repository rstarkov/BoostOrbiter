//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

namespace borb { namespace util {

    inline double clamp(double value, double min, double max)
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

} }
