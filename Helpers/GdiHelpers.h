#ifndef __BOOSTORBITER_HELPERS_GDIHELPERS_H__
#define __BOOSTORBITER_HELPERS_GDIHELPERS_H__

namespace BoostOrbiter
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

#endif // __BOOSTORBITER_HELPERS_GDIHELPERS_H__
