//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "SketchpadHelper.h"

namespace borb {

    using namespace std;

    SketchpadHelper::SketchpadHelper(oapi::Sketchpad* sketchpad, int width, int height)
    {
        _sketchpad = sketchpad;
        _dispSize = min(width, height); // maintain a square display even if this means leaving a blank area. Chances are that MFDs will stay square forever.
        _unitSize = (double) _dispSize / 20.0;
        
        _originX = _originY = 0;

        // Configure defaults
        SetFontMonospace();
        SetTextColor(MfdColorGreen);
        SetTextBackTransparent();
        SetPen(MfdColorGreen);
        SetBrush(NULL);
    }



    shared_ptr<oapi::Font> SketchpadHelper::MakeFont(double height, bool proportional, const string& typeface, FontStyle style, int orientation)
    {
        shared_ptr<oapi::Font> font(oapiCreateFont((int) ceil(height * _unitSize + 0.3 /* fudge factor */), proportional, typeface.c_str(), style, orientation), oapiReleaseFont);
        return font;
    }

    shared_ptr<oapi::Pen> SketchpadHelper::MakePen(PENSTYLE style, double width, DWORD color)
    {
        shared_ptr<oapi::Pen> pen(oapiCreatePen((int) style, (int) (width * _unitSize), color), oapiReleasePen);
        return pen;
    }

    shared_ptr<oapi::Brush> SketchpadHelper::MakeBrush(DWORD color)
    {
        shared_ptr<oapi::Brush> brush(oapiCreateBrush(color), oapiReleaseBrush);
        return brush;
    }



    oapi::Font* SketchpadHelper::GetFontProportional()
    {
        if (!_fontProportional)
            _fontProportional = MakeFont(1.0, true, "Sans");
        return _fontProportional.get();
    }

    oapi::Font* SketchpadHelper::GetFontMonospace()
    {
        if (!_fontMonospace)
            _fontMonospace = MakeFont(1.0, false, "Fixed");
        return _fontMonospace.get();
    }

    oapi::Pen* SketchpadHelper::GetPenInvisible()
    {
        if (!_penInvisible)
            _penInvisible = MakePen(PEN_INVISIBLE, 0, 0);
        return _penInvisible.get();
    }

    oapi::Pen* SketchpadHelper::GetPen(MfdColor color, bool dashed)
    {
        unsigned int index = (unsigned int) color;
        if (!dashed)
        {
            while (index >= _pensStdSolid.size())
                _pensStdSolid.push_back(shared_ptr<oapi::Pen>());
            if (!_pensStdSolid[index])
                _pensStdSolid[index] = MakePen(PEN_SOLID, 0, MfdColorValue.FromEnum(color));
            return _pensStdSolid[index].get();
        }
        else
        {
            while (index >= _pensStdDashed.size())
                _pensStdDashed.push_back(shared_ptr<oapi::Pen>());
            if (!_pensStdDashed[index])
                _pensStdDashed[index] = MakePen(PEN_DASHED, 0, MfdColorValue.FromEnum(color));
            return _pensStdDashed[index].get();
        }
    }

    oapi::Brush* SketchpadHelper::GetBrush(MfdColor color)
    {
        unsigned int index = (unsigned int) color;
        while (index >= _brushesStd.size())
            _brushesStd.push_back(shared_ptr<oapi::Brush>());
        if (!_brushesStd[index])
            _brushesStd[index] = MakeBrush(MfdColorValue.FromEnum(color));
        return _brushesStd[index].get();
    }



