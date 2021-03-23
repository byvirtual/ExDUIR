#pragma once
#include "help_ex.h"

#define CVF_CLIPED 2147483648

struct canvas_s
{
	int dwFlags_;
	int width_;
	int height_;
	void* pContext_;
	void* pGdiInterop_;
	void* pBitmap_;
	wnd_s* pWnd_;
	int c_left_;
	int c_top_;
	int c_right_;
	int c_bottom_;
};

bool _md_create(void* pData, size_t offsetDc, size_t offsetBmp, size_t offsetBits, int width, int height, int* nError);
bool _md_destroy(void* pData, size_t OffsetDc, size_t OffsetBmp, size_t OffsetBits);
bool _canvas_destroy(EXHANDLE hCanvas);
EXHANDLE _canvas_createfromobj(EXHANDLE hObj, int uWidth, int uHeight, int dwFlags, int* nError);
void _canvas_recreate(canvas_s* pCanvas, int width, int height, int* nError);
bool _canvas_resize(EXHANDLE hCanvas, int width, int height);
void _canvas_init(int* nError);
void _canvas_uninit();
void* _canvas_getcontext(EXHANDLE hCanvas, int nType);
bool _canvas_begindraw(EXHANDLE hCanvas);
bool _canvas_enddraw(EXHANDLE hCanvas);
bool _canvas_clear(EXHANDLE hCanvas, int Color);
bool _canvas_drawline(EXHANDLE hCanvas, void* hBrush, float X1, float Y1, float X2, float Y2, float strokeWidth, void* strokeStyle);
bool _canvas_drawrect(EXHANDLE hCanvas, void* hBrush, float left, float top, float right, float bottom, float strokeWidth, void* strokeStyle);
bool _canvas_fillrect(EXHANDLE hCanvas, void* hBrush, float left, float top, float right, float bottom);
bool _canvas_drawroundedrect(EXHANDLE hCanvas, void* hBrush, float left, float top, float right, float bottom, float radiusX, float radiusY, float strokeWidth, void* strokeStyle);
bool _canvas_fillroundedrect(EXHANDLE hCanvas, void* hBrush, float left, float top, float right, float bottom, float radiusX, float radiusY);
bool _canvas_drawpath(EXHANDLE hCanvas, EXHANDLE hPath, void* hBrush, float strokeWidth, void* strokeStyle);
bool _canvas_fillpath(EXHANDLE hCanvas, EXHANDLE hPath, void* hBrush);
bool _canvas_drawellipse(EXHANDLE hCanvas, void* hBrush, float x, float y, float radiusX, float radiusY, float strokeWidth, void* strokeStyle);
bool _canvas_fillellipse(EXHANDLE hCanvas, void* hBrush, float x, float y, float radiusX, float radiusY);
bool _canvas_fillregion(EXHANDLE hCanvas, void* hRgn, void* hBrush);
bool _canvas_drawimagerectrect(EXHANDLE hCanvas, EXHANDLE hImage, float dstLeft, float dstTop, float dstRight, float dstBottom,
	float srcLeft, float srcTop, float srcRight, float srcBottom, int alpha);
bool _canvas_drawimagerect(EXHANDLE hCanvas, EXHANDLE hImage, float Left, float Top, float Right, float Bottom, int alpha);
bool _canvas_drawimage(EXHANDLE hCanvas, EXHANDLE hImage, float Left, float Top, int alpha);
bool _canvas_drawimagefromgrid(EXHANDLE hCanvas, EXHANDLE hImage, float dstLeft, float dstTop, float dstRight, float dstBottom,
	float srcLeft, float srcTop, float srcRight, float srcBottom, float gridPaddingLeft, float gridPaddingTop, float gridPaddingRight,
	float gridPaddingBottom, int dwFlags, int dwAlpha);
bool _canvas_drawimagefrombkgimg_ex(EXHANDLE hCanvas, EXHANDLE hImage, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, void* lpRcSrc, void* lpRCFDst);
bool _canvas_drawimagefrombkgimg(EXHANDLE hCanvas, bkgimg_s* lpBkgImg);
bool _canvas_cliprect(EXHANDLE hCanvas, int left, int top, int right, int bottom);
bool _canvas_resetclip(EXHANDLE hCanvas);
bool _canvas_flush(EXHANDLE hCanvas);
bool _canvas_bitblt(EXHANDLE hCanvas, EXHANDLE sCanvas, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcLeft, int srcTop);
bool _canvas_alphablend(EXHANDLE hCanvas, EXHANDLE sCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom, float srcLeft, float srcTop, float srcRight, float srcBottom, int alpha);
bool _canvas_getsize(EXHANDLE hCanvas, int* width, int* height);
bool _canvas_calctextsize_ex(canvas_s* pCanvas, font_s* pFont, LPCWSTR lpwzText, DWORD dwLen, DWORD dwDTFormat, LPARAM lParam, float layoutWidth, float layoutHeight, int* lpWidth, int* lpHeight, void** pLayout, int* nError);
bool _canvas_calctextsize(EXHANDLE hCanvas, void* hFont, LPCWSTR lpwzText, DWORD dwLen, DWORD dwDTFormat, LPARAM lParam, float layoutWidth, float layoutHeight, int* lpWidth, int* lpHeight);
void _canvas_dx_drawtext_buffer(canvas_s* pCanvas, void* pLayout, int crText, float left, float top, int iGlowsize, int crShadow);
bool _canvas_drawtextex(EXHANDLE hCanvas, void* hFont, int crText, LPCWSTR lpwzText, DWORD dwLen, DWORD dwDTFormat, float left, float top, float right, float bottom, int iGlowsize, int crShadom, LPARAM lParam, void* prclayout);
bool _canvas_drawtext(EXHANDLE hCanvas, void* hFont, int crText, LPCWSTR lpwzText, int dwLen, int dwDTFormat, float left, float top, float right, float bottom);
bool _canvas_blur(EXHANDLE hCanvas, float fDeviation, void* lprc);
bool _canvas_rotate_hue(EXHANDLE hCanvas, float fAngle);
EXHANDLE _canvas_createfrompwnd(wnd_s* pWnd, int width, int height, int dwFlags, int* nError);
void* _canvas_getdc_ex(canvas_s* pCanvas, int* nError);
void* _canvas_getdc(EXHANDLE hCanvas);
void _canvas_releasedc_ex(canvas_s* pCanvas, int* nError);
bool _canvas_releasedc(EXHANDLE hCanvas);
EXHANDLE _canvas_createfromexdui(EXHANDLE hExDui, int width, int height, int dwFlags);

void* _cv_dx_bmp(canvas_s* pCanvas);
void* _cv_context(canvas_s* pCanvas);
void* _cv_dx_gdiinterop(canvas_s* pCanvas);

BOOL _canvas_setantialias(EXHANDLE hCanvas, BOOL antialias);
BOOL _canvas_setimageantialias(EXHANDLE hCanvas, BOOL antialias);
BOOL _canvas_settransform(EXHANDLE hCanvas, D2D1_MATRIX_3X2_F* pMatrix);