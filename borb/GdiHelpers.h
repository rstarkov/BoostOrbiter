//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#ifndef __BOOSTORBITER_BORB_GDIHELPERS_H__
#define __BOOSTORBITER_BORB_GDIHELPERS_H__

namespace borb
{
    namespace GdiHelpers
    {
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

        enum MfdPenStyle
        {
            MfdPenSolid = 1,
            MfdPenDashed = 2,
        };

        DWORD GetDefaultColor(MfdColor color);
        HPEN GetDefaultPen(MfdColor color, MfdPenStyle style);
    }
}

#endif // __BOOSTORBITER_BORB_GDIHELPERS_H__
