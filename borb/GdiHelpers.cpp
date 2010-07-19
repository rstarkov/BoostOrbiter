//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "GdiHelpers.h"

namespace borb {

    using namespace std;

    static vector<DWORD> _mfdColors;
    static vector<CPen> _mfdPens;

    HPEN GetDefaultPen(MfdColor color, MfdPenStyle style)
    {
        size_t index = color * 2 + (style - 1);
        if (index >= _mfdPens.size())
        {
            _mfdPens.resize(0); // CPen isn't entirely compatible with the resize method, so make sure the old ones are cleared
            _mfdPens.resize(index + 1);
        }

        if (_mfdPens[index].IsNull())
        {
            _mfdPens[index] = CPen();
            _mfdPens[index].CreatePen(style == MfdPenSolid ? PS_SOLID : PS_DOT, 1, MfdColorValue.FromEnum(color));
        }

        return _mfdPens[index];
    }

    void DrawTextLeft(HDC hDC, int x, int y, const string& txt)
    {
        RECT rect;
        rect.left = rect.right = x;
        rect.top = rect.bottom = y;
        DrawText(hDC, txt.c_str(), txt.length(), &rect, DT_NOCLIP);
    }

    void DrawTextRight(HDC hDC, int x, int y, const string& txt)
    {
        RECT rect;
        rect.left = rect.right = x;
        rect.top = rect.bottom = y;
        DrawText(hDC, txt.c_str(), txt.length(), &rect, DT_NOCLIP | DT_RIGHT);
    }

    static double _vpScrLeft, _vpLogLeft, _vpHorzScale;
    static double _vpScrTop, _vpLogTop, _vpVertScale;

    inline void VpLogToScr(double logX, double logY, double& scrX, double& scrY)
    {
        scrX = _vpHorzScale * (logX - _vpLogLeft) + _vpScrLeft;
        scrY = _vpVertScale * (logY - _vpLogTop) + _vpScrTop;
    }

    inline void VpLogToScr(double logX1, double logY1, double logX2, double logY2, double& scrXmin, double& scrYmin, double& scrXmax, double& scrYmax)
    {
        VpLogToScr(logX1, logY1, scrXmin, scrYmin);
        VpLogToScr(logX2, logY2, scrXmax, scrYmax);
        if (scrXmin > scrXmax)
            swap(scrXmin, scrXmax);
        if (scrYmin > scrYmax)
            swap(scrYmin, scrYmax);
    }

    void SetViewport(double scrLeft, double scrTop, double scrRight, double scrBottom, double logLeft, double logTop, double logRight, double logBottom)
    {
        _vpScrLeft = scrLeft;
        _vpLogLeft = logLeft;
        _vpHorzScale = (scrRight - scrLeft) / (logRight - logLeft);
        _vpScrTop = scrTop;
        _vpLogTop = logTop;
        _vpVertScale = (scrBottom - scrTop) / (logBottom - logTop);
    }

    void VpLineDraw(HDC hDC, double xf, double yf, double xt, double yt)
    {
        VpLineMoveTo(hDC, xf, yf);
        VpLineDrawTo(hDC, xt, yt);
    }

    void VpLineMoveTo(HDC hDC, double x, double y)
    {
        VpLogToScr(x, y, x, y);
        MoveToEx(hDC, (int) x, (int) y, NULL);
    }

    void VpLineDrawTo(HDC hDC, double x, double y)
    {
        VpLogToScr(x, y, x, y);
        LineTo(hDC, (int) x, (int) y);
    }

    void VpLineDrawArrow(HDC hDC, double xf, double yf, double xt, double yt, double tipsize, VpArrowScaleMode tipscale)
    {
        if (tipscale == VpArrowScaleLogHorz)
            tipsize *= _vpHorzScale;
        else if (tipscale == VpArrowScaleLogVert)
            tipsize *= _vpVertScale;

        double origX, origY, tipX, tipY;
        VpLogToScr(xf, yf, origX, origY);
        VpLogToScr(xt, yt, tipX, tipY);
        MoveToEx(hDC, (int) origX, (int) origY, NULL);
        LineTo(hDC, (int) tipX, (int) tipY);
        double aback = atan2(origY - tipY, origX - tipX);
        LineTo(hDC, (int) (tipX + tipsize*cos(aback+0.3)), (int) (tipY + tipsize*sin(aback+0.3)));
        MoveToEx(hDC, (int) tipX, (int) tipY, NULL);
        LineTo(hDC, (int) (tipX + tipsize*cos(aback-0.3)), (int) (tipY + tipsize*sin(aback-0.3)));
    }

    void VpCircleDraw(HDC hDC, double cx, double cy, double rad)
    {
        double xf, yf, xt, yt;
        VpLogToScr(cx-rad, cy-rad, cx+rad, cy+rad, xf, yf, xt, yt);
        Ellipse(hDC, (int) xf, (int) yf, (int) xt, (int) yt);
    }

    void VpRectDraw(HDC hDC, double x1, double y1, double x2, double y2)
    {
        double xf, yf, xt, yt;
        VpLogToScr(x1, y1, x2, y2, xf, yf, xt, yt);
        Rectangle(hDC, (int) xf, (int) yf, (int) xt, (int) yt);
    }

}
