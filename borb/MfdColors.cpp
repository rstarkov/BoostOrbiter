//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "MfdColors.h"

#include <SimpleIni/SimpleIni.h>

// TODO: allow MFDs to define purpose-based colors, which can be assigned to
// the "standard" MFD colors or custom RGB values via a per-MFD config file.

namespace borb {

    _mfdColorClass MfdColorValue;

    DWORD _mfdColorClass::FromEnum(MfdColor color)
    {
        switch (color)
        {
            case MfdColorGreen: return Green;
            case MfdColorGreenDark: return GreenDark;
            case MfdColorYellow: return Yellow;
            case MfdColorYellowDark: return YellowDark;
            case MfdColorWhite: return White;
            case MfdColorGrey: return Grey;
            case MfdColorRed: return Red;
            case MfdColorRedDark: return RedDark;
            case MfdColorBlue: return Blue;
            case MfdColorBlueDark: return BlueDark;
            case MfdColorGreyDark: return GreyDark;
            case MfdColorGreyDarkDark: return GreyDarkDark;
        }
        return 0;
    }

    std::vector<DWORD> _mfdColorClass::loadColorValues()
    {
        CSimpleIni ini;
        ini.LoadFile("Config/MFD/Default.cfg"); // unfortunately this has comments to the right of values which SimpleIni doesn't understand...
        std::vector<DWORD> _colors;
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_0_BRT", 0x00FF00UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_0_DIM", 0x40A040UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_1_BRT", 0x00FFFFUL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_1_DIM", 0x00A0A0UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_2_BRT", 0xF0F0F0UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_2_DIM", 0x909090UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_3_BRT", 0x6060FFUL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_3_DIM", 0x2020A0UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_4_BRT", 0xFF8080UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_4_DIM", 0xA00000UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_5_BRT", 0x666666UL));
        _colors.push_back(ini.GetOrbiterHexValue("", "COL_5_DIM", 0x303030UL));
        return _colors;
    }

}
