//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#include <PrecompiledBoostOrbiter.h>
#include "SketchpadHelper.h"

namespace borb {

    using namespace std;

#ifndef USE_ORBITERSDK_2010P1_OR_HIGHER

    class Sketchpad : private boost::noncopyable
    {
    public:
        Sketchpad(HDC hDC) : HandleDC(hDC), SelectedBrush(NULL), SelectedPen(NULL), SelectedFont(NULL) { }
        ~Sketchpad();

        HDC HandleDC;
        Brush* SelectedBrush;
        Pen* SelectedPen;
        Font* SelectedFont;

        void SetFont(Font* font);
        void SetPen(Pen* pen);
        void SetBrush(Brush* brush);

        void SetTextColor(DWORD color);
        void SetTextBackColor(DWORD color);
        void SetTextBackTransparent();

        void MoveTo(int x, int y);
        void LineTo(int x, int y);
        void Line(int x1, int y1, int x2, int y2);
        void Rectangle(int x1, int y1, int x2, int y2);
        void Ellipse(int x1, int y1, int x2, int y2);
        void Polygon(const IVECTOR2* pts, int count);
        void Text(int x, int y, const string& text, HORZALIGN horzAlign, VERTALIGN vertAlign);
        void TextBox(int x1, int y1, int x2, int y2, const string& text);
    };

    class Brush : private boost::noncopyable
    {
    public:
        Brush(HBRUSH brush): HandleBrush(brush), SelectedInto(NULL) { }

        HBRUSH HandleBrush;
        Sketchpad* SelectedInto;

        void Unselect()
        {
            if (SelectedInto == NULL) return;
            SelectObject(SelectedInto->HandleDC, (HBRUSH) NULL_BRUSH);
            SelectedInto->SelectedBrush = NULL;
            SelectedInto = NULL;
        }
    };

    class Pen : private boost::noncopyable
    {
    public:
        Pen(HPEN pen): HandlePen(pen), SelectedInto(NULL) { }

        HPEN HandlePen;
        Sketchpad* SelectedInto;

        void Unselect()
        {
            if (SelectedInto == NULL) return;
            SelectObject(SelectedInto->HandleDC, (HPEN) NULL_PEN);
            SelectedInto->SelectedPen = NULL;
            SelectedInto = NULL;
        }
    };

    class Font : private boost::noncopyable
    {
    public:
        Font(HFONT font): HandleFont(font), SelectedInto(NULL) { }

        HFONT HandleFont;
        Sketchpad* SelectedInto;

        void Unselect()
        {
            if (SelectedInto == NULL) return;
            SelectObject(SelectedInto->HandleDC, (HFONT) SYSTEM_FONT);
            SelectedInto->SelectedFont = NULL;
            SelectedInto = NULL;
        }
    };



    Sketchpad::~Sketchpad()
    {
        if (SelectedBrush != NULL)
            SelectedBrush->Unselect();
        if (SelectedPen != NULL)
            SelectedPen->Unselect();
        if (SelectedFont != NULL)
            SelectedFont->Unselect();
    }

    void Sketchpad::SetBrush(Brush* brush)
    {
        if (SelectedBrush != NULL)
            SelectedBrush->Unselect();

        if (brush == NULL)
        {
            SelectObject(HandleDC, GetStockObject(NULL_BRUSH));
            SelectedBrush = NULL;
        }
        else
        {
            brush->Unselect();
            SelectObject(HandleDC, brush->HandleBrush);
            SelectedBrush = brush;
            brush->SelectedInto = this;
        }
    }

    void Sketchpad::SetPen(Pen* pen)
    {
        if (SelectedPen != NULL)
            SelectedPen->Unselect();

        if (pen == NULL)
        {
            SelectObject(HandleDC, GetStockObject(NULL_PEN));
            SelectedPen = NULL;
        }
        else
        {
            pen->Unselect();
            SelectObject(HandleDC, pen->HandlePen);
            SelectedPen = pen;
            pen->SelectedInto = this;
        }
    }

    void Sketchpad::SetFont(Font* font)
    {
        if (SelectedFont != NULL)
            SelectedFont->Unselect();

        if (font == NULL)
        {
            SelectObject(HandleDC, GetStockObject(SYSTEM_FONT));
            SelectedFont = NULL;
        }
        else
        {
            font->Unselect();
            SelectObject(HandleDC, font->HandleFont);
            SelectedFont = font;
            font->SelectedInto = this;
        }
    }

    void Sketchpad::SetTextColor(DWORD color)
    {
        ::SetTextColor(HandleDC, color);
    }

    void Sketchpad::SetTextBackColor(DWORD color)
    {
        ::SetBkMode(HandleDC, OPAQUE);
        ::SetBkColor(HandleDC, color);
    }

    void Sketchpad::SetTextBackTransparent()
    {
        ::SetBkMode(HandleDC, TRANSPARENT);
    }

    void Sketchpad::MoveTo(int x, int y)
    {
        ::MoveToEx(HandleDC, x, y, NULL);
    }