    SketchpadHelper* SketchpadHelper::SetFont(oapi::Font* font)
    {
        _sketchpad->SetFont(font);
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetFontProportional()
    {
        _sketchpad->SetFont(GetFontProportional());
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetFontMonospace()
    {
        _sketchpad->SetFont(GetFontMonospace());
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetPen(oapi::Pen* pen)
    {
        _sketchpad->SetPen(pen);
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetPen(MfdColor color, bool dashed)
    {
        _sketchpad->SetPen(GetPen(color, dashed));
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetPenInvisible()
    {
        _sketchpad->SetPen(GetPenInvisible());
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetBrush(oapi::Brush* brush)
    {
        _sketchpad->SetBrush(brush);
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetBrush(MfdColor color)
    {
        _sketchpad->SetBrush(GetBrush(color));
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetTextColor(DWORD color)
    {
        _sketchpad->SetTextColor(color);
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetTextColor(MfdColor color)
    {
        _sketchpad->SetTextColor(MfdColorValue.FromEnum(color));
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetTextBackColor(DWORD color)
    {
        _sketchpad->SetBackgroundMode(oapi::Sketchpad::BK_OPAQUE);
        _sketchpad->SetBackgroundColor(color);
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetTextBackColor(MfdColor color)
    {
        _sketchpad->SetBackgroundMode(oapi::Sketchpad::BK_OPAQUE);
        _sketchpad->SetBackgroundColor(MfdColorValue.FromEnum(color));
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetTextBackTransparent()
    {
        _sketchpad->SetBackgroundMode(oapi::Sketchpad::BK_TRANSPARENT);
        return this;
    }



    SketchpadHelper* SketchpadHelper::OriginSet(double x, double y)
    {
        _originX = x;
        _originY = y;
        return this;
    }

    SketchpadHelper* SketchpadHelper::OriginMove(double dx, double dy)
    {
        _originX += dx;
        _originY += dy;
        return this;
    }



    SketchpadHelper* SketchpadHelper::LineMoveTo(double x, double y)
    {
        _sketchpad->MoveTo(calcX(x), calcY(y));
        return this;
    }

    SketchpadHelper* SketchpadHelper::LineDrawTo(double x, double y)
    {
        _sketchpad->LineTo(calcX(x), calcY(y));
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawLine(double x1, double y1, double x2, double y2)
    {
        _sketchpad->Line(calcX(x1), calcY(y1), calcX(x2), calcY(y2));
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawRectangle(double x1, double y1, double x2, double y2)
    {
        _sketchpad->Rectangle(calcX(x1), calcY(y1), calcX(x2), calcY(y2));
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawEllipse(double x1, double y1, double x2, double y2)
    {
        _sketchpad->Ellipse(calcX(x1), calcY(y1), calcX(x2), calcY(y2));
        return this;
    }

    void SketchpadHelper::calcXY(const vector<VECTOR2> src, vector<oapi::IVECTOR2>& dest)
    {
        dest.clear();
        for (vector<VECTOR2>::const_iterator iter = src.begin(); iter != src.end(); iter++)
        {
            oapi::IVECTOR2 pt;
            pt.x = calcX(iter->x);
            pt.y = calcY(iter->y);
            dest.push_back(pt);
        }
    }

    SketchpadHelper* SketchpadHelper::DrawPolygon(const vector<VECTOR2> points)
    {
        vector<oapi::IVECTOR2> pts;
        calcXY(points, pts);
        _sketchpad->Polygon(&pts[0], pts.size());
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawPolyline(const vector<VECTOR2> points)
    {
        vector<oapi::IVECTOR2> pts;
        calcXY(points, pts);
        _sketchpad->Polygon(&pts[0], pts.size());
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawTextLine(double x, double y, const string& text, oapi::Sketchpad::TAlign_horizontal horzAlign, oapi::Sketchpad::TAlign_vertical vertAlign)
    {
        _sketchpad->SetTextAlign(horzAlign, vertAlign);
        _sketchpad->Text(calcX(x), calcY(y), text.c_str(), text.size());
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawTextBox(double x1, double y1, double x2, double y2, const string& text)
    {
        _sketchpad->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::TOP);
        _sketchpad->TextBox(calcX(x1), calcY(y1), calcX(x2), calcY(y2), text.c_str(), text.size());
        return this;
    }

}
