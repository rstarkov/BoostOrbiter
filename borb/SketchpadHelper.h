//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#include <PrecompiledBoostOrbiter.h>

#include "Misc.h"
#include "MfdColors.h"

namespace borb {

    enum PENSTYLE
    {
        PEN_INVISIBLE = 0,
        PEN_SOLID = 1,
        PEN_DASHED = 2
    };

    class SketchpadHelper
    {
    public:
        SketchpadHelper(oapi::Sketchpad* sketchpad, int width, int height);

        std::shared_ptr<oapi::Font> MakeFont(double height, bool proportional, const std::string& typeface, FontStyle style = FONT_NORMAL, int orientation = 0);
        std::shared_ptr<oapi::Pen> MakePen(PENSTYLE style, double width, DWORD color); // Use width=0 for the thinnest possible. 0.003 is approx the width of a pixel line on an "average size" MFD.
        std::shared_ptr<oapi::Brush> MakeBrush(DWORD color);

        oapi::Font* GetFontProportional();
        oapi::Font* GetFontMonospace();
        oapi::Pen* GetPenInvisible();
        oapi::Pen* GetPen(MfdColor color, bool dashed = false);
        oapi::Brush* GetBrush(MfdColor color);

        SketchpadHelper* SetFont(oapi::Font* font);
        SketchpadHelper* SetFontProportional();
        SketchpadHelper* SetFontMonospace();
        SketchpadHelper* SetPen(oapi::Pen* pen);
        SketchpadHelper* SetPen(MfdColor color, bool dashed = false);
        SketchpadHelper* SetPenInvisible();
        SketchpadHelper* SetBrush(oapi::Brush* brush);
        SketchpadHelper* SetBrush(MfdColor color);
        SketchpadHelper* SetTextColor(DWORD color);
        SketchpadHelper* SetTextColor(MfdColor color);
        SketchpadHelper* SetTextBackColor(DWORD color);
        SketchpadHelper* SetTextBackColor(MfdColor color);
        SketchpadHelper* SetTextBackTransparent();

        SketchpadHelper* OriginSet(double x, double y);
        SketchpadHelper* OriginMove(double dx, double dy);

        SketchpadHelper* LineMoveTo(double x, double y);
        SketchpadHelper* LineDrawTo(double x, double y);
        SketchpadHelper* DrawLine(double x1, double y1, double x2, double y2);
        SketchpadHelper* DrawRectangle(double x1, double y1, double x2, double y2);
        SketchpadHelper* DrawEllipse(double x1, double y1, double x2, double y2);
        SketchpadHelper* DrawPolygon(const std::vector<VECTOR2> points);
        SketchpadHelper* DrawPolyline(const std::vector<VECTOR2> points);
        SketchpadHelper* DrawTextLine(double x, double y, const std::string& text, oapi::Sketchpad::TAlign_horizontal horzAlign = oapi::Sketchpad::LEFT, oapi::Sketchpad::TAlign_vertical vertAlign = oapi::Sketchpad::TOP);
        SketchpadHelper* DrawTextBox(double x1, double y1, double x2, double y2, const std::string& text);

        inline double CalcButtonY(int numButton) { return 3.1 + 2.85*numButton; }

    private:
        oapi::Sketchpad* _sketchpad; // the underlying Sketchpad that all operations are performed on.

        int _dispSize; // the size of the MFD square side in pixels.
        double _unitSize; // number of pixels per 1.0 of the logical units used by the helper.
        double _originX, _originY;

        std::vector<std::shared_ptr<oapi::Pen>> _pensStdSolid, _pensStdDashed;
        std::vector<std::shared_ptr<oapi::Brush>> _brushesStd;
        std::shared_ptr<oapi::Pen> _penInvisible;
        std::shared_ptr<oapi::Font> _fontProportional, _fontMonospace;

        inline int calcX(double x) { return (int) ((_originX + x) * _unitSize + 0.5); }
        inline int calcY(double y) { return (int) ((_originY + y) * _unitSize + 0.5); }
        void calcXY(const std::vector<VECTOR2> src, std::vector<oapi::IVECTOR2>& dest);
    };

}