    void Sketchpad::LineTo(int x, int y)
    {
        ::LineTo(HandleDC, x, y);
    }

    void Sketchpad::Line(int x1, int y1, int x2, int y2)
    {
        MoveTo(x1, y1);
        LineTo(x2, y2);
    }

    void Sketchpad::Rectangle(int x1, int y1, int x2, int y2)
    {
        ::Rectangle(HandleDC, x1, y1, x2, y2);
    }

    void Sketchpad::Ellipse(int x1, int y1, int x2, int y2)
    {
        ::Ellipse(HandleDC, x1, y1, x2, y2);
    }

    void Sketchpad::Polygon(const IVECTOR2* pts, int count)
    {
        ::Polygon(HandleDC, (const POINT*) pts, count);
    }

    void Sketchpad::Text(int x, int y, const string& text, HORZALIGN horzAlign, VERTALIGN vertAlign)
    {
        UINT horzflag = horzAlign == HA_CENTER ? TA_CENTER : horzAlign == HA_RIGHT ? TA_RIGHT : TA_LEFT;
        UINT vertflag = vertAlign == VA_BASELINE ? TA_BASELINE : vertAlign == VA_BOTTOM ? TA_BOTTOM : TA_TOP;
        SetTextAlign(HandleDC, horzflag | vertflag);
        TextOut(HandleDC, x, y, text.c_str(), text.size());
    }

    void Sketchpad::TextBox(int x1, int y1, int x2, int y2, const string& text)
    {
        SetTextAlign(HandleDC, TA_LEFT | TA_TOP);
        RECT rect = { x1, y1, x2, y2 };
        DrawText(HandleDC, text.c_str(), text.size(), &rect, DT_WORDBREAK);
    }

    shared_ptr<Sketchpad> GetSketchpad(HDC hDC)
    {
        return make_shared<Sketchpad>(hDC);
    }

#endif

    Brush* CreateBrush(DWORD color)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        return oapiCreateBrush(color);
#else
        return new Brush(::CreateSolidBrush(color));
#endif
    }

    void ReleaseBrush(Brush* brush)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        oapiReleaseBrush(brush);
#else
        brush->Unselect();
        DeleteObject(brush->HandleBrush);
        delete brush;
#endif
    }

    Pen* CreatePen(PENSTYLE style, int width, DWORD color)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        return oapiCreatePen((int) style, width, color);
#else
        int pstyle = style == PEN_INVISIBLE ? PS_NULL : style == PEN_DASHED ? PS_DOT : PS_SOLID;
        return new Pen(::CreatePen(pstyle, width, color));
#endif
    }

    void ReleasePen(Pen* pen)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        oapiReleasePen(pen);
#else
        pen->Unselect();
        DeleteObject(pen->HandlePen);
        delete pen;
#endif
    }

    Font* CreateFont(int height, bool proportional, const string& typeface, FONTSTYLE style, int orientation)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        return oapiCreateFont(height, proportional, typeface.c_str(), (FontStyle) style, orientation);
#else
        LOGFONT fontdesc = {};
        fontdesc.lfHeight = height;
        fontdesc.lfEscapement = fontdesc.lfOrientation = orientation;
        fontdesc.lfWeight = (style & FONT_BOLD) ? FW_BOLD : FW_NORMAL;
        fontdesc.lfItalic = (style & FONT_ITALIC) ? TRUE : FALSE;
        fontdesc.lfUnderline = (style & FONT_UNDERLINE) ? TRUE : FALSE;
        fontdesc.lfPitchAndFamily = proportional ? VARIABLE_PITCH : FIXED_PITCH;
        if (typeface == "Fixed")
            strcpy_s(fontdesc.lfFaceName, "Courier New");
        else if (typeface == "Sans")
            strcpy_s(fontdesc.lfFaceName, "Arial");
        else if (typeface == "Serif")
            strcpy_s(fontdesc.lfFaceName, "Times New Roman");
        else
            strcpy_s(fontdesc.lfFaceName, typeface.c_str());
        return new Font(::CreateFontIndirect(&fontdesc));
#endif
    }

    void ReleaseFont(Font* font)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        oapiReleaseFont(font);
#else
        font->Unselect();
        DeleteObject(font->HandleFont);
        delete font;
