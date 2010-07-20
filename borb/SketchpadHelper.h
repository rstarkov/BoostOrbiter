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

    enum FONTSTYLE
    {
	    FONT_NORMAL = 0,
	    FONT_BOLD = 1,
	    FONT_ITALIC = 2,
	    FONT_UNDERLINE = 4
    };

	enum HORZALIGN
    {
		HA_LEFT = 0,
		HA_CENTER = 1,
		HA_RIGHT = 2
	};

	enum VERTALIGN
    {
		VA_TOP = 0,
		VA_BASELINE = 1,
		VA_BOTTOM = 2
	};

#ifdef USE_ORBITERSDK_2010P1_OR_HIGHER

    typedef ::oapi::Sketchpad Sketchpad;
    typedef ::oapi::Brush Brush;
    typedef ::oapi::Pen Pen;
    typedef ::oapi::Font Font;
    typedef ::oapi::IVECTOR2 IVECTOR2;

#else

    class Sketchpad;
    class Brush;
    class Pen;
    class Font;

    union IVECTOR2
    {
	    long data[2];
	    struct { long x, y; };
        IVECTOR2() { x = 0; y = 0; }
        IVECTOR2(long x_, long y_) { x = x_; y = y_; }
    };

    std::shared_ptr<Sketchpad> GetSketchpad(HDC hDC);

#endif

    Brush* CreateBrush(DWORD color);
    void ReleaseBrush(Brush* brush);
    Pen* CreatePen(PENSTYLE style, int width, DWORD color);
    void ReleasePen(Pen* pen);
    Font* CreateFont(int height, bool proportional, const std::string& family, FONTSTYLE style, int orientation);
    void ReleaseFont(Font* font);

    class SketchpadHelper
    {
    public:
        SketchpadHelper(Sketchpad* sketchpad, int width, int height);

        std::shared_ptr<Font> MakeFont(double height, bool proportional, const std::string& typeface, FONTSTYLE style = FONT_NORMAL, int orientation = 0);
        std::shared_ptr<Pen> MakePen(PENSTYLE style, double width, DWORD color); // Use width=0 for the thinnest possible. 0.003 is approx the width of a pixel line on an "average size" MFD.
        std::shared_ptr<Brush> MakeBrush(DWORD color);

        Font* GetFontProportional();
        Font* GetFontMonospace();
        Pen* GetPenInvisible();
        Pen* GetPen(MfdColor color, bool dashed = false);
        Brush* GetBrush(MfdColor color);

        SketchpadHelper* SetFont(Font* font);
        SketchpadHelper* SetFontProportional();
        SketchpadHelper* SetFontMonospace();
        SketchpadHelper* SetPen(Pen* pen);
        SketchpadHelper* SetPen(MfdColor color, bool dashed = false);
        SketchpadHelper* SetPenInvisible();
        SketchpadHelper* SetBrush(Brush* brush);
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
        SketchpadHelper* DrawTextLine(double x, double y, const std::string& text, HORZALIGN horzAlign = HA_LEFT, VERTALIGN vertAlign = VA_TOP);
        SketchpadHelper* DrawTextBox(double x1, double y1, double x2, double y2, const std::string& text);

        inline double CalcButtonY(int numButton) { return 3.1 + 2.85*numButton; }

    private:
        Sketchpad* _sketchpad; // the underlying Sketchpad that all operations are performed on.

        int _dispSize; // the size of the MFD square side in pixels.
        double _unitSize; // number of pixels per 1.0 of the logical units used by the helper.
        double _originX, _originY;

        std::vector<std::shared_ptr<Pen>> _pensStdSolid, _pensStdDashed;
        std::vector<std::shared_ptr<Brush>> _brushesStd;
        std::shared_ptr<Pen> _penInvisible;
        std::shared_ptr<Font> _fontProportional, _fontMonospace;

        inline int calcX(double x) { return (int) ((_originX + x) * _unitSize + 0.5); }
        inline int calcY(double y) { return (int) ((_originY + y) * _unitSize + 0.5); }
        inline int calcFontHeight(double height) { return (int) ceil(height * _unitSize + 0.3 /* fudge factor */); }
        void calcXY(const std::vector<VECTOR2> src, std::vector<IVECTOR2>& dest);
    };

}
