#include "drawing.h"

namespace drawing {
	void SetBrushAlpha(HDC hDC, HBRUSH hBrush, byte alpha) {

	};

	void DrawOpaqueRect(HDC hDC, RECT rect, COLORREF color, byte opacity) {
	};

	RECT GDIRectToRect(Gdiplus::Rect* rect) {
		return { rect->X, rect->Y, rect->Width, rect->Height };
	}
}