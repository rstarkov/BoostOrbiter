//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

namespace borb {

    enum MfdColor
    {
        MfdColorGreen,
        MfdColorGreenDark,
        MfdColorYellow,
        MfdColorYellowDark,
        MfdColorWhite,
        MfdColorGrey,
        MfdColorRed,
        MfdColorRedDark,
        MfdColorBlue,
        MfdColorBlueDark,
        MfdColorGreyDark,
        MfdColorGreyDarkDark,
    };

    class _mfdColorClassBase
    {
    public:
        const DWORD Green;
        const DWORD GreenDark;
        const DWORD Yellow;
        const DWORD YellowDark;
        const DWORD White;
        const DWORD Grey;
        const DWORD Red;
        const DWORD RedDark;
        const DWORD Blue;
        const DWORD BlueDark;
        const DWORD GreyDark;
        const DWORD GreyDarkDark;

    protected:
        _mfdColorClassBase(std::vector<DWORD> const& vals):
             Green(vals[0]),
             GreenDark(vals[1]),
             Yellow(vals[2]),
             YellowDark(vals[3]),
             White(vals[4]),
             Grey(vals[5]),
             Red(vals[6]),
             RedDark(vals[7]),
             Blue(vals[8]),
             BlueDark(vals[9]),
             GreyDark(vals[10]),
             GreyDarkDark(vals[11])
        {
        }
    };

    class _mfdColorClass : public _mfdColorClassBase
    {
    public:
        _mfdColorClass(): _mfdColorClassBase(loadColorValues()) { }
        DWORD FromEnum(MfdColor color);

    private:
        std::vector<DWORD> loadColorValues();
    };

    extern _mfdColorClass MfdColorValue;

}