#endif
    }



    SketchpadHelper::SketchpadHelper(Sketchpad* sketchpad, int width, int height)
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



    shared_ptr<Font> SketchpadHelper::MakeFont(double height, bool proportional, const string& typeface, FONTSTYLE style, int orientation)
    {
        shared_ptr<Font> font(borb::CreateFont(calcFontHeight(height), proportional, typeface.c_str(), style, orientation), borb::ReleaseFont);
        return font;
    }

    shared_ptr<Pen> SketchpadHelper::MakePen(PENSTYLE style, double width, DWORD color)
    {
        shared_ptr<Pen> pen(borb::CreatePen(style, (int) (width * _unitSize), color), borb::ReleasePen);
        return pen;
    }

    shared_ptr<Brush> SketchpadHelper::MakeBrush(DWORD color)
    {
        shared_ptr<Brush> brush(borb::CreateBrush(color), borb::ReleaseBrush);
        return brush;
    }



    Font* SketchpadHelper::GetFontProportional()
    {
        if (!_fontProportional)
            _fontProportional = MakeFont(1.0, true, "Sans");
        return _fontProportional.get();
    }

    Font* SketchpadHelper::GetFontMonospace()
    {
        if (!_fontMonospace)
            _fontMonospace = MakeFont(1.0, false, "Fixed");
        return _fontMonospace.get();
    }

    Pen* SketchpadHelper::GetPenInvisible()
    {
        if (!_penInvisible)
            _penInvisible = MakePen(PEN_INVISIBLE, 0, 0);
        return _penInvisible.get();
    }

    Pen* SketchpadHelper::GetPen(MfdColor color, bool dashed)
    {
        unsigned int index = (unsigned int) color;
        if (!dashed)
        {
            while (index >= _pensStdSolid.size())
                _pensStdSolid.push_back(shared_ptr<Pen>());
            if (!_pensStdSolid[index])
                _pensStdSolid[index] = MakePen(PEN_SOLID, 0, MfdColorValue.FromEnum(color));
            return _pensStdSolid[index].get();
        }
        else
        {
            while (index >= _pensStdDashed.size())
                _pensStdDashed.push_back(shared_ptr<Pen>());
            if (!_pensStdDashed[index])
                _pensStdDashed[index] = MakePen(PEN_DASHED, 0, MfdColorValue.FromEnum(color));
            return _pensStdDashed[index].get();
        }
    }

    Brush* SketchpadHelper::GetBrush(MfdColor color)
    {
        unsigned int index = (unsigned int) color;
        while (index >= _brushesStd.size())
            _brushesStd.push_back(shared_ptr<Brush>());
        if (!_brushesStd[index])
            _brushesStd[index] = MakeBrush(MfdColorValue.FromEnum(color));
        return _brushesStd[index].get();
    }



    SketchpadHelper* SketchpadHelper::SetFont(Font* font)
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

    SketchpadHelper* SketchpadHelper::SetPen(Pen* pen)
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

    SketchpadHelper* SketchpadHelper::SetBrush(Brush* brush)
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
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        _sketchpad->SetBackgroundMode(oapi::Sketchpad::BK_OPAQUE);
        _sketchpad->SetBackgroundColor(color);
#else
        _sketchpad->SetTextBackColor(color);
#endif
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetTextBackColor(MfdColor color)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        _sketchpad->SetBackgroundMode(oapi::Sketchpad::BK_OPAQUE);
        _sketchpad->SetBackgroundColor(MfdColorValue.FromEnum(color));
#else
        _sketchpad->SetTextBackColor(MfdColorValue.FromEnum(color));
#endif
        return this;
    }

    SketchpadHelper* SketchpadHelper::SetTextBackTransparent()
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        _sketchpad->SetBackgroundMode(oapi::Sketchpad::BK_TRANSPARENT);
#else
        _sketchpad->SetTextBackTransparent();
#endif
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

    void SketchpadHelper::calcXY(const vector<VECTOR2> src, vector<IVECTOR2>& dest)
    {
        dest.clear();
        for (vector<VECTOR2>::const_iterator iter = src.begin(); iter != src.end(); iter++)
        {
            IVECTOR2 pt;
            pt.x = calcX(iter->x);
            pt.y = calcY(iter->y);
            dest.push_back(pt);
        }
    }

    SketchpadHelper* SketchpadHelper::DrawPolygon(const vector<VECTOR2> points)
    {
        vector<IVECTOR2> pts;
        calcXY(points, pts);
        _sketchpad->Polygon(&pts[0], pts.size());
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawPolyline(const vector<VECTOR2> points)
    {
        vector<IVECTOR2> pts;
        calcXY(points, pts);
        _sketchpad->Polygon(&pts[0], pts.size());
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawTextLine(double x, double y, const string& text, HORZALIGN horzAlign, VERTALIGN vertAlign)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        _sketchpad->SetTextAlign((oapi::Sketchpad::TAlign_horizontal) horzAlign, (oapi::Sketchpad::TAlign_vertical) vertAlign);
        _sketchpad->Text(calcX(x), calcY(y), text.c_str(), text.size());
#else
        _sketchpad->Text(calcX(x), calcY(y), text, horzAlign, vertAlign);
#endif
        return this;
    }

    SketchpadHelper* SketchpadHelper::DrawTextBox(double x1, double y1, double x2, double y2, const string& text)
    {
#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER
        _sketchpad->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::TOP);
        _sketchpad->TextBox(calcX(x1), calcY(y1), calcX(x2), calcY(y2), text.c_str(), text.size());
#else
        _sketchpad->TextBox(calcX(x1), calcY(y1), calcX(x2), calcY(y2), text);
#endif
        return this;
    }

}
