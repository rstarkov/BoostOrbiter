//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

namespace borb { namespace GdiHelpers {

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

    // Gets the actual 32-bit color value for the specified "standard" color. Respects the color definitions used by Orbiter through "Config/MFD/Default.cfg".
    DWORD GetDefaultColor(MfdColor color);

    // Gets a pen of the specified color and style. Respects the color definitions used by Orbiter.
    HPEN GetDefaultPen(MfdColor color, MfdPenStyle style);

    void DrawTextLeft(HDC hDC, int x, int y, const std::string& txt);
    void DrawTextRight(HDC hDC, int x, int y, const std::string& txt);

    enum VpArrowScaleMode
    {
        VpArrowScaleScreen,
        VpArrowScaleLogHorz,
        VpArrowScaleLogVert,
    };

    // Stores a mapping between screen coordinates and logical coordinates. This mapping is used by GdiHelpers' Vp* functions.
    // The primary difference from GDI's transforms is that only starting positions are affected by the viewport transformation. Things
    // like font sizes, letter shapes and pen widths are intentionally unaffected.
    void SetViewport(double scrLeft, double scrTop, double scrRight, double scrBottom, double logLeft, double logTop, double logRight, double logBottom);

    void VpLineDraw(HDC hDC, double xf, double yf, double xt, double yt);
    void VpLineMoveTo(HDC hDC, double x, double y);
    void VpLineDrawTo(HDC hDC, double x, double y);
    // Draws a line with an arrowhead at the (xt,yt) end. "tipsize" specifies the length of the arrowhead arms (in screen units unless overridden).
    // The arrowhead will be symmetric regardless of any differences in the horz/vert scaling.
    void VpLineDrawArrow(HDC hDC, double xf, double yf, double xt, double yt, double tipsize, VpArrowScaleMode tipscale = VpArrowScaleScreen);
    void VpCircleDraw(HDC hDC, double cx, double cy, double rad);
    void VpRectDraw(HDC hDC, double x1, double y1, double x2, double y2);

} }
