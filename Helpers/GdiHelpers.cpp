#include <PrecompiledBoostOrbiter.h>
#include <SimpleIni/SimpleIni.h>
#include "GdiHelpers.h"

namespace BoostOrbiter
{
    namespace GdiHelpers
    {
        using boost::shared_ptr;

        static std::vector<DWORD> _mfdColors;
        static std::vector<shared_ptr<void>> _mfdPens;

        void initMfdColors()
        {
            CSimpleIni ini;
            ini.LoadFile("Config/MFD/Default.cfg"); // unfortunately this has comments to the right of values which SimpleIni doesn't understand...
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_0_BRT", 0x00FF00UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_0_DIM", 0x40A040UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_1_BRT", 0x00FFFFUL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_1_DIM", 0x00A0A0UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_2_BRT", 0xF0F0F0UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_2_DIM", 0x909090UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_3_BRT", 0x6060FFUL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_3_DIM", 0x2020A0UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_4_BRT", 0xFF8080UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_4_DIM", 0xA00000UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_5_BRT", 0x666666UL));
            _mfdColors.push_back(ini.GetOrbiterHexValue("", "COL_5_DIM", 0x303030UL));
        }

        DWORD GetDefaultColor(MfdColor color)
        {
            if (_mfdColors.empty())
                initMfdColors();
            size_t index = color;
            assert(index >= 0 && index < _mfdColors.size());
            return _mfdColors[index];
        }

        struct GdiObjectDeleter
        {
            void operator()(void * p) { DeleteObject( (HGDIOBJ) p); }
        };

        HPEN GetDefaultPen(MfdColor color, MfdPenStyle style)
        {
            size_t index = color * 2 + (style - 1);
            if (index >= _mfdPens.size())
                _mfdPens.resize(index + 1);

            if (_mfdPens[index])
                return (HPEN) _mfdPens[index].get();
            else
            {
                HPEN hpen = CreatePen(style == MfdPenSolid ? PS_SOLID : PS_DOT, 1, GetDefaultColor(color));
                shared_ptr<void> newPen(hpen, GdiObjectDeleter());
                _mfdPens[index] = newPen;
                return (HPEN) newPen.get();
            }
        }
    }
}
