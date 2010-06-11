//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "util.h"

namespace borb { namespace util {

    using namespace std;

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

} }
